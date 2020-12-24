#include "configreader.h"

//ConfigReader::ConfigReader(QObject *parent) : QObject(parent)
//{
//    reader = new JSON_configreader(parent);
//    reader->enumerateJSONFiles();
//}

ConfigReader::ConfigReader(QString directory, QObject *parent)  : QObject(parent)
{
    reader = new JSON_configreader(directory, nullptr);
    reader->enumerateJSONFiles();
}

QVector<DeviceClass> ConfigReader::getDevices(){
    return reader->getDevices();
}
QVector<LensClass> ConfigReader::getLenses(){
    return reader->getLenses();
}

void ConfigReader::handleErrors(){
    qDebug() << "ConfigReader: Errors need handling...";
}
