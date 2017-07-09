#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\flann\flann.hpp>
#include <opencv2\nonfree\nonfree.hpp>

#include <list>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <cstdlib>

#include "BoVW.h"
//! [namespace]
using namespace cv;
using namespace std;

//Constructor of BoVW
//parameter:
//	words: number of visual words
//	numImages: number of total images
BoVW::BoVW(int words, int numImages)
{
	NUMVOCAB=words;
	numDocument=numImages;
	F.resize(NUMVOCAB);
	IDF.resize(NUMVOCAB);
	Invert.resize(NUMVOCAB);
	score.resize(numDocument);

	TFName = "tf.txt";
	IDFName = "idf.txt";
	VOCName = "dictionary.yml";

}

//ADD trainingModule, LHW
void BoVW::BoVW_training(string file_root_path, string image_list_name_file, string training_output_path)
{
	
	vector<KeyPoint> kps_db;
	vector<KeyPoint> total_sift;
	//file which contains image names
	ifstream Img_list;
	char name[50];

	string file_list_name_total = file_root_path + image_list_name_file;

	Img_list.open(file_list_name_total);

	//sift detector
	cv::SIFT s;
	Mat descriptor;
	Mat features_unclustered;
	vector<Mat> img_descriptor_list;

	cout << "extract SIFT from training image data" << endl;
	while (Img_list.getline(name, sizeof(name)))
	{
		Mat img = imread(file_root_path + name);
		s.detect(img, kps_db);
		s.compute(img, kps_db, descriptor);
		//collects features extracted from images
		features_unclustered.push_back(descriptor);
		cout << name << " extracted" << endl;

		img_descriptor_list.push_back(descriptor);
	}

	Img_list.close();
	int bag_size = NUMVOCAB;

	TermCriteria tc(CV_TERMCRIT_ITER, 100, 0.001);

	int retries = 1;
	int flags = KMEANS_PP_CENTERS;

	//clustering
	BOWKMeansTrainer bowTrainer(bag_size, tc, retries, flags);
	Mat dictionary = bowTrainer.cluster(features_unclustered);


	flann::KDTreeIndexParams indexParams(8);
	kdTree.build(dictionary, indexParams, cvflann::FLANN_DIST_L1);
	cout << "KD Tree Complete" << endl;

	cout << "Calculate inverted file list..." << endl;

	numDocument = 0;
	for (int i = 0; i < img_descriptor_list.size(); i++)
	{
		Mat indices, dists;
		kdTree.knnSearch(img_descriptor_list[i], indices, dists, 1, cv::flann::SearchParams(64));

		ClearF();

		for (int j = 0; j < indices.rows; j++)
		{
			InsertF(indices.at<int>(j, 0));
		}
		for (int j = 0; j < NUMVOCAB; j++)
		{
			InsertTF(j, numDocument, indices.rows);
		}
		numDocument++;
	}

	//save dictionary, TF/IDF value

	cout << "Save visual words and TF IDF..." << endl;
	//save visual words
	FileStorage fs(training_output_path + VOCName, FileStorage::WRITE);
	fs << "vocabulary" << dictionary;
	fs.release();


	//save TF/IDF value
	ofstream idfFile(training_output_path + IDFName);
	ofstream tfFile(training_output_path + TFName);
	for (int i = 0; i < NUMVOCAB; i++)
	{
		IDF[i] = log(NUMVOCAB / (IDF[i] + 1));
		//idfFile << IDF[i] << endl;
	}

	for (int i = 0; i < NUMVOCAB; i++) {
		for (list <invertedList>::iterator it = Invert[i].begin(); it != Invert[i].end(); ++it)
		{
			tfFile << it->image << " " << it->TF << " ";
		}
		tfFile << endl;
	}
	idfFile.close();
	tfFile.close();
}


