#pragma once
#include <QString>
#include <QImage>

#include <string>
#include <vector>
#include <memory>

class GeneratorException : public std::exception
{
public:
    GeneratorException(const std::string& message) :_what(message) {}
    const char* what() const noexcept override { return _what.c_str(); }
    
private:
    std::string _what;
};

class InvalidArgument : public GeneratorException
{
public:
    InvalidArgument(const std::string& message) : GeneratorException(message) {}
};

void generateTexture(const QString& filename, unsigned int widthAndHeight, const std::vector<QImage>& textures);
QString DescriptionFilename(const QString& file);
