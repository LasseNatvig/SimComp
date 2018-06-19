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
    QByteArrayData data[11];
    char stringdata0[112];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RunWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RunWidget_t qt_meta_stringdata_RunWidget = {
    {
QT_MOC_LITERAL(0, 0, 9), // "RunWidget"
QT_MOC_LITERAL(1, 10, 8), // "startSim"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 8), // "resetSim"
QT_MOC_LITERAL(4, 29, 8), // "openFile"
QT_MOC_LITERAL(5, 38, 7), // "newFile"
QT_MOC_LITERAL(6, 46, 10), // "memoryDump"
QT_MOC_LITERAL(7, 57, 13), // "setButtonText"
QT_MOC_LITERAL(8, 71, 12), // "currentIndex"
QT_MOC_LITERAL(9, 84, 18), // "updateProgramLabel"
QT_MOC_LITERAL(10, 103, 8) // "filename"

    },
    "RunWidget\0startSim\0\0resetSim\0openFile\0"
    "newFile\0memoryDump\0setButtonText\0"
    "currentIndex\0updateProgramLabel\0"
    "filename"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RunWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x08 /* Private */,
       3,    0,   50,    2, 0x08 /* Private */,
       4,    0,   51,    2, 0x08 /* Private */,
       5,    0,   52,    2, 0x08 /* Private */,
       6,    0,   53,    2, 0x08 /* Private */,
       7,    1,   54,    2, 0x08 /* Private */,
       9,    1,   57,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, QMetaType::QString,   10,

       0        // eod
};

void RunWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RunWidget *_t = static_cast<RunWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->startSim(); break;
        case 1: _t->resetSim(); break;
        case 2: _t->openFile(); break;
        case 3: _t->newFile(); break;
        case 4: _t->memoryDump(); break;
        case 5: _t->setButtonText((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->updateProgramLabel((*reinterpret_cast< QString(*)>(_a[1]))); break;
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
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
