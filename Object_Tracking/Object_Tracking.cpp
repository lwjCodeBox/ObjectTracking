
// Object_Tracking.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "Object_Tracking.h"
#include "Object_TrackingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CObjectTrackingApp

BEGIN_MESSAGE_MAP(CObjectTrackingApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CObjectTrackingApp 생성

CObjectTrackingApp::CObjectTrackingApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CObjectTrackingApp 개체입니다.

CObjectTrackingApp theApp;


// CObjectTrackingApp 초기화

BOOL CObjectTrackingApp::InitInstance()
{
	CWinApp::InitInstance();

	CObjectTrackingDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	
	return FALSE;
}

