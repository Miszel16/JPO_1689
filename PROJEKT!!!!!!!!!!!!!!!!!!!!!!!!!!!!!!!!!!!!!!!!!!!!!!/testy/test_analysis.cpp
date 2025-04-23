#include "test_analysis.h"
#include <QtTest>
#include "../analysis.h"

void AnalysisTest::testStats() {
    QVariantList values;
    values.append(QVariantMap{{"value", 1.0}, {"date", "2024-04-01T10:00"}});
    values.append(QVariantMap{{"value", 5.0}, {"date", "2024-04-01T11:00"}});
    values.append(QVariantMap{{"value", 3.0}, {"date", "2024-04-01T12:00"}});

    QVariantMap result = Analysis::calculateStats(values);

    // Diagnoza – co zwróciła funkcja
    qDebug() << "Zawartość result:" << result;

    // Sprawdzamy zgodnie z rzeczywistymi kluczami
    QVERIFY(result.contains("min"));
    QVERIFY(result.contains("max"));
    QVERIFY(result.contains("avg"));

    QVERIFY(result["min"].canConvert<double>());
    QVERIFY(result["max"].canConvert<double>());
    QVERIFY(result["avg"].canConvert<double>());
}
