using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace ImageRetrievalEngineUI
{
    /// <summary>
    /// MainWindow.xaml에 대한 상호 작용 논리
    /// </summary>
    public partial class MainWindow : Window
    {
        CenterController cc;
        DebugWindow debug;

        public System.Windows.Controls.Image[] retrievalImgArray;
        public System.Windows.Controls.Image[] retrievalROIImgArray;
        public System.Windows.Controls.TextBlock[] retrievalImgNameArray;

        public MainWindow()
        {
            InitializeComponent();

            cc = new CenterController(this);
            debug = new DebugWindow();
            debug.Show();

            retrievalImgArray = new System.Windows.Controls.Image[CPlusPlusDLLCommunicator.n];
            retrievalROIImgArray = new System.Windows.Controls.Image[CPlusPlusDLLCommunicator.n];
            retrievalImgNameArray = new System.Windows.Controls.TextBlock[CPlusPlusDLLCommunicator.n];

            retrievalImgNameArray[0] = SearchImg1Name;
            retrievalImgNameArray[1] = SearchImg2Name;
            retrievalImgNameArray[2] = SearchImg3Name;
            retrievalImgNameArray[3] = SearchImg4Name;
            retrievalImgNameArray[4] = SearchImg5Name;


            retrievalImgArray[0] = SearchImg1;
            retrievalImgArray[1] = SearchImg2;
            retrievalImgArray[2] = SearchImg3;
            retrievalImgArray[3] = SearchImg4;
            retrievalImgArray[4] = SearchImg5;

            retrievalROIImgArray[0] = SearchROIImg1;
            retrievalROIImgArray[1] = SearchROIImg2;
            retrievalROIImgArray[2] = SearchROIImg3;
            retrievalROIImgArray[3] = SearchROIImg4;
            retrievalROIImgArray[4] = SearchROIImg5;

            if (cc.retriever.initSearchVector() == true)
            {
                MessageBox.Show("Init success to load feature vector...");
            }
        }

        //click button to image open
        private void OpenImg_Click(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();

            dlg.DefaultExt = ".png";
            
            dlg.Filter = "JPG Files (*.jpg)|*.jpg|PNG Files (*.png)|*.png|JPEG Files (*.jpeg)|*.jpeg|GIF Files (*.gif)|*.gif";

            Nullable<bool> result = dlg.ShowDialog();


            if (result == true)
            {
                if (cc.loadingImage(dlg.FileName) == false)
                {
                    MessageBox.Show("Error in load file...");
                }
                
            }
        }

        //click button to search Image
        private void SearchButton_Click(object sender, RoutedEventArgs e)
        {
            cc.retrievalImage();
        }
        //Zoom image에서 mousewheel을 할 경우
        private void ZoomedImage_MouseWheel_1(object sender, MouseWheelEventArgs e)
        {
            ScaleTransform st = new ScaleTransform();

            double zoom = cc.zoomingWithWheel(e.Delta);

            st.ScaleX +=zoom;
            st.ScaleY += zoom;

            ROIDrawCanvas.LayoutTransform = st;

            ZoomScale.Text = (zoom+1).ToString();
            showScrollTxt();


            e.Handled = true;

        }

        private void myScroll_ScrollChanged_1(object sender, ScrollChangedEventArgs e)
        {
            myScroll.UpdateLayout();
            
            showScrollTxt();
        }

        public void showScrollTxt()
        {
            string currX = System.Convert.ToInt32(myScroll.HorizontalOffset).ToString();
            string currY = System.Convert.ToInt32(myScroll.VerticalOffset).ToString();
            string maxX = System.Convert.ToInt32(myScroll.ScrollableWidth).ToString();
            string maxY = System.Convert.ToInt32(myScroll.ScrollableHeight).ToString();

            CenterPosition.Text = currX + "/" + maxX + " : " + currY + "/" + maxY;
        }
        
        //ROI 삭제
        private void DeleteROI_Click_1(object sender, RoutedEventArgs e)
        {
            cc.selector.deleteROI();
        }

        //ROI 수정
        private void ROI_Click_1(object sender, RoutedEventArgs e)
        {
            /*
             * Modify ROI 부분: 현재 개발중...... 170522
             * */
            //cc.selector.enterModifyROIMode();
        }

        //ROI 새로 생성
        private void NewROI_Click_1(object sender, RoutedEventArgs e)
        {
            cc.selector.enterCreateROIMode();
        }

        private void ZoomedImage_PreviewMouseLeftButtonDown_1(object sender, MouseButtonEventArgs e)
        {
            cc.selector.isClickROI(e.GetPosition(ROIDrawCanvas));
        }

        private void ZoomedImage_PreviewMouseLeftButtonUp_1(object sender, MouseButtonEventArgs e)
        {
            cc.selector.isUpROI(e.GetPosition(ROIDrawCanvas));
        }

        private void ZoomedImage_PreviewMouseMove_1(object sender, MouseEventArgs e)
        {
            cc.selector.isMoveROI(e.GetPosition(ROIDrawCanvas));
        }
    }
}
