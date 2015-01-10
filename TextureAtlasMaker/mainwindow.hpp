#pragma once
#include <QMainWindow>
#include <QWidget>
#include <QStringListModel>
#include <memory>

class TextureAtlasMakerWidget : public QWidget
{
    Q_OBJECT
public:
    TextureAtlasMakerWidget(QWidget* parent);
private:
    QStringListModel* _list;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent);
private:
    
};