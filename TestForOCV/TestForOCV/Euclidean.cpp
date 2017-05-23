#include "Euclidean.h"
#include <iostream>

float L2DistanceFunc(Mat LBP1, Mat LBP2)
{
	int dimension = LBP1.rows;

	float distance=0.0;
	for (int i = 0; i < dimension; i++)
	{
		float p1 = (float)LBP1.at<int>(i, 0);
		float p2 = (float)LBP2.at<int>(i, 0);
		distance += (p1 - p2)*(p1 - p2);
	}
	distance = sqrt(distance);
	return distance;
}