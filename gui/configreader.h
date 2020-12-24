#ifndef CONFIGREADER_H
#define CONFIGREADER_H

#include <QObject>
#include <QVector>
#include <QDebug>
#include "global.h"
#include "deviceclass.h"
#include "json_configreader.h"

class ConfigReader : public QObject
{
    Q_OBJECT
public:
    //explicit ConfigReader(QObject *parent = nullptr);
    ConfigReader(QString directory,QObject *parent = nullptr);

signals:
    void configLoaded();
    void configError(QString errormessage);


public slots:
    QVector<DeviceClass> getDevices();
    QVector<LensClass> getLenses();

private:
    QVector<DeviceClass> devices;
    QVector<LensClass> lenses;
    JSON_configreader *reader;

private slots:
    void handleErrors();
};

#endif // CONFIGREADER_H

