#pragma once

#include <QVariant>

class Storage {
public:
    static void saveFullData(const QVariantMap &data, const QString &filename);
    static QVariantMap loadFullData(const QString &filename);

    static void saveSensorDataToFile(const QVariantList &data, const QString &filename);
    static QVariantList loadSensorDataFromFile(const QString &filename);
};
