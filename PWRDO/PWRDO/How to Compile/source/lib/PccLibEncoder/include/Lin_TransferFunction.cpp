#include"opencv2/opencv.hpp"
using namespace cv;
Mat codingTF_PQ(Mat img)
{
	double m1 = (double)2610 / 4096 / 4;
	double m2 = (double)2523 / 4096 * 128;
	double c1 = (double)3424 / 4096;
	double c2 = (double)2413 / 4096 * 32;
	double c3 = (double)2392 / 4096 * 32;
	Mat temp1 = img;
	Mat temp2 = img;
	Mat  quanImg = img;
	pow(img, m1,temp1);
	temp2 = ((c1 + c2*temp1)/ (1 + c3*temp1));
	pow(temp2, m2, quanImg);
	return quanImg;
}
Mat inversePQ(Mat img)
{
	double m1 = (double)2610 / 4096 / 4;
	double m2 = (double)2523 / 4096 * 128;
	double c1 = (double)3424 / 4096;
	double c2 = (double)2413 / 4096 * 32;
	double c3 = (double)2392 / 4096 * 32;
	Mat temp1 = img;
	pow(img, 1 / m2, temp1);
	Mat maxTemp1 = max(temp1 - c1, 0);
	Mat inverseImg = img;
	pow(maxTemp1 / (c2 - c3*temp1), 1 / m1,inverseImg);
	return inverseImg;
}