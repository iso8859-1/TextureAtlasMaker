#include "mainwindow.hpp"

#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QGridLayout>
#include <QCheckBox>
#include <QListView>

QStringList GeneratePowerOf2StringList()
{
    return {"2x2",
        "4x4",
        "8x8",
        "16x16",
        "32x32",
        "64x64",
        "128x128",
        "256x256",
        "512x512",
        "1024x1024",
        "2048x2048",
        "4096x4096",
        "8192x8192",
        "16384x16384",
        "32768x32768",
        "65536x65536"
    };
}

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
    auto centralLayout = new QGridLayout(this);

    auto labelTextureSize = new QLabel("Texture Size:",this);
    auto comboBoxTextureSize = new QComboBox(this);
    comboBoxTextureSize->addItems(GeneratePowerOf2StringList());
    comboBoxTextureSize->setCurrentIndex(13);
    centralLayout->addWidget(labelTextureSize, 0, 0);
    centralLayout->addWidget(comboBoxTextureSize, 0, 1);
    centralLayout->setColumnStretch(1, 1);
    centralLayout->setRowStretch(100, 1);
    
    auto labelTileSize = new QLabel("Tile Size:", this);
    auto comboBoxTileSize = new QComboBox(this);
    comboBoxTileSize->addItems(GeneratePowerOf2StringList());
    comboBoxTileSize->setCurrentIndex(5);
    centralLayout->addWidget(labelTileSize, 1, 0);
    centralLayout->addWidget(comboBoxTileSize, 1, 1);
    
    auto lableTileResize = new QLabel("Resize Tile Graphics?",this);
    auto checkboxTileResize = new QCheckBox(this);
    centralLayout->addWidget(lableTileResize,2,0);
    centralLayout->addWidget(checkboxTileResize,2,1);
    
    auto listOfFiles = new QListView(this);
    centralLayout->addWidget(listOfFiles,3,0,1,-1);
    
    auto addButton = new QPushButton("Add",this);
    centralLayout->addWidget(addButton,4,0);
    auto removeButton = new QPushButton("Remove",this);
    centralLayout->addWidget(removeButton,4,1);
    
    auto generateButton = new QPushButton("Generate",this);
    centralLayout->addWidget(generateButton,5,0,1,-1);
    
    setLayout(centralLayout);
}