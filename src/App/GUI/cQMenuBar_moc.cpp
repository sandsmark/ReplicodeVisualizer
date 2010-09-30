/****************************************************************************
** Meta object code from reading C++ file 'cQMenuBar.h'
**
** Created: Wed Sep 29 21:06:45 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "cQMenuBar.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cQMenuBar.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Visor__cQMenuBar[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x0a,
      32,   17,   17,   17, 0x0a,
      42,   17,   17,   17, 0x0a,
      53,   17,   17,   17, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_Visor__cQMenuBar[] = {
    "Visor::cQMenuBar\0\0OnLoadImage()\0"
    "OnWatch()\0OnFilter()\0OnSettings()\0"
};

const QMetaObject Visor::cQMenuBar::staticMetaObject = {
    { &QMenuBar::staticMetaObject, qt_meta_stringdata_Visor__cQMenuBar,
      qt_meta_data_Visor__cQMenuBar, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Visor::cQMenuBar::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Visor::cQMenuBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Visor::cQMenuBar::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Visor__cQMenuBar))
        return static_cast<void*>(const_cast< cQMenuBar*>(this));
    return QMenuBar::qt_metacast(_clname);
}

int Visor::cQMenuBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMenuBar::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: OnLoadImage(); break;
        case 1: OnWatch(); break;
        case 2: OnFilter(); break;
        case 3: OnSettings(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
