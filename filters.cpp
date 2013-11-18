#include "filters.hpp"
#include "opencv.hpp"

using namespace cv;
using namespace std;

void sharpen(cv::Mat &input, int size, cv::Mat &out)
{
	cv::Mat blurredOutput ;
	// intialise the output using the input
	blurredOutput.create(input.size(), CV_64F) ; //input.type());

	//was CV_64FC1
	cv::Mat kernel = cv::Mat(size, size, CV_64F, cv::Scalar::all(-1));
	kernel.at<double>(size/2, size/2) = size*size;

	// we need to create a padded version of the input
	// or there will be border effects
	int kernelRadiusX = ( kernel.size[0] - 1 ) / 2;
	int kernelRadiusY = ( kernel.size[1] - 1 ) / 2;

	cv::Mat paddedInput;
	cv::copyMakeBorder( input, paddedInput, 
		kernelRadiusX, kernelRadiusX, kernelRadiusY, kernelRadiusY,
		cv::BORDER_REPLICATE );

	// now we can do the convoltion
	for ( int i = 0; i < input.rows; i++ )
	{	
		for( int j = 0; j < input.cols; j++ )
		{
			double sum = 0.0;
			for( int m = -kernelRadiusX; m <= kernelRadiusX; m++ )
			{
				for( int n = -kernelRadiusY; n <= kernelRadiusY; n++ )
				{
					// find the correct indices we are using
					int imagex = i + kernelRadiusX + m;
					int imagey = j + kernelRadiusY + n;
					int kernelx = m + kernelRadiusX;
					int kernely = n + kernelRadiusY;

					// get the values from the padded image and the kernel
					// int - int - uchar
					double imageval = ( double ) paddedInput.at<uchar>( imagex, imagey );
					double kernalval = kernel.at<double>( kernelx, kernely );

					// do the multiplication
					sum += imageval * kernalval;							
				}
			}

			blurredOutput.at<double>(i, j) = (double)sum;
		}
	}

	cv::Mat temp8b ;
	cv::normalize(blurredOutput, temp8b, 0, 255, cv::NORM_MINMAX);
	temp8b.convertTo(out, CV_8U);

}


