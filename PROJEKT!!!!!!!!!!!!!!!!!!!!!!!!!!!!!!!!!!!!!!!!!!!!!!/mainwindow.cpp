#include "mainwindow.h"
#include "storage.h"
#include "analysis.h"
#include <QThread>
#include "dataloaderworker.h"
#include <QThread>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDateTime>
#include <QTimer>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QEventLoop>
#include <QApplication>

MainWindow::MainWindow(QObject *parent)
    : QObject(parent),
    m_networkManager(new QNetworkAccessManager(this)),
    m_sensorViewVisible(false),
    m_chartViewVisible(false),
    m_isOnline(false)
{
    // Połącz sygnały i sloty
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onNetworkReply);

    // Stwórz katalog data jeśli nie istnieje
    QDir().mkpath("data");

    // Inicjalizacja timera sprawdzania połączenia co 30 sekund
    internetCheckTimer = new QTimer(this);
    connect(internetCheckTimer, &QTimer::timeout, this, &MainWindow::checkInternetConnection);
    internetCheckTimer->start(30000); // 30 sekund

    // Inicjalizacja timera zapisu co godzinę
    hourlySaveTimer = new QTimer(this);
    connect(hourlySaveTimer, &QTimer::timeout, this, &MainWindow::saveDataIfDue);
    hourlySaveTimer->start(3600000); // 1 godzina

    // Sprawdź czy jest internet na start
    checkInternetConnectionInitial();
}

MainWindow::~MainWindow() {
    delete m_networkManager;
    delete hourlySaveTimer;
    delete internetCheckTimer;
}

bool MainWindow::isOnline() {
    // Sprawdź dwa różne adresy aby zwiększyć niezawodność detekcji
    QUrl url1("https://api.gios.gov.pl/pjp-api/rest/station/findAll");
    QUrl url2("https://www.google.com");

    // Najpierw sprawdź główne API
    QNetworkRequest request1(url1);
    QNetworkReply *reply1 = m_networkManager->get(request1);
    QEventLoop loop1;
    QTimer timer1;
    timer1.setSingleShot(true);
    connect(reply1, &QNetworkReply::finished, &loop1, &QEventLoop::quit);
    connect(&timer1, &QTimer::timeout, &loop1, &QEventLoop::quit);
    timer1.start(2000); // Krótszy timeout - 2 sekundy
    loop1.exec();

    bool apiOk = reply1->error() == QNetworkReply::NoError && timer1.isActive();
    reply1->deleteLater();

    // Jeśli główne API działa, to mamy połączenie
    if (apiOk) {
        return true;
    }

    // Jeśli główne API nie działa, sprawdź backup
    QNetworkRequest request2(url2);
    QNetworkReply *reply2 = m_networkManager->get(request2);
    QEventLoop loop2;
    QTimer timer2;
    timer2.setSingleShot(true);
    connect(reply2, &QNetworkReply::finished, &loop2, &QEventLoop::quit);
    connect(&timer2, &QTimer::timeout, &loop2, &QEventLoop::quit);
    timer2.start(2000); // Krótszy timeout
    loop2.exec();

    bool backupOk = reply2->error() == QNetworkReply::NoError && timer2.isActive();
    reply2->deleteLater();

    // Jeśli chociaż jeden serwer odpowiada, to uznajemy że jest internet
    return apiOk || backupOk;
}

void MainWindow::checkInternetConnectionInitial() {
    m_status = "⏳ Sprawdzanie połączenia...";
    emit statusChanged();

    // Sprawdź czy jest internet
    m_isOnline = isOnline();

    if (m_isOnline) {
        m_status = "🔗 Połączono z siecią – pobieranie danych...";
        emit statusChanged();

        // Użyj podstawowej metody pobrania stacji

        saveTestDataForAllStations();
    } else {
        m_status = "❌ Brak internetu – dane z pamięci lokalnej";
        emit statusChanged();
        loadDataFromFile();
    }
}

