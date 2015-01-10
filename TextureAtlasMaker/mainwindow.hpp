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
	void appendFile(const QString& filename);
	void appendFiles(const QStringList& files);

    QStringListModel* _list;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent);
private:
    
};