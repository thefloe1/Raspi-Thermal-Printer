/*****************************************************************
* Thermal Printer Interface Library based on QT for the Rasp-Pi
* Based on the Arduino Library from Adafruit
* using QExtSerialPort
* Autor: Tobias Floery
* E-Mail: tobias@floery.net
***************************************************************/

#include "printer.h"

#include <QImage>

#include <iostream>
#include <unistd.h>


Printer::Printer(QObject *parent) :
    QObject(parent)
{
}

// opens the serial port specified py path

bool Printer::open(QString path) {
    port = new QextSerialPort(path);

    if (!port->open(QIODevice::WriteOnly))
        return false;

    // set options
    port->setBaudRate(BAUD19200);
    port->setDataBits(DATA_8);
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);

    usleep(10000);
    return true;
}

// close the serial port
void Printer::close() {
    port->close();
}

// write single byte
void Printer::write(quint8 byte) {
    port->write((const char*)&byte, 1);
}

// write a string
void Printer::write(QString str)
{
    port->write(str.toAscii());
}

// initialize the printer
void Printer::init() {
    reset();
    setStatus(true);
    setControlParameter();
    setPrintDensity();
    setSleepTime();
    setCodeTable();
    setCharacterSet();
    setBarcodePrintReadable();
}

// reset the printer
void Printer::reset() {
    write(27);
    write(64);
    usleep(50000);
}

// sets the printer online (true) or ofline (false)
void Printer::setStatus(bool state) {
	write(27);
	write(61);
	write(state);
}

// set control parameters: heatingDots, heatingTime, heatingInterval
void Printer::setControlParameter(quint8 heatingDots, quint8 heatingTime, quint8 heatingInterval) {
    write(27);
    write(55);
    write(heatingDots);
    write(heatingTime);
    write(heatingInterval);
}

// set sleep Time in seconds, time after last print the printer should stay awake
void Printer::setSleepTime(quint8 seconds) {
    write(27);
    write(56);
    write(seconds);
    usleep(50000);
    write(0xFF);
}

// set double width mode: on=true, off=false
void Printer::setDoubleWidth(bool state) {
    write(27);
    write(state?14:20);

}


// set the print density and break time
void Printer::setPrintDensity(quint8 printDensity, quint8 printBreakTime) {
	write(18);
	write(35);
	write((printBreakTime << 5) | printDensity);
}

// set the used character set
void Printer::setCharacterSet(CharacterSet set) {
    write(27);
    write(82);
    write(set);
}

// set the used code table
void Printer::setCodeTable(CodeTable table) {
    write(27);
    write(116);
    write(table);
}

// feed single line
void Printer::feed(void) {
    write(10);
}

// feed <<lines>> lines
void Printer::feed(quint8 lines) {
    write(27);
    write(74);
    write(lines);
}

// set line spacing
void Printer::setLineSpacing(quint8 spacing) {
    write(27);
    write(51);
    write(spacing);
}

// set Align Mode: LEFT, MIDDLE, RIGHT
void Printer::setAlign(AlignMode align) {
    write(27);
    write(97);
    write(align);
}

// set how many blanks should be kept on the left side
void Printer::setLeftBlankCharNums(quint8 space) {
    if (space >= 47) space = 47;

    write(27);
    write(66);
    write(space);
}

// set Bold Mode: on=true, off=false
void Printer::setBold(bool state) {
    write(27);
    write(32);
    write((quint8) state);
    write(27);
    write(69);
    write((quint8) state);
}

// set Reverse printing Mode
void Printer::setReverse(bool state) {
    write(29);
    write(66);
    write((quint8) state);
}

// set Up/Down Mode
void Printer::setUpDown(bool state) {
    write(27);
    write(123);
    write((quint8) state);
}

// set Underline printing
void Printer::setUnderline(bool state) {
    write(27);
    write(45);
    write((quint8) state);
}

// enable / disable the key on the frontpanel
void Printer::setKeyPanel(bool state) {
    write(27);
    write(99);
    write(53);
    write((quint8) state);
}

// where should a readable barcode code be printed
void Printer::setBarcodePrintReadable(PrintReadable n) {
    write(29);
    write(72);
    write(n);
}

// sets the height of the barcode in pixels
void Printer::setBarcodeHeight(quint8 height) {
    if (height <= 1) height = 1;

    write(29);
    write(104);
    write(height);
}

// sets the barcode line widths (only 2 or 3)
void Printer::setBarCodeWidth(quint8 width) {
    if (width <= 2) width=2;
    else if (width >= 3) width=3;

    write(29);
    write(119);
    write(width);
}

// prints a barcode
void Printer::printBarcode(QString data, BarcodeType type) {
    write(29);
    write(107);
    write(type);
    write(data);
    write(0);
}

// print Image, threshold defines grayscale to black&withe threshold level
void Printer::printImage(QImage img, quint8 threshold) {

    QImage bw = img.convertToFormat(QImage::Format_Mono, Qt::MonoOnly|Qt::ThresholdDither|Qt::AvoidDither);

    int width = bw.width()/8;
    int height = bw.height();

    quint8 data[width*height];
    memset(data, 0, width*height);

    // needed to use the QImage::pixel method
    // QImage::bits seemed to return ARGB values 
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

    // split images with height > 255 into parts (from Adafruit)	
    for (int rowStart=0; rowStart<height; rowStart+=256) {
        int chunkHeight = ((height - rowStart) > 255) ? 255 : (height - rowStart);
        write(18);
        write(42);
        write(chunkHeight);
        write(width);

        for (int i=0; i<(width*chunkHeight); i++) {
            write(data[rowStart*width + i]);
        }
        usleep(50000);
    }
}

