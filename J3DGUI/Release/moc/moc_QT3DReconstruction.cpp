/****************************************************************************
** Meta object code from reading C++ file 'QT3DReconstruction.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../QT3DReconstruction.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'QT3DReconstruction.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QT3DReconstruction_t {
    QByteArrayData data[15];
    char stringdata0[296];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QT3DReconstruction_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QT3DReconstruction_t qt_meta_stringdata_QT3DReconstruction = {
    {
QT_MOC_LITERAL(0, 0, 18), // "QT3DReconstruction"
QT_MOC_LITERAL(1, 19, 31), // "on_actionMatchFeature_triggered"
QT_MOC_LITERAL(2, 51, 0), // ""
QT_MOC_LITERAL(3, 52, 22), // "on_actionSFM_triggered"
QT_MOC_LITERAL(4, 75, 14), // "LPSTRtoLPCWSTR"
QT_MOC_LITERAL(5, 90, 7), // "LPCWSTR"
QT_MOC_LITERAL(6, 98, 5), // "char*"
QT_MOC_LITERAL(7, 104, 5), // "szStr"
QT_MOC_LITERAL(8, 110, 27), // "on_action_viewMVS_triggered"
QT_MOC_LITERAL(9, 138, 34), // "on_action_addSensorWidth_trig..."
QT_MOC_LITERAL(10, 173, 27), // "on_action_viewPLY_triggered"
QT_MOC_LITERAL(11, 201, 29), // "on_actionDenseCloud_triggered"
QT_MOC_LITERAL(12, 231, 9), // "timerSlot"
QT_MOC_LITERAL(13, 241, 34), // "on_action_reconstrctMesh_trig..."
QT_MOC_LITERAL(14, 276, 19) // "on_action_triggered"

    },
    "QT3DReconstruction\0on_actionMatchFeature_triggered\0"
    "\0on_actionSFM_triggered\0LPSTRtoLPCWSTR\0"
    "LPCWSTR\0char*\0szStr\0on_action_viewMVS_triggered\0"
    "on_action_addSensorWidth_triggered\0"
    "on_action_viewPLY_triggered\0"
    "on_actionDenseCloud_triggered\0timerSlot\0"
    "on_action_reconstrctMesh_triggered\0"
    "on_action_triggered"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QT3DReconstruction[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x08 /* Private */,
       3,    0,   65,    2, 0x08 /* Private */,
       4,    1,   66,    2, 0x08 /* Private */,
       8,    0,   69,    2, 0x08 /* Private */,
       9,    0,   70,    2, 0x08 /* Private */,
      10,    0,   71,    2, 0x08 /* Private */,
      11,    0,   72,    2, 0x08 /* Private */,
      12,    0,   73,    2, 0x08 /* Private */,
      13,    0,   74,    2, 0x08 /* Private */,
      14,    0,   75,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    0x80000000 | 5, 0x80000000 | 6,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void QT3DReconstruction::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QT3DReconstruction *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_actionMatchFeature_triggered(); break;
        case 1: _t->on_actionSFM_triggered(); break;
        case 2: { LPCWSTR _r = _t->LPSTRtoLPCWSTR((*reinterpret_cast< char*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< LPCWSTR*>(_a[0]) = std::move(_r); }  break;
        case 3: _t->on_action_viewMVS_triggered(); break;
        case 4: _t->on_action_addSensorWidth_triggered(); break;
        case 5: _t->on_action_viewPLY_triggered(); break;
        case 6: _t->on_actionDenseCloud_triggered(); break;
        case 7: _t->timerSlot(); break;
        case 8: _t->on_action_reconstrctMesh_triggered(); break;
        case 9: _t->on_action_triggered(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QT3DReconstruction::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_QT3DReconstruction.data,
    qt_meta_data_QT3DReconstruction,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QT3DReconstruction::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QT3DReconstruction::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QT3DReconstruction.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int QT3DReconstruction::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
