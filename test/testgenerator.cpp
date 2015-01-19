
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "generator.hpp"

#include <QFileInfo>
#include <QFile>
#include <QImage>
#include <QJsonDocument>

namespace {
    
    void Cleanup(const QString& file)
    {
        QFile::remove(file);
        QFile::remove(DescriptionFilename(file));
    }

}

TEST_CASE("generated texture has the desired filename","[functional],[generator]")
{
    QString testfile = "testfile.png";
    Cleanup(testfile);
    generateTexture(testfile,128,{});
    REQUIRE(QFileInfo::exists(testfile)==true);
}

TEST_CASE("generated texture is png file","[functional],[generator]")
{
    QString testfile("testfile.png");
    Cleanup(testfile);
    generateTexture(testfile,128,{});
    QImage image(testfile,"PNG");
    REQUIRE(image.isNull() == false);
}

TEST_CASE("generated texture has the desired size","[functional],[generator]")
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

TEST_CASE("generator generates description file","[functional][generator]")
{
    QString testfile("testfile.png");
    Cleanup(testfile);
    generateTexture(testfile, 128,{});
    REQUIRE(QFileInfo::exists(DescriptionFilename(testfile)));
}

TEST_CASE("generated description file is json","[functional],[generator]")
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