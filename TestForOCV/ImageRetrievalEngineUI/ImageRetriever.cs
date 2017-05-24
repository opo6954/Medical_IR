using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.IO;

using OpenCvSharp.Extensions;
using OpenCvSharp.CPlusPlus;
using OpenCvSharp.UserInterface;
/*
 * Query Image를 Search Space로부터 찾기
 * */
namespace ImageRetrievalEngineUI
{
    class searchImgInfo
    {
        //파일의 이름
        public string fileName;
        //ROI의 개수
        public int roiNumber;

        public searchImgInfo(string _fileName, int _roiNumber)
        {
            fileName = _fileName;
            roiNumber = _roiNumber;
        }

    };
    class ImageRetriever
    {
        CenterController cc;

        List<searchImgInfo> searchSpaceIdxMappingTable = new List<searchImgInfo>();

        bool isInitRetrieval = false;


        public ImageRetriever(CenterController _cc)
        {
            cc = _cc;
        }

        //처음에 프로그램 시작할 때 vector를 읽는 함수임
        public bool initSearchVector()
        {
            //일단 txt파일로부터 vector of ID and ROI를 읽어야 합니다

            //그 전에 dll의 init 읽자
            //LHWLHWLHWLHW

            CPlusPlusDLLCommunicator.initDescriptors();
            isInitRetrieval = CPlusPlusDLLCommunicator.initRetrieval(CPlusPlusDLLCommunicator.searchRootPath, CPlusPlusDLLCommunicator.searchRootPath + CPlusPlusDLLCommunicator.searchVectorPath + "/");
                

            string imgFileName = CPlusPlusDLLCommunicator.searchRootPath + CPlusPlusDLLCommunicator.searchImgPath + "/" + CPlusPlusDLLCommunicator.searchImgListName;
            string roiFileName = CPlusPlusDLLCommunicator.searchRootPath + CPlusPlusDLLCommunicator.searchROIPath + "/" + CPlusPlusDLLCommunicator.searchROIListName;

            if (isInitRetrieval == false)
            {
                MessageBox.Show("Wrong with init feature vector inside dll....");
                return false;
            }

            if (System.IO.File.Exists(imgFileName) == false)
            {
                MessageBox.Show("No file of search list exist; " + imgFileName);
                return false;
            }

            if (System.IO.File.Exists(roiFileName) == false)
            {
                MessageBox.Show("No file of roi list exist; " + roiFileName);
                return false;
            }

            string[] strID = System.IO.File.ReadAllLines(imgFileName, System.Text.Encoding.Default);
            string[] roiFileNameList = System.IO.File.ReadAllLines(roiFileName, System.Text.Encoding.Default);


            int[] roiNumberList = new int[strID.Length];

            for (int i = 0; i < roiFileNameList.Length; i++)
            {
                string tmpName = CPlusPlusDLLCommunicator.searchRootPath + CPlusPlusDLLCommunicator.searchROIPath + "/" + roiFileNameList[i];

                string readText="";
                
                if (System.IO.File.Exists(tmpName) == false)
                {
                    MessageBox.Show("No file of roi list inside exist; " + tmpName);
                    return false;
                }

                using (StreamReader readtext = new StreamReader(tmpName))
                {
                    readText = readtext.ReadLine();
                    readtext.Close();
                }

                int roiNTmp = int.Parse(readText);

                if (roiNTmp >= 0)
                {
                    roiNumberList[i] = roiNTmp;
                }
            }

            if (roiNumberList.Length != strID.Length)
            {
                MessageBox.Show("Not equal number of image file vs roi file...");
                return false;
            }


            for (int i = 0; i < strID.Length; i++)
            {
                searchImgInfo sii = new searchImgInfo(strID[i],roiNumberList[i]);
                searchSpaceIdxMappingTable.Add(sii);
            }


            return true;


            //C++ DLL Communicator로부터 초기화 함수 부르기, 모든 LBP vector 저장해놓기, DLL의 global 상에서... 그리고 Name array of int with ID를 부르기 
        }

        public bool returnRetrievalResult(Mat queryImgROI)
        {
            if (isInitRetrieval == false)
            {
                MessageBox.Show("Not init retrieval engine...");
                return false;
            }
            byte[] myRes = CPlusPlusDLLCommunicator.mat2byteArray(queryImgROI);

            int[] ID = new int[CPlusPlusDLLCommunicator.n];
            int[] ROISeq = new int[CPlusPlusDLLCommunicator.n];

            bool isSuccess = CPlusPlusDLLCommunicator.retrievalCurrImage(myRes, queryImgROI.Width, queryImgROI.Height, CPlusPlusDLLCommunicator.n, ID, ROISeq);

            if (isSuccess == false)
            {
                MessageBox.Show("Wrong with c++ retrieval dll...");
                return false;
            }
            updateRetrievalResult(ID, ROISeq);

            return true;
        }

        //실제 retrieval된 결과를 바탕으로 UI상에 뿌려주기
        //UI상에 알맞는 이미지만 load하면 됨
        public void updateRetrievalResult(int[] ID, int[] ROI)
        {
            for (int i = 0; i < CPlusPlusDLLCommunicator.n; i++)
            {
                int imgIdx = ID[i];

                //image file의 이름만 포함(확장자 포함)
                string imgFileName_Ext = searchSpaceIdxMappingTable[imgIdx].fileName;

                //image file의 확장자 포함된 전체 경로
                string imgFileName_Ext_Full = CPlusPlusDLLCommunicator.searchRootPath + CPlusPlusDLLCommunicator.searchImgPath + "/" + imgFileName_Ext;

                Mat currImg = Cv2.ImRead(imgFileName_Ext_Full);
                if (currImg.Empty() == true)
                {
                    MessageBox.Show("Cannot open image with " + imgFileName_Ext_Full);
                    return;
                }

                //혹여나 시간이 되면 이곳에 ROI를 그려도 좋음

                cc.setTxtbox(cc.myWindow.retrievalImgNameArray[i], imgFileName_Ext);

                cc.drawImage(cc.myWindow.retrievalImgArray[i], currImg);




                int idx_Ext = searchSpaceIdxMappingTable[imgIdx].fileName.LastIndexOf('.');

                if (idx_Ext <= 0)
                {
                    MessageBox.Show("Wrong with extention analysis...");
                    return;
                }

                string roiImgFileName_Ext = imgFileName_Ext;

                int roiIdx = ROI[i];


                roiImgFileName_Ext =  roiImgFileName_Ext.Insert(idx_Ext, "_" + roiIdx.ToString());

                string roiImgFileName_Ext_Full = CPlusPlusDLLCommunicator.searchRootPath + CPlusPlusDLLCommunicator.searchROIImgPath + "/" + roiImgFileName_Ext;


                Mat currROIImg = Cv2.ImRead(roiImgFileName_Ext_Full);
                if (currROIImg.Empty() == true)
                {
                    MessageBox.Show("Cannot open image with " + roiImgFileName_Ext_Full);
                    return;
                }

                cc.drawImage(cc.myWindow.retrievalROIImgArray[i], currROIImg);
   
            }
        }

    }
}
