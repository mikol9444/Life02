
// LifeDlg.h: Headerdatei
//
#pragma once
#include "spritelib.h"
#include "UserInterface.h"
#include "SoundPlayer.h"
#include "GameDataReader.h"

// CLifeDlg-Dialogfeld
class CLifeDlg : public CDialogEx
{
public:
	CLifeDlg(CWnd* pParent = nullptr);	// Standardkonstruktor
	// Dialogfelddaten
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LIFE_DIALOG };
#endif
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNMCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult);


protected:
	
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	HICON m_hIcon;
	DECLARE_MESSAGE_MAP()
	//-------------------CUSTOM---------------------
	//--------------FIELDS/VARIABLES----------------
	CDIB M_buffer;  // Sprite buffer 
	CSprite M_background; // Background image
	CSprite M_title;	// Title text
	CSprite M_buttons[10];	// Array of sprites used for representation of Buttons
	CSprite M_teutone[7];	// little extras :P
	CSpriteList M_spritelist; // List of sprites 
	CSpriteList M_cellList; //List of Cellsprites to update it independent from userinterface
	Board M_board;	// Custom Board class with usefull functions to initialize and update M_grid
	SpriteInfo M_grid[36][36]; // Grid of Structs with a Sprite and a bool variable representing a single Cell in the game
	int N_grid[36][36] = {}; // Grid of integers to save and count the neighbours of living cells for next generations
	int M_fillrate; // Range 0-100% in CSliderCtrl 

	//------------------------------LifeDlg.cpp------------------------------
	virtual void InitializeBackground(); // Set workspaces for each CspriteList and initialize Background , Title , and Teutone
	virtual void InitializeButtons(); // Createas a grid of Buttons with 4 columns
	virtual void SpriteSetup(CSprite* image, char* path, CSize size, CSize positio,int layerID); //Helper function for quick Image setup 
	virtual void InitializeGrid(); //Call A CreateBoardFunction and Insert sprites in spritelist
	virtual void MoveTeutone(); // Moves Teutonen-knights left and right with randomized speedrange 

	//------------------------------UserInterface.cpp --------------------------
	virtual void HoverButton(CSprite* hit); //Hover specified button if hit == mousecursor
	virtual void HoverCell(CSprite* hit); // Hover a cell if hit == mousecurspr
	virtual void MouseDown(CPoint point); // check for collisions with sprites on mousebuttondown
	virtual void FillRandom(int M_fillrate); //can be selected while dragging slider handle 
};
