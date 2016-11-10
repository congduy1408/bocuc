#ifndef BORDERV_5_H
#define BORDERV_5_H

#endif // BORDERV_5_H

#include<iostream>
#include<math.h>
#include<stdlib.h>


////////include opencv lib////////

#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

struct Cutregion{
    int num=0;
    int length=0;
//    int xt0;
//    int xt1;
//    int xt2;
//    int xt3;
    int xl[10];
    int xr[10];
};

struct CutY{
    int yt0;
    int yt1;
    int length=0;
    int num=0;
};


void Detect_Border( Mat& img_src,string str,Mat img_gray);
int *Draw_Histogram( Mat img_src,int mode);
void Cut_Vertical(int vHisto[],int xL, int xR, Cutregion& cut);
void Cut_Horizontal(string str,Mat img_src_h,Mat img_gray);





