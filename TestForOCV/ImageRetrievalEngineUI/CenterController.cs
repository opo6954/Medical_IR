using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.Windows.Media.Imaging;
using System.Windows.Media;
using System.Windows;


using OpenCvSharp.Extensions;
using OpenCvSharp.CPlusPlus;
using OpenCvSharp.UserInterface;

/*
 * 중앙에서 관련 처리를 담당하는 클래스임
 * */
namespace ImageRetrievalEngineUI
{

    public struct ROIRegion
    {
        public int x;
        public int y;
        public int w;
        public int h;

    };

    class CenterController
    {
        public ImageLoader loader;
        public ImageRetriever retriever;
        public ROISelector selector;
        public ImageZoomer zoomer;

        public ROIRegion currROI;
        


        public Image queryImg;
        public Mat queryImgMat;
        public Mat queryImgMat_ROI;

        public MainWindow myWindow;

        public CenterController(MainWindow _myWindow)
        {
            loader = new ImageLoader(this);
            retriever = new ImageRetriever(this);
            selector = new ROISelector(this);
            zoomer = new ImageZoomer(this);
            myWindow = _myWindow;
            
        }

        //relation between UI and each function
        public bool loadingImage(string filePath)
        {
            return loader.loadImage(filePath);
        }

        public bool retrievalImage()
        {
            if (loader.isLoadImage == false)
            {
                MessageBox.Show("No image load...");
                return false;
            }

            if (selector.isGetROI == false)
            {
                MessageBox.Show("No ROI region is selected...");
                return false;
            }
            if (queryImgMat_ROI.Empty() == true)
            {
                MessageBox.Show("Query Image ROI not found...");
                return false;
            }

            retriever.returnRetrievalResult(queryImgMat_ROI);

            return true;
        }

        //for zooming image...
        public double zoomingWithWheel(int delta)
        {
            return zoomer.mouseWheelZoom(delta);
        }
        //for getting scrollinfo
        public void setScrollInfo(int x, int y, int mx, int my)
        {
            zoomer.setCurrScroll(x, y, mx, my);
        }


        //주어진 canvas에 4개의 파라미터를 가진 사각형을 그리기d
        public void drawRectangle(System.Windows.Controls.Canvas canvas, System.Windows.Shapes.Rectangle rectangle, int x, int y)
        {
            rectangle.Fill = new SolidColorBrush() { Color = Colors.Red, Opacity = 0.45f };


            canvas.Dispatcher.Invoke(System.Windows.Threading.DispatcherPriority.Normal,
                new Action(
                    delegate()
                    {
                        //canvas에 rectange이 추가되지 않은 경우에만...
                        if (canvas.Children.Contains(rectangle) == false)
                        {
                            canvas.Children.Add(rectangle);
                            System.Windows.Controls.Canvas.SetTop(rectangle, y);
                            System.Windows.Controls.Canvas.SetLeft(rectangle, x);
                        }
                    }));
        }

        public bool deleteRectangle(System.Windows.Controls.Canvas canvas, System.Windows.Shapes.Rectangle rectangle)
        {
            bool returnValue = false;
            canvas.Dispatcher.Invoke(System.Windows.Threading.DispatcherPriority.Normal,
                new Action(
                    delegate()
                    {
                        //canvas에 rectange이 추가되지 않은 경우에만...
                        if (canvas.Children.Contains(rectangle) == true)
                        {
                            canvas.Children.Remove(rectangle);
                            returnValue = true;
                        }
                        else
                            returnValue = false;
                    }));

            return returnValue;
        }




        //from bitmapImage
        public void drawImage(System.Windows.Controls.Image imgOnControl, BitmapImage targetImage)
        {
            imgOnControl.Dispatcher.Invoke(System.Windows.Threading.DispatcherPriority.Normal,
                new Action(
                    delegate()
                    {
                        imgOnControl.Source = targetImage;
                    }));
        }


        //set image with respect to wb
        public void drawImage(WriteableBitmap wb, Mat source)
        {
            wb.Dispatcher.Invoke(System.Windows.Threading.DispatcherPriority.Normal,
                new Action(
                    delegate()
                    {
                        WriteableBitmapConverter.ToWriteableBitmap(source, wb);
                    }));
        }
        // set image wrt C# image control
        public void drawImage(System.Windows.Controls.Image imageInstance, Mat source)
        {
            imageInstance.Dispatcher.Invoke(System.Windows.Threading.DispatcherPriority.Normal,
                new Action(
                    delegate()
                    {
                        WriteableBitmap wb = new WriteableBitmap(source.Width, source.Height, 72, 72, PixelFormats.Bgr24, null);
                        WriteableBitmapConverter.ToWriteableBitmap(source, wb);
                        imageInstance.Source = wb;
                    }));
        }

        //set text box to given txt
        public void setTxtbox(System.Windows.Controls.TextBox txtOnControl, string txt)
        {
            txtOnControl.Dispatcher.Invoke(System.Windows.Threading.DispatcherPriority.Normal,
                new Action(
                    delegate()
                    {
                        txtOnControl.Text = txt;
                    }));
        }

        //set text block to given txt
        public void setTxtbox(System.Windows.Controls.TextBlock txtOnControl, string txt)
        {
            txtOnControl.Dispatcher.Invoke(System.Windows.Threading.DispatcherPriority.Normal,
                new Action(
                    delegate()
                    {
                        txtOnControl.Text = txt;
                    }));
        }

    }



}
