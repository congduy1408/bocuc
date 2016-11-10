#include "borderv_5.h"
#include "binarize.h"
using namespace std;
using namespace cv;

#define UBUNTU
//// Code nay tim histogram cua anh va tu do uoc luong ra cac gia tri xL,xR,yU,yD
/// tu cac gia tri tren ta co the tim duoc duong bien cua vung text
///

int img_height = 0;
int img_width = 0;
Cutregion Cut;
CutY cutY;

void Detect_Border( Mat& img_src,string str,Mat img_gray)
{

    img_height = img_src.rows;
    img_width = img_src.cols;
    Mat img_src_h;
    int *vHisto;   // Histogram theo chieu doc
    int *hHisto;   // Histogram theo chieu ngang
/////////Tinh duong bien trai phai va tao 1 anh moi //////////
    vHisto = Draw_Histogram(img_src,0);
    Cut_Vertical(vHisto,0,img_width,Cut);
    Rect vertical(Cut.xl[0],0,Cut.xr[0]-Cut.xl[0],img_height);
    img_src_h = Mat(img_src,vertical).clone();
/////////Tinh bieu do chieu ngangay tren anh vua tao //////////
   // hHisto = Draw_Histogram(img_src_h,1);

    Rect boundRect(Cut.xl[0],0,Cut.xr[0]-Cut.xl[0],img_height);
    img_src = Mat(img_src,boundRect).clone();
    img_gray = Mat(img_gray,boundRect).clone();
    Cut_Horizontal(str,img_src_h,img_gray);
    Cut.num = 0;
    cutY.num=0;
    imwrite("result.jpg",img_src);
//    namedWindow("CUT",WINDOW_NORMAL);
//    imshow("CUT",img_src);
//    waitKey(0);

}

int *Draw_Histogram( Mat img_src,int mode)
{


    int *vHisto = (int*)malloc(img_src.cols*sizeof(int));
    int *hHisto = (int*)malloc(img_src.rows*sizeof(int));
    ////mode 0: chieu doc, mode 1: chieu ngang ///////////

    if(mode == 0)
        {
            for(int x=0;x < img_width;x++)
                {
                   int vCount = 0;
                   for(int y=0; y<img_height;y++)
                       {
                          int t = img_src.at<uchar>(y,x);
                          if(t==255) vCount++;
                       }
                   vHisto[x] = vCount;
                }
            Mat vHisImg(img_height, img_width, CV_8UC1,Scalar(255, 255, 255));

            // Ve Histogram
            for(int i = 0; i<img_width; i++)
                {
                    line(vHisImg, Point(i, img_height), Point(i, img_height - vHisto[i]), Scalar(0,0,0),1,8,0);
                }
          imwrite("vHisto.jpg",vHisImg);
//          namedWindow("vHisto.jpg",WINDOW_NORMAL);
//          imshow("vHisto.jpg",vHisImg);
//          waitKey(0);
             return vHisto;
        }
    else if(mode==1)
        {
            for(int y=0; y<img_height; y++)
                {
                    int hCount = 0;
                    for(int x=0; x<img_width; x++)
                        {
                            int t = img_src.at<uchar>(y,x);
                            if(t==255) hCount++;
                        }
                    hHisto[y] = hCount;
                }
            Mat hHisImg(img_width, img_height, CV_8UC1,Scalar(255,255,255));

            // Ve Histogram
            for(int i = 0; i < img_height;i++)
                {
                    line(hHisImg, Point(i, img_width), Point(i,img_width- hHisto[i]), Scalar(0,0,0),1,8,0);
                }
            imwrite("hHisto.jpg",hHisImg);

//            namedWindow("hHisto.jpg",WINDOW_NORMAL);
//            imshow("hHisto.jpg",hHisImg);
//            waitKey(0);
            return hHisto;
        }
return 0;
}

int Cal_thresh(int sample[],int y0,int y1)
{
    int max = sample[0];
    for(int j=1;j<y1-y0;j++)
        {
            if(max<sample[j]) max = sample[j];
        }
    return max;
}

