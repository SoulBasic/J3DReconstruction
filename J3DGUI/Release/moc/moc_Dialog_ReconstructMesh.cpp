/****************************************************************************
** Meta object code from reading C++ file 'Dialog_ReconstructMesh.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../Dialog_ReconstructMesh.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Dialog_ReconstructMesh.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Dialog_ReconstructMesh_t {
    QByteArrayData data[6];
    char stringdata0[139];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Dialog_ReconstructMesh_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Dialog_ReconstructMesh_t qt_meta_stringdata_Dialog_ReconstructMesh = {
    {
QT_MOC_LITERAL(0, 0, 22), // "Dialog_ReconstructMesh"
QT_MOC_LITERAL(1, 23, 17), // "on_btn_OK_clicked"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 21), // "on_btn_CANCEL_clicked"
QT_MOC_LITERAL(4, 64, 36), // "on_pushButton_browseInputDir_..."
QT_MOC_LITERAL(5, 101, 37) // "on_pushButton_browseOutputDir..."

    },
    "Dialog_ReconstructMesh\0on_btn_OK_clicked\0"
    "\0on_btn_CANCEL_clicked\0"
    "on_pushButton_browseInputDir_clicked\0"
    "on_pushButton_browseOutputDir_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Dialog_ReconstructMesh[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x08 /* Private */,
       3,    0,   35,    2, 0x08 /* Private */,
       4,    0,   36,    2, 0x08 /* Private */,
       5,    0,   37,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Dialog_ReconstructMesh::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Dialog_ReconstructMesh *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_btn_OK_clicked(); break;
        case 1: _t->on_btn_CANCEL_clicked(); break;
        case 2: _t->on_pushButton_browseInputDir_clicked(); break;
        case 3: _t->on_pushButton_browseOutputDir_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject Dialog_ReconstructMesh::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_Dialog_ReconstructMesh.data,
    qt_meta_data_Dialog_ReconstructMesh,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Dialog_ReconstructMesh::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Dialog_ReconstructMesh::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Dialog_ReconstructMesh.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int Dialog_ReconstructMesh::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
