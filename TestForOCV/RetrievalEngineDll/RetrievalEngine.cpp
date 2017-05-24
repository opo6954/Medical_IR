#pragma once
#include "RetrievalEngine.h"
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>

using namespace std;
using namespace cv;

//DLL 상에서의 global variable들...
//...path Name....
//feature vector들을 저장해 놓는 곳
string searchVectorPath;
//search space에 있는 이미지를 저장해 놓는 곳
string searchImgPath;

//search space에 있는 이미지들의 ROI의 값을 저장해 놓는 곳
string searchRoiPath;

//search space에 있는 이미지들의 ROI의 이미지를 저장해 놓는 곳
string searchRoiImgPath;


//이 부분은 추후에 parameter.ini에 넣자. 일단 지금은 걍 하드코딩으로 넣읍시다~!~!
string imgFolderName = "IMG";
string roiFolderName = "ROI";
string roiImgFolderName = "ROIIMG";

string roiImgListName = "imgROIList.txt";
string roiListName = "ROItxtList.txt";
string imgListName = "imgList.txt";

bool isInitRetrieval = false;

//새로운 strucuture
//search space를 저장하기 위한 structure
struct imageInfo
{
	//파일 이름
	string fileName;

	//ROI 개수
	int roiNumber;

	//LBP vector(according to number of ROI
	vector<Mat> LBPVector;
};

//retrieval를 위한 structure
struct imageRank
{
	int imgIdx;
	int roiIdx;
	float distance;

};


Mat LBPdescriptor(Mat img);
float L2DistanceFunc(Mat LBP1, Mat LBP2);
bool loadImageFileName(vector<string>& _fileNameVector);
bool loadImageROINumberName(vector<int>& _roiNumberVector);


//search space에 존재하는 image의 정보가 포함됨(ROI 정보, LBP vector 정보 등...
vector<imageInfo> searchSpaceImageInfo;

//feature 학습하는 함수
//imageInputRoot로써 searchImg 폴더를 경로로 넣으면 될듯
//직접 LBP 추출하는 거
bool trainingSearchSpace(const char* _imageInputRootPath, const char* _vectorOutputRootPath)
{
	searchImgPath = _imageInputRootPath + imgFolderName;
	searchRoiPath = _imageInputRootPath + roiFolderName;
	searchRoiImgPath = _imageInputRootPath + roiImgFolderName;

	searchVectorPath = _vectorOutputRootPath;

	//이미지의 파일명을 모두 불러오기
	

	vector<string> fileNameVector;
	vector<int> roiNumberVector;

	
	//image file 이름을 읽어오기
	if(loadImageFileName(fileNameVector) == false)
		return false;

	//각 image file 이름별로 ROI 개수 읽어오기
	if(loadImageROINumberName(roiNumberVector) == false)
		return false;


	//size check하기, 무조건 fileName과 roiNumberVector의 크기는 같아야 함...~!~!
	if(fileNameVector.size() != roiNumberVector.size())
	{
		std::cout << "Not same number in image file and ROI file..." << std::endl;
		std::cout << "image file number: " << fileNameVector.size() << std::endl;
		std::cout << "ROI number: " << roiNumberVector.size() << std::endl;

		return false;
	}

	//ROI를 읽어서 학습을 하자
	for(int i=0; i<fileNameVector.size(); i++)
	{
		cout << i << " th file extracting vector..." << endl;
		int roiNumber = roiNumberVector[i];
		for(int j=0; j<roiNumber; j++)
		{
			string fileName_Ext = fileNameVector[i];
		
			string insertedStr = "_" + to_string(j);

			string roiName_Ext = fileName_Ext;
		
			int idx_Ext = roiName_Ext.find_last_of(".");
			if(idx_Ext <= 0)
			{
				cout << "wrong with file extension analysis..." << endl;
				return false;
			}
			roiName_Ext.insert(idx_Ext,insertedStr);

			string filePath = searchRoiImgPath + "/" + roiName_Ext;
			string roiName = roiName_Ext;

			roiName.erase(idx_Ext+2, roiName_Ext.length() - idx_Ext);

			Mat roiImg = imread(filePath);

			Mat lbpDescriptor = LBPdescriptor(roiImg);

			//ROI 이름으로 xml 형태로 저장하자

			cv::FileStorage fs = cv::FileStorage(searchVectorPath + "/" + roiName + ".xml", cv::FileStorage::WRITE);
			fs << "Vector" << lbpDescriptor;

			fs.release();
		}
	}
}

