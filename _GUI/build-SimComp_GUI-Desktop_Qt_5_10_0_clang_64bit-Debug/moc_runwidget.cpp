/****************************************************************************
** Meta object code from reading C++ file 'runwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../_src/runwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'runwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RunWidget_t {
    QByteArrayData data[19];
    char stringdata0[228];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RunWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RunWidget_t qt_meta_stringdata_RunWidget = {
    {
QT_MOC_LITERAL(0, 0, 9), // "RunWidget"
QT_MOC_LITERAL(1, 10, 5), // "reset"
QT_MOC_LITERAL(2, 16, 0), // ""
QT_MOC_LITERAL(3, 17, 8), // "openFile"
QT_MOC_LITERAL(4, 26, 7), // "newFile"
QT_MOC_LITERAL(5, 34, 16), // "openMemoryWindow"
QT_MOC_LITERAL(6, 51, 21), // "openPreformanceWindow"
QT_MOC_LITERAL(7, 73, 13), // "setButtonText"
QT_MOC_LITERAL(8, 87, 12), // "currentIndex"
QT_MOC_LITERAL(9, 100, 14), // "updateFilename"
QT_MOC_LITERAL(10, 115, 8), // "filename"
QT_MOC_LITERAL(11, 124, 15), // "runFromShortCut"
QT_MOC_LITERAL(12, 140, 16), // "stepFromShortCut"
QT_MOC_LITERAL(13, 157, 16), // "nextFromShortCut"
QT_MOC_LITERAL(14, 174, 11), // "runFinished"
QT_MOC_LITERAL(15, 186, 17), // "updatePerformance"
QT_MOC_LITERAL(16, 204, 9), // "writeInfo"
QT_MOC_LITERAL(17, 214, 4), // "info"
QT_MOC_LITERAL(18, 219, 8) // "startSim"

    },
    "RunWidget\0reset\0\0openFile\0newFile\0"
    "openMemoryWindow\0openPreformanceWindow\0"
    "setButtonText\0currentIndex\0updateFilename\0"
    "filename\0runFromShortCut\0stepFromShortCut\0"
    "nextFromShortCut\0runFinished\0"
    "updatePerformance\0writeInfo\0info\0"
    "startSim"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RunWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x08 /* Private */,
       3,    0,   85,    2, 0x08 /* Private */,
       4,    0,   86,    2, 0x08 /* Private */,
       5,    0,   87,    2, 0x08 /* Private */,
       6,    0,   88,    2, 0x08 /* Private */,
       7,    1,   89,    2, 0x08 /* Private */,
       9,    1,   92,    2, 0x08 /* Private */,
      11,    0,   95,    2, 0x08 /* Private */,
      12,    0,   96,    2, 0x08 /* Private */,
      13,    0,   97,    2, 0x08 /* Private */,
      14,    0,   98,    2, 0x08 /* Private */,
      15,    0,   99,    2, 0x08 /* Private */,
      16,    1,  100,    2, 0x08 /* Private */,
      18,    0,  103,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void,

       0        // eod
};

void RunWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RunWidget *_t = static_cast<RunWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->reset(); break;
        case 1: _t->openFile(); break;
        case 2: _t->newFile(); break;
        case 3: _t->openMemoryWindow(); break;
        case 4: _t->openPreformanceWindow(); break;
        case 5: _t->setButtonText((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->updateFilename((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->runFromShortCut(); break;
        case 8: _t->stepFromShortCut(); break;
        case 9: _t->nextFromShortCut(); break;
        case 10: _t->runFinished(); break;
        case 11: _t->updatePerformance(); break;
        case 12: _t->writeInfo((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->startSim(); break;
        default: ;
        }
    }
}

const QMetaObject RunWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_RunWidget.data,
      qt_meta_data_RunWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *RunWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RunWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RunWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int RunWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
struct qt_meta_stringdata_SimulatorThread_t {
    QByteArrayData data[1];
    char stringdata0[16];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SimulatorThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SimulatorThread_t qt_meta_stringdata_SimulatorThread = {
    {
QT_MOC_LITERAL(0, 0, 15) // "SimulatorThread"

    },
    "SimulatorThread"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SimulatorThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void SimulatorThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject SimulatorThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_SimulatorThread.data,
      qt_meta_data_SimulatorThread,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SimulatorThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SimulatorThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SimulatorThread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int SimulatorThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