// mexhat
void mexHat(cv::Mat &input, cv::Mat &out)
{
	int size = 17;
	cv::Mat blurredOutput ;
	// intialise the output using the input
	blurredOutput.create(input.size(), CV_64F) ; //input.type());

	//was CV_64FC1
	cv::Mat kernel = cv::Mat(size, size, CV_64F, cv::Scalar::all(0));
	int jajko[17][17] = 
		{
			{0,0,0,0,0,0,-1,-1,-1,-1,-1,0,0,0,0,0,0 },
			{0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,0 },
			{0,0,-1,-1,-1,-2,-3,-3,-3,-3,-3,-2,-1,-1,-1,0,0 },
			{0,0,-1,-1,-2,-3,-3,-3,-3,-3,-3,-3,-2,-1,-1,0,0 },
			{0,-1,-1,-2,-3,-3,-3,-2,-3,-2,-3,-3,-3,-2,-1,-1,0},
			{0,-1,-2,-3,-3,-3,0,2,4,2,0,-3,-3,-3,-2,-1,0},
			{-1,-1,-3,-3,-3,0,4,10,12,10,4,0,-3,-3,-3,-1,-1},
			{-1,-1,-3, -3,-2,2,10,18,21,18,10,2,-2,-3,-3,-1,-1},
			{-1,-1,-3, -3,-3,4,12,21,24,21,12,4,-3,-3,-3,-1,-1},
			{-1,-1,-3, -3,-2,2,10,18,21,18,10,2,-2,-3,-3,-1,-1},
			{-1,-1,-3,-3,-3,0,4,10,12,10,4,0,-3,-3,-3,-1,-1},
			{0,-1,-2,-3,-3,-3,0,2,4,2,0,-3,-3,-3,-2,-1,0},
			{0,-1,-1,-2,-3,-3,-3,-2,-3,-2,-3,-3,-3,-2,-1,-1,0},
			{0,0,-1,-1,-2,-3,-3,-3,-3,-3,-3,-3,-2,-1,-1,0,0},
			{0,0,-1,-1,-1,-2,-3,-3,-3,-3,-3,-2,-1,-1,-1,0,0},
			{0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,0,0,0 },
			{0,0,0,0,0,0,-1,-1,-1,-1,-1,0,0,0,0,0,0 }
		};
	// kernel.at<double>(size/2, size/2) = size*size;
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size; ++j)
		{
			kernel.at<double>(i,j) = jajko[i][j];
		}
	}

	// we need to create a padded version of the input
	// or there will be border effects
	int kernelRadiusX = ( kernel.size[0] - 1 ) / 2;
	int kernelRadiusY = ( kernel.size[1] - 1 ) / 2;

	input.convertTo(input, CV_64F);

	cv::Mat paddedInput;
	cv::copyMakeBorder( input, paddedInput, 
		kernelRadiusX, kernelRadiusX, kernelRadiusY, kernelRadiusY,
		cv::BORDER_REPLICATE );

	// now we can do the convoltion
	for ( int i = 0; i < input.rows; i++ )
	{	
		for( int j = 0; j < input.cols; j++ )
		{
			double sum = 0.0;
			for( int m = -kernelRadiusX; m <= kernelRadiusX; m++ )
			{
				for( int n = -kernelRadiusY; n <= kernelRadiusY; n++ )
				{
					// find the correct indices we are using
					int imagex = i + kernelRadiusX + m;
					int imagey = j + kernelRadiusY + n;
					int kernelx = m + kernelRadiusX;
					int kernely = n + kernelRadiusY;

					// get the values from the padded image and the kernel
					// int - int - uchar
					double imageval = ( double ) paddedInput.at<double>( imagex, imagey );
					double kernalval = kernel.at<double>( kernelx, kernely );

					// do the multiplication
					sum += imageval * kernalval;							
				}
			}

			blurredOutput.at<double>(i, j) = (double)sum;
		}
	}

	cv::Mat temp8b ;
	cv::normalize(blurredOutput, temp8b, 0, 255, cv::NORM_MINMAX);
	temp8b.convertTo(out, CV_8U);

}


//line detect
void sobel(const cv::Mat& image, cv::Mat& xDeriv, cv::Mat& yDeriv, cv::Mat& grad, cv::Mat& arc)
{
	double minVal, maxVal;
	cv::Sobel(image, xDeriv, CV_64F, 1, 0);
	cv::Sobel(image, yDeriv, CV_64F, 0, 1);
	cv::Mat xPow, yPow;
	cv::pow(xDeriv, 2, xPow);
	cv::pow(yDeriv, 2, yPow);
	cv::Mat temp = xPow + yPow;
	cv::Mat tempFloat;
	temp.convertTo(tempFloat, CV_64F);
	cv::Mat gradNorm;
	cv::sqrt(tempFloat, grad);
	cv::Mat divided, arcNorm;
	arc = ( cv::Mat(xDeriv.rows, xDeriv.cols, CV_64F) ).clone() ;
	cv::divide(yDeriv, xDeriv, divided);
	cout << "sobel doing" << endl;
	for(int i = 0; i < divided.rows; i++)
	{
		for(int j =0; j < divided.cols; j++)
		{	
			arc.at<double>(i, j) = (double)atan2(yDeriv.at<double>(i,j), xDeriv.at<double>(i,j)) ;//* 180 / PI;
		}
	}
	cout << "sobel done" << endl;
}


