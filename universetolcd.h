#ifndef UNIVERSETOLCD_H
#define UNIVERSETOLCD_H

#define HTTP

#include <QObject>
#include <QTimer>
#include <QString>

#ifdef HTTP
#include <QUrl>
#include <QtNetwork/QtNetwork>
#endif

#ifdef OLA
#include <ola/DmxBuffer.h>
#include <ola/Callback.h>
#include <ola/OlaCallbackClient.h>
#include <ola/OlaClientWrapper.h>
#include <ola/OlaClient.h>
#endif


#include "hd44780_i2c.h"

#include <qjson/parser.h>

using namespace std;

class UniverseToLCD : public QObject
{
    Q_OBJECT
public:
    explicit UniverseToLCD(QObject *parent = 0);
    void init();

    HD44780_I2C lcd;

    QString dmxuni;
    qint16 interval;

#ifdef HTTP
    QUrl url;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
#endif

#ifdef OLA
    ola::OlaCallbackClientWrapper olaWrapper;
    ola::OlaCallbackClient *olaClient;
    ola::DmxBuffer olaBuffer;
#endif

private:
#ifdef HTTP
    QTimer *timer;
#endif
#ifdef OLA
    void RegisterComplete(const string &error);
    void NewDmx(unsigned int universe, const ola::DmxBuffer &buffer, const string &error);
#endif
    QByteArray fetchDmxData();

#ifdef HTTP
public slots:
    void startUpdate();
    void httpFinished();
#endif
};

#endif // UNIVERSETOLCD_H