void MainWindow::checkInternetConnection() {
    bool currentOnlineStatus = isOnline();

    // Jeśli status połączenia się zmienił
    if (currentOnlineStatus != m_isOnline) {
        m_isOnline = currentOnlineStatus;

        if (m_isOnline) {
            // Przejście z offline do online
            m_status = "🔗 Połączono z siecią – odświeżanie danych...";
            emit statusChanged();

            // Załaduj dane z API w zależności od aktualnego widoku
            if (m_chartViewVisible && !m_selectedSensor.isEmpty()) {
                // Jesteśmy w widoku wykresu, odśwież dane czujnika
                int sensorId = m_selectedSensor["id"].toInt();
                m_networkManager->get(QNetworkRequest(QUrl("https://api.gios.gov.pl/pjp-api/rest/data/getData/" + QString::number(sensorId))));
            }
            else if (m_sensorViewVisible && !m_selectedStation.isEmpty()) {
                // Jesteśmy w widoku czujników, odśwież dane stacji
                int stationId = m_selectedStation["stationId"].toInt();
                m_networkManager->get(QNetworkRequest(QUrl("https://api.gios.gov.pl/pjp-api/rest/station/sensors/" + QString::number(stationId))));
                m_networkManager->get(QNetworkRequest(QUrl("https://api.gios.gov.pl/pjp-api/rest/aqindex/getIndex/" + QString::number(stationId))));
            }
            else {
                // Jesteśmy w widoku stacji, odśwież listę stacji
                fetchStations();
            }
        } else {
            // Przejście z online do offline
            m_status = "🔴 Utracono połączenie – tryb offline";
            emit statusChanged();

            // Wczytaj dane z pliku lokalnego
            loadDataFromFile();
        }
    }
}

void MainWindow::loadDataFromFile() {
    qDebug() << "-------------------------------------------------------------------------";
    qDebug() << "Wczytywanie danych z pliku lokalnego";

    // Najpierw zdiagnozuj plik
    debugJsonFile();

    QFile file("data/data.json");
    if (!file.exists()) {
        qWarning() << "⚠️ Plik data.json nie istnieje!";
        m_status = "⚠️ Brak zapisanych danych lokalnych";
        emit statusChanged();
        return;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "⚠️ Nie można otworzyć pliku data.json:" << file.errorString();
        m_status = "⚠️ Brak dostępu do danych lokalnych";
        emit statusChanged();
        return;
    }

    QByteArray rawData = file.readAll();
    file.close();

    if (rawData.isEmpty()) {
        qWarning() << "⚠️ Plik data.json jest pusty!";
        m_status = "⚠️ Plik danych jest pusty";
        emit statusChanged();
        return;
    }

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(rawData, &error);
    if (error.error != QJsonParseError::NoError) {
        qWarning() << "⚠️ Błąd parsowania JSON:" << error.errorString();
        m_status = "⚠️ Uszkodzone dane lokalne";
        emit statusChanged();
        return;
    }

    QVariantMap data = doc.toVariant().toMap();
    qDebug() << "Wczytano dane z pliku, znaleziono klucze:" << data.keys();

    // Podstawowe dane stacji
    if (data.contains("stations")) {
        QVariant stationsData = data["stations"];

        if (stationsData.canConvert<QVariantList>()) {
            m_allStations = stationsData.toList();
            m_filteredStations = m_allStations;
            qDebug() << "✅ Wczytano" << m_allStations.size() << "stacji z pliku lokalnego";
            emit stationsChanged();
        } else {
            qWarning() << "⚠️ Dane stacji w pliku nie są listą!";
        }
    } else {
        qWarning() << "⚠️ Brak listy stacji w pliku lokalnym";
    }

    // Czas ostatniej aktualizacji
    if (data.contains("timestamp")) {
        m_lastUpdate = data["timestamp"].toString();
        emit lastUpdateChanged();
    }

    // Status
    m_status = "📋 Dane z pamięci lokalnej (" + QDateTime::currentDateTime().toString("HH:mm:ss") + ")";
    emit statusChanged();
}

void MainWindow::fetchStations() {
    if (m_isOnline) {
        m_status = "⏳ Pobieranie stacji pomiarowych...";
        emit statusChanged();
        m_networkManager->get(QNetworkRequest(QUrl("https://api.gios.gov.pl/pjp-api/rest/station/findAll")));
    } else {
        m_status = "📋 Lista stacji (offline)";
        emit statusChanged();
        loadDataFromFile();
    }
}

