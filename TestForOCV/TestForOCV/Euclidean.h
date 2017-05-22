#pragma once
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

using namespace cv;
/*
*****************************************
L2DistanceFunction
Input: Two LBP descriptor feature(form of Mat)
Output: distance between two LBP descriptor(scalar)
*****************************************
*/

float L2DistanceFunc(Mat LBP1, Mat LBP2);
