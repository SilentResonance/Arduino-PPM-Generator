#ifndef JSON_CONFIGREADER_H
#define JSON_CONFIGREADER_H

#include <QObject>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <QDebug>
#include "deviceclass.h"
#include "cameraclass.h"
#include "lensclass.h"

class JSON_configreader : public QObject
{
    Q_OBJECT
public:
    //explicit JSON_configreader(QObject *parent = nullptr);
    explicit JSON_configreader(QObject *parent = nullptr);
    JSON_configreader(QString directory, QObject *parent = nullptr);
    virtual ~JSON_configreader() {};
    QVector<DeviceClass> getDevices();
    QVector<LensClass> getLenses();
public slots:
    void enumerateJSONFiles();
    void enumerateJSONFiles(QString subdirectory);
    void setConfigDirectory(QString path);

signals:
    void jsonError(QString errorMessage);
    void foundDevices(int numDevices, int numCameras, int numLenses);

private:
    QStringList jsonFilenames;
    QString configDirectory;
    QVector<QJsonDocument> configurations;
    QVector<DeviceClass> devices;
    QVector<CameraClass> cameras;
    QVector<LensClass> lenses;

};

#endif // JSON_CONFIGREADER_H