void MainWindow::fetchSensors(int stationId) {
    QString stationIdStr = QString::number(stationId);
    qDebug() << "-------------------------------------------------------------------------";
    qDebug() << "Pobieranie czujników dla stacji ID:" << stationIdStr;

    if (m_isOnline) {
        m_status = "⏳ Pobieranie czujników dla stacji...";
        emit statusChanged();
        m_networkManager->get(QNetworkRequest(QUrl("https://api.gios.gov.pl/pjp-api/rest/station/sensors/" + stationIdStr)));
        m_networkManager->get(QNetworkRequest(QUrl("https://api.gios.gov.pl/pjp-api/rest/aqindex/getIndex/" + stationIdStr)));
    } else {
        m_status = "📋 Lista czujników (offline)";
        emit statusChanged();

        // Sprawdź i zdiagnozuj plik data.json
        debugJsonFile();

        // Wczytaj dane z pliku JSON
        QFile file("data/data.json");
        if (!file.exists()) {
            qWarning() << "⚠️ Plik data.json nie istnieje!";
            m_status = "⚠️ Brak zapisanych danych lokalnych";
            emit statusChanged();
            return;
        }

        if (!file.open(QIODevice::ReadOnly)) {
            qWarning() << "⚠️ Nie można otworzyć pliku data.json:" << file.errorString();
            m_status = "⚠️ Brak dostępu do danych lokalnych";
            emit statusChanged();
            return;
        }

        QByteArray rawData = file.readAll();
        file.close();

        if (rawData.isEmpty()) {
            qWarning() << "⚠️ Plik data.json jest pusty!";
            m_status = "⚠️ Plik danych jest pusty";
            emit statusChanged();
            return;
        }

        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(rawData, &error);
        if (error.error != QJsonParseError::NoError) {
            qWarning() << "⚠️ Błąd parsowania JSON:" << error.errorString();
            m_status = "⚠️ Błąd wczytywania danych lokalnych";
            emit statusChanged();
            return;
        }

        QVariantMap data = doc.toVariant().toMap();
        qDebug() << "Wczytano dane z pliku, znaleziono klucze:" << data.keys();

        // Wczytaj czujniki
        if (data.contains("sensors")) {
            QVariantMap sensorsMap = data["sensors"].toMap();
            qDebug() << "Klucze stacji w sensors:" << sensorsMap.keys();

            if (sensorsMap.contains(stationIdStr)) {
                QVariant sensorsData = sensorsMap[stationIdStr];

                if (sensorsData.canConvert<QVariantList>()) {
                    m_sensors = sensorsData.toList();
                    qDebug() << "✅ Wczytano" << m_sensors.size() << "czujników dla stacji" << stationId;

                    // Dodaj brakujące lastValue z pomiarów sensor_XXX.json
                    for (int i = 0; i < m_sensors.size(); ++i) {
                        QVariantMap sensor = m_sensors[i].toMap();
                        int sensorId = sensor["id"].toInt();

                        QString filename = "data/sensor_" + QString::number(sensorId) + ".json";
                        QVariantList data = Storage::loadSensorDataFromFile(filename);

                        if (!data.isEmpty()) {
                            QVariantMap firstEntry = data.first().toMap();
                            sensor["lastValue"] = firstEntry["value"];
                            sensor["lastDate"] = firstEntry["date"];
                        } else {
                            sensor["lastValue"] = "Brak danych";
                            sensor["lastDate"] = "";
                        }

                        m_sensors[i] = sensor;
                    }


                    // Debug pierwszego czujnika jeśli istnieje
                    if (!m_sensors.isEmpty()) {
                        qDebug() << "Pierwszy czujnik zawiera klucze:" << m_sensors.first().toMap().keys();
                    }
                } else {
                    qWarning() << "⚠️ Dane czujników dla stacji" << stationId << "nie są listą!";
                    m_sensors.clear();
                }
            } else {
                qWarning() << "⚠️ Brak danych czujników dla stacji" << stationId << "w pliku";
                m_sensors.clear();
            }
            emit sensorsChanged();
        } else {
            qWarning() << "⚠️ Brak sekcji sensors w pliku data.json";
            m_sensors.clear();
            emit sensorsChanged();
        }

        // Wczytaj indeks
        if (data.contains("indices")) {
            QVariantMap indicesMap = data["indices"].toMap();
            qDebug() << "Klucze stacji w indices:" << indicesMap.keys();

            if (indicesMap.contains(stationIdStr)) {
                QVariant indexData = indicesMap[stationIdStr];

                if (indexData.canConvert<QVariantMap>()) {
                    m_airQualityIndex = indexData.toMap();
                    qDebug() << "✅ Wczytano indeks dla stacji" << stationId << "z kluczami:" << m_airQualityIndex.keys();
                } else {
                    qWarning() << "⚠️ Dane indeksu dla stacji" << stationId << "nie są mapą!";
                    m_airQualityIndex = QVariantMap();
                    m_airQualityIndex["stIndexLevel"] = "Brak danych (offline)";
                    m_airQualityIndex["stIndexLevelId"] = -1;
                }
            } else {
                qWarning() << "⚠️ Brak danych indeksu dla stacji" << stationId << "w pliku";
                m_airQualityIndex = QVariantMap();
                m_airQualityIndex["stIndexLevel"] = "Brak danych (offline)";
                m_airQualityIndex["stIndexLevelId"] = -1;
            }
            emit airQualityIndexChanged();
        } else {
            qWarning() << "⚠️ Brak sekcji indices w pliku data.json";
            m_airQualityIndex = QVariantMap();
            m_airQualityIndex["stIndexLevel"] = "Brak danych (offline)";
            m_airQualityIndex["stIndexLevelId"] = -1;
            emit airQualityIndexChanged();
        }

        m_sensorViewVisible = true;
        emit sensorViewVisibleChanged();
    }
}

