#ifndef BOVW_H
#define BOVM_H
#include "resource.h"
#include <list>

/**
	@class BoVW
	@date  2017/07/05
	@brief class which makes and matches SIFT features using bag of visual words

	@class BoVW
	@data 2017/07/06
	@Modified by HWLee
	@Divide training and test
	@Add write input and output 

*/

using namespace std;
using namespace cv;

class BoVW
{
	

public:
	BoVW(int words, int numImages);	//constructor



	//image root path가 주어지면 그 안에 있는 이미지를 이용해서 training하기(visual word 뽑기)
	//그리고 TF IDF list 만들고 저장하기
	
	//img_list 필요
	void BoVW_training(std::string image_root_path, std::string image_list_name_file, std::string training_output_path);
	std::vector<string> BoVW::BoVW_matcher_from_saved_training(std::string training_file_list, std::string training_file_root, cv::Mat img, int nOftopRank);
	
	
	void BoVW_maker(); //bag making function
	void BoVW_Init(); //bag initialization - making inverted tf-idf


	std::vector<std::string> BoVW_matcher(cv::Mat img,int number); // matching function given an image

	
	/*
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
	*/
	





private:
	typedef struct scoring
	{
		std::string index;
		float score;
	}scoring; //use for scoring
	typedef struct invertedList
	{
		int image;
		float TF;
	}invertedList; // use for TF version
	cv::Mat dictionary; //bag of words
	cv::flann::Index kdTree; // kd tree

	//items for TF-IDF
	std::vector <unsigned char> F;
	std::vector <float> IDF;
	std::vector <scoring> score;
	std::vector <std::list<invertedList>>Invert;
	
	//parameters
	int numDocument;
	int NUMVOCAB;

	//Name of data in search space
	vector<string> searchDataName;

	//path for training output
	string TFName;
	string IDFName;
	string VOCName;
	
	//private functions which compute TF-IDF
	void InsertTF(int t, int name, int numFeatures);
	void InsertF(int t);
	void ClearF();
	void InsertIDF(int t);
	static bool compare_score(const scoring &a, const scoring &b);
};

#endif