// detect lines and threshold + log() to display
// return vector with hough space
// void
std::vector<std::vector<int> > detectLines(const cv::Mat& grad, const cv::Mat& arc, cv::Mat& out)
{
	// cv::Mat lineHoughSpace = cv::Mat(grad.rows, grad.cols, CV_64F, cv::Scalar::all(0));

	// theta ranges from 0-2PI = 0 - 628
	// tho is dynamically adjusted from 0 to max_val based on diagonal
	int diagonalSize = round(sqrt((double)std::pow((double)(grad.rows), 2) + (double)std::pow((double)(grad.cols),2)));
	// cout << diagonalSize << "  " << grad.rows << "  " << grad.cols << endl ;
	diagonalSize *= 2;

	 // diagonalSize = round(diagonalSize/5);
	 // cout << diagonalSize << endl;

	std::vector<std::vector<int> > houghSpace (round(diagonalSize/5), std::vector<int>(628, 0) ) ;
	// cv::Mat lineHoughSpace = cv::Mat(round(diagonalSize/5), 314+10, CV_64F, cv::Scalar::all(0)); //628
	cv::Mat lineHoughSpace = cv::Mat(LINEYDIM, 314+10, CV_64F, cv::Scalar::all(0)); //628


	// int ujemne = 0;
	// int dodatnie = 0;
	// double rem1 = 0;
	// double rem2 = 0;

	for(int i = 0; i < grad.rows; ++i)
	{
		for (int j = 0; j < grad.cols; ++j)
		{
			if (grad.at<double>(i, j) > HOUGHDETECTTRESHOLD)
			{

				//LINE DETECTION
				// for (int th = round(arc.at<double>(i, j)-DTH); th < round(arc.at<double>(i, j)+DTH); ++th)
				int trows = lineHoughSpace.rows ;
				int tcols = lineHoughSpace.cols ;
				for (int th = (arc.at<double>(i,j)*100)-DTH; th < (arc.at<double>(i,j)*100)+DTH; ++th)
				{
					if (th<0) continue;
					// cout << double(th)/100 << endl;
					// double rho = i * cos(double(th)/100)+ j*sin(double(th)/100) ; //first
					double rho = j * cos(double(th)/100)+ i*sin(double(th)/100) ; //second
					// cout << rho+diagonalSize/2 << endl;

					//invrease haff pace
					// if(round(double(rho/10))<trows && round(double(rho/10))>=0 && round(double(th)/10)<tcols && round(double(th/10))>0)
					// {
					// 	lineHoughSpace.at<double>(round(double(rho/10)), round(double(th/10)) ) += 1 ;
					// }

					// if (rho+diagonalSize/2 <0)
					// {
					// 	ujemne++;
					// } else {
					// 	dodatnie++;
					// }
					// if (rho<rem1) rem1 = rho;
					// if (rho>rem2) rem2 = rho;

					// create hough space
					// if(round(double(rho/10))<trows && round(double(rho/10))>=0 && round(double(th)/10)<tcols && round(double(th/10))>0)
					// {
						// cout << "first" << round(th) << " " << round(rho+diagonalSize/2) << endl;

						double temporary = rho ;
						//shift
						temporary += diagonalSize/2 ;
						// scale to 0:1
						temporary /= diagonalSize;
						// rescale to LINeY
						temporary *= LINEYDIM;


						houghSpace[round((rho+diagonalSize/2)/5)-1][round(th) ] += 1 ;
						// cout << "second" << round(th) << " " << round(rho+diagonalSize/2) << endl;
						lineHoughSpace.at<double>(round(temporary), round(th)) += 1 ;
					// }

				}
			}
		}
	}

	// cout << "ujemne: " << ujemne << " " << dodatnie << endl;
	// cout << rem1 << "  " << rem2 << endl;

	// cout << "tu" << endl;
	cv::Mat temp8b ;
	cv::normalize(lineHoughSpace, temp8b, 0, 255, cv::NORM_MINMAX);
	temp8b.convertTo(out, CV_8U);

	//print haff space fol LINE
	//take logs
	for (int i = 0; i < lineHoughSpace.rows; ++i)
	{
		for (int j = 0; j < lineHoughSpace.cols; ++j)
		{
			if (lineHoughSpace.at<double>(i,j) != 0)
			{
				lineHoughSpace.at<double>(i,j) = log( lineHoughSpace.at<double>(i,j) ) ;
			}
		}
	}
	//scale
	cv::Mat temp8Bit;
	cv::normalize(lineHoughSpace, temp8Bit, 0, 255, cv::NORM_MINMAX);
	temp8Bit.convertTo(lineHoughSpace, CV_8U);
	for (int i = 0; i < lineHoughSpace.rows; ++i)
	{
		for (int j = 0; j < lineHoughSpace.cols; ++j)
		{
			if (lineHoughSpace.at<uchar>(i,j) < LINETHRESHOLD)//220 pretty good
			{
				lineHoughSpace.at<uchar>(i,j) = 0  ;
			}
		}
	}
	//convert
	if(SHOW) cv::imshow("Hough space Line", lineHoughSpace) ;
	if(SHOW) waitKey();

	return houghSpace;
}


