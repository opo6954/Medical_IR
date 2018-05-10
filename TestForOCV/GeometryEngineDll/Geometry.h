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

/**
@class Geometry
@data 2018/04/17
*/

extern "C" __declspec(dllexport) bool loadImages(char* images, char* bbox, int length);///< ��ü �̹����� �ҷ����� ���� ��̸� �����

extern "C" __declspec(dllexport) bool get_top_N_location(const char * test_img, int left, int right, int top, int down, int label[], int index[], char* img_name, int top_num);///< �ش� �׽�Ʈ �̹����� ��ġ�� ���� ����� ������ �̹����� �ҷ��´�
extern "C" __declspec(dllexport) bool get_top_N_area(const char * test_img, int left, int right, int top, int down, int label[], int index[], char* img_name, int top_num);///< �ش� �׽�Ʈ �̹����� ���̰� ���� ����� ������ �̹����� �ҷ��´�
extern "C" __declspec(dllexport) bool get_top_N_ratio(const char * test_img, int left, int right, int top, int down, int label[], int index[], char* img_name, int top_num);///< �ش� �׽�Ʈ �̹����� ������ ���� ����� ������ �̹����� �ҷ��´�

/*
__declspec(dllexport) bool get_top_ten_location(const char * test_img, int left, int right, int top, int down, int label[], int index[], char* name, int rnum, char sep);///< �ش� �׽�Ʈ �̹����� ��ġ�� ���� ����� ������ �̹����� �ҷ��´�
__declspec(dllexport) bool get_top_ten_area(const char * test_img, int left, int right, int top, int down, int label[], int index[], char* name, int rnum, char sep);///< �ش� �׽�Ʈ �̹����� ���̰� ���� ����� ������ �̹����� �ҷ��´�
__declspec(dllexport) bool get_top_ten_ratio(const char * test_img, int left, int right, int top, int down, int label[], int index[], char* name, int rnum, char sep);///< �ش� �׽�Ʈ �̹����� ������ ���� ����� ������ �̹����� �ҷ��´�
*/
__declspec(dllexport) void showImage(int index, int rank);
