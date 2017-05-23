using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using OpenCvSharp.Extensions;
using OpenCvSharp.CPlusPlus;
using OpenCvSharp.UserInterface;
/*
 * Query Image를 Search Space로부터 찾기
 * */
namespace ImageRetrievalEngineUI
{
    class ImageRetriever
    {
        CenterController cc;
        public ImageRetriever(CenterController _cc)
        {
            cc = _cc;
        }

        //처음에 프로그램 시작할 때 vector를 읽는 함수임
        public void initSearchVector()
        {
            //C++ DLL Communicator로부터 초기화 함수 부르기, 모든 LBP vector 저장해놓기, DLL의 global 상에서... 그리고 Name array of int with ID를 부르기 
        }

        public bool returnRetrievalResult(Mat queryImgROI, int[] imageResult)
        {
            //C++ DLL Communicator로부터 retrieval 함수 부르기, array of int with ID를 가져옴
            return true;
        }

    }
}
