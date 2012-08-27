#ifndef PRINTER_H
#define PRINTER_H

#include <QObject>
#include <QImage>
#include "qextserialport.h"


class Printer : public QObject
{
    Q_OBJECT

public:
    enum CharacterSet {
        USA,
        FRANCE,
        GERMANY,
        UK,
        DENMARK,
        SWEDEN,
        ITALY,
        SPAIN,
        JAPAN,
        NORWAY,
        DENMARK2,
        SPAIN2,
        LATINAMERICA,
        KOREA
    };

    enum CodeTable {
        PC437,
        PC850
    };

    enum AlignMode {
        LEFT,
        MIDDLE,
        RIGHT
    };

    enum PrintReadable {
        NONE,
        ABOVE,
        BELOW,
        BOTH
    };

    enum BarcodeType {
        UPCA,
        UPCE,
        EAN13,
        EAN8,
        CODE39,
        I25,
        CODEBAR,
        CODE93,
        CODE128,
        CODE11,
        MSI
    };

    explicit Printer(QObject *parent = 0);
    
    bool open(QString path);
    void close();

    void write(quint8 byte);
    void write(QString str);

    void reset();
    void setControlParameter(quint8 heatingDots=20, quint8 heatingTime=255, quint8 heatingInterval=250);
    void setSleepTime(quint8 seconds = 0);

    void setDoubleWidth(bool state=false);
    void setBold(bool state=false);
    void setReverse(bool state=false);
    void setUpDown(bool state=false);
    void setUnderline(bool state=false);
    void setKeyPanel(bool state=false);

    void setCharacterSet(CharacterSet set=USA);
    void setCodeTable(CodeTable table=PC437);
    void feed();
    void feed(quint8 lines);
    void setLineSpacing(quint8 spacing=32);
    void setAlign(AlignMode align=LEFT);
    void setLeftBlankCharNums(quint8 space=0);


    void setBarcodePrintReadable(PrintReadable n=BELOW);
    void setBarcodeHeight(quint8 height=50);
    void setBarCodeWidth(quint8 width=3);
    void printBarcode(QString data, BarcodeType type=UPCA);
    void init();
    void printImage(QImage img, quint8 threshold=127);
signals:
    
public slots:

private:
    QextSerialPort *port;


};

#endif // PRINTER_H
