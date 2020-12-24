#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>

enum class DeviceType {
    Generic,
    Camera,
    Lens,
    Servo
};

enum class MountType {
    None,
    EF,
    MFT
};

typedef struct {
    QString         name;
    int              raw;
} TStepStruct;

typedef struct {
    QString        value;
    QString         unit;
    int              raw;
} TUnitStepStruct;

#endif // GLOBAL_H