//ADD testModule from saved training file, LHW
vector<string> BoVW::BoVW_matcher_from_saved_training(std::string training_file_list,string training_file_root, Mat img, int nOftopRank)
{
	//IDF Loader
	ifstream idfFile(training_file_root + TFName);
	ifstream tfFile(training_file_root + IDFName);

	for (int i = 0; i < NUMVOCAB; i++)
	{
		idfFile >> IDF[i];
	}
	for (int i = 0; i < NUMVOCAB; i++) {
		string line;
		getline(tfFile, line);
		stringstream ss(line);
		int previews = -1;
		while (ss)
		{
			char c;
			invertedList node;
			ss >> node.image;
			if (previews == node.image)
				break;
			ss >> node.TF;
			Invert[i].push_back(node);
			previews = node.image;
		}
	}

	//dictionary loader
	FileStorage fs(training_file_root + VOCName, FileStorage::READ);
	fs["vocabulary"] >> dictionary;
	fs.release();



	cv::SIFT s;
	Mat descriptor;
	vector<KeyPoint> kps_db;
	Mat indices, dists;
	s.detect(img, kps_db);
	s.compute(img, kps_db, descriptor);
	//KD tree
	flann::KDTreeIndexParams indexParams(8);
	kdTree.build(dictionary, indexParams, cvflann::FLANN_DIST_L1);; kdTree.knnSearch(descriptor, indices, dists, 1, cv::flann::SearchParams(32));


	string img_dir = training_file_list;
	ifstream Img_list;
	char name[50];
	Img_list.open(img_dir);
	int i = 0;
	//saving image names
	while (Img_list.getline(name, sizeof(name)))
	{
		score[i].index = name;
		score[i].score = 0.0;
		i++;
	}
	Img_list.close();


	//scoring
	ClearF();
	for (int i = 0; i < indices.rows; i++)
	{
		InsertF(indices.at<int>(i, 0));
	}

	for (int i = 0; i < NUMVOCAB; i++) {
		if (F[i]>0)
		for (list <invertedList>::iterator it = Invert[i].begin(); it != Invert[i].end(); ++it)
		{
			float TF = (float)F[i] / (float)indices.rows;
			score[it->image].score += (abs(TF - it->TF) - (TF)-(it->TF))*IDF[i];
		}
	}

	//rank in ascending order
	sort(score.begin(), score.end(), compare_score);
	vector<string> result;

	for (int j = 0; j<nOftopRank; j++)
	{
		result.push_back(score[j].index);
	}

	return result;
}

//Makes visual words from images in database
void BoVW::BoVW_maker()
{
	/*
	Training image로 VW 만들기
	*/
	//image directory
	string img_dir = "../../DB/Chest_PA_nodule_mask_100case_20170428/jpeg/";
	vector<KeyPoint> kps_db;
	vector<KeyPoint> total_sift;
	//file which contains image names
	ifstream Img_list;
	char name[50];
	Img_list.open(img_dir + "img_list_txt");

	//sift detector
	cv::SIFT s; 
	Mat descriptor;
	Mat features_unclustered;
	while(Img_list.getline(name,sizeof(name)))
	{
		Mat img =imread(img_dir+name);
		s.detect(img,kps_db);
		s.compute(img,kps_db,descriptor);
		//collects features extracted from images
		features_unclustered.push_back(descriptor);
		cout <<name <<" extracted"<<endl;
	}

	Img_list.close();
	int bag_size= NUMVOCAB;

	TermCriteria tc(CV_TERMCRIT_ITER,100,0.001);

	int retries =1;
	int flags=KMEANS_PP_CENTERS;

	//clustering
	BOWKMeansTrainer bowTrainer(bag_size,tc,retries,flags);
	Mat dictionary = bowTrainer.cluster(features_unclustered);

	//save visual words
	FileStorage fs("dictionary.yml",FileStorage::WRITE);
	fs << "vocabulary" <<dictionary;
	fs.release();

}


//BoVW initializing TF-IDF
//Needs to call before matching images