void Cut_Vertical(int vHisto[],int xL, int xR, Cutregion& cut)
{
    int W = xR-xL;
//    cut.xl = 0;
//    cut.xr = 0;
    int sum = 0;
    for(int j=xL;j<xR;j++)
        {
            sum = sum + vHisto[j];
        }
    int L4 = sum/W;
    cout<<"L4 = "<<L4<<endl;
    for(int x=xL;x<xR;x++)
        {
            if(vHisto[x] < L4/4)
                {
                    if(cut.length!=0)
                        {
                            if(cut.length>(W/3))
                                {
                                    cut.num++;
                                    if(cut.num==1)
                                    {
                                        cut.xr[0] = x ;
                                        cut.xl[0] = cut.xr[0]-cut.length;
                                    }
                                  else if(cut.num>1)
                                    {
                                        cut.xr[cut.num-1] = x;
                                        cut.xl[cut.num-1] = cut.xr[cut.num-1] - cut.length;
                                    }
                                    cut.length=0;
                                }
                            else cut.length = 0;
                        }

                }
            else cut.length++;
        }
   // cout<<cut.num<<endl;
    free(vHisto);
}

void Cut_Horizontal(string str,Mat img_src_h,Mat img_gray)
{
    vector<std::vector<cv::Point> > contours;
    vector<cv::Vec4i> hierarchy;
string direct = "photo" + str;
string output;
#ifdef UBUNTU
system(("cd /home/congduy/Desktop/DoAn/KQ/tachchu3/ && rm -rf " + direct).c_str());
system(("cd /home/congduy/Desktop/DoAn/KQ/tachchu3/ && mkdir " + direct).c_str());
#endif
#ifdef PI
system(("cd /home/pi/Desktop/DoAn/KQ/tachchu3/ && rm -rf " + direct).c_str());
system(("cd /home/pi/Desktop/DoAn/KQ/tachchu3/ && mkdir " + direct).c_str());
#endif
    ///////////// Tim cac contour trong anh////////////
//    Mat img_gray_clone = img_gray.clone();
//    Adap(img_gray,"n",img_gray);
Mat drawing = Mat::zeros( img_src_h.size(), CV_8UC3 );
    findContours(img_src_h,contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE); // thay img_line thanh img_src_h de tach ca dong
    vector<Rect> rect_cc( contours.size() );
    vector<Rect> rect_char;
    int num_of_char=0;
    Mat img_gray_clone = img_gray.clone();
    /////// Loai cac contour co kich thuoc nho ///////
    for( int i = 0; i< contours.size(); i++ )
    {
        rect_cc[i] = boundingRect( Mat (contours[i]));
        if (500>rect_cc[i].width && 500>rect_cc[i].height ) {
            if(10<rect_cc[i].width && 10<rect_cc[i].height) {
                drawContours( drawing, contours, i, (255,255,255), 1, 8, hierarchy, 0, cv::Point() );

            rect_char.push_back(rect_cc[i]);
            num_of_char++;
            }
        }
    }
    ///////////// sort contours by y ///////////
    for(int i = 0;i<num_of_char;i++) {
       for(int temp=i+1;temp<num_of_char;temp++)
       {
           if (rect_char[i].tl().y >= rect_char[temp].tl().y)
           {
               vector<Rect> rect_temp(1);
               rect_temp[0]=rect_char[i];
               rect_char[i]=rect_char[temp];
               rect_char[temp]=rect_temp[0];
           }
       }
       }
    ///// test sap xep theo y ///////
//    for(int i=0; i <num_of_char;i++) {
//        stringstream y;
//        y<<i;
//        string char_num = y.str();
//        Mat img_char = Mat(img_gray,rect_char[i]).clone();
//        output = char_num + ".jpg";
//        imwrite("/home/congduy/Desktop/DoAn/KQ/tachchu3/"+direct+"/"+output,img_char);
//    }
      ////////// noi cac contour thuoc cung 1 dong ////////
      int line = 1;
      /*
      vector<vector<Rect> > linechar;
      vector<Rect> rows;
//      Scalar color[3] ={(255,0,0),(0,255,0),(0,0,255)};
      while(!rect_char.empty()) {
          for(int  a = 1;a<rect_char.size();a++){
          if(rect_char[0].tl().y >= rect_char[a].tl().y && rect_char[0].tl().y<=rect_char[a].br().y) {
              rows.push_back(rect_char[a]);
              rectangle( img_gray_clone, rect_char[a], (255,255,255), 3, 8, 0 );
              rect_char.erase(rect_char.begin()+a);
          } else if(rect_char[0].tl().y <= rect_char[a].tl().y && rect_char[0].br().y>=rect_char[a].tl().y) {
              rows.push_back(rect_char[a]);
              rectangle( img_gray_clone, rect_char[a], (255,255,255), 3, 8, 0 );
              rect_char.erase(rect_char.begin()+a);
           }

          }
          line++;
          rows.push_back(rect_char[0]);
          rect_char.erase(rect_char.begin());
          linechar.push_back(rows);
          rows.clear();
      }
*/
      vector<vector<Rect> > linechar;
      vector<Rect> rows;
//      Scalar color[3] ={(255,0,0),(0,255,0),(0,0,255)};
//      int topy = rect_char[0].tl().y;
//      int boty = rect_char[0].br().y;
      Rect temp;
      Rect temp2;

      while(!rect_char.empty()) {
          int a = 0;
          for( a;a<rect_char.size();a++){
          if(rect_char[0].tl().y >= rect_char[a].tl().y && rect_char[0].tl().y <= rect_char[a].br().y) {
              if(rect_char[0].tl().x > rect_char[a].br().x || rect_char[0].br().x < rect_char[a].tl().x) {
                temp = rect_char[0];
                temp2 = rect_char[a];
                rect_char[0] = rect_char[a];
                rect_char.erase(rect_char.begin()+a);
                rectangle( img_gray_clone, temp, (255,255,255), 3, 8, 0 );
                rows.push_back(temp);
                a = 0;
              }
          } else if(rect_char[0].tl().y <= rect_char[a].tl().y && rect_char[0].br().y>=rect_char[a].tl().y) {
              if(rect_char[0].tl().x > rect_char[a].br().x || rect_char[0].br().x < rect_char[a].tl().x) {
                temp = rect_char[0];
                temp2 = rect_char[a];
                rect_char[0] = rect_char[a];
                rect_char.erase(rect_char.begin()+a);
                rectangle( img_gray_clone, temp, (255,255,255), 3, 8, 0 );
                rows.push_back(temp);
                a = 0;
              }
           } /*else {
              line++;
              linechar.push_back(rows);
//              cout<<"word current "<<rect_char[0].tl().y<<"-"<<rect_char[a].br().y<<"word next"<<rect_char[i+1].tl().y<<"-"<<rect_char[i+1].br().y<<endl;
              rows.clear();
              //rectangle( img_gray_clone, rect_char[i], (255,255,255), 3, 8, 0 );
           }*/
          }
          line++;
          rows.push_back(temp2);
          rectangle( img_gray_clone, temp2, (255,255,255), 3, 8, 0 );
          rect_char.erase(rect_char.begin());
          linechar.push_back(rows);
          rows.clear();
      }


      //////// sap xep lai cac contour theo x va save //////////
      for(int i=0;i<linechar.size();i++){
           for(int j=0;j<linechar[i].size();j++) {
               for(int temp=j+1;temp<linechar[i].size();temp++) {
                   if (linechar[i][j].tl().x >= linechar[i][temp].tl().x){
                    vector<Rect> rect_temp(1);
                    rect_temp[0]=linechar[i][j];
                    linechar[i][j]=linechar[i][temp];
                    linechar[i][temp]=rect_temp[0];
                   }
               }
           }
           for(int a=0;a<linechar[i].size();a++) {
              stringstream x;
              x<<i;
              string line_num = x.str();
              stringstream y;
              y<<a;
              string char_num = y.str();
              Mat img_char = Mat(img_gray,linechar[i][a]).clone();
              output = line_num  + "_" + char_num + ".jpg";
              imwrite("/home/congduy/Desktop/DoAn/KQ/tachchu3/"+direct+"/"+output,img_char);
           }
       }
    #ifdef UBUNTU
    imwrite("/home/congduy/Desktop/DoAn/KQ/contour/" + str + ".jpg",img_gray_clone);
    imwrite("/home/congduy/Desktop/DoAn/KQ/runlength/" + str + ".jpg",drawing);
    #endif
    #ifdef PI
    imwrite("/home/pi/Desktop/DoAn/KQ/contour/" + str + ".jpg",drawing);
    #endif
}










