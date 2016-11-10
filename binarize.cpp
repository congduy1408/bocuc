#include "binarize.h"

using namespace std;
using namespace cv;

/// Gom cac giai thuat nhi phan hoa nhu threshold, adaptiveThreshold, Canny edge
/// va Sauvola
///



string binary;

int Canny_thresh1 = 100;

void Thres(Mat img_src,Mat& img_binary)
{
    medianBlur(img_src,img_src,3);
    threshold(img_src, img_binary, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
}

void Adap(Mat img_src,Mat& img_binary)
{
    medianBlur(img_src,img_src,5);
    adaptiveThreshold(img_src,img_binary,255,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY_INV,7,3);
}

void Can(Mat img_src,Mat& img_binary)
{
//    medianBlur(img_src,img_src,3);
    Canny(img_src,img_binary,Canny_thresh1,Canny_thresh1*3,3);
//    Mat element = getStructuringElement(MORPH_RECT, Size(3,3), Point(-1,-1) );
//    morphologyEx(img_binary, img_binary, MORPH_DILATE, element);
//    dilate(img_binary,img_binary, cv::Mat(), cv::Point(-1, -1));
}

void Sau(Mat img_src,Mat& img_binary) {
    int window = 5;
    int k = 0.3;
    medianBlur(img_src,img_src,7);
    img_binary=img_src.clone();
    int height = img_src.rows;
    int width  = img_src.cols;
    for(int y=1;y<height;y++) {
        for(int x=1;x<width;x++) {
            // calculate mean
             int min_y = std::max(0, y - window/2);
             int max_y = std::min(height - 1, y + window/2);
             int min_x = std::max(0, x - window/2);
             int max_x = std::min(width - 1, x + window/2);
             int acc = 0;
             for (int j = min_y; j <= max_y; j++) {
               for (int i = min_x; i <= max_x; i++) {
                   acc += (int) img_src.at<uchar>(j,i);
               }
             }
             acc = acc/(window*window);
             // calculate dev
             uchar cur = img_src.at<uchar>(y,x);
             uchar d_x = img_src.at<uchar>(y, std::max(0, x - 1));
             uchar d_y = img_src.at<uchar>(std::max(0, y - 1), x);
             int dev = (int) cur - d_x - d_y;
             // calculate thresh
             int normalized_dev = 1 + k * (( dev / 128) - 1);
             uchar val = img_src.at<uchar>(y,x) < normalized_dev*acc;
             img_binary.at<uchar>(y,x) = (val == 0 ? 0: 255);
        }
    }
}
