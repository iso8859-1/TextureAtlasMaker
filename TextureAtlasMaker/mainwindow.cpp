#include "mainwindow.hpp"

#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent)
: QMainWindow(parent)
{
    setWindowTitle("Texture Atlas Maker");
    auto centralWidget = new TextureAtlasMakerWidget(this);
    setCentralWidget(centralWidget);
}

TextureAtlasMakerWidget::TextureAtlasMakerWidget(QWidget* parent)
: QWidget(parent)
{
    auto labelTextureSize = new QLabel("Texture Size:",this);
    auto comboBoxTextureSize = new QComboBox(this);
    auto layoutTextureSize = new QHBoxLayout();
    layoutTextureSize->addWidget(labelTextureSize);
    layoutTextureSize->addWidget(comboBoxTextureSize);
    
    auto labelTileSize = new QLabel("Tile Size:", this);
    auto comboBoxTileSize = new QComboBox(this);
    auto layoutTileSize = new QHBoxLayout();
    layoutTileSize->addWidget(labelTileSize);
    layoutTileSize->addWidget(comboBoxTileSize);
    
    auto centralLayout = new QVBoxLayout(this);
    centralLayout->addLayout(layoutTextureSize);
    centralLayout->addLayout(layoutTileSize);
    
    setLayout(centralLayout);
}