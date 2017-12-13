#include <sstream>
#include <string>
#include <iostream>

#include <ctime>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <thread>

#include<stdio.h>
#include<string.h>    //strlen

#include <netdb.h>
#include <netinet/in.h>

#include <math.h>

//#include <opencv2\highgui.h>
#include "opencv2/highgui/highgui.hpp"
//#include <opencv2\cv.h>
#include "opencv2/opencv.hpp"

#include "sockets.h"

// Converts degrees to radians.
#define degreesToRadians(angleDegrees) (angleDegrees * M_PI / 180.0)

// Converts radians to degrees.
#define radiansToDegrees(angleRadians) (angleRadians * 180.0 / M_PI)

using namespace std;
using namespace cv;
//initial min and max HSV filter values.
//these will be changed using trackbars
int H_MIN = 0;
int H_MAX = 256;
int S_MIN = 0;
int S_MAX = 256;
int V_MIN = 0;
int V_MAX = 256;
int ind=0;
int lastInd=-1;
//default capture width and height
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;
//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS = 50;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 20 * 20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT*FRAME_WIDTH / 1.5;
//names that will appear at the top of each window
const std::string windowName = "Original Image";
const std::string windowName1 = "HSV Image";
const std::string windowName2 = "Thresholded Image";
const std::string windowName3 = "After Morphological Operations";
const std::string trackbarWindowName = "Trackbars";

typedef struct _color_tracking
{
	int H_MIN;
	int H_MAX;
	int S_MIN;
	int S_MAX;
	int V_MIN;
	int V_MAX;
}color_tracking;

color_tracking tracking[5];

void on_mouse(int e, int x, int y, int d, void *ptr)
{
	if (e == EVENT_LBUTTONDOWN)
	{
		cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
}

void on_trackbar(int, void*)
{//This function gets called whenever a
 // trackbar position is changed

	if(ind!=lastInd)
	{
		setTrackbarPos("H_MIN", trackbarWindowName, tracking[ind].H_MIN);
		setTrackbarPos("H_MAX", trackbarWindowName, tracking[ind].H_MAX);
		setTrackbarPos("S_MIN", trackbarWindowName, tracking[ind].S_MIN);
		setTrackbarPos("S_MAX", trackbarWindowName, tracking[ind].S_MAX);
		setTrackbarPos("V_MIN", trackbarWindowName, tracking[ind].V_MIN);
		setTrackbarPos("V_MAX", trackbarWindowName, tracking[ind].V_MAX);
	}
	else
	{
		 tracking[ind].H_MIN = H_MIN;
		 tracking[ind].H_MAX = H_MAX;
		 tracking[ind].S_MIN = S_MIN;
		 tracking[ind].S_MAX = S_MAX;
		 tracking[ind].V_MIN = V_MIN;
		 tracking[ind].V_MAX = V_MAX;
	}
	lastInd = ind;
}

void createTrackbars() {
	//create window for trackbars


	namedWindow(trackbarWindowName, 0);
	//create memory to store trackbar name on window
	char TrackbarName[50];
	sprintf(TrackbarName, "H_MIN", H_MIN);
	sprintf(TrackbarName, "H_MAX", H_MAX);
	sprintf(TrackbarName, "S_MIN", S_MIN);
	sprintf(TrackbarName, "S_MAX", S_MAX);
	sprintf(TrackbarName, "V_MIN", V_MIN);
	sprintf(TrackbarName, "V_MAX", V_MAX);
	sprintf(TrackbarName, "TARGET: 0-us 1-usfront 2-them", ind);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH),
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->
	createTrackbar("H_MIN", trackbarWindowName, &H_MIN, 256, on_trackbar);
	createTrackbar("H_MAX", trackbarWindowName, &H_MAX, 256, on_trackbar);
	createTrackbar("S_MIN", trackbarWindowName, &S_MIN, 256, on_trackbar);
	createTrackbar("S_MAX", trackbarWindowName, &S_MAX, 256, on_trackbar);
	createTrackbar("V_MIN", trackbarWindowName, &V_MIN, 256, on_trackbar);
	createTrackbar("V_MAX", trackbarWindowName, &V_MAX, 256, on_trackbar);
	createTrackbar("TARGET: 0-us 1-usfront 2-them", trackbarWindowName, &ind, 2, on_trackbar);
}

string intToString(int number) {

	std::stringstream ss;
	ss << number;
	return ss.str();
}

