#ifndef BINARIZE_H
#define BINARIZE_H

#endif // BINARIZE_H

#include<iostream>
#include<math.h>
#include <vector>
#include <algorithm>

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

void Thres(Mat img_src,Mat& img_binary);

void Adap(Mat img_src,Mat& img_binary);

void Can(Mat img_src,Mat& img_binary);

void Sau(Mat img_src,Mat& img_binary);

