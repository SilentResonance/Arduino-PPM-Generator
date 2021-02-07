#include "json_configreader.h"

JSON_configreader::JSON_configreader(QObject *parent) : QObject (parent)
{
    if(!QFile::exists(configDirectory)) {
        configDirectory = QCoreApplication::applicationDirPath();
        emit jsonError(tr("JSON: Config directory does not exist. Using default"));
    }
    //qDebug() << configDirectory;
}

JSON_configreader::JSON_configreader(QString directory, QObject *parent) : QObject (parent)
{
    QString absPath = QCoreApplication::applicationDirPath() + "/" + directory;
    //qDebug() << absPath;
    if(!QFile::exists(absPath)) {
        configDirectory = QCoreApplication::applicationDirPath() +  "/../../devices";
        emit jsonError(tr("JSON: Config directory does not exist. Using default"));
    } else {
        configDirectory = absPath;
    }
    //qDebug() << configDirectory;
}

void JSON_configreader::enumerateJSONFiles(){
    QDir directory(configDirectory);
    QStringList filters;
    QJsonParseError error;
    filters << "*.json" << "*.JSON";
    jsonFilenames = directory.entryList(filters);
    if(jsonFilenames.isEmpty()){
        emit jsonError(tr("JSON: No JSON files found in ") + configDirectory);
    }
    foreach (const QString &jsonFile, jsonFilenames){
        QFile configFile(configDirectory + QDir::separator() + jsonFile);
        //qDebug() << jsonFile;
        configFile.open(QIODevice::ReadOnly);
        QJsonDocument doc;
        doc = QJsonDocument::fromJson(configFile.readAll(), &error);
        configFile.close();
        if(doc.isNull()) {
            emit jsonError(tr("JSON: Error reading json file ") + jsonFile + " error " + error.errorString());
            qDebug() << error.errorString();
            configFile.close();
            continue;
        }
        configurations.append(doc);
    }
    foreach (const QJsonDocument &confDoc, configurations) {
        QJsonObject jObject = confDoc.object();
        DeviceType type;
        QString brand;
        QString name;
        MountType mount;
        QVector<QString> protocols;
        QVector<TChannelStruct> channels;
        QVector<TIrisStruct>  iris;
        QVector<TUnitStepStruct>  zoom;
        QVector<TUnitStepStruct> focus;
        QVector<TRangeStruct> audio;
        QVector<TStepStruct> fps;
        QVector<TStepStruct> codecs;
        // Checking device type
        //qDebug() << jObject["type"].toString();
        if(jObject["type"].toString().toLower() == "camera"){
            type = DeviceType::Camera;
        } else if (jObject["type"].toString().toLower() == "generic") {
            type = DeviceType::Generic;
        } else if (jObject["type"].toString().toLower() == "servo") {
            type = DeviceType::Servo;
        } else if (jObject["type"].toString().toLower() == "lens") {
            type = DeviceType::Lens;
        } else {
            type = DeviceType::Generic;
        }
        // Get name and brand
        name = jObject["name"].toString();
        brand = jObject["brand"].toString();
        // Get supported protocols
        QJsonValue valuesArray = jObject.value("protocols");
        if(valuesArray.isArray()){
            QJsonArray jArray = valuesArray.toArray();
            for (int i = 0; i < jArray.size(); i++) {
              protocols.append(jArray[i].toString());
            };
            //protocols.append(valuesArray[0].toString());
        } else if(valuesArray.isString()) {
            protocols.append(jObject["protocols"].toString());
        }
        // Get channel information
        QJsonValue channelList = jObject.value("channels");
        if(channelList.isArray()){
            QJsonArray jArray = channelList.toArray();
            for (int i = 0; i < jArray.size(); i++) {
                QJsonObject proto = jArray[i].toObject();
                TChannelStruct chan;
                chan.id = proto["id"].toInt();
                chan.name = proto["name"].toString();
                if(proto["min"].isNull()) {
                    chan.min = 0;
                } else {
                    chan.min = proto["min"].toInt();
                }
                if(proto["max"].isNull()) {
                    chan.max = 2047;
                } else {
                    chan.max = proto["max"].toInt();
                }
                if(proto["type"].toString().toLower() == "raw"){
                    chan.chType = ChannelType::Raw;
                } else if(proto["type"].toString().toLower() == "values") {
                    chan.chType = ChannelType::Values;
                } else if(proto["type"].toString().toLower() == "stepupdown") {
                    chan.chType = ChannelType::StepUpDown;
                    chan.min = 352;
                    chan.max = 1743;
                } else if(proto["type"].toString().toLower() == "toggle") {
                    chan.chType = ChannelType::Toggle;
                } else {
                    chan.chType = ChannelType::Values;
                }
                channels.append(chan);
            }

        } else if(channelList.isString()) {
            //channels.append(proto["channels"].toString());
        }
        // Get mount information
        QJsonValue mountValue = jObject.value("mount");
        if(mountValue.isNull()){
            mount = MountType::None;
        } else {
            if(mountValue.toString().toLower() == "ef") {
                mount = MountType::EF;
            } else if(mountValue.toString().toLower() == "mft") {
                mount = MountType::MFT;
            } else {
                mount = MountType::None;
            }
        }
        // Get iris information
        QJsonValue irisValues = jObject.value("iris");
        if(irisValues.isArray()){
            QJsonArray jArray = irisValues.toArray();
            for (int i = 0; i < jArray.size(); i++) {
                QJsonObject apertureStep = jArray[i].toObject();
                TIrisStruct irisStruct;
                irisStruct.id = apertureStep["id"].toInt();
                irisStruct.value = apertureStep["value"].toDouble();
                irisStruct.raw = apertureStep["raw"].toInt();
                iris.append(irisStruct);
            }
        }
        // Get zoom information
        QJsonValue zoomValues = jObject.value("zoom");
        if(zoomValues.isArray()){
            QJsonArray jArray = zoomValues.toArray();
            for (int i = 0; i < jArray.size(); i++) {
                QJsonObject zoomStep = jArray[i].toObject();
                TUnitStepStruct zoomStruct;
                zoomStruct.value = zoomStep["value"].toString(QString::number(i));
                zoomStruct.unit = zoomStep["unit"].toString("");
                zoomStruct.raw = zoomStep["raw"].toInt();
                zoom.append(zoomStruct);
            }
        }
        // Get focus information
        QJsonValue focusValues = jObject.value("focus");
        if(focusValues.isArray()){
            QJsonArray jArray = focusValues.toArray();
            for (int i = 0; i < jArray.size(); i++) {
                QJsonObject focusStep = jArray[i].toObject();
                TUnitStepStruct focusStruct;
                focusStruct.value = focusStep["value"].toString(QString::number(i));
                focusStruct.unit = focusStep["unit"].toString("");
                focusStruct.raw = focusStep["raw"].toInt();
                focus.append(focusStruct);
            }
        }
        // Get audio information
        QJsonValue audioValues = jObject.value("audio");
        if(audioValues.isArray()){
            QJsonArray jArray = audioValues.toArray();
            for (int i = 0; i < jArray.size(); i++) {
                QJsonObject audioRange = jArray[i].toObject();
                TRangeStruct audioStruct;
                audioStruct.unit = audioRange["unit"].toString();
                QStringList range = audioRange["range"].toString().split(":");
                if(range.size() == 3) { // range definden min:step:max
                    audioStruct.min = range[0].toInt();
                    audioStruct.max = range[2].toInt();
                    audioStruct.stepsize = range[1].toInt();
                } else if(range.size() == 2){ // range defined min:max assuming stepsize 1
                    audioStruct.min = range[0].toInt();
                    audioStruct.max = range[1].toInt();
                    audioStruct.stepsize = 1;
                } else {
                    audioStruct.min = 0;
                    audioStruct.max = 0;
                    audioStruct.stepsize = 0;
                }
                audio.append(audioStruct);
            }
        }
        // Get fps settings
        QJsonValue fpsValues = jObject.value("fps");
        if(fpsValues.isArray()){
            QJsonArray jArray = fpsValues.toArray();
            for (int i = 0; i < jArray.size(); i++) {
                QJsonObject fpsStep = jArray[i].toObject();
                TStepStruct fpsValues;
                fpsValues.name = fpsStep["name"].toString();
                fpsValues.raw = fpsStep["raw"].toInt();
                fps.append(fpsValues);
            }
        }
        // Get codec settings
        QJsonValue codecValues = jObject.value("codecs");
        if(codecValues.isArray()){
            QJsonArray jArray = codecValues.toArray();
            for (int i = 0; i < jArray.size(); i++) {
                QJsonObject codecStep = jArray[i].toObject();
                TStepStruct codecValues;
                codecValues.name = codecStep["name"].toString();
                codecValues.raw = codecStep["raw"].toInt();
                codecs.append(codecValues);
            }
        }


        // Append new found device to internal lists
        if(type == DeviceType::Lens){
            LensClass device(type,brand,name,mount,iris,zoom,focus);
            lenses.append(device);
        } else if(type == DeviceType::Camera){
            CameraClass device(type,brand,name,protocols,channels,audio,fps,codecs);
            cameras.append(device);
        } else {
            DeviceClass device(type,brand,name,protocols,channels);
            devices.append(device);
        }
    }
    emit foundDevices(devices.size(), cameras.size(), lenses.size());
}

QVector<DeviceClass> JSON_configreader::getDevices(){
    return devices;
}
QVector<LensClass> JSON_configreader::getLenses(){
    return lenses;
}

