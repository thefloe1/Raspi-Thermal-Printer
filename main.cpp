#include <QtCore/QCoreApplication>

#include <iostream>

#include "printer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    Printer *p = new Printer();
    std::cout << "Trying to open port" << std::endl;

    bool res = p->open("/dev/ttyAMA0");
    std::cout << "Status: " << res << std::endl;

    if (!res) {
        std::cerr << "Error opening port, aborting" << std::endl;
        return (0);
    }

    p->init();

    p->write("Printer Test\n");
    
    p->setReverse(true);
    p->write("Reverse ON\n");
    p->setReverse(false);

    p->setBold(true);
    p->write("Bold ON\n");
    p->setBold(false);

    p->setUpDown(true);
    p->write("UpDown ON\n");
    p->setUpDown(false);

    p->setUnderline(true);
    p->write("Underline ON\n");
    p->setUnderline(false);

    p->setDoubleWidth(true);
    p->write("Double Width ON\n");
    p->setDoubleWidth(false);
    
    p->setAlign(Printer::MIDDLE);
    p->write("middle align\n");
    p->setAlign(Printer::RIGHT);
    p->write("right align\n");

    p->setAlign(Printer::LEFT);
   
    p->printBarcode("12345678", Printer::EAN8);

    //p->printImage(QImage("./tux.jpg"));

    std::cout << "Closing Device" << std::endl;
    p->close();

    return 1;

    return a.exec();
}
