#include"opencv2/opencv.hpp"
#include"Lin_ordjnd.h"
#include"Lin_predict.h"
#include<fstream>
#define r      3
#define R      3
#define min_th  32
using namespace std;
Mat GetPreImg(Mat img)
{
	
	Mat min_sigma = GetLaJnd(img);
	int height = img.rows;
	int width = img.cols;
	for (int h = 0; h < height; ++h)
	{
		double* jndgdata = min_sigma.ptr<double>(h);
		for (int w = 0; w < width; ++w)
		if (jndgdata[w] < min_th)
			jndgdata[w] = min_th;
	}
	Mat preImg = GetARImg(img, min_sigma);
	return preImg;
}
Mat GetVar(Mat img)
{
	int height = img.rows;
	int width = img.cols;
	const int size = 2 * r + 1;
	Mat kern = Mat::ones(size, size, CV_64FC1);
	kern = kern / (size*size);
	Mat MeanImg;
	filter2D(img, MeanImg, img.depth(), kern);
	MeanImg = MeanImg.mul(MeanImg);
	Mat SqrImg(img.rows, img.cols, CV_64FC1);
	SqrImg = img.mul(img);
	Mat MeanSqr;
	filter2D(SqrImg, MeanSqr, SqrImg.depth(), kern);
	Mat Var;
	Var = MeanSqr - MeanImg;
	Mat SqrVar;
	sqrt(Var, SqrVar);
	return SqrVar;
}
Mat GetARImg(Mat img, Mat min_sigma)
{
	if (img.type() != CV_64FC1)
		img.convertTo(img, CV_64FC1, 1.0);
	int height = img.rows;
	int width = img.cols;
	Mat var = GetVar(img);
	Mat sigma_val(min_sigma.rows, min_sigma.cols, CV_64FC1);
	sigma_val = min_sigma.mul(min_sigma);
	for (int h = 0; h < height; ++h)
	{
		double* min_sm_data = min_sigma.ptr<double>(h);
		double* sm_data = sigma_val.ptr<double>(h);
		double* var_data = var.ptr<double>(h);
		for (int w = 0; w < width; ++w)
		{
			if (var_data[w] > min_sm_data[w])
			{
				sm_data[w] = (min_sm_data[w] * sqrt(min_sm_data[w] / var_data[w]))*(min_sm_data[w] * sqrt(min_sm_data[w] / var_data[w]));

			}
		}
	}
	Mat Mat_Pad = GetPadArr(img, R + r);
	Mat Img_Pad = GetSubPad(Mat_Pad, R);
	const int size = 2 * r + 1;
	Mat kern = Mat::ones(size, size, CV_64FC1);
	kern = kern / (size*size);
	Mat Img_Reco = Mat::zeros(height, width, CV_64FC1);
	Mat Weight_Mat = Mat::zeros(height, width, CV_64FC1);
	Mat Max_Weight = Mat::zeros(height, width, CV_64FC1);
	for (int h = 0; h < 2 * R; ++h)
	{
		for (int w = 0; w <2 * R; ++w)
		{
			if (h == R&&w == R)
				continue;
			//计算差值
			Mat Img_Move = Mat_Pad(Rect(w, h, width + 2 * r, height + 2 * r));
			Mat Mat_Dif = (Img_Pad - Img_Move);
			Mat_Dif = Mat_Dif.mul(Mat_Dif);
			Mat Sum_Val;
			filter2D(Mat_Dif, Sum_Val, Mat_Dif.depth(), kern);
			Mat sub_SumV = Sum_Val(Rect(r, r, width, height));
			Mat Mat_Simi;
			exp((-sub_SumV / sigma_val), Mat_Simi);
			//重建值
			Img_Reco = Img_Reco + Img_Move(Rect(r, r, width, height)).mul(Mat_Simi);
			Weight_Mat = Weight_Mat + Mat_Simi;
			for (int h = 0; h < height; ++h)
			{
				double* Sim_data = Mat_Simi.ptr<double>(h);
				double* M_data = Max_Weight.ptr<double>(h);
				for (int w = 0; w < width; ++w)
				if (Sim_data[w] > M_data[w])
				{
					M_data[w] = Sim_data[w];
				}
			}

		}
	}
	Img_Reco = Img_Reco + Max_Weight.mul(img);
	Weight_Mat = Weight_Mat + Max_Weight;
	Mat Img_Reconst = Img_Reco / Weight_Mat;
	//Img_Reconst.convertTo(Img_Reconst, CV_8UC1,1);
	//Img_Reconst.convertTo(Img_Reconst, CV_64FC1, 1);
	return Img_Reconst;
}
Mat GetPadArr(Mat img, int adsize)
{
	int height = img.rows;
	int width = img.cols;
	int PadH = height + 2 * adsize;
	int PadW = width + 2 * adsize;
	Mat Pad(PadH, PadW, CV_64FC1);
	for (int h = 0; h < PadH; ++h)
	{
		double* Pdata = Pad.ptr<double>(h);
		double* Orgdata;
		if (h < adsize)
		{
			Orgdata = img.ptr<double>(adsize - 1 - h);
		}
		else if (h - height - adsize + 1>0)
		{
			Orgdata = img.ptr<double>(height - (h - height - adsize + 1));
		}
		else
		{
			Orgdata = img.ptr<double>(h - adsize);
		}

		for (int w = 0; w < PadW; ++w)
		{
			if (w < adsize)
			{
				Pdata[w] = Orgdata[adsize - 1 - w];
			}
			else if (w - width - adsize + 1>0)
			{
				Pdata[w] = Orgdata[width - (w - width - adsize + 1)];
			}
			else
			{
				Pdata[w] = Orgdata[w - adsize];
			}
		}
	}

	return Pad;
}
Mat GetSubPad(Mat Mat_Pad, int subSzie)
{

	int Height = Mat_Pad.rows;
	int Width = Mat_Pad.cols;
	Mat SubPad(Height - 2 * subSzie, Width - 2 * subSzie, CV_64FC1);
	for (int h = subSzie; h < Height - subSzie; ++h)
	{
		double* Pad = Mat_Pad.ptr<double>(h);
		double* SPad = SubPad.ptr<double>(h - subSzie);
		for (int w = subSzie; w < Width - subSzie; ++w)
		{
			SPad[w - subSzie] = Pad[w];
		}
	}
	return SubPad;
}