//vector 저장된 곳을 기준으로...
bool initRetrieval(const char* _imageInputRootPath, const char* _vectorOutputRootPath)
{
	
	//meta 파일 저장
	searchImgPath = _imageInputRootPath + imgFolderName;
	searchRoiPath = _imageInputRootPath + roiFolderName;
	searchRoiImgPath = _imageInputRootPath + roiImgFolderName;

	//vector 저장된 곳
	searchVectorPath = _vectorOutputRootPath;

	vector<string> fileNameVector;
	vector<int> roiNumberVector;

	

	//일단 ROI image의 이름을 vector에 몽땅 저장하기

	//image file 이름을 읽어오기
	if(loadImageFileName(fileNameVector) == false)
		return false;

	//각 image file 이름별로 ROI 개수 읽어오기
	if(loadImageROINumberName(roiNumberVector) == false)
		return false;

	//size check하기, 무조건 fileName과 roiNumberVector의 크기는 같아야 함...~!~!
	if(fileNameVector.size() != roiNumberVector.size())
	{
		std::cout << "Not same number in image file and ROI file..." << std::endl;
		std::cout << "image file number: " << fileNameVector.size() << std::endl;
		std::cout << "ROI number: " << roiNumberVector.size() << std::endl;

		return false;
	}

	//ROI를 읽어서 학습을 하자
	for(int i=0; i<fileNameVector.size(); i++)
	{
		cout << i << " th file load vector..." << endl;
		int roiNumber = roiNumberVector[i];
		string fileName_Ext = fileNameVector[i];

		int idx_Ext = fileName_Ext.find_last_of(".");

		if(idx_Ext <= 0)
		{
			cout << "Wrong with extension analysis..." << endl;
			return false;
		}

		string fileName = fileName_Ext;
		fileName.erase(idx_Ext, fileName_Ext.length() - idx_Ext);

		imageInfo info;
		info.fileName = fileName;
		info.roiNumber = roiNumber;
		
		for(int j=0; j<roiNumber; j++)
		{

			string insertedStr = "_" + to_string(j);

			string roiName_Ext = fileName_Ext;
		
			idx_Ext = roiName_Ext.find_last_of(".");
			if(idx_Ext <= 0)
			{
				cout << "Wrong with extension analysis..." << endl;
				return false;
			}

			roiName_Ext.insert(idx_Ext,insertedStr);

			string filePath = searchRoiImgPath + "/" + roiName_Ext;
			string roiName = roiName_Ext;
			roiName.erase(idx_Ext+2, roiName_Ext.length() - idx_Ext);

			Mat lbpDescriptor;

			//ROI 이름으로 xml 형태로 저장하자

			cv::FileStorage fs = cv::FileStorage(searchVectorPath + "/" + roiName + ".xml", cv::FileStorage::READ);
			if(fs.isOpened() == false)
			{
				cout << "Cannot open xml file..." << endl;
				return false;
			}

			fs["Vector"] >> lbpDescriptor;

			if(lbpDescriptor.rows > 0 && lbpDescriptor.cols > 0)
				info.LBPVector.push_back(lbpDescriptor);
			else
			{
				cout << "Wrong with lbpDescriptor..., rows or cols size is equal or less than 0..." << endl;
				return false;
			}


			fs.release();
		}

		searchSpaceImageInfo.push_back(info);

	}

	isInitRetrieval = true;

	return true;
}

bool initDescriptors()
{
	cout << "Nothing to init for descriptor..." << endl;
	return true;
}

//실제 unsigned char* 타입으로 데이터가 들어 올 때의 lbp distance를 비교해서 n개의 가장 비슷한 ROI랑 ID를 넘겨줍니다.


/*
예를들어서 가장 비슷한 n개의 ID가 각각 4 7 8 5 6 이고 각각의 ROI 순서가 0 1 3 1 0일 경우
ID의 경우 4 7 8 5 6
ROI의 경우 0 1 3 1 0
으로 포인터 형태로 출력이 됩니다.

추후에 c#에서 각 부분을 읽어올 수 있어야 하고
c#에서는 각 image별로 vector형태로 저장되어 있고(순서는 text 순서)
그 순서를 ID를 통해 바로 가지고 올 수 있어야 합니당
c#에도 search space에 대한 array 정보를 가지고 있어야 겠지요
*/

