using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Media;
using System.Drawing;
using System.Windows.Media.Imaging;

using OpenCvSharp.Extensions;
using OpenCvSharp.CPlusPlus;
using OpenCvSharp.UserInterface;

namespace ImageRetrievalEngineUI
{
    class ImageLoader
    {
        public Image queryImg;
        public Mat queryimgMat;
        CenterController cc;
        public bool isLoadImage = false;

        public ImageLoader(CenterController _cc)
        {
            cc = _cc;
        }

        //image를 path로부터 불러오는 함수
        public bool loadImage(string fileName)
        {
            
            if (System.IO.File.Exists(fileName) == false)
                return false;

            queryImg = Image.FromFile(fileName);

            queryimgMat = Cv2.ImRead(fileName);

            cc.queryImg = queryImg;
            cc.queryImgMat = queryimgMat.Clone();

            cc.drawImage(cc.myWindow.QueryImg,new BitmapImage(new Uri(fileName)));
            cc.drawImage(cc.myWindow.ZoomedImage, new BitmapImage(new Uri(fileName)));

            //extract the only Name of file...
            string [] tmpStr = fileName.Split('\\');

            //information 설정...
            cc.setTxtbox(cc.myWindow.FileNameBox, tmpStr[tmpStr.Length-1]);
            cc.setTxtbox(cc.myWindow.TypeNameBox, "CXR");
            cc.setTxtbox(cc.myWindow.ResolutionBox, queryImg.Size.Width.ToString() + " X " + queryImg.Size.Height.ToString());

            isLoadImage = true;
            
            return true;
             
        }
    }
}
