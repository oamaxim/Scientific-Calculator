#include "MainWindow.h"
#include <QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.resize(500, 400);
    window.show();

    return app.exec();
}