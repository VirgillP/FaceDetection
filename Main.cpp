// CPP program to detects faces from a camera
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file


//Include required header files from OpenCV directory
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

#include <iostream>
#include <stdio.h>  


#include "DetectAndDraw.h"

using namespace std;



int main(int argc, const char** argv)

{	
	
	cv::namedWindow("Live Recording", cv::WINDOW_AUTOSIZE);

	//Class
	DetectAndDraw detect_and_draw;


	int deviceID = 1;          // 0 = open fixed laptop camera, 1 =  open i.e. usb webcam
	int apiID = cv::CAP_OPENNI_QVGA_60HZ;
	cv::CascadeClassifier face_cascade, eyes_cascade;
	double scale = 1;

	cv::VideoCapture capture(deviceID, apiID);
	if (!capture.isOpened()) {

		cerr << "ERROR! Unable to open camera\n";
		return -1;
	}
	else {
		// Capture frames from video and detect faces
		cerr << "Face detection started... " << endl;
	}



	double fps = capture.set(cv::CAP_PROP_FPS, 30); // set the framerate of the created video stream
	cv::Size size((int)capture.get(cv::CAP_PROP_FRAME_WIDTH), (int)capture.get(cv::CAP_PROP_FRAME_HEIGHT));

	//--- INITIALIZE VIDEOWRITER
	cv::VideoWriter writer;
	cv::String filename = "./live/live.mp4v"; // name of the output video file, file is saved in project parent folder
	int codec = cv::VideoWriter::fourcc('m', 'p', '4', 'v'); // select desired codec i.e. mpeg, H264, mp4 (must be available at runtime)
	writer.open(filename, codec, fps, size);


	// check if we succeeded
	if (!writer.isOpened()) {
		cerr << "Could not open the output video file for write\n";
		return -1;

	}

	string cascadeName;
	string nestedCascadeName;

	cv::CommandLineParser parser(argc, argv,
		"{help h||}"
		"{cascade|data/lbpcascades/lbpcascade_frontalface.xml|}"
		"{nested-cascade|data/haarcascades/haarcascade_eye_tree_eyeglasses.xml|}"
	);
	if (parser.has("help"))
	{
		parser.printMessage();
		return 0;
	}
	cascadeName = parser.get<string>("cascade");
	nestedCascadeName = parser.get<string>("nested-cascade");

	if (!parser.check())
	{
		parser.printErrors();
		return 0;
	}
	if (!eyes_cascade.load(cv::samples::findFileOrKeep(nestedCascadeName)))
		cerr << "WARNING: Could not load haarcascade_eye_tree_eyeglasses." << endl;
	if (!face_cascade.load(cv::samples::findFile(cascadeName)))
	{
		cerr << "ERROR: Could not load lbpcascade_frontalface cascade." << endl;
		return -1;
	}


	cv::Mat input;

	for (;;)
	{

		// get one frame from camera to know frame size and type
		capture >> input;

		// check if we succeeded
		if (input.empty()) {
			cerr << "ERROR! blank frame grabbed\n";
			return -1;
		}

		cv::Mat frame1 = input.clone();
		detect_and_draw.detectAndDraw(frame1, face_cascade, eyes_cascade, scale);
		writer.write(frame1); // encode the frame into the videofile stream
		cv::imshow("Live Recording", frame1);

		//char c = (char)cv::waitKey(1);
		//Press q or Esc to exit from window
		//if (c == 27 || c == 'q' || c == 'Q')
		if (cv::waitKey(1) >= 0)
			break;
	}


	capture.release();
	writer.release();

	cv::destroyAllWindows();

	return 0;

}



