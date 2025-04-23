#include <QtTest>
#include "test_storage.h"
#include "test_analysis.h"

int main(int argc, char *argv[]) {
    int status = 0;

    {
        StorageTest tc;
        status |= QTest::qExec(&tc, argc, argv);
    }

    {
        AnalysisTest tc;
        status |= QTest::qExec(&tc, argc, argv);
    }

    return status;
}