// Detect circles with Hough transform
// void
std::vector<std::vector<std::vector<int> > > detectCircles(const cv::Mat& grad, const cv::Mat& arc, cv::Mat& out)
{
	// cv::vector<cv::Vec3d> circles ; //x,y,r
	// std::vector<std::vector<std::vector<int> > > houghSpace (HOUGHY, std::vector<std::vector<int> > (HOUGHX, std::vector<int>(RMAX-RMIN, 0) ) ) ;
	std::vector<std::vector<std::vector<int> > > houghSpace (grad.rows, std::vector<std::vector<int> > (grad.cols, std::vector<int>(RMAX-RMIN, 0) ) ) ;
	cv::Mat circleHoughSpace = cv::Mat(grad.rows, grad.cols, CV_64F, cv::Scalar::all(0));
	// threshold the gradient image after normalization
	// cv::Mat gradNorm(grad.rows, grad.cols, CV_64F) ;

	for(int i = 0; i < grad.rows; ++i)
	{
		for (int j = 0; j < grad.cols; ++j)
		{
			if (grad.at<double>(i, j) > HOUGHDETECTTRESHOLD)
			{

				// CIRCLE DETECTION
				for (int r = RMIN; r < RMAX; ++r)
				{
					//shifted by RMAX to make scaling easier task
					double x1 = j+r*cos(arc.at<double>(i,j)) ;
					double x2 = j-r*cos(arc.at<double>(i,j)) ;
					double y1 = i+r*sin(arc.at<double>(i,j)) ;
					double y2 = i-r*sin(arc.at<double>(i,j)) ;

					int trows = circleHoughSpace.rows ;
					int tcols = circleHoughSpace.cols ;

					if ( round(y1)<trows && round(y1)>0 && round(x1)>0 && round(x1)<tcols )
					{
						circleHoughSpace.at<double>(round(y1), round(x1) ) += 1 ;
						// houghSpace[y1*HOUGHY/trows][x1*HOUGHX/tcols][r-RMIN] += 1 ;
						houghSpace[round(y1)][round(x1)][r-RMIN] += 1 ;
					}
					if ( round(y1)<trows && round(y1)>0 && round(x2)>0 && round(x2)<tcols )
					{
						circleHoughSpace.at<double>( round(y1), round(x2)  ) += 1 ;
						// houghSpace[y1*HOUGHY/trows][x2*HOUGHX/tcols][r-RMIN] += 1 ;
						houghSpace[round(y1)][round(x2)][r-RMIN] += 1 ;
					}
					if ( round(y2)<trows && round(y2)>0 && round(x1)>0 && round(x1)<tcols )
					{
						circleHoughSpace.at<double>(  round(y2), round(x1)  ) += 1 ;
						// houghSpace[y2*HOUGHY/trows][x1*HOUGHX/tcols][r-RMIN] += 1 ;
						houghSpace[round(y2)][round(x1)][r-RMIN] += 1 ;
					}
					if ( round(y2)<trows && round(y2)>0 && round(x2)>0 && round(x2)<tcols )
					{
						circleHoughSpace.at<double>(  round(y2), round(x2)  ) += 1 ;
						// houghSpace[y2*HOUGHY/trows][x2*HOUGHX/tcols][r-RMIN] += 1 ;
						houghSpace[round(y2)][round(x2)][r-RMIN] += 1 ;

					}
				}
			}
		}
	}

	cv::Mat temp8b ;
	cv::normalize(circleHoughSpace, temp8b, 0, 255, cv::NORM_MINMAX);
	temp8b.convertTo(out, CV_8U);

	//print haff space fol CIRCLE
	//take logs
	cv::Mat temp8Bit;
	for (int i = 0; i < circleHoughSpace.rows; ++i)
	{
		for (int j = 0; j < circleHoughSpace.cols; ++j)
		{
			if (circleHoughSpace.at<double>(i,j) != 0)
			{
				circleHoughSpace.at<double>(i,j) = log( circleHoughSpace.at<double>(i,j) ) ;
			}
		}
	}
	//scale
	cv::normalize(circleHoughSpace, temp8Bit, 0, 255, cv::NORM_MINMAX);
	temp8Bit.convertTo(circleHoughSpace, CV_8U);
	for (int i = 0; i < circleHoughSpace.rows; ++i)
	{
		for (int j = 0; j < circleHoughSpace.cols; ++j)
		{
			if (circleHoughSpace.at<uchar>(i,j) < CIRCLETHRESHOLD)//220 pretty good
			{
				circleHoughSpace.at<uchar>(i,j) = 0  ;
			}
		}
	}
	//convert
	if(SHOW) cv::imshow("Hough space Circle", circleHoughSpace) ;
	// imwrite( "output.jpg", circleHoughSpace );
	if(SHOW) waitKey();

	return houghSpace;
}

