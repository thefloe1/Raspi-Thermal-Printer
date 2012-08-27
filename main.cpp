#include <QtCore/QCoreApplication>

#include <iostream>

#include "printer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    Printer *p = new Printer();
    std::cout << "Trying to open port" << std::endl;

    bool res = p->open("/dev/pts/2");
    std::cout << "Status: " << res << std::endl;

    if (!res) {
        std::cerr << "Error opening port, aborting" << std::endl;
        return (0);
    }

    std::cout << "Writing a byte" << std::endl;
    p->printImage(QImage("./img.png"));

    std::cout << "Closing Device" << std::endl;
    p->close();

    return 1;

    return a.exec();
}
