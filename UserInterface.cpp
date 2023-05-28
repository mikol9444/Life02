#include "pch.h"
#include "UserInterface.h"
#include "LifeDlg.h"
using namespace std;

CString UserInterface::DebugMousePosition(CPoint point) {
	CString s;
	s.Format(L"Mauskoordinaten %d : %d", point.x, point.y);
	return s;
}
CSprite* UserInterface::LoadSprite(CSprite* sprite, char* path, int sizeX, int sizeY) {
	if (!sprite->Load(path, CSize(sizeX, sizeY)))
		AfxMessageBox(L"Sprite cant be loaded ");
	return sprite;
}

#pragma region Board class functions

void Board::CreateBoard(SpriteInfo grid[36][36]) {
	for (int i = 0; i < 36; i++)
	{
		for (int j = 0; j < 36; j++)
		{
			UserInterface::LoadSprite(&grid[i][j].sprite, "sprites/cells_16x16.bmp", 16, 16);
			SpriteInfo* info = &grid[i][j];
			info->sprite.SetZ(15);
			info->sprite.SetPosition(110 + j * 16, 300 + i * 16);
			info->isAlive = false;

		}
	}
}
bool Board::GetCellStatus(SpriteInfo* info) {
	return info->isAlive;
}
void Board::ToggleCell(SpriteInfo* info) {
	info->isAlive = !info->isAlive;
	info->isAlive ? info->sprite.SetSpriteNumber(0, 2) : info->sprite.SetSpriteNumber(0, 0);
}
void Board::IterateArray(SpriteInfo grid[36][36], int N_grid[36][36]) {
	for (int i = 0; i < 36; ++i) {
		for (int j = 0; j < 36; ++j) {
			CountNeighbours(grid, N_grid,i, j);
		}
	}
}
void Board::CountNeighbours(SpriteInfo grid[36][36], int N_grid[36][36], int k, int l) {
	for (int i = k - 1; i <= k + 1; ++i) {
		for (int j = l - 1; j <= l + 1; ++j) {
			if (i >= 0 && i < 36 && j >= 0 && j < 36 && (i != k || j != l)) {
				if (grid[i][j].isAlive)
				{
					N_grid[k][l] += 1;
				}
			}

		}
	}
}

void Board::UpdateBoard(SpriteInfo grid[36][36], int N_grid[36][36]) {
	for (int i = 0; i < 36; ++i) {
		for (int j = 0; j < 36; ++j) {
			// ATTENTION: first if statement is an easy approach to prevent gliders to stuck on borders and the output can differ from orignal game of life rules 
			// to avoid it just comment out 
			if (i == 35 || j == 35 || i==0 || j==0) {
				grid[i][j].isAlive = false;
				grid[i][j].sprite.SetSpriteNumber(0, 0);
			}

			if ((N_grid[i][j] == 2 || N_grid[i][j] == 3) && grid[i][j].isAlive) {
				grid[i][j].isAlive=true;
				grid[i][j].sprite.SetSpriteNumber(0, 2);
			}
			else if (N_grid[i][j] == 3) {
				grid[i][j].isAlive = true;
				grid[i][j].sprite.SetSpriteNumber(0, 2);
			}
			else if (N_grid[i][j] > 3 || N_grid[i][j] < 2) {
				grid[i][j].isAlive = false;
				grid[i][j].sprite.SetSpriteNumber(0, 0);
			}
			else {
				grid[i][j].isAlive = false;
				grid[i][j].sprite.SetSpriteNumber(0, 0);
			}

		}
	}
	// Reset the neighbour count
	for (int i = 0; i < 36; i++)
	{
		for (int j = 0; j < 36; j++)
		{
			N_grid[i][j] = 0;
		}
	}
}

void Board::ClearBoard(SpriteInfo grid[36][36]) {
	for (int i = 0; i < 36; i++)
	{
		for (int j = 0; j < 36; j++)
		{
			grid[i][j].isAlive = false;
			grid[i][j].sprite.SetSpriteNumber(0, 0);
		}
	}
}

#pragma endregion 

#pragma region CLifeDlg extensions / User Interactions

void CLifeDlg::MouseDown(CPoint point) {
	CSprite* hitButton = M_spritelist.HitTest(point);
	if (hitButton == &M_buttons[0]) {
		//AfxMessageBox(L"timer has been set");
		SetTimer(1, 100, 0);
		SetTimer(2, 50, 0);
	}

	if (hitButton == &M_buttons[1]) {
		CClientDC dc(this);
		M_board.IterateArray(M_grid, N_grid);
		M_board.UpdateBoard(M_grid, N_grid);
		MoveTeutone();
		M_cellList.Update(&dc, 0, 0);
		M_spritelist.Update(&dc, 0, 0);

	}
	if (hitButton == &M_buttons[2]) {
		KillTimer(1);
		KillTimer(2);
	}
	if (hitButton == &M_buttons[3]) {
		M_board.ClearBoard(M_grid);
	}
	if (hitButton == &M_buttons[4]) {
		GameDataReader::ReadData("persistance/pattern1.txt", M_grid);
	}
	if (hitButton == &M_buttons[5]) {
		GameDataReader::ReadData("persistance/pattern2.txt", M_grid);
	}
	if (hitButton == &M_buttons[6]) {
		GameDataReader::ReadData("persistance/pattern3.txt", M_grid);
	}
	if (hitButton == &M_buttons[7]) {
		//GameDataReader::SaveData("persistance/pattern4.txt", M_grid);
		GameDataReader::SaveCustom(M_grid);
	}
	if (hitButton == &M_buttons[8]) {
		M_board.ClearBoard(M_grid);
		FillRandom(M_fillrate);
	}
	if (hitButton == &M_buttons[9]) {
		exit(0);
	}
	// Toggle Cell status: dead <-> alive
	CSprite* hitCell = M_cellList.HitTest(point);
	// Very i
	for (int i = 0; i < 36; i++) {
		for (int j = 0; j < 36; j++) {
			CSprite* gridSprite = &(M_grid[i][j].sprite);
			if (hitCell == gridSprite) {
				M_board.ToggleCell(&(M_grid[i][j]));
			}
		}
	}
}

void CLifeDlg::HoverButton(CSprite* hit) {
	for (CSprite& button : M_buttons) {
		int number;
		button.GetSpriteNumber(&number, 0);
		hit == &button ? button.SetSpriteNumber(number, 1) : button.SetSpriteNumber(number, 0);
	}

}
void CLifeDlg::HoverCell(CSprite* hit) {
	for (int i = 0; i < 36; i++)
	{
		for (int j = 0; j < 36; j++)
		{
			SpriteInfo* info = &(M_grid[i][j]);
			CSprite* sprite = &(info->sprite);
			if (hit == sprite)
			{
				sprite->SetSpriteNumber(0, 1);
			}
			else {
				info->isAlive ? sprite->SetSpriteNumber(0, 2) : sprite->SetSpriteNumber(0, 0);
			}
		}
	}
}

void CLifeDlg::FillRandom(int M_fillrate) {

	for (int i = 0; i < 36; ++i) {
		for (int j = 0; j < 36; ++j) {
			if (rand()%100+1 <= M_fillrate) {
				M_grid[i][j].isAlive = true;
				M_grid[i][j].sprite.SetSpriteNumber(0, 2);
			}
			else {
				M_grid[i][j].isAlive = false;
				M_grid[i][j].sprite.SetSpriteNumber(0, 0);
			}
		}
	}
}
#pragma endregion