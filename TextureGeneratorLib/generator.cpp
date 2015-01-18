#include "generator.hpp"

#include <QImage>
#include <QFileInfo>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>


void generateTexture(const QString& filename, unsigned int width, unsigned int height)
{
    if (QFileInfo::exists(filename))
    {
        throw InvalidArgument("file does already exist");
    }
    QImage texture(width,height,QImage::Format_ARGB32);
    texture.save(filename,"PNG");
    QFile descriptionFile(DescriptionFilename(filename));
    descriptionFile.open(QFile::WriteOnly);
    QJsonObject description;
    QJsonDocument descriptionDoc(description);
    descriptionFile.write(descriptionDoc.toJson());
}

QString DescriptionFilename(const QString& file)
{
    QFileInfo fi(file);
    return fi.baseName()+".json";
}