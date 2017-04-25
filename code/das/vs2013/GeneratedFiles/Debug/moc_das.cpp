/****************************************************************************
** Meta object code from reading C++ file 'das.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/framework/das.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'das.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DAS_t {
    QByteArrayData data[10];
    char stringdata0[81];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DAS_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DAS_t qt_meta_stringdata_DAS = {
    {
QT_MOC_LITERAL(0, 0, 3), // "DAS"
QT_MOC_LITERAL(1, 4, 6), // "OnOpen"
QT_MOC_LITERAL(2, 11, 0), // ""
QT_MOC_LITERAL(3, 12, 6), // "OnPlay"
QT_MOC_LITERAL(4, 19, 6), // "OnFind"
QT_MOC_LITERAL(5, 26, 12), // "OnFullScreen"
QT_MOC_LITERAL(6, 39, 16), // "OnLanuageChanged"
QT_MOC_LITERAL(7, 56, 8), // "QAction*"
QT_MOC_LITERAL(8, 65, 7), // "pAction"
QT_MOC_LITERAL(9, 73, 7) // "OnAbout"

    },
    "DAS\0OnOpen\0\0OnPlay\0OnFind\0OnFullScreen\0"
    "OnLanuageChanged\0QAction*\0pAction\0"
    "OnAbout"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DAS[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x08 /* Private */,
       3,    0,   45,    2, 0x08 /* Private */,
       4,    0,   46,    2, 0x08 /* Private */,
       5,    0,   47,    2, 0x08 /* Private */,
       6,    1,   48,    2, 0x08 /* Private */,
       9,    0,   51,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,

       0        // eod
};

void DAS::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DAS *_t = static_cast<DAS *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OnOpen(); break;
        case 1: _t->OnPlay(); break;
        case 2: _t->OnFind(); break;
        case 3: _t->OnFullScreen(); break;
        case 4: _t->OnLanuageChanged((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 5: _t->OnAbout(); break;
        default: ;
        }
    }
}

const QMetaObject DAS::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_DAS.data,
      qt_meta_data_DAS,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *DAS::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DAS::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_DAS.stringdata0))
        return static_cast<void*>(const_cast< DAS*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int DAS::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
