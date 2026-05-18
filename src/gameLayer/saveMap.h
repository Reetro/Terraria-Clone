#pragma once
#include <vector>
#include <blocks.h>
#include <fstream>

// Save all block data to a file
bool saveBlockDataToFile(const std::vector<Block> &blocks, int w, int h, const char *fileName);

// Load block data from a file
bool loadBlockDataFromFile(std::vector<Block> &blocks, int &w, int &h, const char *fileName);

// Read into vector
bool readEntireFile(const char *fileName, std::vector<unsigned char> &outData);

// Read into caller-supplied buffer (won't write more than maxSize bytes)
bool readEntireFile(const char *fileName, void *buffer, size_t maxSize, size_t &bytesRead);

// Write data to file
bool writeEntireFile(const char *fileName, const void *data, size_t size);

// Get file size
size_t getFileSize(const char *fileName);