void BoVW::BoVW_Init()
{
	//read BoVW 
	FileStorage fs("dictionary.yml",FileStorage::READ);
	fs["vocabulary"] >> dictionary;
	fs.release();

	//KD tree
	flann::KDTreeIndexParams indexParams(8);
	kdTree.build(dictionary,indexParams,cvflann::FLANN_DIST_L1);
	cout << "KD Tree Complete" << endl;

	string img_dir = "../../DB/Chest_PA_nodule_mask_100case_20170428/jpeg/";
	vector<KeyPoint> kps_db;
	ifstream Img_list;
	char name[50];
	Img_list.open(img_dir + "img_list.txt");

	cv::SIFT s;
	Mat descriptor;
	numDocument=0;
	while(Img_list.getline(name,sizeof(name)))
	{
		//get image descriptor
		Mat img =imread(img_dir+name);
		Mat indices, dists;
		s.detect(img,kps_db);
		s.compute(img,kps_db,descriptor);
		cout <<name <<" extracted"<<endl;

		searchDataName.push_back(name);

		img.empty();
		kps_db.empty();
		kdTree.knnSearch(descriptor,indices,dists,1,cv::flann::SearchParams(64));

		//making tf-idf records
		ClearF();
		for (int j = 0; j < indices.rows; j++)
		{
			InsertF(indices.at<int>(j, 0));
		}
		for (int j = 0; j < NUMVOCAB; j++)
		{
			InsertTF(j, numDocument, indices.rows);
		}
		numDocument++;
	}
	Img_list.close();

	/* for comments I marked under can be use when the inverted File needs to be saved*/

	ofstream idfFile("idf.txt");
	ofstream tfFile("tf.txt");
	for (int i = 0; i < NUMVOCAB; i++)
	{
		IDF[i] = log(NUMVOCAB / (IDF[i] + 1));
		//idfFile << IDF[i] << endl;
	}
	
	for (int i = 0; i < NUMVOCAB; i++) {
		for (list <invertedList>::iterator it = Invert[i].begin(); it != Invert[i].end(); ++it)
		{
			tfFile << it->image << " " <<it->TF<<" ";
		}
		tfFile << endl;
	}
	idfFile.close();
	tfFile.close();
	
}

//Constructor of BoVW
//parameter:
//	img: input images
//	number: number of top similar images to see
//	return: a list of name of similar images


vector<string> BoVW::BoVW_matcher(Mat img,int number)
{
	
	//if invertedfile has been saved
	ifstream idfFile("idf.txt");
	ifstream tfFile("tf.txt");
	for (int i = 0; i < NUMVOCAB; i++)
	{
		idfFile >> IDF[i];
	}
	for (int i = 0; i < NUMVOCAB; i++) {
		string line;
		getline(tfFile,line);
		stringstream ss(line);
		int previews=-1;
		while(ss)
		{
			char c;
			invertedList node;
			ss >> node.image;
			if(previews==node.image)
				break;
			ss >>node.TF;
			Invert[i].push_back(node);
			previews = node.image;
		}
	}

	FileStorage fs("dictionary.yml",FileStorage::READ);
	fs["vocabulary"] >> dictionary;
	fs.release();
	

	
	
	
	


	cv::SIFT s;
	Mat descriptor;
	vector<KeyPoint> kps_db;
	Mat indices, dists;
	s.detect(img,kps_db);
	s.compute(img,kps_db,descriptor);
	//KD tree
	flann::KDTreeIndexParams indexParams(8);
	kdTree.build(dictionary, indexParams, cvflann::FLANN_DIST_L1);; kdTree.knnSearch(descriptor, indices, dists, 1, cv::flann::SearchParams(32));

	string img_dir = "../../DB/Chest_PA_nodule_mask_100case_20170428/jpeg/";
	ifstream Img_list;
	char name[50];
	Img_list.open(img_dir + "img_list.txt");
	int i=0;
	//saving image names
	while(Img_list.getline(name,sizeof(name)))
	{
		score[i].index=name;
		score[i].score=0.0;
		i++;
	}
	Img_list.close();

	//scoring
	ClearF();
	for (int i = 0; i < indices.rows; i++)
	{
		InsertF(indices.at<int>(i, 0));
	}

	for (int i = 0; i < NUMVOCAB; i++) {
		if (F[i]>0)
			for (list <invertedList>::iterator it = Invert[i].begin(); it != Invert[i].end(); ++it)
			{
				float TF = (float)F[i] / (float)indices.rows;
				score[it->image].score += (abs(TF - it->TF)-(TF)-(it->TF))*IDF[i];
			}
	}

	//rank in ascending order
	sort(score.begin(), score.end(), compare_score);
	vector<string> result;
	for(int j=0;j<number;j++)
	{
		result.push_back(score[j].index);
	}
	return result;
}
bool BoVW::compare_score(const scoring &a, const scoring &b)
{
	return a.score < b.score;
}

void BoVW::ClearF()
{
	for (int i = 0; i < NUMVOCAB; i++)
		F[i] = 0;
}

void BoVW::InsertF(int t)
{
	F[t]++;
}

void BoVW::InsertTF(int t, int name, int numFeatures)
{
	if (F[t] > 0) {
		InsertIDF(t);
		invertedList node;
		node.image = name;
		node.TF = (float)F[t]/(float)numFeatures;
		Invert[t].push_back(node);
	}
}


void BoVW::InsertIDF(int t)
{
	IDF[t]++;
}

