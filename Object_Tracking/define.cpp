#include "pch.h"
#include "define.h"

void swap(int *v1, int *v2) {
	int temp = *v1;
	*v1 = *v2;
	*v2 = temp;
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void On_Mouse(int event, int x, int y, int flags, void *userdata)
{
#if 1 // ���� ����
	TMouseCursor *p_data = (TMouseCursor *)userdata;

	//cv::Mat img_result = p_data->img_color.clone();

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
		
		// ���콺 �巡�� ������ �����ʿ��� �������� ����� ���. 
		if (p_data->start_x > p_data->end_x) {
			swap(&p_data->start_x, &p_data->end_x);
		}
		// ���콺 �巡�� ������ �Ʒ����� �������� ����� ���.
		if (p_data->start_y > p_data->end_y) {
			swap(&p_data->start_y, &p_data->end_y);
		}
			
		// â ������ ����.		
		cv::Rect r = cv::getWindowImageRect("WebCam");		
		int w = r.width;
		int h = r.height;

		// ���� ���� ����.
		p_data->bRange = true;

		// ������ ������ ������ â�� ��� ���.
		if (p_data->start_x <= -1 || p_data->start_y <= -1 ||
			p_data->end_x > w || p_data->end_y > h)
			p_data->bRange = false;


		// ������ ������ ��ġ �� �ߵ����� ���.
		if (p_data->end_x == p_data->start_x || p_data->end_y == p_data->start_y)
			p_data->bRange = false;
		 						
		p_data->step = 3;
	}
	// ��Ŭ�� �Ҷ� �� - �׽�Ʈ��
	else if (event == cv::EVENT_RBUTTONDOWN) {			
		p_data->rc_x = x;
		p_data->rc_y = y;

		p_data->step = 4;
	}

#elif 0 // ���߱�
	switch (event) {
	case EVENT_LBUTTONDOWN:
		ptOld = Point(x, y);
		cout << "EVENT_LBUTTONDOWN: " << x << ", " << y << endl;
		break;
	case EVENT_LBUTTONUP:
		cout << "EVENT_LBUTTONUP: " << x << ", " << y << endl;
		break;
	case EVENT_MOUSEMOVE:
		if (flags & EVENT_FLAG_LBUTTON) {
			line(img, ptOld, Point(x, y), Scalar(0, 255, 255), 2);
			imshow("img", img);
			ptOld = Point(x, y);
		}
		break;
	default:
		break;
	}
#endif
}
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+