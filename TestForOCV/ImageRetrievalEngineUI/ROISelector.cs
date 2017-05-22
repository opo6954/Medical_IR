using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Media;
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
            }
            return true;
        }

        public bool deleteROI()
        {
            currROI.x = -1;
            currROI.y = -1;
            currROI.w = -1;
            currROI.h = -1;

            return cc.deleteRectangle(cc.myWindow.ROIDrawCanvas, rectangle);
        }

        public void sendROI()
        {
            System.Drawing.Image image = cc.queryImg;
            
        }

    }
}
