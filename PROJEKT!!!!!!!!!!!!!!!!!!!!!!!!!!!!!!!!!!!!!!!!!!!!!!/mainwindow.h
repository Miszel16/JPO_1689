#pragma once

#include <QObject>
#include <QVariant>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>

#include "dataloaderworker.h"
class MainWindow : public QObject {
    Q_OBJECT

public:
    QThread *m_workerThread = nullptr;
    DataSaverWorker *m_dataWorker = nullptr;
    explicit MainWindow(QObject *parent = nullptr);
    ~MainWindow();

    // Metody wywoływane z QML
    Q_INVOKABLE void fetchStations();
    Q_INVOKABLE void fetchSensors(int stationId);
    Q_INVOKABLE void openSensorChart(QVariantMap sensor);
    Q_INVOKABLE void goBackToStationList();
    Q_INVOKABLE void goBackToSensorList();
    Q_INVOKABLE void filterStationsByCity(const QString &city);
    Q_INVOKABLE void saveDataToFileOnly();
    Q_INVOKABLE void setSelectedStation(QVariantMap station);
    Q_INVOKABLE void saveTestDataForAllStations(); // Nowa metoda

    // Właściwości dostępne z QML
    Q_PROPERTY(QVariantList stations READ stations NOTIFY stationsChanged)
    Q_PROPERTY(QVariantList sensors READ sensors NOTIFY sensorsChanged)
    Q_PROPERTY(QVariantList sensorMeasurements READ sensorMeasurements NOTIFY sensorMeasurementsChanged)
    Q_PROPERTY(QVariantMap selectedStation READ selectedStation NOTIFY selectedStationChanged)
    Q_PROPERTY(QVariantMap selectedSensor READ selectedSensor NOTIFY selectedSensorChanged)
    Q_PROPERTY(QVariantMap airQualityIndex READ airQualityIndex NOTIFY airQualityIndexChanged)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    Q_PROPERTY(bool sensorViewVisible READ sensorViewVisible NOTIFY sensorViewVisibleChanged)
    Q_PROPERTY(bool chartViewVisible READ chartViewVisible NOTIFY chartViewVisibleChanged)
    Q_PROPERTY(QString lastUpdate READ lastUpdate NOTIFY lastUpdateChanged)

signals:
    void stationsChanged();
    void sensorsChanged();
    void sensorMeasurementsChanged();
    void selectedStationChanged();
    void selectedSensorChanged();
    void airQualityIndexChanged();
    void statusChanged();
    void sensorViewVisibleChanged();
    void chartViewVisibleChanged();
    void lastUpdateChanged();

private slots:
    void onNetworkReply(QNetworkReply *reply);
    void checkInternetConnection();
    void saveDataIfDue();
    void onBackgroundSaveFinished();

private:
    QNetworkAccessManager *m_networkManager;
    QTimer *hourlySaveTimer;
    QTimer *internetCheckTimer;

    QVariantList m_allStations;
    QVariantList m_filteredStations;
    QVariantList m_sensors;
    QVariantList m_sensorMeasurements;
    QVariantMap m_selectedStation;
    QVariantMap m_selectedSensor;
    QVariantMap m_airQualityIndex;

    QString m_status;
    QString m_lastUpdate;
    bool m_sensorViewVisible;
    bool m_chartViewVisible;
    bool m_isOnline;  // Flaga wskazująca czy mamy połączenie z internetem

    // Pomocnicze metody prywatne
    bool isOnline();
    void loadDataFromFile();
    void checkInternetConnectionInitial();
    void debugJsonFile(); // Nowa metoda diagnostyczna

    // Metody obsługi odpowiedzi z API
    void handleStationsReply(const QJsonDocument &doc);
    void handleSensorsReply(const QJsonDocument &doc);
    void handleAirQualityIndexReply(const QJsonDocument &doc);
    void handleSensorDataReply(const QJsonDocument &doc, int sensorId);

    // Gettery dla właściwości QML
    QVariantList stations() const { return m_filteredStations; }
    QVariantList sensors() const { return m_sensors; }
    QVariantList sensorMeasurements() const { return m_sensorMeasurements; }
    QVariantMap selectedStation() const { return m_selectedStation; }
    QVariantMap selectedSensor() const { return m_selectedSensor; }
    QVariantMap airQualityIndex() const { return m_airQualityIndex; }
    QString status() const { return m_status; }
    QString lastUpdate() const { return m_lastUpdate; }
    bool sensorViewVisible() const { return m_sensorViewVisible; }
    bool chartViewVisible() const { return m_chartViewVisible; }
};
