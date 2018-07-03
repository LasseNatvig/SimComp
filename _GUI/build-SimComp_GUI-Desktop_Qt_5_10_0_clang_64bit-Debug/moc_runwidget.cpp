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
    QByteArrayData data[25];
    char stringdata0[245];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RunWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RunWidget_t qt_meta_stringdata_RunWidget = {
    {
QT_MOC_LITERAL(0, 0, 9), // "RunWidget"
QT_MOC_LITERAL(1, 10, 18), // "performanceChanged"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 4), // "MIPS"
QT_MOC_LITERAL(4, 35, 15), // "filenameChanged"
QT_MOC_LITERAL(5, 51, 8), // "filename"
QT_MOC_LITERAL(6, 60, 6), // "output"
QT_MOC_LITERAL(7, 67, 7), // "message"
QT_MOC_LITERAL(8, 75, 23), // "instructionCountChanged"
QT_MOC_LITERAL(9, 99, 16), // "instructionCount"
QT_MOC_LITERAL(10, 116, 13), // "memoryChanged"
QT_MOC_LITERAL(11, 130, 4), // "step"
QT_MOC_LITERAL(12, 135, 3), // "run"
QT_MOC_LITERAL(13, 139, 4), // "next"
QT_MOC_LITERAL(14, 144, 5), // "reset"
QT_MOC_LITERAL(15, 150, 14), // "openFileDialog"
QT_MOC_LITERAL(16, 165, 4), // "open"
QT_MOC_LITERAL(17, 170, 7), // "newFile"
QT_MOC_LITERAL(18, 178, 4), // "save"
QT_MOC_LITERAL(19, 183, 6), // "saveAs"
QT_MOC_LITERAL(20, 190, 4), // "undo"
QT_MOC_LITERAL(21, 195, 4), // "redo"
QT_MOC_LITERAL(22, 200, 14), // "updateFilename"
QT_MOC_LITERAL(23, 215, 11), // "runFinished"
QT_MOC_LITERAL(24, 227, 17) // "updatePerformance"

    },
    "RunWidget\0performanceChanged\0\0MIPS\0"
    "filenameChanged\0filename\0output\0message\0"
    "instructionCountChanged\0instructionCount\0"
    "memoryChanged\0step\0run\0next\0reset\0"
    "openFileDialog\0open\0newFile\0save\0"
    "saveAs\0undo\0redo\0updateFilename\0"
    "runFinished\0updatePerformance"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RunWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      19,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  109,    2, 0x06 /* Public */,
       4,    1,  112,    2, 0x06 /* Public */,
       6,    1,  115,    2, 0x06 /* Public */,
       8,    1,  118,    2, 0x06 /* Public */,
      10,    0,  121,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    0,  122,    2, 0x0a /* Public */,
      12,    0,  123,    2, 0x0a /* Public */,
      13,    0,  124,    2, 0x0a /* Public */,
      14,    0,  125,    2, 0x0a /* Public */,
      15,    0,  126,    2, 0x0a /* Public */,
      16,    1,  127,    2, 0x0a /* Public */,
      17,    0,  130,    2, 0x0a /* Public */,
      18,    0,  131,    2, 0x0a /* Public */,
      19,    0,  132,    2, 0x0a /* Public */,
      20,    0,  133,    2, 0x0a /* Public */,
      21,    0,  134,    2, 0x0a /* Public */,
      22,    1,  135,    2, 0x08 /* Private */,
      23,    0,  138,    2, 0x08 /* Private */,
      24,    0,  139,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void RunWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RunWidget *_t = static_cast<RunWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->performanceChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->filenameChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->output((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->instructionCountChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->memoryChanged(); break;
        case 5: _t->step(); break;
        case 6: _t->run(); break;
        case 7: _t->next(); break;
        case 8: _t->reset(); break;
        case 9: _t->openFileDialog(); break;
        case 10: _t->open((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->newFile(); break;
        case 12: _t->save(); break;
        case 13: _t->saveAs(); break;
        case 14: _t->undo(); break;
        case 15: _t->redo(); break;
        case 16: _t->updateFilename((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 17: _t->runFinished(); break;
        case 18: _t->updatePerformance(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (RunWidget::*_t)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RunWidget::performanceChanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (RunWidget::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RunWidget::filenameChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (RunWidget::*_t)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RunWidget::output)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (RunWidget::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RunWidget::instructionCountChanged)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (RunWidget::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RunWidget::memoryChanged)) {
                *result = 4;
                return;
            }
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
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void RunWidget::performanceChanged(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RunWidget::filenameChanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void RunWidget::output(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void RunWidget::instructionCountChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void RunWidget::memoryChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
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
