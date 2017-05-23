#include "Euclidean.h"

float L2DistanceFunc(Mat LBP1, Mat LBP2)
{
	int dimension = LBP1.cols;
	float distance=0.0;
	for (int i = 0; i < dimension; i++)
	{
		float p1 = LBP1.at<float>(i, 0);
		float p2 = LBP2.at<float>(i, 0);
		distance += (p1 - p2)*(p1 - p2);
	}
	distance = sqrt(distance);
	return distance;
}