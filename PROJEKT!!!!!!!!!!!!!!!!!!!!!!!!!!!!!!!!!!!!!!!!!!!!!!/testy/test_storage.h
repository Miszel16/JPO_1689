#pragma once
#include <QObject>

class StorageTest : public QObject {
    Q_OBJECT

private slots:
    void testSaveAndLoad();
};
