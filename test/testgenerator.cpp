
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "generator.hpp"

#include <QFileInfo>
#include <QFile>
#include <QImage>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QColor>

namespace {
    
    void Cleanup(const QString& file)
    {
        QFile::remove(file);
        QFile::remove(DescriptionFilename(file));
    }

}

TEST_CASE("generated texture has the desired filename","[functional],[texture]")
{
    QString testfile = "testfile.png";
    Cleanup(testfile);
    generateTexture(testfile,128,{});
    REQUIRE(QFileInfo::exists(testfile)==true);
}

TEST_CASE("generated texture is png file","[functional],[texture]")
{
    QString testfile("testfile.png");
    Cleanup(testfile);
    generateTexture(testfile,128,{});
    QImage image(testfile,"PNG");
    REQUIRE(image.isNull() == false);
}

TEST_CASE("generated texture has the desired size","[functional],[texture]")
{
    QString testfile("testfile.png");
    Cleanup(testfile);
    unsigned int width = 128;
    generateTexture(testfile, width,{});
    QImage image(testfile,"PNG");
    CHECK(image.width()==width);
    CHECK(image.height()==width);
}

TEST_CASE("generator throws IllegalArgument if file already exists","[functional][generator]")
{
    QString testfile("testfile.png");
    Cleanup(testfile);
    unsigned int width = 128;
    generateTexture(testfile, width,{});
    CHECK_THROWS_AS(generateTexture(testfile, width,{}), InvalidArgument);
}

TEST_CASE("generator generates description file","[functional][description]")
{
    QString testfile("testfile.png");
    Cleanup(testfile);
    generateTexture(testfile, 128,{});
    REQUIRE(QFileInfo::exists(DescriptionFilename(testfile)));
}

TEST_CASE("generated description file is json","[functional],[description]")
{
    QString testfile("testfile.png");
    Cleanup(testfile);
    generateTexture(testfile, 128,{});
    QFile descriptionFile(DescriptionFilename(testfile));
    descriptionFile.open(QFile::ReadOnly);
    auto data = descriptionFile.readAll();
    auto doc = QJsonDocument::fromJson(data);
    REQUIRE(doc.isNull()==false);
}

TEST_CASE("generated description file contains one entry per texture in atlas","[functional][description]")
{
    QString testfile("testfile.png");
    Cleanup(testfile);
    const int numTextures = 10;
    std::vector<std::tuple<QString,QImage>> textures;
    for (int i=0; i<numTextures; ++i)
    {
        std::stringstream number;
        number<<i;
        textures.push_back(std::make_tuple<QString,QImage>(std::string("test"+number.str()).c_str(),QImage(64,64,QImage::Format_ARGB32)));
    }
    generateTexture(testfile, 128, textures);
    QFile tmp(DescriptionFilename(testfile));
    tmp.open(QFile::ReadOnly);
    auto description = QJsonDocument::fromJson(tmp.readAll());
    CHECK(description.isNull()==false);
    QJsonObject textureDescription = description.object();
    const auto& textureCollection = textureDescription["textures"].toArray();
    REQUIRE(textureCollection.count()==numTextures);
}

TEST_CASE("generator throws if multiple textures contain the same id-string","[functional][generator]")
{
    QString testfile("testfile.png");
    Cleanup(testfile);
    const int numTextures = 2;
    std::vector<std::tuple<QString,QImage>> textures;
    for (int i=0; i<numTextures; ++i)
    {
        textures.push_back(std::make_tuple<QString,QImage>("test",QImage(64,64,QImage::Format_ARGB32)));
    }
    CHECK_THROWS_AS(generateTexture(testfile, 128, textures),InvalidArgument);
}

TEST_CASE("generated description file entries contain strings passed as texture identification","[functional][description]")
{
    QString testfile("testfile.png");
    Cleanup(testfile);
    std::vector<std::tuple<QString, QImage>> textures;
    textures.push_back(std::make_tuple<QString,QImage>("myTile",QImage(64,32,QImage::Format_ARGB32)));
    generateTexture(testfile, 128, textures);
    QFile tmp(DescriptionFilename(testfile));
    tmp.open(QFile::ReadOnly);
    auto description = QJsonDocument::fromJson(tmp.readAll());
    auto textureDescription = description.object();
    auto textureArray = textureDescription["textures"].toArray();
    CHECK(textureArray.empty()==false);
    auto firstTexture = textureArray[0].toObject();
    CHECK(firstTexture["ID"].toString()=="myTile");
    CHECK(firstTexture["width"].toInt()==64);
    CHECK(firstTexture["height"].toInt()==32);
}

