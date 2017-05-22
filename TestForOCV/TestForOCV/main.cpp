#include <iostream>
#include <fstream>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

#include "LBP.h"
#include "Euclidean.h"



using namespace cv;
using namespace std;


string testDir = "../data/queryImg/";
string searchDir = "../data/searchImg/";
string searchROIDir = "../data/searchImg/ROI/";


string testImg = "011000286_01Nodule_1.jpg";
string testROI = "011000286_01Nodule.txt";

Mat loadImg(string fileName);
vector<Rect> loadROI(string fileName);
void testImgNROI();


int main()
{

	cout << "Image N ROI Test..............." << endl;




	Mat img = loadImg(searchDir +  testImg);;
	vector<Rect> myROI = loadROI(searchROIDir +  testROI);

	cout << "Image Name: " << testImg << endl;
	cout << "ROI Name: " << testROI << endl;

	cout << "ROI Info: " << endl;

	for(int i=0; i<myROI.size(); i++)
		cout << myROI[i].x << " " << myROI[i].y << " " << myROI[i].width << " " << myROI[i].height << endl;

	rectangle(img,myROI[0],Scalar::all(50),3);

	Mat resizeImg;
	
	resize(img,resizeImg,Size(img.size().width/2, img.size().height/2));


	
	
	imshow("testImg",resizeImg);
	waitKey(0);



	cout << "LBP test..." << endl;

	Mat a = LBPdescriptor(img(myROI[0]),0,0);

	cout << "LBP working...................." << endl;


	cout << "Distance measure test..." << endl;

	Rect roiOrigin;
	Rect roiModified;


	int offsetX=10;
	int offsetY=10;

	roiOrigin.x = myROI[0].x;
	roiOrigin.y = myROI[0].y;
	roiOrigin.width = myROI[0].width;
	roiOrigin.height = myROI[0].height;



	roiModified.x = myROI[0].x + offsetX;
	roiModified.y = myROI[0].y + offsetY;
	roiModified.width = myROI[0].width;
	roiModified.height = myROI[0].height;

	Mat lbp1 = LBPdescriptor(img(roiOrigin),0,0);
	Mat lbp2 = LBPdescriptor(img(roiModified),0,0);


	float distance = L2DistanceFunc(lbp1, lbp2);

	cout << "L2 distance between slight translation of ROI is " << distance << endl;

	return 0;
}



Mat loadImg(string fileName)
{
	Mat img = imread(fileName);

	return img;
}

vector<Rect> loadROI(string fileName)
{
	ifstream fin(fileName);

	vector<Rect> rectSeq;

	int n;

	fin >> n;

	if(n > 0)
	{
		int x,y,w,h;

		for(int i=0; i<n; i++)
		{
			fin >> x >> y >> w >> h;

			if(x > 0 && y > 0 && w > 0 && h > 0)
				rectSeq.push_back(Rect(x,y,w-x,h-y));
		}
	}

	return rectSeq;
}


