#pragma once

#ifndef __DEFINE_H__
#define __DEFINE_H__

#include <opencv2/opencv.hpp>

struct TMouseCursor{
	bool mouse_is_pressing = false;
	bool bRange;

	int start_x, start_y, end_x, end_y;
	int step = 0;

	cv::Mat img_color;
	cv::Point ptOld;

	// 우클릭 할때 씀 - 테스트용
	int rc_x, rc_y;
};
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

void swap(int *v1, int *v2);
void On_Mouse(int event, int x, int y, int flags, void *userdata);
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

#endif