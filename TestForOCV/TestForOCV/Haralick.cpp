#include "Haralick.h"
#include "Feature extractor.h"
#include <iostream>
#define PI 3.141592654


#define acc_glcm true	//true : make accumulated glcm directly, false : make 3dimensional glcm according to direction

#define ASM //AngularSecondMoment
#define Contrast
#define Correlation
#define SSV //Sum of squares:variance
#define IDM //Inverse Difference Moment
#define SA	//Sum Average
#define SV //Sum Variance
#define SE //Sum Entropy
#define Entropy
#define DV //Difference Variance
#define DE //Difference Entropy
#define IMCX	//Information Measures of Correlation
#define IMCY	//Information Measures of Correlation
//#define MCC //Maximum correlation coefficient <--¹Ì±¸Çö



Mat Haralick(Mat img, int p1, int d)
{
	Mat H = Mat::zeros(1, 13, CV_32FC1);	//Haralick features
	float e = 0.0000015;		//for log(p+e)
								//p1=1 ; 0 deg(right)
								//p1=2 ; 0 deg, 90 deg
								//p1=4 ; 0 deg, 45 deg, 90 deg, 135 deg(counter clockwise)
								//p1=8 ; 0 deg, 45 deg, 90 deg, 135 deg, 180 deg, 225 deg, 270 deg, 315 deg, 360 deg
								//d; distance, defult=1

								//Check p1
	if (p1 < 1 || p1>8) {
		p1 = 4;
	}
	switch (p1) {
	case 3: p1 = 4; break;
	case 5:
	case 6:
	case 7: p1 = 8; break;
	}
	//check # of directions
	std::cout << "# of direction is " << p1 << std::endl;

	Mat im_gray;	//gray image

					//image to gray
	if (img.channels() == 3)
		cvtColor(img, im_gray, CV_RGB2GRAY);
	else
		im_gray = img;

	int R = 0;	//normalized factor

#if !acc_glcm
				//initiate glcm
	const int sz[3] = { p1, 256, 256 };
	//Mat glcm(3, sz, CV_32SC1);
	Mat a_tmp_glcm = Mat::zeros(3, sz[3], CV_32SC1);

	//glcm(degree,pix1,pix2)
	//compute glcm
	for (int i = 0; i<p1; i++) {
		int r1 = 0, r2 = 0;
		int c1 = 0, c2 = 0;

		double deg;
		if (p1 > 4) {
			deg = 360 / p1*i;
		}
		else {
			deg = 180 / p1*i;
		}

		//check degree
		//std::cout << i + 1 << "th degree : " << deg << std::endl;

		if (deg > 0 && deg < 180) r1 = d;
		if (deg > 180 && deg < 360) r2 = -d;
		if (deg > 90 && deg < 270) c1 = d;
		if (deg < 90 || deg>270) c2 = -d;

		double rad = deg / 180 * PI;
		//check rad
		//std::cout << i + 1 << "th radian : " << rad << std::endl;

		for (int row = 0 + r1; row < im_gray.rows + r2; row++) {
			for (int col = 0 + c1; col < im_gray.cols + c2; col++) {
				//count at (center,comparison_target)
				unsigned center = im_gray.at<uchar>(row, col);
				unsigned comparison_target = im_gray.at<uchar>(row + d*round(-sin(rad)), col + d*round(cos(rad)));
				//std::cout << glcm.at<int32_t>(i, center, comparison_target) << std::endl;
				a_tmp_glcm.at<int32_t>(i, center, comparison_target)++;
				R++;
			}
		}
	}

	Mat tmp_glcm = Mat(256, 256, CV_32SC1);
	for (int i = 0; i < p1; i++) {
		for (int j = 0; j < 256; j++) {
			for (int k = 0; k < 256; k++) {
				tmp_glcm.at<int32_t>(j, k) = a_tmp_glcm.at<int32_t>(i, j, k);
			}
		}
	}
#endif

#if acc_glcm
	//initiate glcm
	//Mat glcm(3, sz, CV_32SC1);
	Mat tmp_glcm = Mat::zeros(256, 256, CV_32SC1);

	//glcm(degree,pix1,pix2)
	//compute glcm
	for (int i = 0; i<p1; i++) {
		int r1 = 0, r2 = 0;
		int c1 = 0, c2 = 0;

		double deg;
		if (p1 > 4) {
			deg = 360 / p1*i;
		}
		else {
			deg = 180 / p1*i;
		}

		//check degree
		//std::cout << i + 1 << "th degree : " << deg << std::endl;

		if (deg > 0 && deg < 180) r1 = d;
		if (deg > 180 && deg < 360) r2 = -d;
		if (deg > 90 && deg < 270) c1 = d;
		if (deg < 90 || deg>270) c2 = -d;

		double rad = deg / 180 * PI;

		//std::cout << "deg : " << deg << " " << r1 << " " << r2 << " " << c1 << " " << c2 << std::endl;

		for (int row = 0 + r1; row < im_gray.rows + r2; row++) {
			for (int col = 0 + c1; col < im_gray.cols + c2; col++) {
				//count at (center,comparison_target)
				unsigned center = im_gray.at<uchar>(row, col);
				unsigned comparison_target = im_gray.at<uchar>(row + d*round(-sin(rad)), col + d*round(cos(rad)));
				//std::cout << glcm.at<int32_t>(i, center, comparison_target) << std::endl;
				tmp_glcm.at<int32_t>(center, comparison_target)++;
				R++;
			}
		}
		//std::cout << "R : " << R << std::endl;
	}
	Mat glcm;

#endif	//get glcm

	tmp_glcm.convertTo(glcm, CV_32FC1);	//integer type to float type
	Mat n_glcm = Mat::zeros(256, 256, CV_32FC1);	//To save normalized matrix
	float p_xuy[2 * 256 - 1] = { 0 }, p_xny[256] = { 0 }, px[256] = { 0 }, py[256] = { 0 };	//p(x+y),p(x-y)
	float u_px = 0, u_py = 0, v_px = 0, v_py = 0, u_p_xny = 0;	//mean and variance of px and py
	float HXY = 0, HXY1 = 0, HXY2 = 0, HX = 0, HY = 0;	//HXY is the same with f9 Entropy

#ifdef MCC
	Mat Q = Mat::zeros(256, 256, CV_32FC1);
	Mat Q_eigen = Mat::zeros(1, 256, CV_32FC1);
#endif

	/*
	//Check normalized matrix ; sum(all)=1
	float test = 0;
	for (int i = 0; i < 256; i++) {
	for (int j = 0; j < 256; j++) {
	//nomalize
	if (glcm.at<float>(i, j) != 0) {
	test += glcm.at<float>(i, j);
	}
	}
	}
	std::cout << "glcm summation : " << test << ", " << R << std::endl;
	*/


	//1st for
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			//nomalize
			if (glcm.at<float>(i, j) != 0) {
				n_glcm.at<float>(i, j) = glcm.at<float>(i, j) / R;

				px[i] += n_glcm.at<float>(i, j);
				py[j] += n_glcm.at<float>(i, j);
				p_xuy[i + j] += n_glcm.at<float>(i, j);
				p_xny[abs(i - j)] += n_glcm.at<float>(i, j);
				u_px += i*n_glcm.at<float>(i, j);
				u_py += j*n_glcm.at<float>(i, j);
				u_p_xny += abs(i - j)*n_glcm.at<float>(i, j);
			}
		}
	}

	/*
	//Check normalized matrix ; sum(all)=1
	float test = 0;
	for (int i = 0; i < 256; i++) {
	for (int j = 0; j < 256; j++) {
	//nomalize
	if(n_glcm.at<float>(i,j) != 0){
	test+= n_glcm.at<float>(i, j);
	}
	}
	}
	std::cout << "normalized matrix summation : " << test << ", " << R << std::endl;
	*/

	/*
	//Check
	std::cout << "px : " << px << std::endl;
	std::cout << "py : " << py << std::endl;
	std::cout << "u_px : " << u_px << std::endl;
	std::cout << "u_py : " << u_py << std::endl;
	std::cout << "u_p_xny : " << u_p_xny << std::endl;

	for (int i = 0; i < 255; i++) {
	std::cout << p_xuy[i] << " " << std::endl;
	if (p_xuy[i] == 0) {
	std::cout << "p_xuy == 0 : " << i << std::endl;
	}
	std::cout << p_xny[i] << " " << std::endl;
	if (p_xny[i] == 0) {
	std::cout << "p_xny == 0 : " << i << std::endl;
	}
	}
	*/

	//2nd for
	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
			if (n_glcm.at<float>(i, j) != 0) {
				v_px += pow(i - u_px, 2)*n_glcm.at<float>(i, j);
				v_py += pow(j - u_py, 2)*n_glcm.at<float>(i, j);

				HXY -= n_glcm.at<float>(i, j)*log(n_glcm.at<float>(i, j) + e);	//same with entropy(f9)

				HXY1 -= n_glcm.at<float>(i, j)*log(px[i] * py[j] + e);
			}
			HXY2 -= px[i] * py[j] * log(px[i] * py[j] + e);

