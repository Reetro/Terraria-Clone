#include "saveMap.h"
#include "asserts.h"

struct BlockSaveRepresentation1
{

    std::uint16_t type = 0;

    Block toBlock()
    {
        Block b;
        b.type = type;
        return b;
    }

};

const int VERSION = 1;


BlockSaveRepresentation1 toBlockRepresentation(Block b)
{
    BlockSaveRepresentation1 rez;
    rez.type = b.type;
    return rez;
}

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

    f.write((const char *)&VERSION, sizeof(VERSION));
    f.write((const char*)&w, sizeof(w));
    f.write((const char*)&h, sizeof(h));

    for (int i = 0; i < blocks.size(); i++)
    {
        auto b = toBlockRepresentation(blocks[i]);
        f.write((const char *)&b.type, sizeof(b.type));
    }

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

    if (!f.is_open()) { return false; }

    int readVersion = 0;

    f.read((char *)&readVersion, sizeof(readVersion));
    f.read((char *)&w, sizeof(w));
    f.read((char *)&h, sizeof(h));

    if (!f || w <= 0 || h <= 0)
    {
        f.close();
        return false;
    }

    if (w > 10000) { f.close(); return false; } //probably corrupt data
    if (h > 10000) { f.close(); return false; } //probably corrupt data

    // Read block data

    switch (readVersion)
    {
        case 1:
        {

            size_t blockCount = w * h;
            blocks.resize(blockCount);

            for (int i = 0; i < blockCount; i++)
            {
                BlockSaveRepresentation1 read;
                f.read((char *)&read, sizeof(read));

                if (!f)
                {
                    blocks.clear();
                    w = 0;
                    h = 0;
                    f.close();
                    return false;
                }

                blocks[i] = read.toBlock();
            }
            break;
        }

        default:
        {
            //incorect version
            w = 0;
            h = 0;
            f.close();
            return false;
        }
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
