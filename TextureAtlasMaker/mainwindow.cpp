#include "mainwindow.hpp"

#include <QPushButton>

MainWindow::MainWindow(QWidget* parent)
: QMainWindow(parent)
{
    QPushButton* button = new QPushButton("Hello world !",this);
    setCentralWidget(button);
}