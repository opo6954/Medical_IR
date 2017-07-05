#ifndef BOVW_H
#define BOVM_H
#include "resource.h"
#include <list>

class BoVW
{
public:
	BoVW(int words, int numImages);
	void BoVW_maker();
	void BoVW_Init();
	std::vector<std::string> BoVW_matcher(cv::Mat img,int number);
private:
	typedef struct scoring
	{
		std::string index;
		float score;
	}scoring;
	typedef struct invertedList
	{
		int image;
		float TF;
	}invertedList;
	cv::Mat dictionary;
	cv::flann::Index kdTree;
	std::vector <unsigned char> F;
	std::vector <float> IDF;
	std::vector <scoring> score;
	std::vector <std::list<invertedList>>Invert;
	int numDocument;
	int NUMVOCAB;
	
	void InsertTF(int t, int name, int numFeatures);
	void InsertF(int t);
	void ClearF();
	void InsertIDF(int t);
	static bool compare_score(const scoring &a, const scoring &b);
};

#endif