void MainWindow::openSensorChart(QVariantMap sensor) {
    m_selectedSensor = sensor;
    emit selectedSensorChanged();

    int sensorId = sensor["id"].toInt();

    if (m_isOnline) {
        m_status = "⏳ Pobieranie danych pomiarowych...";
        emit statusChanged();
        m_networkManager->get(QNetworkRequest(QUrl("https://api.gios.gov.pl/pjp-api/rest/data/getData/" + QString::number(sensorId))));
    } else {
        // Wczytaj dane pomiarowe z pliku
        QString filename = "data/sensor_" + QString::number(sensorId) + ".json";
        QFile file(filename);
        if (file.exists() && file.open(QIODevice::ReadOnly)) {
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
            file.close();
            m_sensorMeasurements = doc.toVariant().toList();
            qDebug() << "✅ Wczytano" << m_sensorMeasurements.size() << "pomiarów dla czujnika" << sensorId;
        } else {
            qWarning() << "⚠️ Brak danych pomiarowych dla czujnika" << sensorId;
            m_sensorMeasurements.clear();
        }
        emit sensorMeasurementsChanged();

        if (m_sensorMeasurements.isEmpty()) {
            m_status = "⚠️ Brak danych dla wykresu (offline)";
        } else {
            m_status = "📊 Wykres (dane offline)";
        }
        emit statusChanged();
    }

    m_chartViewVisible = true;
    m_sensorViewVisible = false;
    emit chartViewVisibleChanged();
    emit sensorViewVisibleChanged();
}

void MainWindow::setSelectedStation(QVariantMap station) {
    m_selectedStation = station;
    emit selectedStationChanged();
}

void MainWindow::goBackToStationList() {
    m_sensorViewVisible = false;
    emit sensorViewVisibleChanged();
    m_status = m_isOnline ? "📋 Lista stacji pomiarowych" : "📋 Lista stacji (offline)";
    emit statusChanged();
}

void MainWindow::goBackToSensorList() {
    m_chartViewVisible = false;
    m_sensorViewVisible = true;
    emit chartViewVisibleChanged();
    emit sensorViewVisibleChanged();
    QString stationName = m_selectedStation["stationName"].toString();
    m_status = m_isOnline ?
                   "📋 Lista czujników dla stacji " + stationName :
                   "📋 Lista czujników dla stacji " + stationName + " (offline)";
    emit statusChanged();
}

void MainWindow::filterStationsByCity(const QString &city) {
    if (city.trimmed().isEmpty()) {
        m_filteredStations = m_allStations;
    } else {
        m_filteredStations.clear();
        for (const QVariant &station : m_allStations) {
            QVariantMap s = station.toMap();
            if (s["city"].toString().contains(city, Qt::CaseInsensitive))
                m_filteredStations.append(s);
        }
    }
    emit stationsChanged();

    if (m_filteredStations.isEmpty()) {
        m_status = "❓ Nie znaleziono stacji dla miasta: " + city;
    } else {
        m_status = "✅ Znaleziono " + QString::number(m_filteredStations.size()) + " stacji dla: " + city;
        if (!m_isOnline) {
            m_status += " (offline)";
        }
    }
    emit statusChanged();
}

void MainWindow::saveDataIfDue() {
    QTime currentTime = QTime::currentTime();

    // Zapisuj co godzinę (gdy minuty == 0)
    if (currentTime.minute() == 0) {
        qDebug() << "Wykonuję automatyczny pełny zapis danych (co godzinę)";

        if (m_isOnline) {
            // Pobierz i zapisz wszystkie dane
            saveTestDataForAllStations();
        }
    }
}

