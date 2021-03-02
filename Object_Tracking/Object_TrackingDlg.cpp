
// Object_TrackingDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Object_Tracking.h"
#include "Object_TrackingDlg.h"
#include "afxdialogex.h"

#include "define.h"

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
	ON_WM_MOUSEMOVE()
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

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CObjectTrackingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CObjectTrackingDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// COM 컴포넌트 사용을 중지한다.
	CoUninitialize();

	OnBnClickedStopBtn();
}


void CObjectTrackingDlg::OnBnClickedOk()
{	
	//CDialogEx::OnOK();
}


void CObjectTrackingDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

void CObjectTrackingDlg::OnBnClickedStartBtn()
{
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

	if (capture == NULL) {
		capture = new cv::VideoCapture(pos, cv::CAP_DSHOW);

		if (!capture->isOpened()) {
			MessageBox(_T("캠을 열수 없습니다. \n"));
			return;
		}

		// picture control의 가로, 세로 크기를 구함.
		CRect r;
		GetDlgItem(IDC_PC_VIEW)->GetClientRect(&r);

		// 웹캠 크기를  width x heigh으로 지정    
		//capture->set(cv::CAP_PROP_FRAME_WIDTH, r.Width());
		//capture->set(cv::CAP_PROP_FRAME_HEIGHT, r.Height());		
	}

	// 일정 주기로 웹캠으로부터 영상을 가져오기 위해 타이머를 사용합니다. 
	SetTimer(1000, 30, NULL);
}


void CObjectTrackingDlg::OnTimer(UINT_PTR nIDEvent)
{
#if 1
	switch (nIDEvent) {

	case 1000:
		// 참고 사이트 >> https://webnautes.tistory.com/824

		//mat_frame가 입력 이미지입니다. 
		//capture->read(mat_frame); <- 아래 코드와 같음. 강사님은 아래 코드가 더 좋다고 함.
		*capture >> mat_frame;

		if (mat_frame.empty()) {
			break; // End of video stream
		}

		//이곳에 OpenCV 함수들을 적용합니다.

		//여기에서는 그레이스케일 이미지로 변환합니다.
		//cvtColor(mat_frame, mat_frame, cv::COLOR_BGR2GRAY);

		// picture control의 가로, 세로 크기를 구해서 사이즈를 맞춤.
		CRect rr;
		GetDlgItem(IDC_PC_VIEW)->GetClientRect(&rr);
		cv::resize(mat_frame, mat_frame, cv::Size(rr.Width(), rr.Height()));

		//cv::setMouseCallback("Color", OnMouseMove, this); //Select_Area

		//화면에 보여주기 위한 처리입니다.
		int bpp = 8 * mat_frame.elemSize();
		assert((bpp == 8 || bpp == 24 || bpp == 32));

		int padding = 0;
		//32 bit image is always DWORD aligned because each pixel requires 4 bytes
		if (bpp < 32)
			padding = 4 - (mat_frame.cols % 4);

		if (padding == 4)
			padding = 0;

		int border = 0;
		//32 bit image is always DWORD aligned because each pixel requires 4 bytes
		if (bpp < 32)
			border = 4 - (mat_frame.cols % 4);

		cv::Mat mat_temp;
		if (border > 0 || mat_frame.isContinuous() == false)
		{
			// Adding needed columns on the right (max 3 px)
			cv::copyMakeBorder(mat_frame, mat_temp, 0, 0, 0, border, cv::BORDER_CONSTANT, 0);
		}
		else
			mat_temp = mat_frame;

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

#endif
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CObjectTrackingDlg::OnBnClickedStopBtn()
{
	if(capture != NULL) {
		// 타이머 종료
		KillTimer(1000);

		// VideoCapture 닫기
		capture->release();

		// cv::VideoCapture객체 파괴
		delete capture;
		capture = NULL;
	}
}

void CObjectTrackingDlg::Select_Area(int event, int x, int y, int flags, void *userdata)
{
	select_pos *p_data = (select_pos *)userdata;

	cv::Mat img_result = p_data->img_color.clone();

	if (event == cv::EVENT_LBUTTONDOWN) {
		p_data->mouse_is_pressing = true;
		p_data->start_x = x;
		p_data->start_y = y;

		p_data->step = 1;
	}
	else if (event == cv::EVENT_MOUSEMOVE) {

		if (p_data->mouse_is_pressing) {
			p_data->end_x = x;
			p_data->end_y = y;

			p_data->step = 2;
		}
	}
	else if (event == cv::EVENT_LBUTTONUP) {
		p_data->mouse_is_pressing = false;

		p_data->end_x = x;
		p_data->end_y = y;

		if ((10 > (p_data->end_x - p_data->start_x))) {
			std::cout << "x 범위가 작습니다." << std::endl;
			p_data->bRange = false;
		}
		else if ((10 > (p_data->end_y - p_data->start_y))) {
			std::cout << "y 범위가 작습니다." << std::endl;
			p_data->bRange = false;
		}
		else {
			p_data->bRange = true;
		}

		p_data->step = 3;
	}
}