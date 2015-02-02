#include "generator.hpp"

#include <QFileInfo>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <set>

QJsonObject FileInformation(const QString& filename, int tileSize);

int isPowerOfTwo (unsigned int x)
{
    return ((x != 0) && ((x & (~x + 1)) == x));
}

void generateTexture(const QString& filename, unsigned int widthAndHeight, const std::vector<std::tuple<QString, QImage>>& textures)
{
    if (QFileInfo::exists(filename))
    {
        throw InvalidArgument("file does already exist");
    }
    if (!isPowerOfTwo(widthAndHeight))
    {
        throw InvalidArgument("requested texture size is not a power of 2");
    }
    //check for duplicate descriptions
    std::set<QString> keys;
    for (const auto& i : textures)
    {
        if (keys.find(std::get<0>(i))==keys.end())
        {
            keys.insert(std::get<0>(i));
        }
        else
        {
            throw InvalidArgument("multiple textures with same id detected");
        }
        auto size = std::get<1>(i).size();
        if (!isPowerOfTwo(size.height()) || !isPowerOfTwo(size.width()))
        {
            throw InvalidArgument("texture with non-power-of-2 side detected");
        }
    }
    QImage texture(widthAndHeight,widthAndHeight,QImage::Format_ARGB32);
    texture.save(filename,"PNG");
    QFile descriptionFile(DescriptionFilename(filename));
    descriptionFile.open(QFile::WriteOnly);
    QJsonObject description;
    int tileSize = 32;
    description["fileinfo"]=FileInformation(filename, tileSize);
    QJsonArray textureDescriptions;
    for (const auto& i : textures)
    {
        QJsonObject textureJson;
        textureJson["ID"] = std::get<0>(i);
        textureDescriptions.push_back(textureJson);
    }
    description["textures"]=textureDescriptions;
    QJsonDocument descriptionDoc(description);
    descriptionFile.write(descriptionDoc.toJson());
}

QJsonObject FileInformation(const QString& filename, int tileSize)
{
    QJsonObject result;
    result["version"]=1;
    result["texturefile"]=filename;
    result["tilesize"]=tileSize;
    return result;
}

QString DescriptionFilename(const QString& file)
{
    QFileInfo fi(file);
    return fi.baseName()+".json";
}