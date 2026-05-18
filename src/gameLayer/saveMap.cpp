#include "saveMap.h"
#include "asserts.h"

bool saveBlockDataToFile(const std::vector<Block> &blocks, const int w, const int h, const char *fileName)
{
    std::ofstream f(fileName, std::ios::binary);

    if (!f.is_open())
    {
        return false;
    }

    permaAssertCommentDevelopement(blocks.size() == w * h);
    permaAssertDevelopement(blocks.size() != 0);

    if (blocks.size() != w * h)
    {
        return false;
    }

    if (blocks.size() == 0)
    {
        return false;
    }

    f.write((const char*)&w, sizeof(w));
    f.write((const char*)&h, sizeof(h));

    f.write((const char *)blocks.data(), blocks.size() * sizeof(Block));

    return true;
}

bool writeEntireFile(const char *fileName, const void *data, size_t size)
{
    std::ofstream f(fileName, std::ios::binary);
    if (!f.is_open())
    {
        return false;
    }

    f.write(static_cast<const char *>(data), size);

    return static_cast<bool>(f);
}

bool loadBlockDataFromFile(std::vector<Block> &blocks, int &w, int &h, const char *fileName)
{
    blocks.clear();
    w = 0;
    h = 0;

    std::ifstream f(fileName, std::ios::binary);

    f.read(reinterpret_cast<char *>(&w), sizeof(w));
    f.read(reinterpret_cast<char *>(&h), sizeof(h));

    if (!f || w <= 0 || h <= 0)
    {
        f.close();
        return false;
    }

    // Probably corrupt data
    if (w > 10000)
    {
        return false;
    }

    if (h > 10000)
    {
        return false;
    }

    size_t blockCount = w * h;
    blocks.resize(blockCount);

    f.read(reinterpret_cast<char *>(blocks.data()), sizeof(Block) * blockCount);

    if (!f)
    {
        blocks.clear();
        w = 0;
        h = 0;
        f.close();
        return false;
    }

    for (int i = 0; i < blocks.size(); i++)
    {
        blocks[i].sanitize();
    }

    f.close();
    return true;
}

bool readEntireFile(const char *fileName, std::vector<unsigned char> &outData)
{
    outData.clear();

    std::ifstream f(fileName, std::ios::binary);
    if (!f.is_open())
    {
        return false;
    }

    f.seekg(0, std::ios::end);
    size_t size = f.tellg();
    f.seekg(0, std::ios::beg);

    outData.resize(size);
    f.read(reinterpret_cast<std::istream::char_type *>(outData.data()), size);

    if (!f)
    {
        outData.clear();
        return false;
    }

    return true;
}

bool readEntireFile(const char *fileName, void *buffer, size_t maxSize, size_t &bytesRead)
{
    bytesRead = 0;

    std::ifstream f(fileName, std::ios::binary);
    if (!f.is_open())
    {
        return false;
    }

    f.seekg(0, std::ios::end);
    size_t fileSize = f.tellg();
    f.seekg(0, std::ios::beg);

    size_t toRead = (fileSize < maxSize) ? fileSize : maxSize;

    f.read(static_cast<char *>(buffer), toRead);

    if (!f)
    {
        return false;
    }

    bytesRead = toRead;
    return true;
}

size_t getFileSize(const char *fileName)
{
    std::ifstream f(fileName, std::ios::binary);
    if (!f.is_open())
    {
        return 0;
    }

    return f.tellg();
}
