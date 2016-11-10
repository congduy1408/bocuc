#include"Bloomberg.h"

using namespace std;
using namespace cv;

Mat tmp,tmp16;
int element_shape = MORPH_RECT;

void Reduction(Mat img_src,int T)
{
    int img_width = img_src.cols;
    int img_height = img_src.rows;
    if((img_width%2) == 1) img_width--;
    if((img_height%2) == 1) img_height--;

    Mat img_reduct(img_src.rows/2,img_src.cols/2,CV_8U,Scalar(255));

//    cout<<"W = "<<img_width<<"H = "<<img_height<<endl;
    for(int y = 0;y < img_height; y=y+2)
    {
        for(int x = 0;x < img_width; x=x+2)
        {
            if((img_src.at<uchar>(y,x)+img_src.at<uchar>(y+1,x)+img_src.at<uchar>(y,x+1)+ img_src.at<uchar>(y+1,x+1) )>= T*255)
            {
                img_reduct.at<uchar>(y/2,x/2) = 255;
            }
            else
            {
                img_reduct.at<uchar>(y/2,x/2) = 0;
            }
        }
    }
    tmp = img_reduct.clone();
}

void Expansion(Mat img_src)
{
    int img_width = img_src.cols;
    int img_height = img_src.rows;
//    if((img_width%2) == 1) img_width--;
//    if((img_height%2) == 1) img_height--;

    Mat img_exp(img_height*2,img_width*2,CV_8U,Scalar(255));
    for(int y=0;y<img_height;y++)
    {
        for(int x=0;x<img_width;x++)
        {
            img_exp.at<uchar>(2*y,2*x) =
            img_exp.at<uchar>(2*y+1,2*x) =
            img_exp.at<uchar>(2*y,2*x+1) =
            img_exp.at<uchar>(2*y+1,2*x+1) = img_src.at<uchar>(y,x);
        }
    }
      tmp = img_exp.clone();
}

void seedFill(Mat img_seed, Mat img_16)
{
    for(int y=0;y<img_seed.rows;y++)
    {
        for(int x=0;x<img_seed.cols;x++)
        {
            if(img_seed.at<uchar>(y,x) == 0)
            {
                if(img_seed.at<uchar>(y+1,x)+img_seed.at<uchar>(y-1,x)+img_seed.at<uchar>(y,x+1)+img_seed.at<uchar>(y,x-1) > 0 )
                    img_seed.at<uchar>(y,x) = img_16.at<uchar>(y,x);
                else img_seed.at<uchar>(y,x) = 0;
            }
        }
    }
    for(int y=img_seed.rows-1;y>=0;y--)
    {
        for(int x=img_seed.cols-1;x>=0;x--)
        {
            if(img_seed.at<uchar>(y,x) == 0)
            {
                if(img_seed.at<uchar>(y+1,x)+img_seed.at<uchar>(y-1,x)+img_seed.at<uchar>(y,x+1)+img_seed.at<uchar>(y,x-1) > 0 )
                    img_seed.at<uchar>(y,x) = img_16.at<uchar>(y,x);
                else img_seed.at<uchar>(y,x) = 0;
            }
        }
    }
    tmp = img_seed.clone();
}

void Mod1(Mat img_src) // HOLE FILLING
{
    Mat img_fill = img_src.clone();
for(int i=0;i<5;i++) {
    Mat element = getStructuringElement(MORPH_RECT, Size(3,3), Point(-1,-1) );
    morphologyEx(img_fill, img_fill, MORPH_CLOSE, element);
}
tmp = img_fill.clone();
}

void Bloomberg_main(Mat img_src,Mat& img_blom_out)
{

    int step = 1;
    while(step != 0){
    switch(step)
        {
        case 1:{ //4x1, T=1
            Reduction(img_src,1);
//            imwrite("img_reduct.jpg",tmp);
            step = 2;
            }
            break;
        case 2:{ //4x1 -> 16x1, T=1
             Reduction(tmp,1);
             Mod1(tmp);
             imwrite("img_mod.jpg",tmp);
//             imwrite("img_reduct.jpg",tmp);
             tmp16 = tmp.clone();
             step = 3;
             }
             break;
        case 3:{ //4x1, T=4
             Reduction(tmp,4);
//             imwrite("img_reduct.jpg",tmp);
             step = 4;
             }
             break;
        case 4:{ //4x1, T=3, opening 5x5
             Reduction(tmp,3);
             Mat element = getStructuringElement(element_shape, Size(5,5), Point(2,2) );
             morphologyEx(tmp, tmp, MORPH_OPEN, element);
//             imwrite("img_reduct.jpg",tmp);
             step = 5;
             }
             break;
        case 5:{ //1x4
             Expansion(tmp);
             imwrite("img_reduct.jpg",tmp);
             step = 6;
             }
             break;
        case 6:{ //1x4, create mask
             Expansion(tmp);
             resize(tmp,tmp,tmp16.size());
             seedFill(tmp,tmp16);
             Mat element = getStructuringElement(element_shape, Size(3,3), Point(1,1) );
             morphologyEx(tmp, tmp, MORPH_DILATE, element);
             imwrite("img_reduct.jpg",tmp);
             step = 7;
             }
         break;
        case 7:{ //1x4 twice
             Expansion(tmp);
             Expansion(tmp);
             resize(tmp,tmp,img_src.size());
             imwrite("img_blom.jpg",tmp);
//             img_blom_out = img_src.clone();
             for(int y=0;y<img_src.rows;y++) {
                 for(int x=0;x<img_src.cols;x++) {
                     if(tmp.at<uchar>(y,x) == 255)
                         img_blom_out.at<uchar>(y,x) = 0;
                 }
             }
//             bitwise_not(tmp,tmp);
//             bitwise_and(img_src, tmp,tmp);
//             tmp = img_src_clone.clone();
             imwrite("img_reduct.jpg",img_blom_out);
             step = 0;
             }
         break;
        }

}
}







