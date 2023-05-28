#include "pch.h"
#include "framework.h"
#include "Life.h"
#include "LifeDlg.h"
#include "afxdialogex.h"
#include <cstdlib>  // Include the <cstdlib> header for the rand() function
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma region Initialization

CLifeDlg::CLifeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LIFE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLifeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

HCURSOR CLifeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
BEGIN_MESSAGE_MAP(CLifeDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()

	ON_WM_LBUTTONDOWN()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CLifeDlg::OnNMCustomdrawSlider1)
END_MESSAGE_MAP()


// CLifeDlg-Meldungshandler

BOOL CLifeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	InitializeBackground();
	InitializeButtons();
	InitializeGrid();
	SoundPlayer::Play("audio/Anomaly.wav", true, 5);
	GameDataReader::ReadData("persistance/pattern0.txt",M_grid);
	//board.ToggleCell(0, 0);
	return TRUE;  // TRUE zurückgeben, wenn der Fokus nicht auf ein Steuerelement gesetzt wird
}

void CLifeDlg::InitializeBackground() {
	if (!M_buffer.Load("sprites/bmp_background_1200x1000.bmp"))
	{
		AfxMessageBox(L"background konnte nicht geladen werden");
		OnCancel();
	}
	//M_buffer.Load("sprites/bmp_background_1200x1000.bmp");
	M_spritelist.SetWorkspace(&M_buffer);
	M_cellList.SetWorkspace(&M_buffer);

	UserInterface::LoadSprite(&M_background, "sprites/bmp_background_1200x1000.bmp", 1200, 1000);
	SetWindowPos(NULL, 0, 0,
		M_background.DibWidth(), // Fensterbreite
		M_background.DibHeight() + 25, // Fensterh�he
		SWP_NOZORDER | SWP_NOMOVE);
	M_spritelist.Insert(&M_background);
	M_background.SetZ(0);

	UserInterface::LoadSprite(&M_title, "sprites/bmp_Title2_800x203.bmp", 800, 203);
	M_title.SetPosition(200, 100);
	M_spritelist.Insert(&M_title);

	static int index = 0;
	for (int i = 0; i < 7; i++)
	{
		SpriteSetup(&M_teutone[i],
			"sprites/15x_teutonall_80x100.bmp",
			CSize(80, 100), CSize(rand()%1200,
				M_background.DibHeight() - 100)
			,1
		);
		M_teutone[i].SetSpriteNumber(rand()%15,0);
	}
	//Add additional knight at the right corner of a screen. He will indicate if a Timer is active
 }



void CLifeDlg::InitializeButtons() {
	int buttonWidth = 64; // Width of each button sprite
	int buttonHeight = 64; // Height of each button sprite
	int buttonSpacingX = 10; // Horizontal spacing between buttons
	int buttonSpacingY = 10; // Vertical spacing between buttons
	int buttonsPerRow = 4; // Number of buttons per row
	int numRows = (8 + buttonsPerRow - 1) / buttonsPerRow; // Calculate the number of rows

	for (int i = 0; i < 10; i++) {
		int row = i / buttonsPerRow; // Calculate the row index
		int col = i % buttonsPerRow; // Calculate the column index

		int posX = 750 + (buttonWidth + buttonSpacingX) * col; // Calculate the X position
		int posY = 300 + (buttonHeight + buttonSpacingY) * row; // Calculate the Y position

		SpriteSetup(&M_buttons[i], "sprites/bmp_buttons_64x64.bmp", CSize(buttonWidth, buttonHeight), CSize(posX, posY),3);
		M_buttons[i].SetSpriteNumber(i, 0);
	}	
}

void CLifeDlg::SpriteSetup(CSprite* image, char* path, CSize size, CSize position,int layerID) {
	UserInterface::LoadSprite(image, path, size.cx, size.cy);
	image->SetPosition(position.cx, position.cy);
	image->SetZ(layerID);
	M_spritelist.Insert(image);
}
void CLifeDlg::InitializeGrid() {
	M_board.CreateBoard(M_grid);
	for (int k = 0; k < 36; k++)
	{
		for (int j = 0; j < 36; j++)
		{
			M_cellList.Insert(&M_grid[k][j].sprite);
		}
	}
}

