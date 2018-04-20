
#include "Geometry.h"

using namespace geo;
list<string> file_list;
int main()
{
	/*
	vector <int> numfeatures;
	ifstream size("size.txt");
	while (!size.eof())
	{
		int num;
		size >> num;
		numfeatures.push_back(num);
	}
	size.close();
	*/
	//vector<string> lesion_image;
	//vector<string> lesion_text;

	char* lesion_image[6];
	char* lesion_text[6];
	Mat label(0, 2, CV_32SC1);
	Mat input_data(0, 1, CV_32SC1);

	vector<geometric> test;
	vector<geometric> extractor;

	ifstream features("features_pool.txt");
	ifstream testfeatures("test_pool.txt");
	Geometry geoFeature;

	lesion_image[0] = (char*)malloc(strlen("D:/ChestPA_6Class/1.nodule/PNG/") + 1);
	lesion_image[1] = (char*)malloc(strlen("D:/ChestPA_6Class/3.consolidation/PNG/") + 1);
	lesion_image[2] = (char*)malloc(strlen("D:/ChestPA_6Class/4.interstitial_opacity/PNG/") + 1);
	lesion_image[3] = (char*)malloc(strlen("D:/ChestPA_6Class/9.cardiomegaly/PNG/") + 1);
	lesion_image[4] = (char*)malloc(strlen("D:/ChestPA_6Class/10.pleural_effusion/PNG/") + 1);
	lesion_image[5] = (char*)malloc(strlen("D:/ChestPA_6Class/11.pneumothorax/PNG/") + 1);

	strcpy(lesion_image[0], "D:/ChestPA_6Class/1.nodule/PNG/");
	strcpy(lesion_image[1], "D:/ChestPA_6Class/3.consolidation/PNG/");
	strcpy(lesion_image[2], "D:/ChestPA_6Class/4.interstitial_opacity/PNG/");
	strcpy(lesion_image[3], "D:/ChestPA_6Class/9.cardiomegaly/PNG/");
	strcpy(lesion_image[4], "D:/ChestPA_6Class/10.pleural_effusion/PNG/");
	strcpy(lesion_image[5], "D:/ChestPA_6Class/11.pneumothorax/PNG/");

	lesion_text[0] = (char*)malloc(strlen("D:/ChestPA_6Class/1.nodule/PNG/") + 1);
	lesion_text[1] = (char*)malloc(strlen("D:/ChestPA_6Class/3.consolidation/PNG/") + 1);
	lesion_text[2] = (char*)malloc(strlen("D:/ChestPA_6Class/4.interstitial_opacity/PNG/") + 1);
	lesion_text[3] = (char*)malloc(strlen("D:/ChestPA_6Class/9.cardiomegaly/PNG/") + 1);
	lesion_text[4] = (char*)malloc(strlen("D:/ChestPA_6Class/10.pleural_effusion/PNG/") + 1);
	lesion_text[5] = (char*)malloc(strlen("D:/ChestPA_6Class/11.pneumothorax/PNG/") + 1);

	strcpy(lesion_text[0], "D:/ChestPA_6Class/1.nodule/TXT/");
	strcpy(lesion_text[1], "D:/ChestPA_6Class/3.consolidation/TXT/");
	strcpy(lesion_text[2], "D:/ChestPA_6Class/4.interstitial_opacity/TXT/");
	strcpy(lesion_text[3], "D:/ChestPA_6Class/9.cardiomegaly/TXT/");
	strcpy(lesion_text[4], "D:/ChestPA_6Class/10.pleural_effusion/TXT/");
	strcpy(lesion_text[5], "D:/ChestPA_6Class/11.pneumothorax/TXT/");
	/*
	lesion_image.push_back("D:/ChestPA_6Class/1.nodule/PNG/");
	lesion_image.push_back("D:/ChestPA_6Class/3.consolidation/PNG/");
	lesion_image.push_back("D:/ChestPA_6Class/4.interstitial_opacity/PNG/");
	lesion_image.push_back("D:/ChestPA_6Class/9.cardiomegaly/PNG/");
	lesion_image.push_back("D:/ChestPA_6Class/10.pleural_effusion/PNG/");
	lesion_image.push_back("D:/ChestPA_6Class/11.pneumothorax/PNG/");

	lesion_text.push_back("D:/ChestPA_6Class/1.nodule/TXT/");
	lesion_text.push_back("D:/ChestPA_6Class/3.consolidation/TXT/");
	lesion_text.push_back("D:/ChestPA_6Class/4.interstitial_opacity/TXT/");
	lesion_text.push_back("D:/ChestPA_6Class/9.cardiomegaly/TXT/");
	lesion_text.push_back("D:/ChestPA_6Class/10.pleural_effusion/TXT/");
	lesion_text.push_back("D:/ChestPA_6Class/11.pneumothorax/TXT/");
	*/
	geoFeature.loadImages(lesion_image, lesion_text, 6);


	while (!testfeatures.eof())
	{
		geometric temp;
		testfeatures >> temp.name >> temp.label >> temp.x >> temp.y >> temp.top_x >> temp.top_y >> temp.bottom_x >> temp.bottom_y >> temp.area >> temp.ratio;
		test.push_back(temp);
	}


	random_shuffle(test.begin(), test.end());
	//ofstream writer("top_ten_accuracy.txt");
	ofstream writer("top_twenty_accuracy.txt");

	double average = 0.0;
	int count = 0;
	int prev = 0;
	for (int i = 0; i < test.size(); i++)
	{
		Mat test_img = imread(test[i].name);
		resize(test_img, test_img, Size(600, 600));
		rectangle(test_img, Point(test[i].top_x, test[i].top_y), Point(test[i].bottom_x, test[i].bottom_y), Scalar(0, 0, 255), 3);
		resize(test_img, test_img, Size(300, 300));


		cout << "test:" << test[i].label << endl;
		imshow("test", test_img);
		int label[10];
		int index[10];
		char** top_ten = geoFeature.get_top_ten_location(test[i].name.c_str(), test[i].top_x, test[i].top_y, test[i].bottom_x, test[i].bottom_y, label,index);
		Mat top[10];

		int j = 1;
		int total_score = 0;
		if (prev != test[i].label)
		{
			average /= (double)count;
			writer << " label : " << prev << " number of sample:" << count << " average: " << average << endl;
			average = 0.0;
			count = 0;
			prev++;
		}
		int k = 0;
		for (int j = 0; j < 10; j++)
		{
			if (test[i].label == label[j])
				total_score++;
			if (k < 5) {
				int _index = index[j];
				geoFeature.showImage(_index, k + 1);
				k++;
				cout << "top" << k << ": " << label[j] << " ";
			}
		}
		cout << endl;
		//cout << "score : " << total_score << "/10 " << endl;
		average += (double)total_score;
		count++;
		char ch = waitKey();
		if (ch == 27) break;

	}
	average /= (double)count;
	writer << " label : " << prev << " number of sample:" << count << " average: " << average << endl;
	int a;
	cin >> a;
	return 0;
}
