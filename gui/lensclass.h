#ifndef LENSCLASS_H
#define LENSCLASS_H

#include <QObject>
#include "global.h"

typedef struct {
    uint16_t          id;
    double         value;
    uint16_t         raw;
} TIrisStruct;

//typedef struct {
//    QString        value;
//    QString         unit;
//    uint16_t         raw;
//} TFocusStruct;

//typedef struct {
//    QString        value;
//    QString         unit;
//    uint16_t         raw;
//} TZoomStruct;

class LensClass
{
public:
    LensClass(DeviceType type, QString brand, QString name, \
              MountType mount, QVector<TIrisStruct>  iris, \
              QVector<TUnitStepStruct>  zoom, QVector<TUnitStepStruct> focus);
    DeviceType getDeviceType();
    QString getBrand();
    QString getName();
    MountType getMount();
    QVector<TIrisStruct> getIris();
    QVector<TUnitStepStruct> getZoom();
    QVector<TUnitStepStruct> getFocus();
signals:

public slots:

private:
    DeviceType type;
    QString brand;
    QString name;
    MountType mount;
    QVector<TIrisStruct>  iris;
    QVector<TUnitStepStruct>  zoom;
    QVector<TUnitStepStruct> focus;
};

#endif // LENSCLASS_H
