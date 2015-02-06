#include "generator.hpp"

#include <QFileInfo>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QColor>
#include <QPainter>

#include <set>

QJsonObject FileInformation(const QString& filename, int tileSize);

QImage prepareTexture(unsigned int widthAndHeight)
{
    QImage texture(widthAndHeight,widthAndHeight,QImage::Format_ARGB32);
    texture.fill(QColor(255,255,255,0));
    return texture;
}


class AutoSavingTexture
{
public:
    AutoSavingTexture(const QString& filename, unsigned int widthAndheight)
    : m_filename(filename)
    , m_texture(prepareTexture(widthAndheight))
    { }
    ~AutoSavingTexture()
    {
        m_texture.save(m_filename, "PNG");
    }
    
    QImage& operator->() { return m_texture; }
    QImage& getTexture() { return m_texture; }
private:
    QString m_filename;
    QImage m_texture;
};

class JsonFile
{
public:
    JsonFile(const QString& filename)
    : m_filename(filename)
    {}
    
    ~JsonFile()
    {
        QFile file(m_filename);
        file.open(QFile::WriteOnly);
        QJsonDocument descriptionDoc(m_root);
        file.write(descriptionDoc.toJson());
        file.close();
    }
    
    QJsonObject& getRoot() { return m_root; }
private:
    QJsonObject m_root;
    QString m_filename;
};

int isPowerOfTwo (unsigned int x)
{
    return ((x != 0) && ((x & (~x + 1)) == x));
}

const std::vector<int>& GetSortedListOfPowersOf2()
{
    static std::vector<int> powers = {
        //2147483648, larger than maximum int!
        1073741824,
        536870912,
        268435456,
        134217728,
        67108864,
        33554432,
        16777216,
        8388608,
        4194304,
        2097152,
        1048576,
        524288,
        262144,
        131072,
        65536,
        32768,
        16384,
        8192,
        4096,
        2048,
        1024,
        512,
        256,
        128,
        64,
        32,
        16,
        8,
        4,
        2,
        1 };
    return powers;
}

int DetectTileSize(const std::vector<std::tuple<QString, QImage>>& textures)
{
    if (textures.empty())
    {
        return 0;
    }
    //find maximum dimension for images
    int max = 0;
    std::set<int> dimensions;
    for (const auto& i : textures)
    {
        const auto& image = std::get<1>(i);
        max = std::max(image.height(), max);
        max = std::max(image.width(), max);
        dimensions.insert(image.height());
        dimensions.insert(image.width());
    }
    //find largest power of 2 that is larger or equal to max
    const auto& powers = GetSortedListOfPowersOf2();
    int index = powers.size()-1;
    while (powers[index]<max)
    {
        index-=1;
    }
    //loop through textures. If power at index is divisor of all widht() and height(), we're done
    //otherwise, decrement index.
    while (index<powers.size()-1)
    {
        bool divisible = true;
        for (auto dim : dimensions) {
            if (dim % powers[index] !=0) {
                divisible = false;
            }
        }
        if (divisible)
        {
            break;
        }
        index+=1;
    }
    return powers[index];
}

void addTile(const QString& tileid, const QImage& src, QJsonArray& descriptions, QImage& textureAtlas, int x, int y)
{
    QJsonObject textureJson;
    textureJson["ID"] = tileid;
    textureJson["width"] = src.width();
    textureJson["height"] = src.height();
    textureJson["x"] = x;
    textureJson["y"] = y;
    descriptions.push_back(textureJson);
    QPainter painter(&textureAtlas);
    painter.drawImage(QRect(x,y,src.width(),src.height()), src, QRect(0,0,src.width(),src.height()));
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
    }
    AutoSavingTexture texture(filename,widthAndHeight);
    JsonFile descriptionFile(DescriptionFilename(filename));
    auto& description = descriptionFile.getRoot();
    int tileSize = DetectTileSize(textures);
    description["fileinfo"]=FileInformation(filename, tileSize);
    QJsonArray textureDescriptions;
    int x = 0;
    int y = 0;
    for (const auto& i : textures)
    {
        addTile(std::get<0>(i), std::get<1>(i), textureDescriptions, texture.getTexture(), x, y);
        x += tileSize;
        if (x+tileSize>texture.getTexture().width())
        {
            x = 0;
            y += tileSize;
        }
    }
    description["textures"]=textureDescriptions;
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