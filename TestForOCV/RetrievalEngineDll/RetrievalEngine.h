#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

using namespace cv;

#ifdef __cplusplus
extern "C" {
#endif

	//Retrieval �ʱ�ȭ(����� LBP descriptor ��������
	__declspec(dllexport) bool initRetrieval();

	//Descriptor �ʱ�ȭ�̱� �ѵ� �ϴ� C++�󿡼��� ó���ص� �ɵ�...
	__declspec(dllexport) bool initDescriptors();

	//image�� input���� �ָ� ���� ����� n���� image�� ������
	__declspec(dllexport) bool retrievalCurrImage();




#ifdef __cplusplus
}
#endif

