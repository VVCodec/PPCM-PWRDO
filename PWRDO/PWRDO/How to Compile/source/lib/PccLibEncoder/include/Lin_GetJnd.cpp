#include "opencv2/opencv.hpp"
#include"Lin_GetJnd.h"
#include"Lin_ordjnd.h"
#include"Lin_predict.h"
#include"Lin_disjnd.h"
using namespace cv;
extern double**  g_jnd;    //设置一个全局变量来存储JND的值
extern double ** BlockJnd; //记录一帧块的JND的平均值
extern int cuWsize ;
extern int cuHsize ;
extern int cuWmargin ;
extern int cuHmargin ;
extern double *g_OneDBlcokJnd; //开辟一个空间用来储存每一帧的的区域 平均JND的值
void JndPro(Mat orgImg)
{
	int height = orgImg.rows;
	int width = orgImg.cols;
	orgImg.convertTo(orgImg, CV_64FC1, 1.0);
	Mat preImg = GetPreImg(orgImg);
	Mat FreEng = abs(orgImg - preImg);

	Mat ordJnd = GetOrdJND(preImg);
	Mat disJnd = GetDisJnd(orgImg, FreEng);
	Mat JndVal = ordJnd + disJnd - 0.3 * min(disJnd, ordJnd);
	//JndVal = ordJnd;
	/*double min0, max0;
	minMaxLoc(disJnd, &min0, &max0);
	disJnd.convertTo(disJnd, CV_64FC1, 1.0 / max0);
	imshow("DisJnd", disJnd);
	waitKey(0);


	double min1, max1;
	minMaxLoc(JndVal, &min1, &max1);
	JndVal.convertTo(JndVal, CV_64FC1, 1.0 / max1 );
	imshow("FinalJnd", JndVal);
	waitKey(0);*/
	


	for (int h = 0; h< height; ++h)
	{
		double* Jnddata = JndVal.ptr<double>(h);
		for (int w = 0; w < width; ++w)
			g_jnd[h][w] = Jnddata[w];
	}
	calcWeight(width, height);
	return;
}

//根据JND的值计算权重
void calcWeight(int picWidth, int picHeight)
{
	double FramJnd = 0;
	for (int i = 0; i < cuHsize - 1; i++)
	for (int j = 0; j < cuWsize - 1; j++)
	{
		for (int ii = 0; ii < 64; ii++)
		for (int jj = 0; jj < 64; jj++)
			BlockJnd[i][j] += g_jnd[ i * 64 + ii][j * 64 + jj];
		BlockJnd[i][j] = BlockJnd[i][j] / pow(64, 2.0);
	}
	//end of for 
	//下面的代码开始处理边缘块

	//处理行边缘
	for (int i = 0; i < cuHsize - 1; i++)
	{
		for (int ii = 0; ii < 64; ii++)
		for (int jj = 0; jj < cuWmargin; jj++)
			BlockJnd[i][cuWsize - 1] += g_jnd[ i * 64 + ii][(cuWsize - 1) * 64 + jj];
		BlockJnd[i][cuWsize - 1] = BlockJnd[i][cuWsize - 1] / (cuWmargin * 64);
	}

	//处理列边缘
	for (int j = 0; j < cuWsize - 1; j++)
	{
		for (int ii = 0; ii < cuHmargin; ii++)
		for (int jj = 0; jj < 64; jj++)
			BlockJnd[cuHsize - 1][j] += g_jnd[ + (cuHsize - 1) * 64 + ii][j * 64 + jj];
		BlockJnd[cuHsize - 1][j] = BlockJnd[cuHsize - 1][j] / (cuHmargin * 64);
	}
	//处理右下角最后一个块
	for (int ii = 0; ii < cuHmargin; ii++)
	{
		for (int jj = 0; jj < cuWmargin; jj++)
			BlockJnd[cuHsize - 1][cuWsize - 1] += g_jnd[ + (cuHsize - 1) * 64 + ii][(cuWsize - 1) * 64 + jj];
		BlockJnd[cuHsize - 1][cuWsize - 1] = BlockJnd[cuHsize - 1][cuWsize - 1] / (cuHmargin * cuWmargin);
	}
	for (int i = 0; i < picHeight; i++)
	for (int j = 0; j < picWidth; j++)
	{
		FramJnd += g_jnd[i][j];
	}
	FramJnd = FramJnd / (picHeight*picWidth);
	double *flag = g_OneDBlcokJnd;
	for (int i = 0; i < cuHsize; i++)
	for (int j = 0; j < cuWsize; j++)
	{
		BlockJnd[i][j] = 0.7 + 0.6 / (1 + exp(-4 * (BlockJnd[i][j] - FramJnd) / FramJnd));
		*flag = BlockJnd[i][j];
		flag++;

	}
}
