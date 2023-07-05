#ifndef _PREDICET_H_
#define _PREDICET_H_
#include"opencv2/opencv.hpp"
Mat GetPreImg(Mat img);
Mat GetARImg(Mat img, Mat min_sigma);
Mat GetVar(Mat img);
Mat GetPadArr(Mat img,int adsize);
Mat GetSubPad(Mat Mat_Pad, int subSzie);
#endif