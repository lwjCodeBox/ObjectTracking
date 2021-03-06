
// Object_TrackingDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Object_Tracking.h"
#include "Object_TrackingDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Direct Show 기술을 사용하기 위해 포함시켜야 하는 라이브러리
#pragma comment(lib, "strmiids.lib")

// CObjectTrackingDlg 대화 상자

CObjectTrackingDlg::CObjectTrackingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OBJECT_TRACKING_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CObjectTrackingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_dev_list);
	DDX_Control(pDX, IDC_PC_VIEW, m_pc_view);
}

BEGIN_MESSAGE_MAP(CObjectTrackingDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_Start_BTN, &CObjectTrackingDlg::OnBnClickedStartBtn)
	ON_BN_CLICKED(IDC_STOP_BTN, &CObjectTrackingDlg::OnBnClickedStopBtn)
	ON_BN_CLICKED(IDOK, &CObjectTrackingDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CObjectTrackingDlg::OnBnClickedCancel)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CObjectTrackingDlg 메시지 처리기

BOOL CObjectTrackingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);   // COM 컴포넌트를 사용하기 위해! 
	MakeDeviceList();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void CObjectTrackingDlg::MakeDeviceList()
{
	int index = 0;
	ICreateDevEnum *p_create_dev_enum;
	ULONG fetched_count;
	IMoniker *p_moniker;
	IPropertyBag *p_property_bag;
	VARIANT var;
	var.vt = VT_BSTR;

	// 시스템에 연결된 장치 정보를 얻는 컴포넌트를 생성한다.
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void **)&p_create_dev_enum);
	if (SUCCEEDED(hr)) {
		// 영상 장치와 관련된 장치 정보를 구성하는 인터페이스를 생성한다.
		IEnumMoniker *p_em;
		hr = p_create_dev_enum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &p_em, 0);

		if (SUCCEEDED(hr)) {
			p_em->Reset();
			// 영상 장치 목록을 탐색한다.
			while (hr = p_em->Next(1, &p_moniker, &fetched_count), hr == S_OK) {
				hr = p_moniker->BindToStorage(0, 0, IID_IPropertyBag, (void **)&p_property_bag);
				if (SUCCEEDED(hr)) {
					// 장치의 이름을 얻는다.
					hr = p_property_bag->Read(L"FriendlyName", &var, NULL);
					if (hr == NOERROR) {
						// 정상적으로 장치 이름을 얻었다면 리스트 박스에 추가하고 해당 장치를
						// 사용하기 위한 인터페이스를 해당 항목에 추가로 저장한다.						
						index = m_dev_list.InsertString(index, (LPCTSTR)var.bstrVal);
						m_dev_list.SetItemDataPtr(index, p_moniker);
						SysFreeString(var.bstrVal);
						p_moniker->AddRef();

						index++;
					}
					p_property_bag->Release();
				}
			}
			p_em->Release();

			// 첫 번째 영상 장치를 기본으로 선택한다.
			if (m_dev_list.GetCount()) m_dev_list.SetCurSel(0);
		}
		p_create_dev_enum->Release();
	}
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CObjectTrackingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CObjectTrackingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void CObjectTrackingDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// COM 컴포넌트 사용을 중지한다.
	CoUninitialize();

	OnBnClickedStopBtn();
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void CObjectTrackingDlg::OnBnClickedOk()
{	
	//CDialogEx::OnOK();

	// face detection configuration
	cv::CascadeClassifier face_classifier;

	face_classifier.load("{YOUR_OPENCV_PATH}/data/haarcascades/haarcascade_frontalface_default.xml");

}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void CObjectTrackingDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void CObjectTrackingDlg::OnBnClickedStartBtn()
{
#if NotUsedOpenCV
	int pos = m_dev_list.GetCurSel(); // 현재 콤보 박스에 선택된 위치를 얻어옴.

	CRect rect;
	GetClientRect(&rect);

	CDC *pDC = m_pc_view.GetWindowDC();

	CBrush myBrush(RGB(0, 255, 0)); // dialog background color <- 요기 바꾸면 됨.
	CBrush *pOld = pDC->SelectObject(&myBrush);
	BOOL bRes = pDC->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOld); // restore old brush

	myBrush.DeleteObject();
	ReleaseDC(pDC);

	if (mp_cap == NULL) {
		mp_cap = new cv::VideoCapture(pos, cv::CAP_DSHOW);

		if (!mp_cap->isOpened()) {
			MessageBox(_T("캠을 열수 없습니다. \n"));
			return;
		}
		
	/*
		// picture control의 가로, 세로 크기를 구함.
		CRect r;
		GetDlgItem(IDC_PC_VIEW)->GetClientRect(&r);

		// 웹캠 크기를  width x heigh으로 지정    
		//capture->set(cv::CAP_PROP_FRAME_WIDTH, r.Width());
		//capture->set(cv::CAP_PROP_FRAME_HEIGHT, r.Height());		
	*/
	}

	// 일정 주기로 웹캠으로부터 영상을 가져오기 위해 타이머를 사용합니다. 
	SetTimer(1000, 30, NULL);

	
