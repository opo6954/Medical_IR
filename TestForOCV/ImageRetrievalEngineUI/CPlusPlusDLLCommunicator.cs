using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

using OpenCvSharp;
using OpenCvSharp.CPlusPlus;
using OpenCvSharp.Extensions;

/*
 * C++의 dll을 C#으로 불러와서 포팅하는 함수임
 * Static class로써 수행함
 * Static으로 불러와야 함
 * */
namespace ImageRetrievalEngineUI
{
    class CPlusPlusDLLCommunicator
    {
        //일단 이 부분에 static으로 path를 죄다 넣자
        //원래는 parameter.ini에서 직접 불러서 해야 합니다

        //search할 부분의 가장 root가 되는 path임
        public static string searchRootPath ="../../../../data/searchImg/";

        //search할 image의 list가 담긴 txt의 path
        public static string searchImgListName = "imgList.txt";

        //search할 image가 담긴 path임
        public static string searchImgPath = "IMG";

        //search할 vector가 담긴 path임
        public static string searchVectorPath = "featureVectors";

        

        //search할 ROI의 txt가 담긴 path임
        public static string searchROIPath = "ROI";

        //search할 ROI의 list가 담긴 txt의 path임
        public static string searchROIListName = "ROItxtList.txt";

        //search할 ROI의 image가 담긴 path임
        public static string searchROIImgPath = "ROIIMG";

        //k-nn에서 k의 값임, 즉 5번째까지 가까운 이미지를 가져옴
        public static int n = 5;
        

        [DllImport("RetrievalEngineDll.dll", CharSet=CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        extern public static bool trainingSearchSpace([MarshalAs(UnmanagedType.LPStr)] string _imageInputRootPath, [MarshalAs(UnmanagedType.LPStr)] string _vectorOutputRootPath);

        [DllImport("RetrievalEngineDll.dll", CharSet=CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        extern public static bool initRetrieval([MarshalAs(UnmanagedType.LPStr)] string _imageInputRootPath, [MarshalAs(UnmanagedType.LPStr)] string _vectorOutputRootPath);

        [DllImport("RetrievalEngineDll.dll", CharSet=CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        extern public static bool initDescriptors();

        [DllImport("RetrievalEngineDll.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        extern public static bool retrievalCurrImage(byte[] img, int width, int height, int n, int[] ID, int[] ROISeq);


        public static byte[] mat2byteArray(Mat m)
        {
            int width = m.Width;
            int height = m.Height;

            int channel = m.Channels();

            int size = width * height * channel;

            byte[] imgData = new byte[size];

            Marshal.Copy(m.Clone().Data, imgData, 0, size);

            return imgData;
        }


        /*
         * 이 부분에서 array of ID랑 array of ROISeq를 c#에서 할당한 후 c++에서 걍 값만 넣게 할 수 있고 
         * 아니면 c#에서는 걍 초기화 안한 int array를 주고 c++에서 초기화하고 값을 넣어서 보내줘도 됨
         * */

        

    }
    /*
     * 	//특정 폴더 안에 있는 ROI의 LBP vector를 추출해서 저장하기(학습하기)
	//DLL상으로는 전체 function만 노출시키고 내부적으로는 1개 image vector 저장 등의 작업을 거치자
	//bool trainingSearchSpace(const char* _imageInputRootPath, const char* _vectorOutputRootPath)
	__declspec(dllexport) bool trainingSearchSpace(const char* _imageInputRootPath, const char* _vectorOutputRootPath);

	//Retrieval 초기화(저장된 LBP descriptor 가져오기
	__declspec(dllexport) bool initRetrieval(const char* _imageInputRootPath, const char* _vectorOutputRootPath);

	//Descriptor 초기화이긴 한데 일단 C++상에서만 처리해도 될듯...
	__declspec(dllexport) bool initDescriptors();

	//image를 input으로 주면 가장 비슷한 n개의 image를 리턴함
	__declspec(dllexport) bool retrievalCurrImage(unsigned char* img, int width, int height, int n, int* ID, int* ROISeq);
     * */
}
