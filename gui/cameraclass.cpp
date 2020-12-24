#include "cameraclass.h"

CameraClass::CameraClass(DeviceType type, QString brand, QString name, QVector<QString> protocols, \
                         QVector<TChannelStruct> channels, QVector<TRangeStruct> audio, \
                         QVector<TStepStruct> fps, QVector<TStepStruct> codecs)
    :DeviceClass(type, brand, name, protocols, channels)
{
    this->audio = audio;
    this->fps = fps;
    this->codecs = codecs;
}

QVector<TRangeStruct> CameraClass::getAudio(){
    return this->audio;
}
QVector<TStepStruct> CameraClass::getFps(){
    return this->fps;
}
QVector<TStepStruct> CameraClass::getCodecs(){
    return this->codecs;
}
