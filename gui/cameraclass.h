#ifndef CAMERACLASS_H
#define CAMERACLASS_H

#include "deviceclass.h"

enum class CodecType {
    RAW,
    RAW3_1,
    ProResHQ,
    ProRes422,
    ProResLT,
    ProResProxy
};

typedef struct {
    uint16_t          id;
    double         value;
    QString         name;
    uint16_t         raw;
    QString        range;
    QString         unit;
} TValueStruct;

typedef struct {
    QString         unit;
    double           min;
    double           max;
    double      stepsize;
} TRangeStruct;

class CameraClass : public DeviceClass
{
public:
    CameraClass(DeviceType type, QString brand, QString name, QVector<QString> protocols, \
                QVector<TChannelStruct> channels, QVector<TRangeStruct> audio, \
                QVector<TStepStruct> fps, QVector<TStepStruct> codecs);
    QVector<TRangeStruct> getAudio();
    QVector<TStepStruct> getFps();
    QVector<TStepStruct> getCodecs();

private:
    QVector<TRangeStruct> audio;
    QVector<TStepStruct> fps;
    QVector<TStepStruct> codecs;
};

#endif // CAMERACLASS_H
