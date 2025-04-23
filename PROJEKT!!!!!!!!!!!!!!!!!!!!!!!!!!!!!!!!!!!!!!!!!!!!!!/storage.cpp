#include "storage.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QDir>

void Storage::saveFullData(const QVariantMap &data, const QString &filename) {
    // Utwórz katalog nadrzędny, jeśli nie istnieje
    QFileInfo fileInfo(filename);
    QDir dir = fileInfo.dir();
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc = QJsonDocument::fromVariant(data);
        file.write(doc.toJson());
        file.close();
        qDebug() << "💾 Zapisano pełne dane do pliku:" << filename;
    } else {
        qWarning() << "❗ Nie można zapisać danych do:" << filename;
    }
}

QVariantMap Storage::loadFullData(const QString &filename) {
    QFile file(filename);
    if (!file.exists()) {
        qWarning() << "⚠️ Plik danych nie istnieje:" << filename;
        return {};
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "⚠️ Nie można otworzyć pliku danych:" << filename;
        return {};
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "⚠️ Błąd parsowania JSON:" << parseError.errorString();
        return {};
    }

    return doc.toVariant().toMap();
}

void Storage::saveSensorDataToFile(const QVariantList &data, const QString &filename) {
    // Utwórz katalog nadrzędny, jeśli nie istnieje
    QFileInfo fileInfo(filename);
    QDir dir = fileInfo.dir();
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc = QJsonDocument::fromVariant(data);
        file.write(doc.toJson());
        file.close();
        qDebug() << "💾 Zapisano dane czujnika do:" << filename;
    } else {
        qWarning() << "❗ Nie można zapisać czujnika do:" << filename;
    }
}

QVariantList Storage::loadSensorDataFromFile(const QString &filename) {
    QFile file(filename);
    if (!file.exists()) {
        qWarning() << "⚠️ Plik czujnika nie istnieje:" << filename;
        return {};
    }

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "⚠️ Brak danych czujnika w pliku:" << filename;
        return {};
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "⚠️ Błąd parsowania JSON czujnika:" << parseError.errorString();
        return {};
    }

    return doc.toVariant().toList();
}
