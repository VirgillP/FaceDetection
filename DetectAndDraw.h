#pragma once


#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc_c.h>


#include <iostream>
#include <stdio.h>


class DetectAndDraw
{


public:
	DetectAndDraw();

	void detectAndDraw(cv::Mat& img, cv::CascadeClassifier& face_cascade,
		cv::CascadeClassifier& eyes_cascade, double scale);

};

