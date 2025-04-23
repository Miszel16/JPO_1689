#pragma once
#include <QVariant>

class Analysis {
public:
    static QVariantMap calculateStats(const QVariantList &data);
};
