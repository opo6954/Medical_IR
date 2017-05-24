#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

#define PI 3.141592654

using namespace cv;

#ifdef __cplusplus
extern "C" {
#endif
	/*
	��� search space�� �����ϴ� �� ����ұ�?

	�ϴ� img list�� ROI list in text�� ����

	�̰Ÿ� ������ c#������ �а� c++������ ���� ���� ���� �о�� ��






	*/

	//Ư�� ���� �ȿ� �ִ� ROI�� LBP vector�� �����ؼ� �����ϱ�(�н��ϱ�)
	//DLL�����δ� ��ü function�� �����Ű�� ���������δ� 1�� image vector ���� ���� �۾��� ��ġ��
	//bool trainingSearchSpace(const char* _imageInputRootPath, const char* _vectorOutputRootPath)
	__declspec(dllexport) bool trainingSearchSpace(const char* _imageInputRootPath, const char* _vectorOutputRootPath);

	//Retrieval �ʱ�ȭ(����� LBP descriptor ��������
	__declspec(dllexport) bool initRetrieval(const char* _imageInputRootPath, const char* _vectorOutputRootPath);

	//Descriptor �ʱ�ȭ�̱� �ѵ� �ϴ� C++�󿡼��� ó���ص� �ɵ�...
	__declspec(dllexport) bool initDescriptors();

	//image�� input���� �ָ� ���� ����� n���� image�� ������
	__declspec(dllexport) bool retrievalCurrImage(unsigned char* img, int width, int height, int n, int* ID, int* ROISeq);







#ifdef __cplusplus
}
#endif



