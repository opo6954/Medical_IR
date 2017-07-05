#ifndef BOVW_H
#define BOVM_H
#include "resource.h"
#include <list>

/**
	@class BoVW
	@date  2017/07/05
	@brief class which makes and matches SIFT features using bag of visual words
*/
class BoVW
{
public:
	BoVW(int words, int numImages);	//constructor
	void BoVW_maker(); //bag making function
	void BoVW_Init(); //bag initialization - making inverted tf-idf
	std::vector<std::string> BoVW_matcher(cv::Mat img,int number); // matching function given an image

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
	
	//private functions which compute TF-IDF
	void InsertTF(int t, int name, int numFeatures);
	void InsertF(int t);
	void ClearF();
	void InsertIDF(int t);
	static bool compare_score(const scoring &a, const scoring &b);
};

#endif