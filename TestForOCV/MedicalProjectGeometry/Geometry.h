#pragma once
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
#include <list>

using namespace std;
using namespace cv;

typedef struct geometric
{
	string name;
	int top_x;
	int top_y;
	int bottom_x;
	int bottom_y;

	int label;
	int x;
	int y;
	float ratio;
	int area;
}geometric;

typedef struct score
{
	string name;
	int label;
	double distance;
	int index;
}score;

/*
@class Geometry
@data 2018/04/17
*/
void loadImages(char* images, char* bbox, int length);///< ��ü �̹����� �ҷ����� ���� ��̸� �����
char** get_top_ten_location(const char * test_img, int left, int right, int top, int down, int label[], int index[]);///< �ش� �׽�Ʈ �̹����� ��ġ�� ���� ����� ������ �̹����� �ҷ��´�
char** get_top_ten_area(const char * test_img, int left, int right, int top, int down, int label[], int index[]);///< �ش� �׽�Ʈ �̹����� ���̰� ���� ����� ������ �̹����� �ҷ��´�
char** get_top_ten_ratio(const char * test_img, int left, int right, int top, int down, int label[], int index[]);///< �ش� �׽�Ʈ �̹����� ������ ���� ����� ������ �̹����� �ҷ��´�
void showImage(int index, int rank);