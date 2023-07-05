#ifndef _ORDJND_H
#define _ORDJND_H
#include"opencv2/opencv.hpp"
using namespace cv;
//Mat Convet2Doule(const Mat IputImg);
Mat GetOrdJND(Mat img);
Mat GetLaJnd(Mat img);
Mat GetCmJnd( Mat img);
Mat GetBlockLuma(Mat img);
Mat GetLogDif( Mat img);
Mat GetLumDif(Mat img);
//Mat Filter2(char*B[],Mat img);
#endif // !