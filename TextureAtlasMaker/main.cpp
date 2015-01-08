#include <QApplication>

#include "mainwindow.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Texture Atlas Maker");
    
    MainWindow window(nullptr);
    window.show();
    
    return app.exec();
}
