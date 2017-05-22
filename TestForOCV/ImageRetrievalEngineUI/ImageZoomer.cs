using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ImageRetrievalEngineUI
{
    class ImageZoomer
    {
        CenterController cc;
        double zoom = 0.0;
        int currX;
        int currY;
        int maxX;
        int maxY;


        public ImageZoomer(CenterController _cc)
        {
            cc = _cc;
        }
        public void setCurrScroll(int _currX, int _currY, int _maxX, int _maxY)
        {
            currX = _currX;
            currY = _currY;
            maxX = _maxX;
            maxY = _maxY;
        }

        public double mouseWheelZoom(int delta)
        {
            if (delta > 0)
                zoom = zoom + 0.1;
            else
                zoom = zoom - 0.1;
            if (zoom < 0)
                zoom = 0.0;
            return zoom;
        }
    }
}