bool retrievalCurrImage(unsigned char* img, int width, int height, int n, int* ID, int* ROISeq)
{
	//만일 retrieval init이 되어 있는 경우에만 retrieval를 수행합니다
	if(isInitRetrieval == true)
	{
		if(searchSpaceImageInfo.size() <= 0)
		{
			cout << "No search space imageInfo exist..." << endl;
			return false;
		}

		unsigned char* tmpPointer = img;

		Mat imgMat(height, width, CV_8UC3);
		imgMat.data = tmpPointer;

		Mat lbpQuery = LBPdescriptor(imgMat);

		//이 부분에서 query unsigned char*를 mat으로 바꿔야 합네당

		vector<imageRank> imageRetrievalResult;
		

		for(int i=0; i<searchSpaceImageInfo.size(); i++)
		{
			for(int j=0; j<searchSpaceImageInfo[i].LBPVector.size(); j++)
			{
				imageRank ir;
				ir.imgIdx = i;
				ir.roiIdx = j;
				ir.distance = L2DistanceFunc(lbpQuery, searchSpaceImageInfo[i].LBPVector[j]);

				imageRetrievalResult.push_back(ir);
			}
		}

		//n번 반복해서 max 값을 찾으면 됩니다

		//공간 할당

		int* checkID = new int[n];
		for(int i=0; i<n; i++)
			checkID[i] = -1;
		
		for(int k=0; k<n; k++)
		{
			float minDist = 99999999999;
			imageRank minRank;
			int minIdx=-1;

			for(int i=0; i<imageRetrievalResult.size(); i++)
			{
				bool pass = false;

				for(int j=0; j<n; j++)
					{
						if(checkID[j] >= 0)
						{
							if(i == checkID[j])
							{
								pass = true;
							}
						}
					}
				if(pass == true)
					continue;

				if(minDist > imageRetrievalResult[i].distance)
				{
					minDist = imageRetrievalResult[i].distance;
					minRank.distance = minDist;
					minRank.imgIdx = imageRetrievalResult[i].imgIdx;
					minRank.roiIdx = imageRetrievalResult[i].roiIdx;
					minIdx = i;
				}
			}
			cout<<k <<" th closest image name: " << searchSpaceImageInfo[imageRetrievalResult[minIdx].imgIdx].fileName << endl;

			ID[k] = minRank.imgIdx;
			ROISeq[k] = minRank.roiIdx;
			checkID[k] = minIdx;
		}



	}

	return true;

}






bool loadImageROINumberName(vector<int>& _roiNumberVector)
{
	ifstream fin = std::ifstream(searchRoiPath + "/" + roiListName, std::ios::in);

	if(fin != NULL)
	{
		while(fin.eof() == false)
		{
			char sTemp[256];
			fin >> sTemp;

			if(!strcmp(sTemp, "")) continue;

			std::string roiId = std::string(sTemp);

			std::ifstream finROI(searchRoiPath + "/" + roiId, std::ios::in);

			if(finROI != NULL)
			{
				while(finROI.eof() == false)
				{
					char sTemp2[256];
					finROI >> sTemp2;

					if(!strcmp(sTemp2, "")) continue;

					std::string roiNum = std::string(sTemp2);

					int nOfROI = atoi(roiNum.c_str());

					_roiNumberVector.push_back(nOfROI);

					if(nOfROI > 0)
						finROI.close();					
				}
			}
		}
	}
	else
	{
		cout <<"Wrong with open roi list file with " << searchRoiPath + "/" + roiListName << endl;
		return false;
	}

	fin.close();

	return true;
}


bool loadImageFileName(vector<string>& _fileNameVector)
{
	std::ifstream fin(searchImgPath + "/" + imgListName, std::ios::in);

	if(fin !=  NULL)
	{
		while(fin.eof() == false)
		{
			char sTemp[256];
			fin >> sTemp;

			if(!strcmp(sTemp,"")) continue;

			std::string id = std::string(sTemp);

			_fileNameVector.push_back(id);
		}
	}
	else
	{
		cout <<"Wrong with open image list file with " << searchImgPath + "/" + imgListName << endl;
		return false;
	}
	fin.close();

	return true;
}









double round(double value)
{
	return value < 0.0 ? ceil(value - 0.5) : floor(value + 0.5);
}
//ROI image 자체가 들어갈 때의 LBP descriptor 계산하기
Mat LBPdescriptor(Mat img)
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


