#include"Lin_ordjnd.h"
#include"Lin_TransferFunction.h"
#include"opencv2/opencv.hpp"
#define bitdepth 10
#define r 3
Mat GetBlockLuma( Mat img)
{
	if (img.type() != CV_64FC1)
		img.convertTo(img, CV_64FC1, 1.0);
	assert(img.channels() == 1);    //此代码只处理灰度图像
	Mat kern = (Mat_<double>(5, 5) << 1.0 / 32, 1.0 / 32, 1.0 / 32, 1.0 / 32, 1.0 / 32,
		1.0 / 32, 2.0 / 32, 2.0 / 32, 2.0 / 32, 1.0 / 32,
		1.0 / 32, 2.0 / 32, 0.0 / 32, 2.0 / 32, 1.0 / 32,
		1.0 / 32, 2.0 / 32, 2.0 / 32, 2.0 / 32, 1.0 / 32,
		1.0 / 32, 1.0 / 32, 1.0 / 32, 1.0 / 32, 1.0 / 32);
	Mat filImg;
	filter2D(img, filImg, img.depth(), kern);
	int height = filImg.rows;
	int width = filImg.cols;
	for (int h = 0; h < height; ++h)
	{
		double* orgdata = filImg.ptr<double>(h);
		for (int w = 0; w < width; ++w)
			orgdata[w] = cvFloor(orgdata[w]);
			
	}
	return filImg;
}
Mat GetLaJnd(Mat img)
{
	if (img.type() != CV_64FC1)
		img.convertTo(img, CV_64FC1, 1.0);
	Mat BgImg = GetBlockLuma(img);  
	int lumaMin = 16 * pow(2, bitdepth - 8);
	int lumaMax = 235 * pow(2, bitdepth - 8);
	int lumaRange = lumaMax - lumaMin;
	Mat lumaTable = Mat::zeros(lumaRange+1, 1, CV_64FC1);//初始化
	for (int h = 0; h <= lumaRange; ++h)
		lumaTable.at<double>(h) = (double)h / lumaRange;    //赋值加归一化
	double ratio = 0.9 * 4096 / lumaRange;
	Mat orgLuma = 10000 * inversePQ(lumaTable);
	Mat cviBarten(orgLuma.size(), CV_64FC1);
	for (int h = 0; h <= lumaRange; ++h)
	{
		if (h != lumaRange)
			cviBarten.at<double>(h) = (1 / ratio)* (orgLuma.at<double>(h + 1) - orgLuma.at<double>(h)) / (orgLuma.at<double>(h + 1) + orgLuma.at<double>(h));
		else
			cviBarten.at<double>(h) = cviBarten.at<double>(h - 1);
	}
	
	int height = BgImg.rows;
	int width = BgImg.cols;
	Mat lumaJnd(BgImg.size(), CV_64FC1);
	for (int h = 0; h < height; ++h)
	{
		double* orgdata = BgImg.ptr<double>(h);
		double* jnddata = lumaJnd.ptr<double>(h);
		for (int w = 0; w < width; ++w)
	    		jnddata[w] = cviBarten.at<double>((int)(orgdata[w]-lumaMin));	
	}

	return  1000*lumaJnd;
}
Mat GetLumDif( Mat img)
{
	if (img.type() != CV_64FC1)
		img.convertTo(img, CV_64FC1, 1.0);
	assert(img.channels() == 1);    //此代码只处理灰度图像
	Mat kern1 = (Mat_<double>(5, 5) << 0, 0, 0, 0, 0,
		1.0 , 3.0 , 8.0 , 3.0 , 1.0,
		0, 0, 0, 0, 0,
		-1.0 , -3.0 , -8.0 , -3.0 , -1.0,
		0, 0, 0, 0, 0);
	Mat kern2 = (Mat_<double>(5, 5) << 0, 0, 1.0 , 0, 0,
		0, 8.0 , 3.0 , 0, 0,
		1.0 , 3.0 , 0, -3.0 , -1.0 ,
		0, 0, -3.0 , -8.0 , 0,
		0, 0, -1.0 , 0, 0);
	Mat kern3 = (Mat_<double>(5, 5) << 0, 0, 1.0 , 0, 0,
		0, 0, 3.0 , 8.0 , 0,
		-1.0 , -3.0 , 0, 3.0 , 1.0 ,
		0, -8.0 , -3.0 , 0, 0,
		0, 0, -1.0 , 0, 0);
	Mat kern4 = (Mat_<double>(5, 5) << 0, 1.0 , 0, -1.0 , 0,
		0, 3.0 , 0, -3.0  , 0,
		0, 8.0 , 0, -8.0 , 0,
		0, 3.0 , 0, -3.0 , 0,
		0, 1.0 , 0, -1.0 , 0);
	int height = img.rows;
	int width = img.cols;
	Mat Grad1 , Grad2 , Grad3 , Grad4 ;
	filter2D(img, Grad1, img.depth(), kern1);
	filter2D(img, Grad2, img.depth(), kern2);
	filter2D(img, Grad3, img.depth(), kern3);
	filter2D(img, Grad4, img.depth(), kern4);
	Grad1 = Grad1 / 16;
	Grad2 = Grad2 / 16;
	Grad3 = Grad3 / 16;
	Grad4 = Grad4 / 16;

	Mat LumDif =Grad1;
	for (int h = 0; h <height; ++h)
	{  
		double* g1 = Grad1.ptr<double>(h);
		double* g2 = Grad2.ptr<double>(h);
		double* g3 = Grad3.ptr<double>(h);
		double* g4 = Grad4.ptr<double>(h);
		double* res = LumDif.ptr<double>(h);	
		for (int w = 0; w < width; ++w)
		{
			double max = 0;
			if (abs(g1[w])>max)                        
				max = abs(g1[w]);
			if (abs(g2[w])>max)
				max = abs(g2[w]);
			if (abs(g3[w])>max)
				max = abs(g3[w]);
			if (abs(g4[w])>max)
				max = abs(g4[w]);
			res[w] = max;

		}
	}
	return LumDif;
}
Mat  GetLogDif( Mat img)
{
	Mat LogDif = img;
	int height = img.rows;
	int width = img.cols;
	int Th = 80;
	for (int h = 0; h < height; ++h)
	{
		const double* orgdata = img.ptr<double>(h);
		double* logdata = LogDif.ptr<double>(h);
		for (int w = 0; w < width; ++w)
			logdata[w] = Th*log10(1 + (float)orgdata[w] / Th) / log10(4);;
	}
	return LogDif;
}
Mat GetCmJnd( Mat img)
{
	if (img.type() != CV_64FC1)
		img.convertTo(img, CV_64FC1, 1.0);
	double eta = 0.025;
	Mat LumDif = GetLumDif(img);
	Mat edgeMap;
	Mat eightImg=img / 4;
	eightImg.convertTo(eightImg, CV_8UC1, 1.0);
	Canny(eightImg, edgeMap, 200, 400, 3);
	//imshow("CANNY", edgeMap);
   // waitKey(0);
	
	edgeMap.convertTo(edgeMap, CV_64FC1, 1.0/255);
	Mat superEdge = 1 - 0.9*edgeMap;
	Mat burEng;
	//GaussianBlur(superEdge, burEng, Size(2 * r + 1, 2 * r + 1), (double)(2 * r + 1) / 3);
	GaussianBlur(superEdge, burEng, Size(5,5), 0,0);
	/*Mat GuassianKer = (Mat_<double>(5, 5) << 2, 4, 5, 4, 2,
		4, 9, 12, 9, 4,
		5, 12, 15, 12, 5,
		4, 9, 12, 9, 4,
		2,   4,   5,   4,   2);
	
	filter2D(superEdge, burEng, superEdge.depth(), GuassianKer / 159);
	*/
	Mat JndCM = eta*LumDif.mul(superEdge);
//	imshow("JND_CM"	,JndCM);
	//waitKey(0);
	return JndCM;
	/*
	Mat LumDif = GetLumDif(img);
	Mat logDiff = GetLogDif(LumDif);
	Mat BgLum = GetBlockLuma(img);
	float landa = 0.5;
	Mat alpha = 0.0001*BgLum + 0.115;
	Mat belta = landa - 0.01*BgLum;
	LumDif = LumDif.mul(alpha);
	Mat JndCM = abs(LumDif + belta);
	return JndCM;
	*/
}
Mat GetOrdJND(Mat img)
{
	
	Mat JndLa = GetLaJnd(img);
	Mat JndCm = GetCmJnd(img);
	Mat FinalJnd = JndLa + JndCm - 0.3 * min(JndLa, JndCm);
	/*double min1, max1;
	minMaxLoc(JndLa, &min1, &max1);
	FinalJnd.convertTo(JndLa, CV_64FC1, 1.0 / (max1-min1));
	imshow("JndLa", JndLa);
	waitKey(0);
	/*imshow("JndCm", JndCm);
	waitKey(0);
	double min, max;
	minMaxLoc(FinalJnd, &min, &max);
	FinalJnd.convertTo(FinalJnd, CV_64FC1, 1.0/(max-min));
	
	imshow("OrdJnd", FinalJnd);
	waitKey(0);
	*/
	//FinalJnd = GetCmJnd(img);

	/*
	int height = FinalJnd.rows;
	int width = FinalJnd.cols;
	for (int h = 0; h < height; ++h)
	{
		const double* Jnddata = FinalJnd.ptr<double>(h);
		const double* orddata = JndLa.ptr<double>(h);
		const double* disdata = JndCm.ptr<double>(h);
		for (int w = 0; w < width; ++w)
		{
			std::cout << orddata[w] << "  " << disdata[w] << "  " << Jnddata[w] << std::endl;
		}
	}*/

	return FinalJnd;
}
