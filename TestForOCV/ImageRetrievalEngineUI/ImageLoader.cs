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




/*
 * Query Image 불러오기
 * 
 * private void btn_browse_Click(object sender, System.EventArgs e)
        {
            try
            {
                OpenFileDialog open = new OpenFileDialog();
                open.Filter = "Image Files(*.jpg; *.jpeg; *.gif; *.bmp)|*.jpg; *.jpeg; *.gif; *.bmp";
               if (open.ShowDialog() == DialogResult.OK)
                {
                    Bitmap bit = new Bitmap(open.FileName);
                 pictureBox1.Image = bit;
                }
            }
            catch (Exception)
            {
                throw new ApplicationException("Failed loading image");
            }
        }
 * */
namespace ImageRetrievalEngineUI
{
    class ImageLoader
    {
        public Image queryImg;
        public Mat img;
        CenterController cc;

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

            img = Cv2.ImRead(fileName);

            cc.queryImg = queryImg;

            cc.drawImage(cc.myWindow.QueryImg,new BitmapImage(new Uri(fileName)));
            cc.drawImage(cc.myWindow.ZoomedImage, new BitmapImage(new Uri(fileName)));

            //extract the only Name of file...
            string [] tmpStr = fileName.Split('\\');

            //information 설정...
            cc.setTxtbox(cc.myWindow.FileNameBox, tmpStr[tmpStr.Length-1]);
            cc.setTxtbox(cc.myWindow.TypeNameBox, "CXR");
            cc.setTxtbox(cc.myWindow.ResolutionBox, queryImg.Size.Width.ToString() + " X " + queryImg.Size.Height.ToString());
            
            return true;
             
        }
    }
}
