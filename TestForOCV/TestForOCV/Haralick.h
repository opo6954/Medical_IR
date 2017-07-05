#pragma once
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

using namespace cv;
/*
*****************************************
GLCM descriptor function
Input: Image(form of Mat) and some parameter(p1;# of direction{1,2,4,8}, d;distance>0)
Output: GLCM descriptor(form of Mat)
*****************************************
*/

Mat Haralick(Mat img, int p1, int d);
