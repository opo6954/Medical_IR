#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\imgproc\imgproc.hpp"

using namespace cv;

#ifdef __cplusplus
extern "C" {
#endif

	//Retrieval 초기화(저장된 LBP descriptor 가져오기
	__declspec(dllexport) bool initRetrieval();

	//Descriptor 초기화이긴 한데 일단 C++상에서만 처리해도 될듯...
	__declspec(dllexport) bool initDescriptors();

	//image를 input으로 주면 가장 비슷한 n개의 image를 리턴함
	__declspec(dllexport) bool retrievalCurrImage();




#ifdef __cplusplus
}
#endif

