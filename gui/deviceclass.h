#ifndef DEVICECLASS_H
#define DEVICECLASS_H

#include <QObject>
#include "global.h"

enum class ChannelType {
    Raw,
    Values,
    StepUpDown,
    Toggle
};

typedef struct {
    int                 id;
    QString           name;
    ChannelType     chType;
    int                min;
    int                max;
} TChannelStruct;

class DeviceClass
{
public:
    DeviceClass(DeviceType type, QString brand, QString name, \
                         QVector<QString> protocols, QVector<TChannelStruct> channels);
    DeviceType getDeviceType();
    QString getBrand();
    QString getName();
    QVector<QString> getProtocols();
    QVector<TChannelStruct> getChannels();

public slots:

private:
    DeviceType type;
    QString brand;
    QString name;
    QVector<QString> protocols;
    QVector<TChannelStruct> channels;

};

#endif // DEVICECLASS_H
