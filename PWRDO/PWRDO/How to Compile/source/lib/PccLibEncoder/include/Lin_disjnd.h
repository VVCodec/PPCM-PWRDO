#ifndef _DISJND_H_
#define _DISJND_H_
#include"opencv2/opencv.hpp"
using namespace cv;
Mat GetDisJnd(Mat orgImg, Mat freEng);
Mat GetEdge(Mat orgImg, int size);
#endif
