#include "opencv.hpp"
#include "objdetect/objdetect.hpp"
#include "highgui/highgui.hpp"
#include "imgproc/imgproc.hpp"
#include "filters.hpp"

#include <iostream>
#include <stdio.h>
#include <math.h>       /* pow */



using namespace std;
using namespace cv;

/** Function Headers */
void detectAndSave( Mat frame );

/** Global variables */
String logo_cascade_name = "darts_o/original/dartcascade.xml";// haarcascade_frontalface_default.xml";

CascadeClassifier logo_cascade;

string window_name = "Capture - Face detection";

/** @function main */
int main( int argc, const char** argv )
{
	CvCapture* capture;
	Mat frame = imread(argv[1], CV_LOAD_IMAGE_COLOR);

	//-- 1. Load the cascades
	//logo_cascade.load(logo_cascade_name);
	if( !logo_cascade.load( logo_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

	detectAndSave( frame );

	return 0;
}


/** @function detectAndSave */
void detectAndSave( Mat frame )
{
	std::vector<Rect> faces;
	Mat frame_gray;

	cvtColor( frame, frame_gray, CV_BGR2GRAY );
	equalizeHist( frame_gray, frame_gray );

	Mat original = frame_gray.clone();

	// medianBlur(frame_gray, frame_gray, 3) ;

	// show original image
	if(SHOW) imshow("gray",frame_gray);
	if(SHOW) waitKey();

	GaussianBlur(frame_gray, frame_gray, Size(3, 3), 1) ; //0.7 was OK
	if(SHOW) imshow("gaus blurred",frame_gray);
	if(SHOW) waitKey();

	sharpen(frame_gray, 3, frame_gray);
	imshow("sharpen",frame_gray);
	if(SHOW) waitKey();
	Mat sharpened = frame_gray.clone();

	cv::normalize(frame_gray, frame_gray, 0, 122, cv::NORM_MINMAX);

	Mat darken = frame_gray.clone();


	// threshold to produce solid black(remove shades)
	for(int i = 0; i < frame_gray.rows; ++i)
	{
		for (int j = 0; j < frame_gray.cols; ++j)
		{
			uchar tr = frame_gray.at<uchar>(i, j);
			if(tr < 75)
			{
				frame_gray.at<uchar>(i, j) = 0 ;
			}

		}
	}
	if(SHOW) imshow("prev",frame_gray);
	if(SHOW) waitKey();


	//detect lines
	cv::Mat xDeriv, yDeriv, grad_ory, grad_trs, arc_ory, arc_trs, output, out2, outcirc ;//frame_gray
	sobel(darken, xDeriv, yDeriv, grad_ory, arc_ory);//original
	sobel(sharpened, xDeriv, yDeriv, grad_trs, arc_trs);//original
	detectCircles(grad_ory, arc_ory, outcirc);
	detectLines(grad_trs, arc_trs, output);
	//detect lines only in circle with adaptive threshold
	//in selectedby circles regionns look for line hough spectrum similar to the one of dartboard.bmp
	//najlepiej zrob thersholiding and XOR
	extractRegion(original, out2, 50, 50, 20);
	if (checkHomogenity(original)) cout<<"homogeneous" << endl;
	else cout<<"IN-homogeneous"<<endl;


////////////////////////////////////////////////////////////////////////////////
		cv::Mat temp8Bit;
	cv::normalize(outcirc, temp8Bit, 0, 255, cv::NORM_MINMAX);
	temp8Bit.convertTo(outcirc, CV_8U);
	for (int i = 0; i < outcirc.rows; ++i)
	{
		for (int j = 0; j < outcirc.cols; ++j)
		{
			if (outcirc.at<uchar>(i,j) < CIRCLETHRESHOLD)//220 pretty good
			{
				outcirc.at<uchar>(i,j) = 0  ;
			}
		}
	}
	mexHat(outcirc, output);
	imshow("mex", output);
	waitKey();
////////////////////////////////////////////////////////////////////////////////

	// Blur the image to smooth the noise
	Mat blurred ;
	GaussianBlur(frame_gray, blurred, Size(3, 3), 1.2) ;
	// medianBlur(blurred, blurred, 3) ;

	//-- Detect faces
	logo_cascade.detectMultiScale( blurred, faces, 1.1, 1, 0|CV_HAAR_SCALE_IMAGE, Size(50, 50), Size(500,500) );
	std::cout << faces.size() << std::endl;

	for( int i = 0; i < faces.size(); i++ )
	{
		Mat tmp;
		extractRegion(original, tmp, faces[i].x, faces[i].y, faces[i].width);
		if(!checkHomogenity(tmp))
			rectangle(frame, Point(faces[i].x, faces[i].y), Point(faces[i].x + faces[i].width, faces[i].y + faces[i].height), Scalar( 0, 255, 0 ), 2);
	}

	//-- Save what you got
	if(SHOW) imshow("output",frame);
	if(SHOW) waitKey();
	// imwrite( "output.jpg", frame );

}
