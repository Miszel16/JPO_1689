#include "test_storage.h"
#include <QtTest>
#include "../storage.h"

void StorageTest::testSaveAndLoad() {
    QVariantMap data;
    data["abc"] = 123;

    Storage::saveFullData(data, "data/test_data.json");
    QVariantMap loaded = Storage::loadFullData("data/test_data.json");

    QVERIFY(loaded.contains("abc"));
    QCOMPARE(loaded["abc"].toInt(), 123);
}
