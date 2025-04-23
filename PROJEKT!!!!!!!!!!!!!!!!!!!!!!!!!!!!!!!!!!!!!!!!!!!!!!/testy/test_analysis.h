#pragma once
#include <QObject>

class AnalysisTest : public QObject {
    Q_OBJECT

private slots:
    void testStats();
};
