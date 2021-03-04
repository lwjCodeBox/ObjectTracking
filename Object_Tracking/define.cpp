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
#if 1 // 영역 선택
	TMouseCursor *p_data = (TMouseCursor *)userdata;

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
			//std::cout << "x 범위가 작습니다." << std::endl;
			p_data->bRange = false;
		}
		else if ((10 > (p_data->end_y - p_data->start_y))) {
			//std::cout << "y 범위가 작습니다." << std::endl;
			p_data->bRange = false;
		}
		else {
			p_data->bRange = true;
		}

		p_data->step = 3;
	}

#elif 0 // 선긋기
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