void drawObject(int x, int y, Mat &frame) {

	//use some of the openCV drawing functions to draw crosshairs
	//on your tracked image!

	//UPDATE:JUNE 18TH, 2013
	//added 'if' and 'else' statements to prevent
	//memory errors from writing off the screen (ie. (-25,-25) is not within the window!)

	circle(frame, Point(x, y), 20, Scalar(0, 255, 0), 2);
	if (y - 25 > 0)
		line(frame, Point(x, y), Point(x, y - 25), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(x, 0), Scalar(0, 255, 0), 2);
	if (y + 25 < FRAME_HEIGHT)
		line(frame, Point(x, y), Point(x, y + 25), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(x, FRAME_HEIGHT), Scalar(0, 255, 0), 2);
	if (x - 25 > 0)
		line(frame, Point(x, y), Point(x - 25, y), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(0, y), Scalar(0, 255, 0), 2);
	if (x + 25 < FRAME_WIDTH)
		line(frame, Point(x, y), Point(x + 25, y), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(FRAME_WIDTH, y), Scalar(0, 255, 0), 2);

	putText(frame, intToString(x) + "," + intToString(y), Point(x, y + 30), 1, 1, Scalar(0, 255, 0), 2);
	//cout << "x,y: " << x << ", " << y;

}
void morphOps(Mat &thresh) {

	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 3px by 3px rectangle

	Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));
	//dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8, 8));

	erode(thresh, thresh, erodeElement);
	erode(thresh, thresh, erodeElement);


	dilate(thresh, thresh, dilateElement);
	dilate(thresh, thresh, dilateElement);



}
void trackFilteredObject(int &x, int &y, Mat threshold, Mat &cameraFeed) {

	Mat temp;
	threshold.copyTo(temp);
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//find contours of filtered image using openCV findContours function
	findContours(temp, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();
		//if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
		if (numObjects < MAX_NUM_OBJECTS) {
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {

				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;

				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				//we only want the object with the largest area so we safe a reference area each
				//iteration and compare it to the area in the next iteration.
				if (area > MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea) {
					x = moment.m10 / area;
					y = moment.m01 / area;
					objectFound = true;
					refArea = area;
				}
				else objectFound = false;


			}
			//let user know you found an object
			//if (objectFound == true) {
				//putText(cameraFeed, "Tracking Object", Point(0, 50), 2, 1, Scalar(0, 255, 0), 2);
				//draw object location on screen
				//cout << x << "," << y;
				//drawObject(x, y, cameraFeed);

		//	}


		}
		else putText(cameraFeed, "TOO MUCH NOISE! ADJUST FILTER", Point(0, 50), 1, 2, Scalar(0, 0, 255), 2);
	}
}
/*int main(int argc, char *argv[])
{
	int socketfd = connect(20232,"193.226.12.217");

  processBuffer(socketfd, "ff");

  shutdown(socketfd,2);

  return 0;
}*/

