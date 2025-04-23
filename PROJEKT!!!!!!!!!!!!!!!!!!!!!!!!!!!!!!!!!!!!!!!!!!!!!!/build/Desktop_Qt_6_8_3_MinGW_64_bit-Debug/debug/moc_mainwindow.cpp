/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../mainwindow.h"
#include <QtNetwork/QSslError>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN10MainWindowE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN10MainWindowE = QtMocHelpers::stringData(
    "MainWindow",
    "stationsChanged",
    "",
    "sensorsChanged",
    "sensorMeasurementsChanged",
    "selectedStationChanged",
    "selectedSensorChanged",
    "airQualityIndexChanged",
    "statusChanged",
    "sensorViewVisibleChanged",
    "chartViewVisibleChanged",
    "lastUpdateChanged",
    "onNetworkReply",
    "QNetworkReply*",
    "reply",
    "checkInternetConnection",
    "saveDataIfDue",
    "onBackgroundSaveFinished",
    "fetchStations",
    "fetchSensors",
    "stationId",
    "openSensorChart",
    "QVariantMap",
    "sensor",
    "goBackToStationList",
    "goBackToSensorList",
    "filterStationsByCity",
    "city",
    "saveDataToFileOnly",
    "setSelectedStation",
    "station",
    "saveTestDataForAllStations",
    "stations",
    "QVariantList",
    "sensors",
    "sensorMeasurements",
    "selectedStation",
    "selectedSensor",
    "airQualityIndex",
    "status",
    "sensorViewVisible",
    "chartViewVisible",
    "lastUpdate"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN10MainWindowE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
      23,   14, // methods
      10,  185, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,  152,    2, 0x06,   11 /* Public */,
       3,    0,  153,    2, 0x06,   12 /* Public */,
       4,    0,  154,    2, 0x06,   13 /* Public */,
       5,    0,  155,    2, 0x06,   14 /* Public */,
       6,    0,  156,    2, 0x06,   15 /* Public */,
       7,    0,  157,    2, 0x06,   16 /* Public */,
       8,    0,  158,    2, 0x06,   17 /* Public */,
       9,    0,  159,    2, 0x06,   18 /* Public */,
      10,    0,  160,    2, 0x06,   19 /* Public */,
      11,    0,  161,    2, 0x06,   20 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
      12,    1,  162,    2, 0x08,   21 /* Private */,
      15,    0,  165,    2, 0x08,   23 /* Private */,
      16,    0,  166,    2, 0x08,   24 /* Private */,
      17,    0,  167,    2, 0x08,   25 /* Private */,

 // methods: name, argc, parameters, tag, flags, initial metatype offsets
      18,    0,  168,    2, 0x02,   26 /* Public */,
      19,    1,  169,    2, 0x02,   27 /* Public */,
      21,    1,  172,    2, 0x02,   29 /* Public */,
      24,    0,  175,    2, 0x02,   31 /* Public */,
      25,    0,  176,    2, 0x02,   32 /* Public */,
      26,    1,  177,    2, 0x02,   33 /* Public */,
      28,    0,  180,    2, 0x02,   35 /* Public */,
      29,    1,  181,    2, 0x02,   36 /* Public */,
      31,    0,  184,    2, 0x02,   38 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 13,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   20,
    QMetaType::Void, 0x80000000 | 22,   23,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   27,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 22,   30,
    QMetaType::Void,

 // properties: name, type, flags, notifyId, revision
      32, 0x80000000 | 33, 0x00015009, uint(0), 0,
      34, 0x80000000 | 33, 0x00015009, uint(1), 0,
      35, 0x80000000 | 33, 0x00015009, uint(2), 0,
      36, 0x80000000 | 22, 0x00015009, uint(3), 0,
      37, 0x80000000 | 22, 0x00015009, uint(4), 0,
      38, 0x80000000 | 22, 0x00015009, uint(5), 0,
      39, QMetaType::QString, 0x00015001, uint(6), 0,
      40, QMetaType::Bool, 0x00015001, uint(7), 0,
      41, QMetaType::Bool, 0x00015001, uint(8), 0,
      42, QMetaType::QString, 0x00015001, uint(9), 0,

       0        // eod
};

Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_ZN10MainWindowE.offsetsAndSizes,
    qt_meta_data_ZN10MainWindowE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN10MainWindowE_t,
        // property 'stations'
        QtPrivate::TypeAndForceComplete<QVariantList, std::true_type>,
        // property 'sensors'
        QtPrivate::TypeAndForceComplete<QVariantList, std::true_type>,
        // property 'sensorMeasurements'
        QtPrivate::TypeAndForceComplete<QVariantList, std::true_type>,
        // property 'selectedStation'
        QtPrivate::TypeAndForceComplete<QVariantMap, std::true_type>,
        // property 'selectedSensor'
        QtPrivate::TypeAndForceComplete<QVariantMap, std::true_type>,
        // property 'airQualityIndex'
        QtPrivate::TypeAndForceComplete<QVariantMap, std::true_type>,
        // property 'status'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // property 'sensorViewVisible'
        QtPrivate::TypeAndForceComplete<bool, std::true_type>,
        // property 'chartViewVisible'
        QtPrivate::TypeAndForceComplete<bool, std::true_type>,
        // property 'lastUpdate'
        QtPrivate::TypeAndForceComplete<QString, std::true_type>,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MainWindow, std::true_type>,
        // method 'stationsChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sensorsChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sensorMeasurementsChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'selectedStationChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'selectedSensorChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'airQualityIndexChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'statusChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'sensorViewVisibleChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'chartViewVisibleChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'lastUpdateChanged'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onNetworkReply'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QNetworkReply *, std::false_type>,
        // method 'checkInternetConnection'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'saveDataIfDue'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'onBackgroundSaveFinished'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'fetchStations'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'fetchSensors'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'openSensorChart'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QVariantMap, std::false_type>,
        // method 'goBackToStationList'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'goBackToSensorList'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'filterStationsByCity'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<const QString &, std::false_type>,
        // method 'saveDataToFileOnly'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'setSelectedStation'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QVariantMap, std::false_type>,
        // method 'saveTestDataForAllStations'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->stationsChanged(); break;
        case 1: _t->sensorsChanged(); break;
        case 2: _t->sensorMeasurementsChanged(); break;
        case 3: _t->selectedStationChanged(); break;
        case 4: _t->selectedSensorChanged(); break;
        case 5: _t->airQualityIndexChanged(); break;
        case 6: _t->statusChanged(); break;
        case 7: _t->sensorViewVisibleChanged(); break;
        case 8: _t->chartViewVisibleChanged(); break;
        case 9: _t->lastUpdateChanged(); break;
        case 10: _t->onNetworkReply((*reinterpret_cast< std::add_pointer_t<QNetworkReply*>>(_a[1]))); break;
        case 11: _t->checkInternetConnection(); break;
        case 12: _t->saveDataIfDue(); break;
        case 13: _t->onBackgroundSaveFinished(); break;
        case 14: _t->fetchStations(); break;
        case 15: _t->fetchSensors((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 16: _t->openSensorChart((*reinterpret_cast< std::add_pointer_t<QVariantMap>>(_a[1]))); break;
        case 17: _t->goBackToStationList(); break;
        case 18: _t->goBackToSensorList(); break;
        case 19: _t->filterStationsByCity((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 20: _t->saveDataToFileOnly(); break;
        case 21: _t->setSelectedStation((*reinterpret_cast< std::add_pointer_t<QVariantMap>>(_a[1]))); break;
        case 22: _t->saveTestDataForAllStations(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 10:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QNetworkReply* >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (MainWindow::*)();
            if (_q_method_type _q_method = &MainWindow::stationsChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _q_method_type = void (MainWindow::*)();
            if (_q_method_type _q_method = &MainWindow::sensorsChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _q_method_type = void (MainWindow::*)();
            if (_q_method_type _q_method = &MainWindow::sensorMeasurementsChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _q_method_type = void (MainWindow::*)();
            if (_q_method_type _q_method = &MainWindow::selectedStationChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _q_method_type = void (MainWindow::*)();
            if (_q_method_type _q_method = &MainWindow::selectedSensorChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _q_method_type = void (MainWindow::*)();
            if (_q_method_type _q_method = &MainWindow::airQualityIndexChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _q_method_type = void (MainWindow::*)();
            if (_q_method_type _q_method = &MainWindow::statusChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _q_method_type = void (MainWindow::*)();
            if (_q_method_type _q_method = &MainWindow::sensorViewVisibleChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
        {
            using _q_method_type = void (MainWindow::*)();
            if (_q_method_type _q_method = &MainWindow::chartViewVisibleChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 8;
                return;
            }
        }
        {
            using _q_method_type = void (MainWindow::*)();
            if (_q_method_type _q_method = &MainWindow::lastUpdateChanged; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 9;
                return;
            }
        }
    }
    if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QVariantList*>(_v) = _t->stations(); break;
        case 1: *reinterpret_cast< QVariantList*>(_v) = _t->sensors(); break;
        case 2: *reinterpret_cast< QVariantList*>(_v) = _t->sensorMeasurements(); break;
        case 3: *reinterpret_cast< QVariantMap*>(_v) = _t->selectedStation(); break;
        case 4: *reinterpret_cast< QVariantMap*>(_v) = _t->selectedSensor(); break;
        case 5: *reinterpret_cast< QVariantMap*>(_v) = _t->airQualityIndex(); break;
        case 6: *reinterpret_cast< QString*>(_v) = _t->status(); break;
        case 7: *reinterpret_cast< bool*>(_v) = _t->sensorViewVisible(); break;
        case 8: *reinterpret_cast< bool*>(_v) = _t->chartViewVisible(); break;
        case 9: *reinterpret_cast< QString*>(_v) = _t->lastUpdate(); break;
        default: break;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN10MainWindowE.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    }
    if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::BindableProperty
            || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::stationsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void MainWindow::sensorsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void MainWindow::sensorMeasurementsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void MainWindow::selectedStationChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void MainWindow::selectedSensorChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void MainWindow::airQualityIndexChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void MainWindow::statusChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void MainWindow::sensorViewVisibleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void MainWindow::chartViewVisibleChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void MainWindow::lastUpdateChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 9, nullptr);
}
QT_WARNING_POP
