#include "hd44780_i2c.h"

HD44780_I2C::HD44780_I2C(QObject *parent) : QObject(parent)
{
}

void HD44780_I2C::init() {
    QString devfile = "/dev/i2c-" + QString("%1").arg(i2cbus);
    file = open(devfile.toLocal8Bit(), O_RDWR);
    if (file < 0) {
        cout << "Failed to open" << devfile.constData() << endl;
      exit(1);
    }

    if (ioctl(file, I2C_SLAVE, i2cadr) < 0) {
        cout << "Failed to ioctl slave address" << endl;
      exit(1);
    }
}

void HD44780_I2C::writebyte(char data, char display, char byte, bool init) {
    //qDebug() << "Sending Byte to LCD:" << QString("%1").arg((int)byte, 2, 16, QChar('0'));

    // Pins of PCF-chip: P7  P6  P5 P4 P3 P2 P1 P0
    //                   EN2 EN1 -- RS D7 D6 D5 D4

    char i2cbyte;

    // Step 1: Setup first nibble (upper 4 bits of byte) (E high)
    i2cbyte = 0xC0 | (data << 4) | (byte >> 4);
    writeI2C(file, &i2cbyte, 1);

    if (init) usleep (1000);

    // Step 2: Let controller read first nibble (Pull E low)
    if (!display)
        i2cbyte = 0x80 | (data << 4) | (byte >> 4);
    else
        i2cbyte = 0x40 | (data << 4) | (byte >> 4);
    writeI2C(file, &i2cbyte, 1);

    if (init)
        usleep(3000);
    else
        usleep(500);

    // Step 3: Setup second nibble (lower 4 bits of byte) (E high)
    i2cbyte = 0xC0 | (data << 4) | (byte & 0xf);
    writeI2C(file, &i2cbyte, 1);

    if (init) usleep (1000);

    // Step 4: Let controller read second nibble (Pull E low)
    if (!display)
        i2cbyte = 0x80 | (data << 4) | (byte & 0xf);
    else
        i2cbyte = 0x40 | (data << 4) | (byte & 0xf);
    writeI2C(file, &i2cbyte, 1);

    if (init)
        usleep(3000);
    else
        usleep(500);
}

void HD44780_I2C::writeI2C(int file, void* data, int length) {
    int val = write(file, data, length);
    //qDebug() << "I2Cwrite:" << QString("%1").arg((char)*(char*)data, 2, 16, QChar('0')) << "returned" << val;

}

void HD44780_I2C::lcdInit() {
    writebyte(0, 0, 0x33, true); // 8bit, 8bit
    writebyte(0, 0, 0x32, true); // 8bit, 4bit
    writebyte(0, 0, 0x28, true); // 2zeilig, 5x7/8 font
    writebyte(0, 0, 0x08, true); // display aus
    writebyte(0, 0, 0x01, true); // display löschen
    writebyte(0, 0, 0x06, true); // cursor wandert nach rechts, kein display shift
    writebyte(0, 0, 0x0C, true); // display ein

    writebyte(0, 1, 0x33, true); // 8bit, 8bit
    writebyte(0, 1, 0x32, true); // 8bit, 4bit
    writebyte(0, 1, 0x28, true); // 2zeilig, 5x7/8 font
    writebyte(0, 1, 0x08, true); // display aus
    writebyte(0, 1, 0x01, true); // display löschen
    writebyte(0, 1, 0x06, true); // cursor wandert nach rechts, kein display shift
    writebyte(0, 1, 0x0C, true); // display ein
}

void HD44780_I2C::lcdClear() {
    writebyte(0, 0, 0x01);
}

void HD44780_I2C::lcdCurPos(char display, char row, char col) {
  char adr = row*40 + col;
  writebyte(0, display, (adr | 0x80));
}

void HD44780_I2C::lcdWrite(char display, QString string) {
    char buf[40];
    memset(buf, 0x20, 40);
    QByteArray array = string.toAscii();

    int i = 0;
    while ((i <= 39) && (i < string.length())) {
        writebyte(1, display, array[i]);
        i++;
    }
}
