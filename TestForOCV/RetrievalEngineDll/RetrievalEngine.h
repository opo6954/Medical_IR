#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

#define PI 3.141592654

using namespace cv;

#ifdef __cplusplus
extern "C" {
#endif
	/*
	어떻게 search space를 정리하는 게 깔끔할까?

	일단 img list와 ROI list in text가 있음

	이거를 가지고 c#에서도 읽고 c++에서도 각기 따로 따로 읽어야 함






	*/

	//특정 폴더 안에 있는 ROI의 LBP vector를 추출해서 저장하기(학습하기)
	//DLL상으로는 전체 function만 노출시키고 내부적으로는 1개 image vector 저장 등의 작업을 거치자
	//bool trainingSearchSpace(const char* _imageInputRootPath, const char* _vectorOutputRootPath)
	__declspec(dllexport) bool trainingSearchSpace(const char* _imageInputRootPath, const char* _vectorOutputRootPath);

	//Retrieval 초기화(저장된 LBP descriptor 가져오기
	__declspec(dllexport) bool initRetrieval(const char* _imageInputRootPath, const char* _vectorOutputRootPath);

	//Descriptor 초기화이긴 한데 일단 C++상에서만 처리해도 될듯...
	__declspec(dllexport) bool initDescriptors();

	//image를 input으로 주면 가장 비슷한 n개의 image를 리턴함
	__declspec(dllexport) bool retrievalCurrImage(unsigned char* img, int width, int height, int n, int* ID, int* ROISeq);







#ifdef __cplusplus
}
#endif