void MainWindow::saveDataToFileOnly() {
    if (m_isOnline) {
        m_status = "💾 Rozpoczynam pełny zapis danych...";
        emit statusChanged();

        // Jeśli online, odśwież dane najpierw
        fetchStations();
    } else {
        m_status = "⚠️ Nie można zaktualizować danych - brak internetu";
        emit statusChanged();
    }
}

void MainWindow::onBackgroundSaveFinished() {
    m_status = "✅ Dane zostały zapisane w tle";
    emit statusChanged();

    // Odświeżamy timestamp po zakończeniu zapisu
    m_lastUpdate = QDateTime::currentDateTime().toString(Qt::ISODate);
    emit lastUpdateChanged();

    qDebug() << "✅ Zapis danych w tle zakończony";
}

void MainWindow::onNetworkReply(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        m_status = "❗ Błąd sieci: " + reply->errorString();
        emit statusChanged();
        reply->deleteLater();
        return;
    }

    QByteArray responseData = reply->readAll();
    if (responseData.isEmpty()) {
        m_status = "❗ Otrzymano pustą odpowiedź z API";
        emit statusChanged();
        reply->deleteLater();
        return;
    }

    QUrl url = reply->url();
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(responseData, &error);

    if (error.error != QJsonParseError::NoError) {
        m_status = "❗ Błąd parsowania JSON: " + error.errorString();
        emit statusChanged();
        reply->deleteLater();
        return;
    }

    qDebug() << "Otrzymano odpowiedź z:" << url.toString();

    if (url.path().contains("/station/findAll")) {
        // Odpowiedź z listy stacji
        handleStationsReply(doc);
    }
    else if (url.path().contains("/station/sensors/")) {
        // Odpowiedź z czujników stacji
        handleSensorsReply(doc);
    }
    else if (url.path().contains("/aqindex/getIndex/")) {
        // Odpowiedź z indeksu jakości powietrza
        handleAirQualityIndexReply(doc);
    }
    else if (url.path().contains("/data/getData/")) {
        // Odpowiedź z danych pomiarowych czujnika
        int sensorId = url.path().split("/").last().toInt();
        handleSensorDataReply(doc, sensorId);
    }

    reply->deleteLater();
}

void MainWindow::handleStationsReply(const QJsonDocument &doc) {
    QJsonArray arr = doc.array();
    m_allStations.clear();

    for (const QJsonValue &val : arr) {
        QJsonObject obj = val.toObject();
        QVariantMap s;
        s["stationId"] = obj["id"].toInt();
        s["stationName"] = obj["stationName"].toString();
        s["lat"] = obj["gegrLat"].toString();
        s["lon"] = obj["gegrLon"].toString();

        if (obj.contains("city") && obj["city"].isObject()) {
            QJsonObject city = obj["city"].toObject();
            s["city"] = city["name"].toString();
        } else {
            s["city"] = "Nieznane miasto";
        }

        s["address"] = obj["addressStreet"].toString();
        m_allStations.append(s);
    }

    m_filteredStations = m_allStations;
    emit stationsChanged();

    m_status = "✅ Załadowano " + QString::number(m_allStations.size()) + " stacji pomiarowych";
    emit statusChanged();

    // Odczytaj istniejące dane z pliku
    QVariantMap fullData;
    QFile file("data/data.json");

    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        QJsonDocument existingDoc = QJsonDocument::fromJson(file.readAll());
        file.close();

        if (!existingDoc.isNull() && !existingDoc.isEmpty()) {
            fullData = existingDoc.toVariant().toMap();
            qDebug() << "Odczytano istniejące dane z pliku, znaleziono klucze:" << fullData.keys();
        } else {
            qDebug() << "Plik istnieje, ale jest pusty lub uszkodzony";
        }
    }

    // Aktualizuj tylko sekcję stacji
    fullData["stations"] = m_allStations;
    fullData["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    // Zapisz zaktualizowane dane
    QDir().mkpath("data");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument saveDoc = QJsonDocument::fromVariant(fullData);
        file.write(saveDoc.toJson());
        file.close();
        qDebug() << "💾 Zapisano" << m_allStations.size() << "stacji do pliku";

        // Wykonaj diagnostykę pliku po zapisie
        debugJsonFile();
    } else {
        qWarning() << "❗ Nie można zapisać danych do pliku:" << file.errorString();
    }

    m_lastUpdate = QDateTime::currentDateTime().toString(Qt::ISODate);
    emit lastUpdateChanged();
}

