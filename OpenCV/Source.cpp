/*#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat img = imread("Lenna.png");
	namedWindow("image", WINDOW_NORMAL);
	imshow("image", img);
	waitKey(0);
	return 0;
}*/

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/** Function Headers */ 
void detectAndDisplay(Mat frame);

/** Global variables */
String face_cascade_name = "C:\\opencv\\build\\etc\\haarcascades\\haarcascade_frontalface_alt.xml"; //Usar dias barras invertidas para o caminho das pastas
String eyes_cascade_name = "C:\\opencv\\build\\etc\\haarcascades\\haarcascade_eye_tree_eyeglasses.xml"; //Usar dias barras invertidas para o caminho das pastas
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
string window_name = "Capture - Face detection";
RNG rng(12345);

/** @function main */
int main(int argc, const char** argv)
{
	//CvCapture* capture; --Version 2.4.x
	VideoCapture capture(-1); //-- Version 3.0
	Mat frame;

	//-- 1. Load the cascades
	if (!face_cascade.load(face_cascade_name)) { printf("--(!)Error loading\n"); return -1; };
	if (!eyes_cascade.load(eyes_cascade_name)) { printf("--(!)Error loading\n"); return -1; };

	//-- 2. Read the video stream
	//capture = cvCaptureFromCAM(-1); --Version 2.4.x
	//if (capture) --Version 2.4.x
	if (capture.isOpened()) //-- Version 3.0
	{
		while (true)
		{
			//frame = cvQueryFrame(capture); --Version 2.4.x
			capture.read(frame); //-- Version 3.0
		
			//-- 3. Apply the classifier to the frame
			if (!frame.empty())
			{
				detectAndDisplay(frame); //Executa uma vez para cada frame capturado
			}
			else
			{
				printf(" --(!) No captured frame -- Break!"); break;
			}

			int c = waitKey(10);
			if ((char)c == 'c') { break; }
		}
	}
	return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay(Mat frame)
{
	std::vector<Rect> faces;
	Mat frame_gray;

	cvtColor(frame, frame_gray, CV_BGR2GRAY); 
	equalizeHist(frame_gray, frame_gray); //It is a method that improves the contrast in an image, in order to stretch out the intensity range

	//-- Detect faces
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

	for (size_t i = 0; i < faces.size(); i++)
	{
		Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		ellipse(frame, center, Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);

		Mat faceROI = frame_gray(faces[i]);
		std::vector<Rect> eyes;

		//-- In each face, detect eyes
		eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

		for (size_t j = 0; j < 1; j++)
		{
			Point center(faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5);
			int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
			circle(frame, center, radius, Scalar(255, 0, 0), 4, 8, 0);
			Point a(center.x, center.y);
			line(frame, a, a, Scalar(110, 220, 0), 2, 8);
			for (size_t j = 1; j < 2; j++)
			{
				Point center(faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5);
				int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
				circle(frame, center, radius, Scalar(255, 0, 0), 4, 8, 0);
				Point b(center.x, center.y);
				line(frame, a, b, Scalar(110, 220, 0), 2, 8);
			}
		}
		

	}
	//-- Show what you got
	imshow(window_name, frame);
}