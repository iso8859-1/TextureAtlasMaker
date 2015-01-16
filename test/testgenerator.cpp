
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "generator.hpp"

#include <QFileInfo>
#include <QFile>
#include <QImage>

TEST_CASE("generated texture has the desired filename","[functional],[generator]")
{
    QString testfile = "testfile.png";
    QFile::remove(testfile);
    generateTexture(testfile,128,128);
    REQUIRE(QFileInfo::exists(testfile)==true);
}

TEST_CASE("generated texture is png file","[functional],[generator]")
{
    QString testfile("testfile.png");
    QFile::remove(testfile);
    generateTexture(testfile,128,128);
    QImage image(testfile,"PNG");
    REQUIRE(image.isNull() == false);
}

TEST_CASE("generated texture has the desired size","[functional],[generator]")
{
    QString testfile("testfile.png");
    QFile::remove(testfile);
    unsigned int width = 128;
    unsigned int height = 256;
    generateTexture(testfile, width, height);
    QImage image(testfile,"PNG");
    CHECK(image.width()==width);
    CHECK(image.height()==height);
}