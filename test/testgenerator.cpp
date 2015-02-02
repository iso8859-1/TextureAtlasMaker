
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
    textures.push_back(std::make_tuple<QString,QImage>("myTile",QImage(64,64,QImage::Format_ARGB32)));
    generateTexture(testfile, 128, textures);
    QFile tmp(DescriptionFilename(testfile));
    tmp.open(QFile::ReadOnly);
    auto description = QJsonDocument::fromJson(tmp.readAll());
    auto textureDescription = description.object();
    auto textureArray = textureDescription["textures"].toArray();
    CHECK(textureArray.empty()==false);
    auto firstTexture = textureArray[0].toObject();
    CHECK(firstTexture["ID"].toString()=="myTile");
}

TEST_CASE("generated description file contains file-info object","[functional][description]")
{
    QString testfile("testfile.png");
    Cleanup(testfile);
    generateTexture(testfile, 128, {});
    QFile tmp(DescriptionFilename(testfile));
    tmp.open(QFile::ReadOnly);
    auto descFile = QJsonDocument::fromJson(tmp.readAll()).object();
    auto fileInfo = descFile["fileinfo"].toObject();
    CHECK(fileInfo["version"].toInt()==1);
    CHECK(fileInfo["texturefile"].toString()==testfile);
    CHECK(fileInfo["tilesize"].toInt()==32);
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