//extract square region with given top left corner and side length
void extractRegion(const Mat& input, Mat& output, int x, int y, int a)
{
	output = Mat(a, a, CV_64F, cv::Scalar::all(0)).clone();
	Mat temp ;
	input.convertTo(temp, CV_64F);

	for (int i = y; i < y+a; ++i)
	{
		for (int j = x; j < x+a; ++j)
		{
			output.at<double>(i-y, j-x) = temp.at<double>(i, j);
		}
	}

	//preview
	cv::Mat temp8Bit;
	cv::normalize(output, temp8Bit, 0, 255, cv::NORM_MINMAX);
	temp8Bit.convertTo(output, CV_8U);
	if(SHOW) cv::imshow("square extraction", output) ;
	if(SHOW) waitKey();


}

bool checkHomogenity(const Mat& input)
{
 int c = 0;
 double d = 0;
 int e =0;
 Mat temp;
 input.convertTo(temp, CV_64F);

 for (int i = 0; i < input.rows; ++i)
 {
 	for (int j = 0; j < input.cols; ++j)
 	{
 		// cout << c << "   " << temp.at<double>(i,j) << endl;
 		++c;
 		d +=  temp.at<double>(i,j);
 	}
 }
  cout << "d: " << d << " c: " << c << " e: " << e << endl;


 d /= c;

 for (int i = 0; i < input.rows; ++i)
 {
 	for (int j = 0; j < input.cols; ++j)
 	{
 		if (d - HOMOGENITYTHRS < temp.at<double>(i,j) && d + HOMOGENITYTHRS > temp.at<double>(i,j))
 		{
 			++e;
 		}
 	}
 }

 cout << "d: " << d << " c: " << c << " e: " << e << endl;
 if (double((double(e)/c)*100) >  HOMOGENITYCOND) return true;
 else return false;

}


