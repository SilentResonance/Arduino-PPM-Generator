#include "lensclass.h"

LensClass::LensClass(DeviceType type, QString brand, QString name, \
                     MountType mount, QVector<TIrisStruct>  iris, \
                     QVector<TUnitStepStruct>  zoom, QVector<TUnitStepStruct> focus)
{
    this->type = type;
    this->brand = brand;
    this->name = name;
    this->mount = mount;
    this->iris = iris;
    this->zoom = zoom;
    this->focus = focus;
}

DeviceType LensClass::getDeviceType() {
    return this->type;
}

QString LensClass::getBrand(){
    return this->brand;
}
QString LensClass::getName(){
    return this->name;
}
MountType LensClass::getMount(){
    return this->mount;
}
QVector<TIrisStruct> LensClass::getIris(){
    return this->iris;
}
QVector<TUnitStepStruct> LensClass::getZoom(){
    return this->zoom;
}
QVector<TUnitStepStruct> LensClass::getFocus(){
    return this->focus;
}

