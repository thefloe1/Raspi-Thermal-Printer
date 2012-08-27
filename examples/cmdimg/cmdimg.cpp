#include <QtCore/QCoreApplication>

#include <iostream>

#include "../../printer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    if (argc != 2) {
        std::cout << "Usage: cmdimg image" << std::endl;
        return 0;
    }

    Printer *p = new Printer();
    std::cout << "Trying to open port" << std::endl;

    bool res = p->open("/dev/ttyAMA0");
    std::cout << "Status: " << res << std::endl;

    if (!res) {
        std::cerr << "Error opening port, aborting" << std::endl;
        return (0);
    }

    p->init();

    p->printImage(QImage(argv[1]));

    std::cout << "Closing Device" << std::endl;
    p->close();

    return 1;

    return a.exec();
}