#else

	if (t_mc == NULL) {
		t_mc = new TMouseCursor;

		m_Selected_Cam = m_dev_list.GetCurSel(); // 현재 콤보 박스에 선택된 위치를 얻어옴.

		if (mp_cap == NULL) {
			mp_cap = new cv::VideoCapture(m_Selected_Cam, cv::CAP_DSHOW);
			
			if (!mp_cap->isOpened()) {
				MessageBox(_T("캠을 열수 없습니다. \n"));
				return;
			}						
		}

		CRect rect;
		GetClientRect(&rect);

		CDC *pDC = m_pc_view.GetWindowDC();

		CBrush myBrush(RGB(0, 255, 0)); // dialog background color <- 요기 바꾸면 됨.
		CBrush *pOld = pDC->SelectObject(&myBrush);
		BOOL bRes = pDC->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
		pDC->SelectObject(pOld); // restore old brush

		myBrush.DeleteObject();
		ReleaseDC(pDC);

		// 일정 주기로 웹캠으로부터 영상을 가져오기 위해 타이머를 사용합니다. 
		SetTimer(1000, 30, NULL);
				
		cv::namedWindow("WebCam", cv::WINDOW_AUTOSIZE); // 창 조절 불가능.
		//cv::namedWindow("WebCam", cv::WINDOW_NORMAL); // 창 조절 가능.		
		
	/*
		// 타이틀 창 삭제.
		cv::setWindowProperty("WebCam", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);
		cv::moveWindow("WebCam", 500, 200);
	
		// picture control의 가로, 세로 크기를 구함.
		CRect r;
		GetDlgItem(IDC_PC_VIEW)->GetClientRect(&r);
		ScreenToClient(&r);
		cv::resizeWindow("WebCam", cv::Size(r.Width(), r.Height()));
	*/
		cv::setMouseCallback("WebCam", On_Mouse, t_mc);
}
#endif
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void CObjectTrackingDlg::OnTimer(UINT_PTR nIDEvent)
{
#if NotUsedOpenCV
	switch (nIDEvent) {

	case 1000:
		*mp_cap >> m_frame;
		
		// End of video stream
		if (m_frame.empty()) break; 		

		
		// 참고 사이트 >> https://webnautes.tistory.com/824

		//mat_frame가 입력 이미지입니다. 
		//capture->read(m_frame); <- 아래 코드와 같음. 강사님은 아래 코드가 더 좋다고 함.
		*mp_cap >> m_frame;

		if (m_frame.empty()) {
			break; // End of video stream
		}

		//이곳에 OpenCV 함수들을 적용합니다.

		//여기에서는 그레이스케일 이미지로 변환합니다.
		//cvtColor(mat_frame, mat_frame, cv::COLOR_BGR2GRAY);

		// picture control의 가로, 세로 크기를 구해서 사이즈를 맞춤.
		CRect rr;
		GetDlgItem(IDC_PC_VIEW)->GetClientRect(&rr);
		cv::resize(m_frame, m_frame, cv::Size(rr.Width(), rr.Height()));

		//화면에 보여주기 위한 처리입니다.
		int bpp = 8 * m_frame.elemSize();
		assert((bpp == 8 || bpp == 24 || bpp == 32));

		int padding = 0;
		//32 bit image is always DWORD aligned because each pixel requires 4 bytes
		if (bpp < 32)
			padding = 4 - (m_frame.cols % 4);

		if (padding == 4)
			padding = 0;

		int border = 0;
		//32 bit image is always DWORD aligned because each pixel requires 4 bytes
		if (bpp < 32)
			border = 4 - (m_frame.cols % 4);

		cv::Mat mat_temp;
		if (border > 0 || m_frame.isContinuous() == false)
		{
			// Adding needed columns on the right (max 3 px)
			cv::copyMakeBorder(m_frame, mat_temp, 0, 0, 0, border, cv::BORDER_CONSTANT, 0);
		}
		else
			mat_temp = m_frame;

		RECT r;
		m_pc_view.GetClientRect(&r);
		cv::Size winSize(r.right, r.bottom);

		cimage_mfc.Create(winSize.width, winSize.height, 24);

		BITMAPINFO *bitInfo = (BITMAPINFO *)malloc(sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));
		bitInfo->bmiHeader.biBitCount = bpp;
		bitInfo->bmiHeader.biWidth = mat_temp.cols;
		bitInfo->bmiHeader.biHeight = -mat_temp.rows;
		bitInfo->bmiHeader.biPlanes = 1;
		bitInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitInfo->bmiHeader.biCompression = BI_RGB;
		bitInfo->bmiHeader.biClrImportant = 0;
		bitInfo->bmiHeader.biClrUsed = 0;
		bitInfo->bmiHeader.biSizeImage = 0;
		bitInfo->bmiHeader.biXPelsPerMeter = 0;
		bitInfo->bmiHeader.biYPelsPerMeter = 0;


		//그레이스케일 인경우 팔레트가 필요
		if (bpp == 8)
		{
			RGBQUAD *palette = bitInfo->bmiColors;
			for (int i = 0; i < 256; i++)
			{
				palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
				palette[i].rgbReserved = 0;
			}
		}

		// Image is bigger or smaller than into destination rectangle
		// we use stretch in full rect

		if (mat_temp.cols == winSize.width && mat_temp.rows == winSize.height)
		{
			// source and destination have same size
			// transfer memory block
			// NOTE: the padding border will be shown here. Anyway it will be max 3px width

			SetDIBitsToDevice(cimage_mfc.GetDC(),
				//destination rectangle
				0, 0, winSize.width, winSize.height,
				0, 0, 0, mat_temp.rows,
				mat_temp.data, bitInfo, DIB_RGB_COLORS);
		}
		else
		{
			// destination rectangle
			int destx = 0, desty = 0;
			int destw = winSize.width;
			int desth = winSize.height;

			// rectangle defined on source bitmap
			// using imgWidth instead of mat_temp.cols will ignore the padding border
			int imgx = 0, imgy = 0;
			int imgWidth = mat_temp.cols - border;
			int imgHeight = mat_temp.rows;

			StretchDIBits(cimage_mfc.GetDC(),
				destx, desty, destw, desth,
				imgx, imgy, imgWidth, imgHeight,
				mat_temp.data, bitInfo, DIB_RGB_COLORS, SRCCOPY);
		}

		HDC dc = ::GetDC(m_pc_view.m_hWnd);
		cimage_mfc.BitBlt(dc, 0, 0);

		::ReleaseDC(m_pc_view.m_hWnd, dc);

		cimage_mfc.ReleaseDC();
		cimage_mfc.Destroy();

		free(bitInfo);

		break;		
#else	
	switch (nIDEvent) {

	case 1000:		
		*mp_cap >> m_frame;
		
		if (m_frame.empty()) {
			break; // End of video stream
		}
				
		switch (t_mc->step) {
		case 1:
			cv::circle(m_frame, cv::Point(t_mc->start_x, t_mc->start_y), 7, cv::Scalar(0, 10, 250), -1); 
			break;

		case 2:
			cv::circle(m_frame, cv::Point(t_mc->start_x, t_mc->start_y), 7, cv::Scalar(250, 10, 0), -1);
			cv::rectangle(m_frame, cv::Point(t_mc->start_x, t_mc->start_y), cv::Point(t_mc->end_x, t_mc->end_y), cv::Scalar(0, 255, 0), 2);
			break;

		case 3:
			if (t_mc->bRange) {				
				/*
				cv::Mat ROI(m_frame, cv::Rect(t_mc->start_x, t_mc->start_y, t_mc->end_x - t_mc->start_x, t_mc->end_y - t_mc->start_y));
				cv::cvtColor(ROI, ROI, cv::COLOR_BGR2GRAY);
				cv::Canny(ROI, ROI, 150, 50);
				cv::cvtColor(ROI, ROI, cv::COLOR_GRAY2BGR);

				ROI.copyTo(m_frame(cv::Rect(t_mc->start_x, t_mc->start_y, t_mc->end_x - t_mc->start_x, t_mc->end_y - t_mc->start_y)));
				*/

				// 선택한 영역 표시.
				cv::rectangle(m_frame, cv::Point(t_mc->start_x, t_mc->start_y), cv::Point(t_mc->end_x, t_mc->end_y), cv::Scalar(255, 0, 0), 2);
								
				char strStart[15], strEnd[15];
				sprintf_s(strStart, "%d, %d", t_mc->start_x, t_mc->start_y);
				sprintf_s(strEnd, "%d, %d", t_mc->end_x, t_mc->end_y);
				cv::putText(m_frame, strStart, cv::Point(t_mc->start_x, t_mc->start_y - 3), cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(255, 255, 120), 1);
				cv::putText(m_frame, strEnd, cv::Point(t_mc->end_x - 20, t_mc->end_y + 15), cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(255, 255, 120), 1);		
				
				///////////////////////////////////////////////////////////////////////////////////
				// https://webnautes.tistory.com/1354
				// CamShift 기능 추가.

				t_mc->step = 5;
				///////////////////////////////////////////////////////////////////////////////////

			}
		
			break;

		case 4: // 우클릭 할때 씀 - 테스트용
			cv::circle(m_frame, cv::Point(t_mc->rc_x, t_mc->rc_y), 7, cv::Scalar(0, 10, 250), -1);

			char strStart[15];
			sprintf_s(strStart, "%d, %d", t_mc->rc_x, t_mc->rc_y);
			cv::putText(m_frame, strStart, cv::Point(20, 20), cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(255, 255, 120), 1);
			
			break;

		case 5:
			//cv::Mat bp, img_hsv;
			//
			//cv::cvtColor(m_frame, img_hsv, cv::COLOR_BGR2HSV);
			//cv::calcBackProject(&img_hsv, 1, channels, objectHistogram, bp, histRange);

			//// Tracking
			//cv::meanShift(bp, roi, cv::TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 10, 1));

			//cv::rectangle(img_color, roi, cv::Scalar(0, 0, 255), 2);
			break;
		}	

	

		// click window close "X" button	
		if (-1 == cv::getWindowProperty("WebCam", 0))
			OnBnClickedStopBtn();
		else
			cv::imshow("WebCam", m_frame);
#endif
	}

	CDialogEx::OnTimer(nIDEvent);
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void CObjectTrackingDlg::OnBnClickedStopBtn()
{
	CRect rect;
	GetClientRect(&rect);

	CDC *pDC = m_pc_view.GetWindowDC();

	CBrush myBrush(RGB(255, 255, 0)); // dialog background color <- 요기 바꾸면 됨.
	CBrush *pOld = pDC->SelectObject(&myBrush);
	BOOL bRes = pDC->PatBlt(0, 0, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOld); // restore old brush

	myBrush.DeleteObject();
	ReleaseDC(pDC);

	if(mp_cap != NULL) {
		// 타이머 종료
		KillTimer(1000);

		// VideoCapture 닫기
		mp_cap->release();

		// cv::VideoCapture객체 파괴
		delete mp_cap;
		mp_cap = NULL;

		// 구조체 동적할당 파괴
		delete t_mc;
		t_mc = NULL;
	
		cv::destroyAllWindows();
	}
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void CObjectTrackingDlg::OnLButtonDown(UINT nFlags, CPoint point)
{		
	//https://take-a-step-first.tistory.com/127
	CRect rt;  // 픽쳐 컨트롤의 사각형 영역 조사
	// 픽쳐 컨트롤의 사각형 좌표를 구함
	// GetClientRect는 크기를 구하니 GetWindowRect 이용...
	GetDlgItem(IDC_PC_VIEW)->GetWindowRect(&rt);

	// GetWindowRect 로 얻은 좌표는 스크린 좌표(물리적) 이니 이를
	// 화면 좌표(논리적, 클라이언트 좌표)로 변환
	ScreenToClient(&rt);

	// 픽쳐 컨트롤의 사각형 영역에 마우스 클릭 좌표(point) 가 있으면...TRUE
	if (rt.PtInRect(point)) {

		CString str;
		str.Format(L"[x >> %d] [y >> %d]", point.x, point.y);		
		//AfxMessageBox(str);		
	}
	else  // 픽쳐 컨트롤 영역에 마우스 클릭 좌표가 없으면... FALSE
		AfxMessageBox(L"Other areas!!");

	
	CDialogEx::OnLButtonDown(nFlags, point);
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+