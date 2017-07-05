#include "BoVW.h"

//! [namespace]
using namespace cv;
using namespace std;

#define NUMVOCAB 200

void BoVW::BoVW_maker()
{
	string img_dir = "../../DB/Chest_PA_nodule_mask_100case_20170428/jpeg/";
	vector<KeyPoint> kps_db;
	vector<KeyPoint> total_sift;
	ifstream Img_list;
	char name[50];
	Img_list.open(img_dir + "img_list.txt");

	cv::SIFT s;
	Mat descriptor;
	Mat features_unclustered;
	while(Img_list.getline(name,sizeof(name)))
	{
		Mat img =imread(img_dir+name);
		s.detect(img,kps_db);
		s.compute(img,kps_db,descriptor);
		features_unclustered.push_back(descriptor);
		cout <<name <<" extracted"<<endl;
	}
	Img_list.close();
	int bag_size= NUMVOCAB;

	TermCriteria tc(CV_TERMCRIT_ITER,100,0.001);

	int retries =1;
	int flags=KMEANS_PP_CENTERS;

	BOWKMeansTrainer bowTrainer(bag_size,tc,retries,flags);
	Mat dictionary = bowTrainer.cluster(features_unclustered);

	FileStorage fs("dictionary.yml",FileStorage::WRITE);
	fs << "vocabulary" <<dictionary;
	fs.release();

}

void BoVW::BoVW_Init()
{
	F.reserve(NUMVOCAB);
	IDF.reserve(NUMVOCAB);
	Invert.reserve(NUMVOCAB);
	Mat dictionary;
	FileStorage fs("dictionary.yml",FileStorage::READ);
	fs["vocabulary"] >> dictionary;
	fs.release();

	//KD tree
	flann::KDTreeIndexParams indexParams(8);
	kdTree.build(dictionary,indexParams,cvflann::FLANN_DIST_L1);
	cout << "KD Tree Complete";

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
		Mat img =imread(img_dir+name);
		Mat indices, dists;
		s.detect(img,kps_db);
		s.compute(img,kps_db,descriptor);
		cout <<name <<" extracted"<<endl;
		kdTree.knnSearch(descriptor,indices,dists,1,cv::flann::SearchParams(32));

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
	for (int i = 0; i < NUMVOCAB; i++)
	{
		IDF[i] = log(NUMVOCAB / (IDF[i] + 1));

	}
	Img_list.close();

}


vector<string> BoVW::BoVW_matcher(Mat img,int number)
{
	score.reserve(numDocument);

	cv::SIFT s;
	Mat descriptor;
	vector<KeyPoint> kps_db;
	Mat indices, dists;
	s.detect(img,kps_db);
	s.compute(img,kps_db,descriptor);
	kdTree.knnSearch(descriptor,indices,dists,1,cv::flann::SearchParams(32));

	string img_dir = "../../DB/Chest_PA_nodule_mask_100case_20170428/jpeg/";
	ifstream Img_list;
	char name[50];
	Img_list.open(img_dir + "img_list.txt");
	int i=0;
	while(Img_list.getline(name,sizeof(name)))
	{
		score[i].index=name;
		score[i].score=0.0;
		i++;
	}
	Img_list.close();
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
	sort(score.begin(), score.end(), compare_score);
	vector<string> result;
	for(int j=0;j<number;j++)
	{
		result.push_back(score[i].index);
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

