#include "assetManager.h"
#include <filesystem>
#include <unordered_map>
#include <string>

namespace fs = std::filesystem;

// Helper function to find the first occurrence of files in the texture packs folder
std::unordered_map<std::string, std::string> scanTexturePacks(const std::string& rootFolder)
{
    std::unordered_map<std::string, std::string> foundFiles;

    if (rootFolder.empty() || !fs::exists(rootFolder) || !fs::is_directory(rootFolder))
    {
        return foundFiles;
    }

    // Loop through each individual texture pack folder
    for (const auto& packDir : fs::directory_iterator(rootFolder))
    {
        if (packDir.is_directory())
        {
            // Recursively scan all files inside this specific texture pack
            for (const auto& entry : fs::recursive_directory_iterator(packDir.path()))
            {
                if (entry.is_regular_file())
                {
                    std::string filename = entry.path().filename().string();

                    // "First match wins": Only add to our map if we haven't found this file yet!
                    if (foundFiles.find(filename) == foundFiles.end())
                    {
                        foundFiles[filename] = entry.path().string();
                    }
                }
            }
        }
    }

    return foundFiles;
}

// Helper to assign the texture from our map, or fallback to default
void assignTexture(Texture2D& textureSlot, const std::string& filename, const std::unordered_map<std::string, std::string>& customFiles)
{
    // Check if our first-match map contains this file
    auto file = customFiles.find(filename);
    if (file != customFiles.end())
    {
        textureSlot = LoadTexture(file->second.c_str());
    }

    // Fallback: If it wasn't in any pack, or failed to load, use default resources
    if (textureSlot.id == 0)
    {
        std::string defaultPath = std::string(RESOURCES_PATH) + filename;
        textureSlot = LoadTexture(defaultPath.c_str());
    }
}

void AssetManager::loadAll(const std::string& customPath)
{
    // 1. Clear out old VRAM data
    unloadAll();

    // 2. Scan the texture packs folder exactly once to grab first-matches
    std::unordered_map<std::string, std::string> customFiles = scanTexturePacks(customPath);

    // 3. Assign textures using our map or defaults
    assignTexture(dirt, "dirt.png", customFiles);
    assignTexture(textures, "textures.png", customFiles);
    assignTexture(frame, "frame.png", customFiles);
    assignTexture(tree, "treetextures.png", customFiles);
    assignTexture(tiles, "texturesWithBackgroundVersion.png", customFiles);
    assignTexture(player, "player.png", customFiles);
    assignTexture(slime, "slime.png", customFiles);
}

void AssetManager::unloadAll()
{
    UnloadTexture(dirt);
    UnloadTexture(textures);
    UnloadTexture(frame);
    UnloadTexture(tree);
    UnloadTexture(tiles);
    UnloadTexture(player);
    UnloadTexture(slime);

    dirt = {};
    textures = {};
    frame = {};
    tree = {};
    tiles = {};
    player = {};
    slime = {};
}