/****************************************************************************
** Meta object code from reading C++ file 'cQTimeBar.h'
**
** Created: Thu Sep 23 19:15:28 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "cQTimeBar.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cQTimeBar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Visor__cQTimeBar[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      25,   18,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Visor__cQTimeBar[] = {
    "Visor::cQTimeBar\0\0iValue\0OnValueChanged(int)\0"
};

const QMetaObject Visor::cQTimeBar::staticMetaObject = {
    { &QToolBar::staticMetaObject, qt_meta_stringdata_Visor__cQTimeBar,
      qt_meta_data_Visor__cQTimeBar, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Visor::cQTimeBar::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Visor::cQTimeBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Visor::cQTimeBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Visor__cQTimeBar))
        return static_cast<void*>(const_cast< cQTimeBar*>(this));
    return QToolBar::qt_metacast(_clname);
}

int Visor::cQTimeBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QToolBar::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: OnValueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
