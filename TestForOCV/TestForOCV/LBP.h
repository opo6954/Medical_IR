#pragma once
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

using namespace cv;
/*
*****************************************
LBP descriptor function
Input: Image(form of Mat) and some parameter?(like size of block, dimension of LBP histogram)(It's up to you)
Output: LBP descriptor(form of Mat)
*****************************************
*/

Mat LBPdescriptor(Mat img, int p1, int p2);
