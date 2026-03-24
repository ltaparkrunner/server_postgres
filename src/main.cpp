#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w{"../../additional/Test_1.csv"};
//    MainWindow w{"..\\additional\\Test_1.csv"};
//    additional\Test_1.csv
    w.show();
    return a.exec();
}
