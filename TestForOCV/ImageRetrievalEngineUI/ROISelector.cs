using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Drawing;


using OpenCvSharp.Extensions;
using OpenCvSharp.CPlusPlus;
using OpenCvSharp.UserInterface;

/*
 * ROI 선택하는 함수임
 * 일단 이 함수에서는 기본적인 WPF 상에서의 Point를 가져오고, 
 * 실제 Retrieval로 넘겨줄 때에는 이미지의 크기와 현재의 줌 상태, 현재의 스크롤 상태에 따라서 이미지를 딸 수 있는 명확한 ROI를 계산하자
 * */
namespace ImageRetrievalEngineUI
{
    class ROISelector
    {
        CenterController cc;
        bool isStartROI = false;
        bool isROIMode = false;//ROI 버튼을 눌러서 ROI 모드일 시 true임

        bool isROICreateMode=false;//ROI 새로 만드는 거 확인하는 플래그
        bool isROIModifyMode = false;//ROI 변경하는 거 확인하는 플래그

        System.Windows.Shapes.Rectangle rectangle = new System.Windows.Shapes.Rectangle();
                

        //ROI 클릭 후 첫 시작 위치
        System.Windows.Point startPos;
        //드롭을 할 때의 ROI 최종 위치
        System.Windows.Point lastPos;

        public ROIRegion currROI;
        public ROIRegion currROIOnImage;

        //실제 original image의 resolution
        int resX, resY;

        //UI 상에 보이는 image의 resolution
        int imgX, imgY;

        Mat roiRegion;


        public ROISelector(CenterController _cc)
        {
            cc = _cc;
        }
        public bool enterModifyROIMode()
        {
            if (isROIModifyMode == false)
            {
                isROIModifyMode = true;
                return true;
            }
            else
                return false;
        }

        public bool enterCreateROIMode()
        {
            if (isROICreateMode == false)
            {
                isROICreateMode = true;

                if (cc.queryImgMat.Empty() == false)
                {
                    resX = cc.queryImgMat.Size().Width;
                    resY = cc.queryImgMat.Size().Height;

                    imgX = System.Convert.ToInt32(cc.myWindow.ZoomedImage.ActualWidth);
                    imgY = System.Convert.ToInt32(cc.myWindow.ZoomedImage.ActualHeight);

                }
                return true;
            }
            else
            {
                return false;
            }
        }
        //처음에 ROI 누를 경우, 시작함
        public bool isClickROI(System.Windows.Point pos)
        {
            if (isStartROI == false && isROICreateMode == true)
            {
                startPos = pos;
                isStartROI = true;
                return true;
            }

            return false;
        }

        public bool isMoveROI(System.Windows.Point pos)
        {
            
            cc.setTxtbox(cc.myWindow.ROIPos, System.Convert.ToInt32(pos.X).ToString() + " , " + System.Convert.ToInt32(pos.Y).ToString());

            if (isStartROI == true)
            {
                lastPos = pos;

                int x = System.Convert.ToInt32(startPos.X);
                int y = System.Convert.ToInt32(startPos.Y);

                int width = System.Convert.ToInt32(startPos.X - lastPos.X);
                int height = System.Convert.ToInt32(startPos.Y - lastPos.Y);

                if(width < 0)
                    width = width * -1;
                if(height < 0)
                    height = height * -1;
                
                rectangle.Width = width;
                rectangle.Height = height;

                cc.setTxtbox(cc.myWindow.ROISize, width.ToString() + " / " +  height.ToString());
                cc.setTxtbox(cc.myWindow.ROIPos, x.ToString() + " , " + y.ToString());


                cc.drawRectangle(cc.myWindow.ROIDrawCanvas,rectangle,x,y);

                return true;
            }
            return false;
        }
        //마우스를 뗄 때의 행동, currROI를
        public bool isUpROI(System.Windows.Point pos)
        {
            if (isStartROI == true)
            {
                
                lastPos = pos;

                currROI.x = System.Convert.ToInt32(startPos.X);
                currROI.y = System.Convert.ToInt32(startPos.Y);
                currROI.w = System.Convert.ToInt32(Math.Abs(lastPos.X - startPos.X));
                currROI.h = System.Convert.ToInt32(Math.Abs(lastPos.Y - startPos.Y));


                isROICreateMode = false;
                isStartROI = false;

                //이 부분에서 ROI 설정이 모두 된 것 이니까 실제 이미지 상에서의 ROI를 계산하자


                currROIOnImage.x = System.Convert.ToInt32((currROI.x / (double)imgX) * resX);
                currROIOnImage.y = System.Convert.ToInt32((currROI.y / (double)imgY) * resY);

                currROIOnImage.w = System.Convert.ToInt32((currROI.w / (double)imgX) * resX);
                currROIOnImage.h = System.Convert.ToInt32((currROI.h / (double)imgY) * resY);

                




                cc.setTxtbox(cc.myWindow.ROISize, currROIOnImage.w.ToString() + " / " + currROIOnImage.h.ToString());


                calculateROI();
                sendROI();




                

                



            }
            return true;
        }

        public bool deleteROI()
        {
            currROI.x = -1;
            currROI.y = -1;
            currROI.w = -1;
            currROI.h = -1;

            roiRegion = new Mat(new OpenCvSharp.CPlusPlus.Size(resX, resY), MatType.CV_8UC3);

            cc.drawImage(cc.myWindow.ROIImg, roiRegion.Clone());

            return cc.deleteRectangle(cc.myWindow.ROIDrawCanvas, rectangle);
        }

        public void calculateROI()
        {
            if (currROIOnImage.x > 0 && currROIOnImage.x + currROIOnImage.w < resX && currROIOnImage.y > 0 && currROIOnImage.y + currROIOnImage.h < resY)
            {
                roiRegion = new Mat(cc.queryImgMat, new OpenCvSharp.CPlusPlus.Rect(currROIOnImage.x, currROIOnImage.y, currROIOnImage.w, currROIOnImage.h));
                cc.drawImage(cc.myWindow.ROIImg, roiRegion.Clone());
            }
        }



        public void sendROI()
        {
            cc.queryImgMat_ROI = roiRegion.Clone();
            
        }

    }
}
