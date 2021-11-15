#include "DetectAndDraw.h"


using namespace std;



DetectAndDraw::DetectAndDraw() {

}


void DetectAndDraw::detectAndDraw(cv::Mat& img, cv::CascadeClassifier& face_cascade, cv::CascadeClassifier& eyes_cascade, double scale)
{
	vector<cv::Rect> faces;



	cv::Mat gray, smallImg;
	cv::Mat smallImgROI;
	double fx = 1 / scale;

	cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY); // Convert to Gray Scale
	

	// Resize the Grayscale Image
	cv::resize(gray, smallImg, cv::Size(), fx, fx, cv::INTER_LINEAR);
	cv::equalizeHist(smallImg, smallImg);

	// Detect faces of different sizes using cascade classifier
	face_cascade.detectMultiScale(smallImg, faces, 1.1,
		2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));

	// Draw circles around the faces
	for (size_t i = 0; i < faces.size(); i++)
	{
		cv::Rect r = faces[i];

		vector<cv::Rect> nestedObjects;
		cv::Point center;
		cv::Scalar colorBlue = cv::Scalar(255, 0, 0); // Blue color for Drawing tool
		cv::Scalar colorRed = cv::Scalar(0, 0, 255); // Red color for Drawing tool
		int radius;

		double aspect_ratio = (double)r.width / r.height;
		if (0.75 < aspect_ratio && aspect_ratio < 1.3)
		{
			center.x = cvRound((r.x + r.width * 0.5) * scale);
			center.y = cvRound((r.y + r.height * 0.5) * scale);
			radius = cvRound((r.width + r.height) * 0.25 * scale);
			circle(img, center, radius, colorBlue, 3, 8, 0);
		}
		else
			rectangle(img, cvPoint(cvRound(r.x * scale), cvRound(r.y * scale)),
				cvPoint(cvRound((r.x + r.width - 1) * scale),
					cvRound((r.y + r.height - 1) * scale)), colorBlue, 3, 8, 0);
		if (eyes_cascade.empty())
			continue;
		smallImgROI = smallImg(r);


		// Detection of eyes int the input image
		eyes_cascade.detectMultiScale(smallImgROI, nestedObjects, 1.1, 2,
			0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));

		// Draw circles around eyes
		for (size_t j = 0; j < nestedObjects.size(); j++)
		{
			cv::Rect nr = nestedObjects[j];
			center.x = cvRound((r.x + nr.x + nr.width * 0.5) * scale);
			center.y = cvRound((r.y + nr.y + nr.height * 0.5) * scale);
			radius = cvRound((nr.width + nr.height) * 0.25 * scale);
			circle(img, center, radius, colorRed, 3, 8, 0);
		}
	}	
}

