
// Object_TrackingDlg.h: 헤더 파일
//

#pragma once

#include <opencv2/opencv.hpp>
#include <dshow.h>   // DirectShow 기술을 사용하기 위해 포함해야 하는 헤더 파일

#include "define.h"

#define NotUsedOpenCV 0

// CObjectTrackingDlg 대화 상자
class CObjectTrackingDlg : public CDialogEx
{
private:
	cv::VideoCapture *mp_cap = NULL;
	cv::Mat m_frame;	
	CImage cimage_mfc;	
	TMouseCursor *t_mc = NULL;
	
	unsigned char m_Selected_Cam;	

// 컴포넌트
public:
	CComboBox m_dev_list;
	CStatic m_pc_view;

private:
	void MakeDeviceList();

public:

	
// 생성입니다.
public:
	CObjectTrackingDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OBJECT_TRACKING_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


public:
	afx_msg void OnBnClickedStartBtn();
	afx_msg void OnBnClickedStopBtn();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);	
};
