#include "mainwindow.hpp"

#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QListView>
#include <QStringListModel>
#include <QGroupBox>
#include <QFileDialog>
#include <QLineEdit>

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
    
    void (QComboBox::*indexChanged)(int) = &QComboBox::currentIndexChanged;
    
    connect(comboBoxTextureSize,
            indexChanged,
            [=](int index)
    {
        if (index<comboBoxTileSize->currentIndex())
        {
            comboBoxTileSize->setCurrentIndex(index);
        }
    });
    
    connect(comboBoxTileSize,
            indexChanged,
            [=](int index)
    {
        if (index>comboBoxTextureSize->currentIndex())
        {
            comboBoxTextureSize->setCurrentIndex(index);
        }
    });
        
    auto filesGroup = new QGroupBox(this);
    auto statusLabel = new QLabel("0 of 65536 slots taken");
    
    auto listOfFiles = new QListView(this);
    _list = new QStringListModel(this);
    
    listOfFiles->setModel(_list);
	listOfFiles->setSelectionMode(QListView::ExtendedSelection);
	listOfFiles->setMinimumWidth(600);
    
    auto adaptLabel = [=](int index){
        //sizes as power of 2
        int textureSize = comboBoxTextureSize->currentIndex()+1;
        int tileSize = comboBoxTileSize->currentIndex()+1;
        //divide by subtracting the exponent and add by itself by squaring it
        int numberOfTilesExponent = (textureSize - tileSize)+(textureSize - tileSize);
        int numberOfTiles = 1 << numberOfTilesExponent;
        int numberOfListEntries = _list->rowCount();
        statusLabel->setText(QString::number(numberOfListEntries)+" of "+QString::number(numberOfTiles)+" slots taken");
    };
    
    
    connect(comboBoxTextureSize
            ,indexChanged
            ,adaptLabel);
    
    connect(comboBoxTileSize
            ,indexChanged
            ,adaptLabel);

	connect(_list
		, &QStringListModel::dataChanged
		, [=](const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles){ adaptLabel(0); });

	connect(_list
		, &QStringListModel::rowsRemoved
		, [=](const QModelIndex & parent, int first, int last){
		adaptLabel(0);
	});
    
    auto addButton = new QPushButton("Add",this);

	connect(addButton
		, &QPushButton::clicked
		, [=](){
		QFileDialog dialog(this);
		dialog.setFileMode(QFileDialog::ExistingFiles);
		QStringList filters;
		filters << "Textures (*.png)"
			<< "Any files (*)";
		dialog.setNameFilters(filters);
		dialog.setViewMode(QFileDialog::Detail);
		QStringList fileNames;
		if (dialog.exec())
		{
			fileNames = dialog.selectedFiles();
		}
		this->appendFiles(fileNames);
	});

    auto buttonlayout = new QHBoxLayout();
    buttonlayout->addWidget(addButton);
    auto removeButton = new QPushButton("Remove",this);

	connect(removeButton
		, &QPushButton::clicked
		, [=](){
		QStringList list;
		for (const auto & i : listOfFiles->selectionModel()->selectedRows())
		{
			_list->removeRow(i.row());
		}
	});

    buttonlayout->addWidget(removeButton);
    
    auto grouplayout = new QVBoxLayout();
    grouplayout->addWidget(statusLabel);
    grouplayout->addWidget(listOfFiles);
    grouplayout->addLayout(buttonlayout);
    
    filesGroup->setLayout(grouplayout);
    
    centralLayout->addWidget(filesGroup, 4, 0, 2, 2);
	centralLayout->setRowStretch(4, 10);
    
    auto outputLabel = new QLabel("output file:");
    centralLayout->addWidget(outputLabel,6,0,1,2);
    auto outputFilename = new QLineEdit(this);
    auto selectOutputFilenameButton = new QPushButton("...",this);
    auto hLayoutOutput = new QHBoxLayout();
    hLayoutOutput->addWidget(outputFilename);
    hLayoutOutput->addWidget(selectOutputFilenameButton);
    
    connect(selectOutputFilenameButton,
            &QPushButton::clicked,
            [=](){
                QFileDialog dialog(this);
                dialog.setFileMode(QFileDialog::AnyFile);
                QStringList filters;
                filters << "Textures (*.png)"
                << "Any files (*)";
                dialog.setNameFilters(filters);
                dialog.setViewMode(QFileDialog::Detail);
                dialog.setAcceptMode(QFileDialog::AcceptSave);
                QStringList fileName;
                if (dialog.exec())
                {
                    fileName = dialog.selectedFiles();
                    if (fileName.count()>0)
                    {
                        outputFilename->setText(fileName[0]);
                    }
                }
            });
    
    centralLayout->addLayout(hLayoutOutput, 7, 0,1,2);
    
    auto generateButton = new QPushButton("Generate",this);
    centralLayout->addWidget(generateButton,8,0,1,-1);
    
    setLayout(centralLayout);
}

void TextureAtlasMakerWidget::appendFile(const QString& filename)
{
	_list->insertRow(_list->rowCount());
	auto index = _list->index(_list->rowCount() - 1);
	_list->setData(index, filename);
}

void TextureAtlasMakerWidget::appendFiles(const QStringList& files)
{
	for (const auto& i : files)
	{
		appendFile(i);
	}
}