
// Life.h: Hauptheaderdatei für die PROJECT_NAME-Anwendung
//

#pragma once

#ifndef __AFXWIN_H__
	#error "'pch.h' vor dieser Datei für PCH einschließen"
#endif

#include "resource.h"		// Hauptsymbole


// CLifeApp:
// Siehe Life.cpp für die Implementierung dieser Klasse
//

class CLifeApp : public CWinApp
{
public:
	CLifeApp();

// Überschreibungen
public:
	virtual BOOL InitInstance();

// Implementierung

	DECLARE_MESSAGE_MAP()
};

extern CLifeApp theApp;
