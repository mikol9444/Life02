#pragma once
#include <string>
#include "LifeDlg.h"
class GameDataReader
{
public:
    static void ReadData(const std::string& filename, SpriteInfo array[36][36]); //Read from a txt file. a dot represents a dead cell and a comma a living one 
    static void SaveDataPath(const std::string& filename, const SpriteInfo array[36][36]); //Save a grid to selected path as string parameter
    static void SaveCustom(SpriteInfo grid[36][36]); //Easy save file to selected directory 
};
