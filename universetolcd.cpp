#include "universetolcd.h"

UniverseToLCD::UniverseToLCD(QObject *parent) : QObject(parent) {
}

void UniverseToLCD::init() {
    lcd.i2cbus = 0;
    lcd.i2cadr = 0x38;
    lcd.init();
    lcd.lcdInit();

#ifdef HTTP
    url.setUrl("http://127.0.0.1:9090/get_dmx?u=" + dmxuni);
    timer = new QTimer(this);
    timer->setInterval(interval);
    connect(timer, SIGNAL(timeout()), this, SLOT(startUpdate()));
    timer->start();
#endif

#ifdef OLA
    // Setup the client, this connects to the server
    if (!olaWrapper.Setup()) {
      cout << "OLA client-setup failed" << endl;
      exit(1);
    }
    olaClient = olaWrapper.GetClient();
    olaClient->SetDmxCallback(ola::NewCallback(this, &UniverseToLCD::NewDmx));
    olaClient->RegisterUniverse(dmxuni.toInt(), ola::REGISTER, ola::NewSingleCallback(this, &UniverseToLCD::RegisterComplete));
#endif
}

#ifdef OLA
void UniverseToLCD::RegisterComplete(const string &error) {
  if (!error.empty()) {
    std::cerr << "Register command failed" << std::endl;
    exit(1);
  }
}

void UniverseToLCD::NewDmx(unsigned int universe, const ola::DmxBuffer &buffer, const string &error) {
    uint8_t buf[512];
    uint length = 512;

    buffer.Get(buf, &length);

    QString row0 = QString("%1 %2 %3 %4 %5").arg(buf[0], 2, 16, QChar('0')).arg(buf[1], 2, 16, QChar('0')).arg(buf[2], 2, 16, QChar('0')).arg(buf[3], 2, 16, QChar('0')).arg(buf[4], 2, 16, QChar('0'));
    QString row1 = QString("%1 %2 %3 %4 %5").arg(buf[5], 2, 16, QChar('0')).arg(buf[6], 2, 16, QChar('0')).arg(buf[7], 2, 16, QChar('0')).arg(buf[8], 2, 16, QChar('0')).arg(buf[9], 2, 16, QChar('0'));

    setCursor(0,0);
    putSting(row0.toUpper());
    setCursor(1,0);
    putSting(row1.toUpper());
}
#endif

#ifdef HTTP
void UniverseToLCD::startUpdate() {
    reply = qnam.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()), this, SLOT(httpFinished()));
}

void UniverseToLCD::httpFinished() {
    QJson::Parser parser;
    bool ok;
    int i;

    QByteArray json = reply->readAll();

    //qDebug() << "HTTP read:" << json;

    if (json.length() < 1024) {
        qDebug() << "PANIC. return!";
        return;
    }

    QVariantMap result = parser.parse (json, &ok).toMap();


    QList<QVariant> list = result["dmx"].toList();

    QString row0 = "";
    QString row1 = "";
    QString row2 = "";
    QString row3 = "";

    i = 0;
    while (i < 13) {
        if (!list[i].canConvert<int>()) return;
        row0.append(QString("%1 ").arg(list[i].toInt(), 2, 16, QChar('0')));
        i++;
    }
    while (i < 26) {
        if (!list[i].canConvert<int>()) return;
        row1.append(QString("%1 ").arg(list[i].toInt(), 2, 16, QChar('0')));
        i++;
    }
    while (i < 39) {
        if (!list[i].canConvert<int>()) return;
        row2.append(QString("%1 ").arg(list[i].toInt(), 2, 16, QChar('0')));
        i++;
    }
    while (i < 52) {
        if (!list[i].canConvert<int>()) return;
        row3.append(QString("%1 ").arg(list[i].toInt(), 2, 16, QChar('0')));
        i++;
    }

    lcd.lcdCurPos(0, 0, 0);
    lcd.lcdWrite(0, row0.toUpper());
    lcd.lcdCurPos(0, 1, 0);
    lcd.lcdWrite(0, row1.toUpper());

    lcd.lcdCurPos(1, 0, 0);
    lcd.lcdWrite(1, row2.toUpper());
    lcd.lcdCurPos(1, 1, 0);
    lcd.lcdWrite(1, row3.toUpper());
}
#endif
