#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w{":/addition/additional/Test_1.csv", ":/addition/additional/config.ini", ":/addition/additional/Aqua.qss"};

    w.show();
    return a.exec();
}
