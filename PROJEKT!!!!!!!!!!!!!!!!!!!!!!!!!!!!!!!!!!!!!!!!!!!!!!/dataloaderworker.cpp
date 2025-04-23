#include "dataloaderworker.h"
#include "storage.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QDebug>
#include <QDateTime>
#include <QDir>

DataSaverWorker::DataSaverWorker(QObject *parent)
    : QObject(parent)
{}

void DataSaverWorker::startSaving() {
    QNetworkAccessManager manager; // <-- WAŻNE: lokalny manager we własnym wątku!

    qDebug() << "💾 Rozpoczynam zapis danych w tle...";

    try {
        QVariantList allStations = fetchStations(&manager);
        QVariantMap sensorsMap;
        QVariantMap indexMap;

        int totalStations = allStations.size();
        int progress = 0;

        for (const QVariant &station : allStations) {
            QVariantMap stationMap = station.toMap();
            int stationId = stationMap["stationId"].toInt();
            QString stationIdStr = QString::number(stationId);

            progress++;
            qDebug() << "Zapisywanie danych stacji" << stationIdStr << "(" << progress << "/" << totalStations << ")";

            // Czujniki
            QVariantList sensors = fetchSensors(stationId, &manager);
            sensorsMap[stationIdStr] = sensors;

            // Pomiary czujników
            for (const QVariant &sensorVar : sensors) {
                QVariantMap sensor = sensorVar.toMap();
                int sensorId = sensor["id"].toInt();

                QVariantList sensorData = fetchSensorData(sensorId, &manager);
                QString filename = "data/sensor_" + QString::number(sensorId) + ".json";
                Storage::saveSensorDataToFile(sensorData, filename);
            }

            // Indeks jakości
            QVariantMap index = fetchAirQualityIndex(stationId, &manager);
            indexMap[stationIdStr] = index;
        }

        QVariantMap fullData;
        fullData["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
        fullData["stations"] = allStations;
        fullData["sensors"] = sensorsMap;
        fullData["indices"] = indexMap;

        QDir().mkpath("data");
        Storage::saveFullData(fullData, "data/data.json");

        qDebug() << "✅ Dane zostały zapisane w tle.";
    }
    catch (const std::exception &e) {
        qWarning() << "❌ Błąd podczas zapisu danych:" << e.what();
    }

    emit savingFinished();
}

QVariantList DataSaverWorker::fetchStations(QNetworkAccessManager *manager) {
    QUrl url("https://api.gios.gov.pl/pjp-api/rest/station/findAll");
    QNetworkReply *reply = manager->get(QNetworkRequest(url));
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "❌ Błąd pobierania stacji:" << reply->errorString();
        reply->deleteLater();
        return {};
    }

    QJsonArray arr = QJsonDocument::fromJson(reply->readAll()).array();
    reply->deleteLater();

    QVariantList result;
    for (const auto &val : arr) {
        QJsonObject obj = val.toObject();
        QVariantMap s;
        s["stationId"] = obj["id"].toInt();
        s["stationName"] = obj["stationName"].toString();
        s["lat"] = obj["gegrLat"].toString();
        s["lon"] = obj["gegrLon"].toString();
        s["city"] = obj["city"].toObject()["name"].toString();
        s["address"] = obj["addressStreet"].toString();
        result.append(s);
    }
    return result;
}

QVariantList DataSaverWorker::fetchSensors(int stationId, QNetworkAccessManager *manager) {
    QUrl url("https://api.gios.gov.pl/pjp-api/rest/station/sensors/" + QString::number(stationId));
    QNetworkReply *reply = manager->get(QNetworkRequest(url));
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "❌ Błąd pobierania czujników dla stacji" << stationId << ":" << reply->errorString();
        reply->deleteLater();
        return {};
    }

    QJsonArray arr = QJsonDocument::fromJson(reply->readAll()).array();
    reply->deleteLater();

    QVariantList result;
    for (const auto &val : arr) {
        QJsonObject obj = val.toObject();
        QVariantMap sensor;
        sensor["id"] = obj["id"].toInt();
        sensor["stationId"] = obj["stationId"].toInt();
        sensor["paramName"] = obj["param"].toObject()["paramName"].toString();
        sensor["paramCode"] = obj["param"].toObject()["paramCode"].toString();
        sensor["paramFormula"] = obj["param"].toObject()["paramFormula"].toString();
        result.append(sensor);
    }
    return result;
}

QVariantList DataSaverWorker::fetchSensorData(int sensorId, QNetworkAccessManager *manager) {
    QUrl url("https://api.gios.gov.pl/pjp-api/rest/data/getData/" + QString::number(sensorId));
    QNetworkReply *reply = manager->get(QNetworkRequest(url));
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "❌ Błąd pobierania danych dla czujnika" << sensorId << ":" << reply->errorString();
        reply->deleteLater();
        return {};
    }

    QJsonObject response = QJsonDocument::fromJson(reply->readAll()).object();
    QJsonArray arr = response["values"].toArray();
    reply->deleteLater();

    QVariantList data;
    for (const QJsonValue &val : arr) {
        QJsonObject o = val.toObject();
        if (!o["value"].isNull()) {
            QVariantMap entry;
            entry["value"] = o["value"].toDouble();
            entry["date"] = o["date"].toString();
            data.append(entry);
        }
    }
    return data;
}

QVariantMap DataSaverWorker::fetchAirQualityIndex(int stationId, QNetworkAccessManager *manager) {
    QUrl url("https://api.gios.gov.pl/pjp-api/rest/aqindex/getIndex/" + QString::number(stationId));
    QNetworkReply *reply = manager->get(QNetworkRequest(url));
    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::NoError) {
        qWarning() << "❌ Błąd pobierania indeksu dla stacji" << stationId << ":" << reply->errorString();
        reply->deleteLater();
        return {};
    }

    QJsonObject idx = QJsonDocument::fromJson(reply->readAll()).object();
    reply->deleteLater();

    QVariantMap idxMap;
    idxMap["id"] = idx["id"].toInt();

    if (!idx["stIndexLevel"].isNull() && idx["stIndexLevel"].isObject()) {
        idxMap["stIndexLevel"] = idx["stIndexLevel"].toObject()["indexLevelName"].toString();
        idxMap["stIndexLevelId"] = idx["stIndexLevel"].toObject()["id"].toInt();
    } else {
        idxMap["stIndexLevel"] = "Brak danych";
        idxMap["stIndexLevelId"] = -1;
    }

    idxMap["stSourceDataDate"] = idx["stSourceDataDate"].toString();
    idxMap["stCalcDate"] = idx["stCalcDate"].toString();
    return idxMap;
}
