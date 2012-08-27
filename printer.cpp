#include "printer.h"


#include <QImage>

#include <iostream>

Printer::Printer(QObject *parent) :
    QObject(parent)
{
}

bool Printer::open(QString path) {
    port = new QextSerialPort(path);

    if (!port->open(QIODevice::WriteOnly))
        return false;

    port->setBaudRate(BAUD19200);
    port->setDataBits(DATA_8);
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);

    return true;
}

void Printer::close() {
    port->close();
}

void Printer::write(quint8 byte) {
    port->write((const char*)&byte, 1);
}

void Printer::write(QString str)
{
    port->write(str.toAscii());
}

void Printer::init() {
    reset();
    setControlParameter();
    setSleepTime();
    setCodeTable();
    setCharacterSet();

}

void Printer::reset() {
    write(27);
    write(64);
}

void Printer::setControlParameter(quint8 heatingDots, quint8 heatingTime, quint8 heatingInterval) {
    write(27);
    write(55);
    write(heatingDots);
    write(heatingTime);
    write(heatingInterval);
}

void Printer::setSleepTime(quint8 seconds) {
    write(27);
    write(56);
    write(seconds);
}

void Printer::setDoubleWidth(bool state) {
    write(27);
    write(state?14:20);

}

void Printer::setCharacterSet(CharacterSet set) {
    write(27);
    write(82);
    write(set);
}

void Printer::setCodeTable(CodeTable table) {
    write(27);
    write(116);
    write(table);
}

void Printer::feed(void) {
    write(10);
}

void Printer::feed(quint8 lines) {
    write(27);
    write(74);
    write(lines);
}

void Printer::setLineSpacing(quint8 spacing) {
    write(27);
    write(51);
    write(spacing);
}

void Printer::setAlign(AlignMode align) {
    write(27);
    write(97);
    write(align);
}

void Printer::setLeftBlankCharNums(quint8 space) {
    if (space >= 47) space = 47;

    write(27);
    write(66);
    write(space);
}

void Printer::setBold(bool state) {
    write(27);
    write(32);
    write((quint8) state);
    write(27);
    write(69);
    write((quint8) state);
}

void Printer::setReverse(bool state) {
    write(29);
    write(66);
    write((quint8) state);
}

void Printer::setUpDown(bool state) {
    write(27);
    write(123);
    write((quint8) state);
}

void Printer::setUnderline(bool state) {
    write(27);
    write(45);
    write((quint8) state);
}

void Printer::setKeyPanel(bool state) {
    write(27);
    write(99);
    write(53);
    write((quint8) state);
}

void Printer::setBarcodePrintReadable(PrintReadable n) {
    write(29);
    write(72);
    write(n);
}

void Printer::setBarcodeHeight(quint8 height) {
    if (height <= 1) height = 1;

    write(29);
    write(104);
    write(height);
}

void Printer::setBarCodeWidth(quint8 width) {
    if (width <= 2) width=2;
    else if (width >= 3) width=3;

    write(29);
    write(119);
    write(width);
}

void Printer::printBarcode(QString data, BarcodeType type) {
    write(29);
    write(107);
    write(type);
    write(data);
    write(0);
}

void Printer::printImage(QImage img, quint8 threshold) {

    QImage bw = img.convertToFormat(QImage::Format_Mono, Qt::MonoOnly|Qt::ThresholdDither|Qt::AvoidDither);

    int width = bw.width()/8;
    int height = bw.height();

    quint8 data[width*height];
    memset(data, 0, width*height);

    for (int y=0; y<bw.height(); y++) {
        for (int x=0; x<bw.width(); x++) {
            quint8 pixel =qGray(bw.pixel(x,y)) > threshold ? 0:1;
            data[y*width+x/8] += (pixel&0x01)<<(7-x%8);
        }
    }
    /*
    for (int y=0; y<bw.height(); y++) {
        for (int x=0; x<width; x++) {
            std::cout << (int)data[y*width+x] << " ";
        }
    }
    std::cout << std::endl;*/
    for (int rowStart=0; rowStart<height; rowStart+=256) {
        int chunkHeight = ((height - rowStart) > 255) ? 255 : (height - rowStart);
        write(18);
        write(42);
        write(chunkHeight);
        write(width);

        for (int i=0; i<(width*chunkHeight); i++) {
            write(data[rowStart*width + i]);
        }
    }
}