// threshold the image with a few levels
void nLvlTrsh( const Mat& input, Mat& output )
{
    // threshold to produce solid black(remove shades)
    for(int i = 0; i < input.rows; ++i)
    {
        for (int j = 0; j < input.cols; ++j)
        {
            uchar tr = input.at<uchar>(i, j);
            // if(tr < 35)
            // {
            //         input.at<uchar>(i, j) = 15 ;//0
            // }
            // else if(tr<70)
            // {
            //         input.at<uchar>(i, j) = 45;//40
            // }
            // else if (tr<105)
            // {
            //         input.at<uchar>(i, j) = 75;//80
            // }
            // else if (tr<140)
            // {
            //         input.at<uchar>(i, j) = 105;//120
            // }
///////////////////////////////////////////////////////////////////////////////
            if(tr < 100)
            {
                    output.at<uchar>(i, j) = 0;
            }
            // else if(tr<75)
            // {
            //         output.at<uchar>(i, j) = 60;
            // }
            // else if (tr<100)
            // {
            //         output.at<uchar>(i, j) = 80;
            // }
            else if (tr<125)
            {
                    output.at<uchar>(i, j) = 100;
            }
            else if (tr<150)
            {
                    output.at<uchar>(i, j) = 120;
            }
            // else if (tr<175)
            // {
            //         output.at<uchar>(i, j) = 140;
            // }
            // else if (tr<200)
            // {
            //         output.at<uchar>(i, j) = 160;
            // }
            else
            {
            	output.at<uchar>(i,j) = 130;
            }

///////////////////////////////////////////////////////////////////////////////
            //BEST EVA
            // if(tr < 50)
            // {
            //         input.at<uchar>(i, j) = 10 ;//0
            // }
            // else if(tr<100)
            // {
            //         input.at<uchar>(i, j) = 50;//40
            // }
            // else if (tr<150)
            // {
            //         input.at<uchar>(i, j) = 90;//80
            // }
            // else if (tr<200)
            // {
            //         input.at<uchar>(i, j) = 130;//120
            // }
            // else if (tr<255)
            // {
            //         input.at<uchar>(i, j) = 190; //180
            // }
////////////////////////////////////////////////////////////////////////////////


//                 if (tr < 25)
//                 {
//                         input.at<uchar>(i, j) = 10 ; //10 ;
//                 }
//                 else if (tr < 50)
//                 {
//                         input.at<uchar>(i, j) = 35 ; //30 ;
//                 }
//                 else if (tr < 75)
//                 {
//                         input.at<uchar>(i, j) = 60 ; //50 ;
//                 }
//                 else if (tr < 100)
//                 {
//                         input.at<uchar>(i, j) =  85; //70 ;
//                 }
//                 else if (tr < 125)
//                 {
//                         input.at<uchar>(i, j) =  110; //90 ;
//                 }
//                 else if (tr < 150)
//                 {
//                         input.at<uchar>(i, j) =  135; //110 ;
//                 }
//                 else if (tr < 175)
//                 {
//                         input.at<uchar>(i, j) =  160; //130 ;
//                 }
//                 else if (tr < 200)
//                 {
//                         input.at<uchar>(i, j) =  185; //150 ;
//                 }
//                 else if (tr < 225)
//                 {
//                         input.at<uchar>(i, j) =  210; //160 ;
//                 }
//                 else if (tr < 255)
//                 {
//                         input.at<uchar>(i, j) =  240; //180 ;
//                 }
            // else if (tr < 210)
            // {
            //         input.at<uchar>(i, j) =  195; //200 ;
            // }
            // else if (tr < 230)
            // {
            //         input.at<uchar>(i, j) =  215; //220 ;
            // }
            // else if (tr < 250)
            // {
            //         input.at<uchar>(i, j) =  235; //240 ;
            // }
            // else if (tr < 255)
            // {
            //         input.at<uchar>(i, j) =  250; //255 ;
            // }
            // else
            // {
            //         input.at<uchar>(i, j) = 0 ;
            // }
        }
    }
}