#ifdef MCC
			for (int k = 0; k < 256; k++) {
				Q.at<float>(i, j) += n_glcm.at<float>(i, k)*n_glcm.at<float>(j, k) / (px[i] * py[k]);
			}
#endif

#ifdef SA	//sum average feature
			if (p_xuy[i + j] != 0) {
				H.at<float>(5) += (i + j)*n_glcm.at<float>(i, j);
				//std::cout << "i : " << i << ", j : " << j << ", add : " << (i + j)*p_xuy[i + j] << ", feature : " << H.at<float>(5) << std::endl;
			}
#endif
		}
	}

	for (int i = 0; i < 256; i++) {
		HX -= px[i] * log(px[i] + e);
		HY -= py[i] * log(py[i] + e);
	}

	/*
	Check
	std::cout << "v_px : " << v_px << std::endl;
	std::cout << "v_py : " << v_py << std::endl;
	std::cout << "HXY : " << HXY << std::endl;
	std::cout << "HXY1 : " << HXY1 << std::endl;
	std::cout << "HXY2 : " << HXY2 << std::endl;
	std::cout << "HX : " << HX << std::endl;
	std::cout << "HY : " << HY << std::endl;
	*/

	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < 256; j++) {
#ifdef ASM	//Energy
			H.at<float>(0) += pow(n_glcm.at<float>(i, j), 2);
#endif

#ifdef Contrast
			H.at<float>(1) += pow(i - j, 2)*n_glcm.at<float>(i, j);
#endif

#ifdef Correlation
			H.at<float>(2) += n_glcm.at<float>(i, j)*(i - u_px)*(j - u_py) / (v_px*v_py);
#endif

#ifdef SSV	//variance only for x
			H.at<float>(3) += pow(i - u_px, 2)*n_glcm.at<float>(i, j);
#endif

#ifdef IDM
			H.at<float>(4) += n_glcm.at<float>(i, j) / (1 + pow(i - j, 2));
#endif

			//SA

#ifdef SV	
			//Note:H.at(ind-1) is SA
			H.at<float>(6) += pow(i + j - H.at<float>(5), 2)*n_glcm.at<float>(i, j);
#endif

#ifdef SE
			if (n_glcm.at<float>(i, j) != 0) {
				H.at<float>(7) -= n_glcm.at<float>(i, j)*log(p_xuy[i + j] + e);
				//std::cout << "8 test : " << n_glcm.at<float>(i, j) << ", " << log(p_xuy[i + j]) << std::endl;
			}
#endif

#ifdef Entropy
			H.at<float>(8) -= n_glcm.at<float>(i, j)*log(n_glcm.at<float>(i, j) + e);
#endif

#ifdef DV
			H.at<float>(9) += pow(abs(i - j) - u_p_xny, 2)*n_glcm.at<float>(i, j);
#endif

#ifdef DE
			if (n_glcm.at<float>(i, j) != 0) {
				H.at<float>(10) -= n_glcm.at<float>(i, j)*log(p_xny[abs(i - j)] + e);
				//std::cout << "11 test : " << n_glcm.at<float>(i, j) << ", " << log(p_xny[i + j] + e) << std::endl;

			}
#endif

		}
	}

#ifdef IMCX	
	H.at<float>(11) = (HXY - HXY1) / max(HX, HY);
#endif

#ifdef IMCY
	H.at<float>(12) = sqrt(1 - exp(-2 * (HXY2 - HXY)));
#endif

#ifdef MCC
	//Find eigenvalues of Q
	//matrix -> Householder transformation -> Hessenberg matrix
	//QR decomposition iteration -> eigen values
	//eigen(n_glcm, Q_eigen,0,255);	//only for symmetric matrix n_glcm
	//H.at<float>(13) = sqrt(Q_eigen[2]);
#endif

	//print haralick feature
	for (int i = 0; i < 13; i++) {
		std::cout << i + 1 << " : " << H.at<float>(i) << std::endl;
	}


	//H = Mat(1,13,CV_32FC1)
	return H;
}