TEST_CASE("generated description file contains file-info object","[functional][description]")
{
    QString testfile("testfile.png");
    Cleanup(testfile);
    std::vector<std::tuple<QString, QImage>> textures;
    textures.push_back(std::make_tuple<QString,QImage>("myTile",QImage(64,64,QImage::Format_ARGB32)));
    generateTexture(testfile, 128, textures);
    QFile tmp(DescriptionFilename(testfile));
    tmp.open(QFile::ReadOnly);
    auto descFile = QJsonDocument::fromJson(tmp.readAll()).object();
    auto fileInfo = descFile["fileinfo"].toObject();
    CHECK(fileInfo["version"].toInt()==1);
    CHECK(fileInfo["texturefile"].toString()==testfile);
    CHECK(fileInfo["tilesize"].toInt()==DetectTileSize(textures));
}

TEST_CASE("generator throws if requested texture size is not a power of 2","[functional][generator]")
{
    QString testfile("testfile.png");
    Cleanup(testfile);
    CHECK_THROWS_AS(generateTexture(testfile, 100, {}), InvalidArgument);
}

//decision: this is unnecessary. Any texture format can be added. It may create smaller tiles though
//and the textures may be teared if they do not fit properly.
/*TEST_CASE("generator throws if textures to be added are not a power of 2","[functional][generator]")
{
    QString testfile("testfile.png");
    Cleanup(testfile);
    std::vector<std::tuple<QString, QImage>> textures;
    textures.push_back(std::make_tuple<QString,QImage>("myTile",QImage(60,64,QImage::Format_ARGB32)));
    CHECK_THROWS_AS(generateTexture(testfile, 128, textures), InvalidArgument);
    textures.clear();
    textures.push_back(std::make_tuple<QString,QImage>("myTile2",QImage(64,60,QImage::Format_ARGB32)));
}*/

TEST_CASE("tile size detection returns the greatest common divisor that is a power of 2","[functional][tilesizedetector]")
{
    std::vector<std::tuple<QString, QImage>> textures;
    textures.push_back(std::make_tuple<QString,QImage>("1",QImage(64,64,QImage::Format_ARGB32)));
    CHECK(DetectTileSize(textures)==64);
    textures.push_back(std::make_tuple<QString,QImage>("2",QImage(32,64,QImage::Format_ARGB32)));
    CHECK(DetectTileSize(textures)==32);
    textures.push_back(std::make_tuple<QString,QImage>("3",QImage(64,60,QImage::Format_ARGB32)));
    CHECK(DetectTileSize(textures)==4);
}

TEST_CASE("tile size detection return 0 if there are no textures","[functional][tilesizedetector]")
{
    CHECK(DetectTileSize({})==0);
}

TEST_CASE("generated texture has a transparent background","[functional][texture]")
{
    QString testfile("testfile.png");
    Cleanup(testfile);
    generateTexture(testfile, 16, {});
    QImage image(testfile,"PNG");
    
    for (int i=0; i<image.width(); ++i)
    {
        for (int k=0; k<image.height(); ++k)
        {
            auto pixel = image.pixel(i, k);
            REQUIRE(qAlpha(pixel) == 0);
        }
    }
}

TEST_CASE("generated texture contains textures passed as argument at the location described in the description file","[funtional][generator]")
{
    QString testfile("testfile.png");
    Cleanup(testfile);

    std::vector<std::tuple<QString, QImage>> textures;
    QImage blue(8,8,QImage::Format_ARGB32);
    blue.fill(QColor(0,0,255));
    textures.push_back(std::make_tuple<QString,QImage>("blue",std::move(blue)));
    QImage green(8,8,QImage::Format_ARGB32);
    green.fill(QColor(0,255,0));
    textures.push_back(std::make_tuple<QString,QImage>("green",std::move(green)));
    QImage red(8,8,QImage::Format_ARGB32);
    red.fill(QColor(255,0,0));
    textures.push_back(std::make_tuple<QString,QImage>("red",std::move(red)));
    
    generateTexture(testfile, 16, textures);
    
    QImage image(testfile,"PNG");
    QFile tmp(DescriptionFilename(testfile));
    tmp.open(QFile::ReadOnly);
    auto description = QJsonDocument::fromJson(tmp.readAll());
    auto textureDescription = description.object();
    auto textureArray = textureDescription["textures"].toArray();
    
    for (int i=0; i<textures.size(); ++i)
    {
        auto tmp = textureArray[i].toObject();
        auto ID = tmp["ID"].toString();
        auto x = tmp["x"].toInt();
        auto y = tmp["y"].toInt();
        auto width = tmp["width"].toInt();
        auto height = tmp["height"].toInt();
        REQUIRE(width!=0);
        REQUIRE(height!=0);
        for (int i=0; i<width; ++i)
        {
            for (int k=0; k<height; ++k)
            {
                if (ID == "blue")
                {
                    REQUIRE(image.pixel(x+i, y+k) == 0xff0000ff);
                }
                else if (ID == "green")
                {
                    REQUIRE(image.pixel(x+i, y+k) == 0xff00ff00);
                }
                else if (ID == "red")
                {
                    REQUIRE(image.pixel(x+i, y+k) == 0xffff0000);
                }
                else
                {
                    REQUIRE(false);
                }
            }
        }
    }
}

