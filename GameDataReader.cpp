#include "pch.h"
#include "GameDataReader.h"
#include <fstream>
#include <iostream>
#include "afxdialogex.h"
#include <windows.h>
void GameDataReader::ReadData(const std::string& filename, SpriteInfo grid[36][36])
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        AfxMessageBox(L"Failed to open file");
        return;
    }

    for (int row = 0; row < 36; ++row)
    {
        std::string line;
        if (std::getline(file, line))
        {
            for (int col = 0; col < 36 && col < line.length(); ++col)
            {

                if (line[col] == ',')
                {
                    grid[row][col].isAlive=true;
                }

                else
                {
                    grid[row][col].isAlive=false;
                }

            }
        }
        else
        {
            AfxMessageBox(L"Failed to read line");

            break;
        }
    }
    //AfxMessageBox(L"Pattern loaded successfully");
    file.close();
}

void GameDataReader::SaveDataPath(const std::string& filename, const SpriteInfo grid[36][36])
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        AfxMessageBox(L"Failed to create file");
        return;
    }

    for (int row = 0; row < 36; ++row)
    {
        for (int col = 0; col < 36; ++col)
        {
            // Write ',' for alive cells and '.' for dead cells
            if (grid[row][col].isAlive)
            {
                file << ',';
            }
            else
            {
                file << '.';
            }
        }
        file << std::endl;  // Move to the next line after each row
    }

    file.close();
    AfxMessageBox(L"Pattern saved successfully");
}
void GameDataReader::SaveCustom(SpriteInfo grid[36][36])
{
    OPENFILENAMEA ofn;
    CHAR szFileName[MAX_PATH] = "";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = "txt";

    if (GetSaveFileNameA(&ofn))
    {
        std::ofstream file(ofn.lpstrFile);
        if (!file.is_open())
        {
            std::cout << "Failed to create file" << std::endl;
            return;
        }

        for (int row = 0; row < 36; ++row)
        {
            for (int col = 0; col < 36; ++col)
            {
                // Write ',' for alive cells and '.' for dead cells
                if (grid[row][col].isAlive)
                {
                    file << ',';
                }
                else
                {
                    file << '.';
                }
            }
            file << std::endl;  // Move to the next line after each row
        }

        file.close();
    }
    else
    {
        DWORD error = CommDlgExtendedError();
    }
}