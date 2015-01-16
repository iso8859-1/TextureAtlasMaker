#include "generator.hpp"

#include <QImage>

void generateTexture(const QString& filename, unsigned int width, unsigned int height)
{
    QImage texture(width,height,QImage::Format_ARGB32);
    texture.save(filename,"PNG");
}