#pragma once

#include <QObject>
#include <QVariantMap>
#include <QVariantList>
#include <QNetworkAccessManager>

class DataSaverWorker : public QObject {
    Q_OBJECT
public:
    explicit DataSaverWorker(QObject *parent = nullptr);

public slots:
    void startSaving();  // Główna metoda uruchamiana w wątku

signals:
    void savingFinished();
    void progressUpdate(int current, int total);

private:
    // WSZYSTKIE przyjmują wskaźnik do lokalnego QNetworkAccessManager
    QVariantList fetchStations(QNetworkAccessManager *manager);
    QVariantList fetchSensors(int stationId, QNetworkAccessManager *manager);
    QVariantList fetchSensorData(int sensorId, QNetworkAccessManager *manager);
    QVariantMap fetchAirQualityIndex(int stationId, QNetworkAccessManager *manager);
};