int main(int argc, char* argv[])
{

	tracking[0].H_MIN = 94;
	tracking[0].H_MAX = 187;
	tracking[0].S_MIN = 38;
	tracking[0].S_MAX = 256;
	tracking[0].V_MIN = 69;
	tracking[0].V_MAX = 256;

	tracking[1].H_MIN = 17;
	tracking[1].H_MAX = 78;
	tracking[1].S_MIN = 17;
	tracking[1].S_MAX = 256;
	tracking[1].V_MIN = 195;
	tracking[1].V_MAX = 256;

	tracking[2].H_MIN = 37;
	tracking[2].H_MAX = 106;
	tracking[2].S_MIN = 70;
	tracking[2].S_MAX = 148;
	tracking[2].V_MIN = 65;
	tracking[2].V_MAX = 256;

	createTrackbars();
	//some boolean variables for different functionality within this
	//program

	setTrackbarPos("H_MIN", trackbarWindowName, tracking[ind].H_MIN);
	setTrackbarPos("H_MAX", trackbarWindowName, tracking[ind].H_MAX);
	setTrackbarPos("S_MIN", trackbarWindowName, tracking[ind].S_MIN);
	setTrackbarPos("S_MAX", trackbarWindowName, tracking[ind].S_MAX);
	setTrackbarPos("V_MIN", trackbarWindowName, tracking[ind].V_MIN);
	setTrackbarPos("V_MAX", trackbarWindowName, tracking[ind].V_MAX);

	bool trackObjects = true;
	bool useMorphOps = true;

	Point p;
	//Matrix to store each frame of the webcam feed
	Mat cameraFeed;
	//matrix storage for HSV image
	Mat HSV;
	//matrix storage for binary threshold image
	Mat us;
	Mat them;
	Mat usDirection;
	//x and y values for the location of the object
	int x = 0, y = 0;
	//create slider bars for HSV filtering
	//createTrackbars();
	//video capture object to acquire webcam feed
	VideoCapture capture;
	//open capture object at location zero (default location for webcam)
	capture.open("rtmp://172.16.254.99/live/nimic");
	do{
		if(!capture.isOpened())
		{
			printf("Can not open capture! Retrying in 2 seconds\n");
			sleep(2);
			capture.open("rtmp://172.16.254.99/live/nimic");
		}
		else{
			break;
		}
	}while(1);
	printf("Opened capture!\n");
	//set height and width of capture frame
	capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
	//start an infinite loop where webcam feed is copied to cameraFeed matrix
	//all of our operations will be performed within this loop

	//FPS
	int frameCounter = 0;
	int tick = 0;
  int fps;
  std::time_t timeBegin = std::time(0);

	cv::Point usCoord,themCoord, usDirectionCoord;

	while (1) {

		frameCounter++;
    std::time_t timeNow = std::time(0) - timeBegin;

		if (timeNow - tick >= 1)
    {
        tick++;
        fps = frameCounter;
        frameCounter = 0;
    }

		//store image to matrix
		capture.read(cameraFeed);

			if(cameraFeed.empty())
				continue;

		//convert frame from BGR to HSV colorspace
		cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);

		cv::putText(cameraFeed, cv::format("Average FPS=%d", fps ), cv::Point(30, 30), cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0,255,255));

		//filter HSV image between values and store filtered image to
		//threshold matrix

		//std::thread t1(inRange, HSV, Scalar(156, 103, 0), Scalar(220, 256, 256), us);
		//std::thread t2(inRange, HSV, Scalar(17, 17, 195), Scalar(78, 256, 256), them);

		//t1.join();
		//t2.join();

		//inRange(HSV, Scalar(156, 103, 0), Scalar(220, 256, 256), us);
		inRange(HSV, Scalar(tracking[0].H_MIN, tracking[0].S_MIN, tracking[0].V_MIN),
		Scalar(tracking[0].H_MAX, tracking[0].S_MAX, tracking[0].V_MAX), us);
		inRange(HSV, Scalar(tracking[1].H_MIN, tracking[1].S_MIN, tracking[1].V_MIN),
		Scalar(tracking[1].H_MAX, tracking[1].S_MAX, tracking[1].V_MAX), them);
		inRange(HSV, Scalar(tracking[2].H_MIN, tracking[2].S_MIN, tracking[2].V_MIN),
		Scalar(tracking[2].H_MAX, tracking[2].S_MAX, tracking[2].V_MAX), usDirection);
		//inRange(HSV, Scalar(17, 17, 195), Scalar(78, 256, 256), them);
		//inRange(HSV, Scalar(37, 70, 65), Scalar(106, 148, 256), usDirection);
		//perform morphological operations on thresholded image to eliminate noise
		//and emphasize the filtered object(s)
		if (useMorphOps)
		{
			morphOps(us);
			morphOps(them);
			morphOps(usDirection);
		}
		//pass in thresholded frame to our object tracking function
		//this function will return the x and y coordinates of the
		//filtered object
		if (trackObjects)
		{
			trackFilteredObject(usCoord.x, usCoord.y, us, cameraFeed);
			drawObject(usCoord.x, usCoord.y, cameraFeed);
			trackFilteredObject(themCoord.x, themCoord.y, them, cameraFeed);
			drawObject(themCoord.x, themCoord.y, cameraFeed);
			trackFilteredObject(usDirectionCoord.x, usDirectionCoord.y, usDirection, cameraFeed);
			drawObject(usDirectionCoord.x, usDirectionCoord.y, cameraFeed);



				double angle = atan2(themCoord.y - usCoord.y, themCoord.x - usCoord.x) - atan2(usDirectionCoord.y - usCoord.y, usDirectionCoord.x - usCoord.x);
				angle = radiansToDegrees(angle);
				//angle = 360 - angle;
				angle = (int)angle % 360;
				//printf("%lf\n",angle);
		}

		line(cameraFeed, usCoord, usDirectionCoord, Scalar(0, 0, 255), 2);
		line(cameraFeed, usCoord, themCoord, Scalar(0, 0, 255), 2);

		//show frames
		//imshow(windowName2, threshold);
		imshow(windowName, cameraFeed);
		//imshow(windowName1, HSV);
		setMouseCallback("Original Image", on_mouse, &p);
		//delay 30ms so that screen can refresh.
		//image will not appear without this waitKey() command
		waitKey(1);
	}

	return 0;
}
