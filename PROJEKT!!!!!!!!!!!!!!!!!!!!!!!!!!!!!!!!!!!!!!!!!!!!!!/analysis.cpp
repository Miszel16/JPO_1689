#include "analysis.h"

QVariantMap Analysis::calculateStats(const QVariantList &data) {
    if (data.isEmpty()) return {};

    double minVal = data[0].toMap()["value"].toDouble();
    double maxVal = minVal;
    double sum = 0;
    QString minDate = data[0].toMap()["date"].toString();
    QString maxDate = minDate;

    for (const QVariant &entry : data) {
        QVariantMap m = entry.toMap();
        double val = m["value"].toDouble();
        QString date = m["date"].toString();

        if (val < minVal) {
            minVal = val;
            minDate = date;
        }
        if (val > maxVal) {
            maxVal = val;
            maxDate = date;
        }
        sum += val;
    }

    QVariantMap stats;
    stats["min"] = minVal;
    stats["max"] = maxVal;
    stats["avg"] = sum / data.size();
    stats["minDate"] = minDate;
    stats["maxDate"] = maxDate;
    return stats;
}
