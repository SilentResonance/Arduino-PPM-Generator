#include "deviceclass.h"

DeviceClass::DeviceClass(DeviceType type, QString brand, QString name, QVector<QString> protocols, \
                         QVector<TChannelStruct> channels)
{
    this->type = type;
    this->brand = brand;
    this->name = name;
    this->protocols = protocols;
    this->channels = channels;
}

DeviceType DeviceClass::getDeviceType() {
    return this->type;
}

QString DeviceClass::getBrand(){
    return this->brand;
}
QString DeviceClass::getName(){
    return this->name;
}
QVector<QString> DeviceClass::getProtocols(){
    return this->protocols;
}
QVector<TChannelStruct> DeviceClass::getChannels(){
    return this->channels;
}
