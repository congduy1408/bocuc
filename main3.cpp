#include <iostream>
#include "borderv_5.h"
#include "string.h"
#include "binarize.h"
#include "img_seg.h"
#include "Bloomberg.h"


using namespace std;
using namespace cv;

#define UBUNTU
//#define PI

int MAX_KERNEL_LENGTH = 2;
int erosion_type = MORPH_RECT;
int erosion_size = 7;
int Canny_thresh = 100;

extern Cutregion Cut;
extern CutY cutY;

int main()
{
    string input;
    string output;
    string mau;
    int bina_mode;
    int img_num;
    const char *filename = NULL;
    cout<<"Cac tap mau: "<<endl;
    system("ls ~/Desktop/DoAn/Mau/");
    cout<<"Chon tap mau: "<<endl;
    cin>>mau;
    cout<<"So anh co trong thu muc: "<<endl;
    system(("ls ~/Desktop/DoAn/Mau/"+ mau + " | grep -c '.' ").c_str());
    cout<<"Chon so anh can chay:  "<<endl;
    cin>>img_num;
    cout<<"chon pp nhi phan"<<endl;
    cin>>bina_mode;
    for(int i=1;i<=img_num;i++)
    {
        clock_t begin = clock();
        stringstream ss;
        ss<<i;
        string str = ss.str();
//        if(i<10)  input = mau+"/"+mau+"_0000" + str + "_result.jpg";
//        else input =  mau+"/"+mau+"_000" + str + "_result.jpg";
    input =  mau+"/"+mau+"_000" + str + ".jpg";
#ifdef UBUNTU
    input = "/home/congduy/Desktop/DoAn/Mau/" + input;
    cout << "File can chuyen: " << input << endl;
#endif

#ifdef PI
    input = "/home/pi/Desktop/DoAn/Mau/rotate/" + input;
    cout << "File can chuyen: " << input << endl;
#endif

    filename = input.c_str();
    Mat img_src = imread(filename,CV_LOAD_IMAGE_GRAYSCALE);
    Mat img_binary;
    Mat img_blom;
//    Mat img_blom_out=img_src.clone();


    Adap(img_src,img_blom);
    Mat img_blom_out=img_blom.clone();

    Bloomberg_main(img_blom,img_blom_out);
//cout<<"test "<<endl;
//cout<<"img blom "<<img_blom_out.rows<<endl;
    switch(bina_mode)
    {
    case 1:
             Thres(img_blom_out,img_binary);
             break;
    case 2:
             Adap(img_blom_out,img_binary);
             break;
    case 3:
             Can(img_blom_out,img_binary);
             break;
    case 4:
             Sau(img_blom_out,img_binary);
             break;
    }


    img_blom= imread("img_reduct.jpg",CV_LOAD_IMAGE_GRAYSCALE);
    Detect_Border(img_binary,str,img_src);
    Mat img_binary_clone= img_binary.clone();
    Mat hHisto = cv::imread("hHisto.jpg",CV_LOAD_IMAGE_GRAYSCALE);
    Mat vHisto = cv::imread("vHisto.jpg",CV_LOAD_IMAGE_GRAYSCALE);
    Mat result = cv::imread("result.jpg",CV_LOAD_IMAGE_GRAYSCALE);
    Mat reduct = cv::imread("img_reduct.jpg",CV_LOAD_IMAGE_GRAYSCALE);
    Mat mod = cv::imread("img_mod.jpg",CV_LOAD_IMAGE_GRAYSCALE);
//    Mat line = cv::imread("line.jpg",CV_LOAD_IMAGE_GRAYSCALE);
    if(i<10)  output = "IMG_0000" + str + "_result.jpg";
    else output = "IMG_000" + str + "_result.jpg";

#ifdef UBUNTU
    cv::imwrite("/home/congduy/Desktop/DoAn/KQ/Bloomberg/"+output,reduct);
    cv::imwrite("/home/congduy/Desktop/DoAn/KQ/Bloomberg16/"+output,mod);
    cv::imwrite("/home/congduy/Desktop/DoAn/KQ/bocuc/hHisto/"+output,hHisto);
    cv::imwrite("/home/congduy/Desktop/DoAn/KQ/bocuc/vHisto/"+output,vHisto);
    cv::imwrite("/home/congduy/Desktop/DoAn/KQ/bocuc/result/"+output,result);
    cv::imwrite("/home/congduy/Desktop/DoAn/KQ/curlline/"+output,img_binary_clone);
//    cv::imwrite("/home/congduy/Desktop/DoAn/KQ/line/"+output,line);
#endif

#ifdef PI
    cv::imwrite("/home/pi/Desktop/DoAn/KQ/Bloomberg/"+output,reduct);
    cv::imwrite("/home/pi/Desktop/DoAn/KQ/Bloomberg16/"+output,mod);
//    cv::imwrite("/home/pi/Desktop/DoAn/KQ/bocuc/hHisto/"+output,hHisto);
//    cv::imwrite("/home/pi/Desktop/DoAn/KQ/bocuc/vHisto/"+output,vHisto);
    cv::imwrite("/home/pi/Desktop/DoAn/KQ/bocuc/result/"+output,result);
//    cv::imwrite("/home/pi/Desktop/DoAn/KQ/curlline/"+output,img_binary);
    cv::imwrite("/home/pi/Desktop/DoAn/KQ/line/"+output,line);
#endif
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    cout<<"time= "<<elapsed_secs<<endl;
    }
       return 0;
}






