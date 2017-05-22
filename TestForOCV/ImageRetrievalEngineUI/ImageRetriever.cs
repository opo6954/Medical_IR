using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
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
        }
    }
}
