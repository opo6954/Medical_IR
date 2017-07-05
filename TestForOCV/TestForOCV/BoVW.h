#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\flann\flann.hpp>
#include <opencv2\nonfree\nonfree.hpp>

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <cstdlib>
#include <list>

class BoVW
{
public:
	void BoVW_maker();
	void BoVW_Init();
	vector<string> BoVW_matcher(Mat img,int number);
private:
	typedef struct scoring
	{
		string index;
		float score;
	}scoring;

	typedef struct invertedList
	{
		int image;
		float TF;
	}invertedList;
	flann::Index kdTree;
	vector <unsigned char> F;
	vector <float> IDF;
	vector <scoring> score;
	vector <list<invertedList>>Invert;
	int numDocument;
	void InsertTF(int t, int name, int numFeatures);
	void InsertF(int t);
	void ClearF();
	void InsertIDF(int t);
	bool compare_score(const scoring &a, const scoring &b);
};