void MainWindow::handleSensorsReply(const QJsonDocument &doc) {
    QJsonArray arr = doc.array();
    m_sensors.clear();

    // Upewnij się, że mamy wybraną stację
    if (m_selectedStation.isEmpty()) {
        qWarning() << "❗ Brak wybranej stacji przy obsłudze czujników";
        return;
    }

    int stationId = m_selectedStation["stationId"].toInt();
    QString stationIdStr = QString::number(stationId);
    qDebug() << "Otrzymano dane czujników dla stacji ID:" << stationIdStr;

    for (const QJsonValue &val : arr) {
        QJsonObject obj = val.toObject();
        QVariantMap s;
        s["id"] = obj["id"].toInt();
        s["stationId"] = obj["stationId"].toInt();
        s["paramName"] = obj["param"].toObject()["paramName"].toString();
        s["paramCode"] = obj["param"].toObject()["paramCode"].toString();
        s["paramFormula"] = obj["param"].toObject()["paramFormula"].toString();
        s["lastValue"] = QVariant(QString("Ładowanie..."));
        m_sensors.append(s);

        // Pobierz dane dla każdego czujnika
        m_networkManager->get(QNetworkRequest(QUrl("https://api.gios.gov.pl/pjp-api/rest/data/getData/" + QString::number(s["id"].toInt()))));
    }

    m_sensorViewVisible = true;
    emit sensorsChanged();
    emit sensorViewVisibleChanged();

    m_status = "📋 Czujniki stacji " + m_selectedStation["stationName"].toString();
    emit statusChanged();

    // Zapisz dane czujników do pliku
    QFile dataFile("data/data.json");
    QVariantMap fullData;

    // Wczytaj istniejące dane, jeśli plik istnieje
    if (dataFile.exists() && dataFile.open(QIODevice::ReadOnly)) {
        QJsonDocument existingDoc = QJsonDocument::fromJson(dataFile.readAll());
        dataFile.close();
        fullData = existingDoc.toVariant().toMap();
    }

    // Dodaj dane stacji, jeśli ich nie ma
    if (!fullData.contains("stations")) {
        fullData["stations"] = m_allStations;
    }

    // Dodaj/aktualizuj sensory dla konkretnej stacji
    if (!fullData.contains("sensors")) {
        fullData["sensors"] = QVariantMap();
    }

    QVariantMap sensorsMap = fullData["sensors"].toMap();
    sensorsMap[stationIdStr] = m_sensors;  // Klucz to ID stacji
    fullData["sensors"] = sensorsMap;

    // Aktualizuj timestamp
    fullData["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    // Zapisz dane
    if (dataFile.open(QIODevice::WriteOnly)) {
        QJsonDocument saveDoc = QJsonDocument::fromVariant(fullData);
        dataFile.write(saveDoc.toJson());
        dataFile.close();
        qDebug() << "💾 Zapisano czujniki dla stacji" << stationIdStr;
    } else {
        qWarning() << "❗ Nie można zapisać danych do pliku";
    }
}

void MainWindow::handleAirQualityIndexReply(const QJsonDocument &doc) {
    QJsonObject obj = doc.object();
    QVariantMap index;

    // Upewnij się, że mamy wybraną stację
    if (m_selectedStation.isEmpty()) {
        qWarning() << "❗ Brak wybranej stacji przy obsłudze indeksu";
        return;
    }

    int stationId = m_selectedStation["stationId"].toInt();
    QString stationIdStr = QString::number(stationId);
    qDebug() << "Otrzymano indeks jakości powietrza dla stacji ID:" << stationIdStr;

    index["id"] = obj["id"].toInt();

    if (!obj["stIndexLevel"].isNull() && obj["stIndexLevel"].isObject()) {
        index["stIndexLevel"] = obj["stIndexLevel"].toObject()["indexLevelName"].toString();
        index["stIndexLevelId"] = obj["stIndexLevel"].toObject()["id"].toInt();
    } else {
        index["stIndexLevel"] = QVariant(QString("Brak danych"));
        index["stIndexLevelId"] = -1;
    }

    index["stSourceDataDate"] = obj["stSourceDataDate"].toString();
    index["stCalcDate"] = obj["stCalcDate"].toString();

    m_airQualityIndex = index;
    emit airQualityIndexChanged();

    // Zapisz indeks do pliku
    QFile dataFile("data/data.json");
    QVariantMap fullData;

    // Wczytaj istniejące dane, jeśli plik istnieje
    if (dataFile.exists() && dataFile.open(QIODevice::ReadOnly)) {
        QJsonDocument existingDoc = QJsonDocument::fromJson(dataFile.readAll());
        dataFile.close();
        fullData = existingDoc.toVariant().toMap();
    }

    // Dodaj dane stacji, jeśli ich nie ma
    if (!fullData.contains("stations")) {
        fullData["stations"] = m_allStations;
    }

    // Dodaj/aktualizuj indeks dla konkretnej stacji
    if (!fullData.contains("indices")) {
        fullData["indices"] = QVariantMap();
    }

    QVariantMap indicesMap = fullData["indices"].toMap();
    indicesMap[stationIdStr] = index;  // Klucz to ID stacji
    fullData["indices"] = indicesMap;

    // Aktualizuj timestamp
    fullData["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);

    // Zapisz dane
    if (dataFile.open(QIODevice::WriteOnly)) {
        QJsonDocument saveDoc = QJsonDocument::fromVariant(fullData);
        dataFile.write(saveDoc.toJson());
        dataFile.close();
        qDebug() << "💾 Zapisano indeks dla stacji" << stationIdStr;
    } else {
        qWarning() << "❗ Nie można zapisać danych do pliku";
    }
}

void MainWindow::handleSensorDataReply(const QJsonDocument &doc, int sensorId) {
    QJsonObject obj = doc.object();
    QJsonArray values = obj["values"].toArray();

    QVariantList parsed;
    double latestValue = -1;
    QString latestDate;

    for (const auto &val : values) {
        QJsonObject o = val.toObject();
        if (!o["value"].isNull()) {
            QVariantMap m;
            m["value"] = o["value"].toDouble();
            m["date"] = o["date"].toString();
            parsed.append(m);

            if (latestValue == -1) {
                latestValue = o["value"].toDouble();
                latestDate = o["date"].toString();
            }
        }
    }

    // Zapisz dane pomiarowe do pliku czujnika
    QString filename = "data/sensor_" + QString::number(sensorId) + ".json";
    QDir().mkpath("data");
    QFile sensorFile(filename);
    if (sensorFile.open(QIODevice::WriteOnly)) {
        QJsonDocument sensorDoc = QJsonDocument::fromVariant(parsed);
        sensorFile.write(sensorDoc.toJson());
        sensorFile.close();
        qDebug() << "💾 Zapisano dane pomiarowe dla czujnika" << sensorId;
    }

    // Aktualizuj wartości w liście czujników
    bool updated = false;
    for (int i = 0; i < m_sensors.size(); ++i) {
        QVariantMap sensor = m_sensors[i].toMap();
        if (sensor["id"].toInt() == sensorId) {
            if (latestValue >= 0) {
                sensor["lastValue"] = QVariant(latestValue);
                sensor["lastDate"] = latestDate;
            } else {
                sensor["lastValue"] = QVariant(QString("Brak danych"));
                sensor["lastDate"] = QVariant(QString(""));
            }
            m_sensors[i] = sensor;
            updated = true;
            break;
        }
    }

    if (updated) {
        emit sensorsChanged();

        // Aktualizuj dane w pliku głównym
        if (!m_selectedStation.isEmpty()) {
            int stationId = m_selectedStation["stationId"].toInt();
            QString stationIdStr = QString::number(stationId);

            QFile dataFile("data/data.json");
            QVariantMap fullData;

            // Wczytaj istniejące dane
            if (dataFile.exists() && dataFile.open(QIODevice::ReadOnly)) {
                QJsonDocument existingDoc = QJsonDocument::fromJson(dataFile.readAll());
                dataFile.close();
                fullData = existingDoc.toVariant().toMap();

                // Aktualizuj dane czujników
                if (fullData.contains("sensors")) {
                    QVariantMap sensorsMap = fullData["sensors"].toMap();
                    sensorsMap[stationIdStr] = m_sensors;
                    fullData["sensors"] = sensorsMap;

                    // Zapisz zaktualizowane dane
                    if (dataFile.open(QIODevice::WriteOnly)) {
                        QJsonDocument saveDoc = QJsonDocument::fromVariant(fullData);
                        dataFile.write(saveDoc.toJson());
                        dataFile.close();
                        qDebug() << "💾 Zaktualizowano dane czujnika w pliku głównym";
                    }
                }
            }
        }
    }

    // Jeśli jesteśmy w widoku wykresu dla tego czujnika
    if (m_chartViewVisible && m_selectedSensor["id"].toInt() == sensorId) {
        m_sensorMeasurements = parsed;
        emit sensorMeasurementsChanged();
        m_status = "📊 Wykres dla " + m_selectedSensor["paramName"].toString();
        emit statusChanged();
    }
}

void MainWindow::saveTestDataForAllStations() {
    if (!m_isOnline) {
        m_status = "❌ Brak internetu – nie można zapisać";
        emit statusChanged();
        return;
    }

    if (m_dataWorker) {
        m_status = "⏳ Zapis już trwa...";
        emit statusChanged();
        return;
    }

    m_status = "💾 Trwa zapisywanie wszystkich danych w tle...";
    emit statusChanged();

    m_workerThread = new QThread(this);
    m_dataWorker = new DataSaverWorker();
    m_dataWorker->moveToThread(m_workerThread);

    connect(m_workerThread, &QThread::started, m_dataWorker, &DataSaverWorker::startSaving);
    connect(m_dataWorker, &DataSaverWorker::savingFinished, this, &MainWindow::onBackgroundSaveFinished);
    connect(m_dataWorker, &DataSaverWorker::savingFinished, m_workerThread, &QThread::quit);
    connect(m_workerThread, &QThread::finished, m_dataWorker, &QObject::deleteLater);
    connect(m_workerThread, &QThread::finished, m_workerThread, &QObject::deleteLater);
    connect(m_workerThread, &QThread::finished, this, [this]() {
        m_dataWorker = nullptr;
        m_workerThread = nullptr;
    });

    m_workerThread->start();
}


void MainWindow::debugJsonFile() {
    QFile file("data/data.json");

    if (!file.exists()) {
        qDebug() << "⚠️ DIAGNOZA: Plik data.json nie istnieje!";
        return;
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "⚠️ DIAGNOZA: Nie można otworzyć pliku data.json:" << file.errorString();
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    if (data.isEmpty()) {
        qDebug() << "⚠️ DIAGNOZA: Plik data.json jest pusty!";
        return;
    }

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);

    if (error.error != QJsonParseError::NoError) {
        qDebug() << "⚠️ DIAGNOZA: Błąd parsowania JSON:" << error.errorString();
        return;
    }

    QVariantMap contentMap = doc.toVariant().toMap();
    qDebug() << "DIAGNOZA: Klucze główne w pliku:" << contentMap.keys();

    if (contentMap.contains("stations")) {
        QVariantList stations = contentMap["stations"].toList();
        qDebug() << "DIAGNOZA: Liczba stacji:" << stations.size();
    } else {
        qDebug() << "⚠️ DIAGNOZA: Brak klucza 'stations' w pliku!";
    }

    if (contentMap.contains("sensors")) {
        QVariantMap sensorsMap = contentMap["sensors"].toMap();
        qDebug() << "DIAGNOZA: Klucze stacji w 'sensors':" << sensorsMap.keys();

        // Sprawdź zawartość dla pierwszej stacji
        if (!sensorsMap.isEmpty()) {
            QString firstStationId = sensorsMap.keys().first();
            QVariantList sensorsList = sensorsMap[firstStationId].toList();
            qDebug() << "DIAGNOZA: Stacja" << firstStationId << "ma" << sensorsList.size() << "czujników";

            if (!sensorsList.isEmpty()) {
                QVariantMap firstSensor = sensorsList.first().toMap();
                qDebug() << "DIAGNOZA: Pierwszy czujnik zawiera klucze:" << firstSensor.keys();
            }
        }
    } else {
        qDebug() << "⚠️ DIAGNOZA: Brak klucza 'sensors' w pliku!";
    }

    if (contentMap.contains("indices")) {
        QVariantMap indicesMap = contentMap["indices"].toMap();
        qDebug() << "DIAGNOZA: Klucze stacji w 'indices':" << indicesMap.keys();

        // Sprawdź zawartość dla pierwszej stacji
        if (!indicesMap.isEmpty()) {
            QString firstStationId = indicesMap.keys().first();
            QVariantMap indexData = indicesMap[firstStationId].toMap();
            qDebug() << "DIAGNOZA: Indeks dla stacji" << firstStationId << "zawiera klucze:" << indexData.keys();
        }
    } else {
        qDebug() << "⚠️ DIAGNOZA: Brak klucza 'indices' w pliku!";
    }
}





