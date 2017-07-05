#include "LBP.h"
#include "Feature extractor.h"
#include <iostream>
#include  <math.h>                   
#define PI 3.141592654

double round(double value)
{
	return value < 0.0 ? ceil(value - 0.5) : floor(value + 0.5);
}

Mat LBPdescriptor(Mat img, int p1, int p2)
{
	Mat im_gray;
	Mat im_lbp(img.rows, img.cols, CV_8UC1);	//To show lbp image
	Mat hist_lbp = Mat::zeros(256, 1, CV_32SC1);

	//image to gray
	if (img.channels() == 3)
		cvtColor(img, im_gray, CV_RGB2GRAY);
	else
		im_gray = img;

	unsigned center = 0;
	unsigned center_lbp = 0;

	for (int row = 1; row < im_gray.rows - 1; row++){
		for (int col = 1; col < im_gray.cols - 1; col++){
			center = im_gray.at<uchar>(row, col);
			center_lbp = 0;

			for (int i = 0; i < 8; i++){
				
				if (center <= im_gray.at<uchar>(row - round(cos((1 - i) * PI / 4)), col - round(sin((1 - i) * PI / 4))))
					//lbp cordinates
					//std::cout << -round(cos((1 - i) * PI / 4)) << ", " << -round(sin((1 - i) * PI / 4)) << std::endl;
					center_lbp += pow(2, i);
			}

			im_lbp.at<uchar>(row, col) = center_lbp;
			hist_lbp.at<int>(center_lbp, 0)++;
			//std::cout << hist_lbp.at<int>(center_lbp, 0) << std::endl;
		}
	}
	//LBP image show
	//imshow("lbp image",im_lbp);
	//waitKey(0);
	//std::cout << hist_lbp << std::endl;
	return hist_lbp;
}