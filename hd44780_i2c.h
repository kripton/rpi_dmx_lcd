#ifndef HD44780_I2C_H
#define HD44780_I2C_H

#include <QObject>
#include <QDebug>

#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <iostream>

#include <unistd.h>

using namespace std;

class HD44780_I2C : public QObject
{
    Q_OBJECT
public:
    explicit HD44780_I2C(QObject *parent = 0);
    qint8 i2cbus;
    qint8 i2cadr;

private:
    int file;

signals:

public slots:
    void init();
    void writebyte(char data, char display, char byte, bool init = false);
    void writeI2C(int file, void* data, int length);
    void lcdInit();
    void lcdClear();
    void lcdCurPos(char display, char row, char col);
    void lcdWrite(char display, QString string);
};

#endif // HD44780_I2C_H
