#include "opencv2/opencv.hpp"
#include"Lin_ordjnd.h"
#include"Lin_predict.h"
#include"Lin_disjnd.h"
#define  r 3
#define  disth 9
using namespace cv;
using namespace std;

Mat GetDisJnd(Mat orgImg, Mat freEng)
{
	
	Mat burEng;
	//GaussianBlur(freEng, burEng, Size(2 * r + 1, 2 * r + 1), (double)(2 * r + 1) / 3);  //用的是这个
	//	GaussianBlur(freEng, burEng, Size(3, 3), 1.5);            //这里是自己设置的参数，上面的参数感觉不靠谱
	GaussianBlur(freEng, burEng, Size(3, 3), 0,0);
	Mat Img_Min = min(freEng, burEng);
	Mat var = GetVar(orgImg);
	Mat DisJnd = freEng;
	int height = freEng.rows;
	int width = freEng.cols;
	for (int h = 0; h < height; ++h)
	{
		double* vardata = var.ptr<double>(h);
		double* Img_Mindata = Img_Min.ptr<double>(h);
		double* DisJnddata = DisJnd.ptr<double>(h);
		for (int w = 0; w < width; ++w)
		{
			if (vardata[w] <  disth)
				DisJnddata[w] = Img_Mindata[w];
			//if (DisJnddata[w]<9)
			//	DisJnddata[w] = 9;
			if (DisJnddata[w]>40)
				DisJnddata[w] = 40;

		}

	}
	//Mat sEdg = GetEdge(orgImg, 1);   //Matlab中传入的是0.7
	//DisJnd = DisJnd.mul(sEdg);
	DisJnd = DisJnd / 3;
	return DisJnd;
}
Mat GetEdge(Mat orgImg, int th)
{
	if (orgImg.type() != CV_64FC1)
		orgImg.convertTo(orgImg, CV_64FC1, 1);
	Mat edgeMap;
	Mat eightImg = orgImg / 4;
	eightImg.convertTo(eightImg, CV_8UC1, 1.0);
	Canny(eightImg, edgeMap, 300, 600, 3);
	
	edgeMap.convertTo(edgeMap, CV_64FC1, 1.0);
	Mat superEdge = 1 - 0.7*edgeMap;
	Mat resEdge;
//	GaussianBlur(superEdge, resEdge, Size(5, 5), 0.8);
	GaussianBlur(superEdge, resEdge, Size(3, 3), 0);
	return resEdge;
}