#pragma endregion 

#pragma region Event Section 
// ------------------EVENT SECTION---------------------------
void CLifeDlg::OnPaint()
{
	CPaintDC dc(this); // Ger�tekontext zum Zeichnen
	M_spritelist.RedrawAll(&dc, 0, 0);
	M_cellList.RedrawAll(&dc, 0, 0);
	CDialogEx::OnPaint();
}

//------------------------------------------------------------------
// ------------------CUSTOM EVENT SECTION---------------------------


void CLifeDlg::OnTimer(UINT_PTR nIDEvent)
{
	CDialogEx::OnTimer(nIDEvent);
	CClientDC dc(this);
	if (nIDEvent == 1)
	{
		M_board.IterateArray(M_grid, N_grid);
		M_board.UpdateBoard(M_grid, N_grid);
		M_cellList.Update(&dc, 0, 0);
	}
	if (nIDEvent == 2)
	{

		MoveTeutone();
		M_spritelist.Update(&dc, 0, 0);
	}


}

void CLifeDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CDialogEx::OnMouseMove(nFlags, point);
	CClientDC dc(this);
	// TODO: Fügen Sie hier Ihren Meldungshandlercode ein, und/oder benutzen Sie den Standard.
	SetWindowText(UserInterface::DebugMousePosition(point));

	CSprite* hitButton = M_spritelist.HitTest(point);
	HoverButton(hitButton);


	CSprite* hitCell = M_cellList.HitTest(point);
	HoverCell(hitCell);
	M_cellList.Update(&dc, 0, 0);
	M_spritelist.Update(&dc, 0, 0);

}


void CLifeDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Fügen Sie hier Ihren Meldungshandlercode ein, und/oder benutzen Sie den Standard.
	MouseDown(point);
	CDialogEx::OnLButtonDown(nFlags, point);
}
//------------------------------------------------------------------

void CLifeDlg::OnNMCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Fügen Sie hier Ihren Handlercode für Benachrichtigungen des Steuerelements ein.
	CSliderCtrl* slider = (CSliderCtrl*)GetDlgItem(IDC_SLIDER1);
	CStatic* label = (CStatic*)GetDlgItem(IDC_STATIC);

	slider->SetRange(0, 100);
	M_fillrate = slider->GetPos();
	CString str;
	str.Format(L"Fillpercentage %% %d ", M_fillrate);
	label->SetWindowTextW(str);
	*pResult = 0;
}
#pragma endregion 

#pragma region Extras
//-------------------------- EXTRAS --------------------------------
void CLifeDlg::MoveTeutone() {
	for (int i = 0; i < 3; i++)
	{
		int currentNumber;
		int randSpeed = (rand() % (5)+1 );
		M_teutone[i].GetSpriteNumber(&currentNumber, 0);

		M_teutone[i].SetSpriteNumber((currentNumber+=1)%15, 0);
		M_teutone[i].GetXPos() < -80 ?                // Short version of if else statement
			M_teutone[i].SetPosition(M_background.DibWidth(), M_teutone[i].GetYPos()) :  // if true
			M_teutone[i].SetPosition(M_teutone[i].GetXPos() - randSpeed, M_teutone[i].GetYPos()); // else 
	}
	for (int i = 3; i < 7; i++)
	{
		int currentNumber;
		int randSpeed = (rand() % (5)+1 );
		M_teutone[i].GetSpriteNumber(&currentNumber, 0);

		M_teutone[i].SetSpriteNumber((currentNumber += 1) % 15, 1);
		M_teutone[i].GetXPos() > M_background.DibWidth()?                // Short version of if else statement
			M_teutone[i].SetPosition(-150, M_teutone[i].GetYPos()) :  // if true
			M_teutone[i].SetPosition(M_teutone[i].GetXPos() + randSpeed, M_teutone[i].GetYPos()); // else 
	}

}

#pragma endregion


