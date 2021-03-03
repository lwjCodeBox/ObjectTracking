#pragma once

typedef struct select_pos{
	bool mouse_is_pressing = false;
	int start_x, start_y, end_x, end_y;
	int step = 0;

	cv::Mat img_color;
	cv::Point ptOld;
	bool bRange;
} sp;

void swap(int *v1, int *v2) {
	int temp = *v1;
	*v1 = *v2;
	*v2 = temp;
}


void On_Mouse(int event, int x, int y, int flags, void *userdata)
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