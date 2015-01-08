#pragma once
#include <QMainWindow>
#include <QWidget>

class TextureAtlasMakerWidget : public QWidget
{
    Q_OBJECT
public:
    TextureAtlasMakerWidget(QWidget* parent);
private:
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent);
private:
};