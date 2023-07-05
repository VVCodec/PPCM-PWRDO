#include <iostream>
#include <fstream>
#include "global.h"
using namespace std;

//size_t      wwidth      = 10;
//size_t      hhight      = 10;
//size_t      rresolution = 10;
//size_t      pprecision  = 10;
//size_t      fframenum   = 10;
//std::string ocname;
//bool        rdoflag = false;
double   gradweight=0;
double** g_jnd;     //全局变量来存储JND的值
double** BlockJnd;  //记录一帧块的JND的平均值
int      cuWsize   = 0;
int      cuHsize   = 0;
int      cuWmargin = 0;
int      cuHmargin = 0;
double*  g_OneDBlcokJnd;  //开辟一个空间用来储存每一帧的的区域平均JND的值
void     g_getBasicImformation( int& picHeight, int& picWidth ) {
  if ( picWidth % 64 == 0 ) {
    cuWsize   = picWidth / 64;
    cuWmargin = 64;
  } else {
    cuWsize   = picWidth / 64 + 1;
    cuWmargin = picWidth % 64;
  }
  if ( picHeight % 64 == 0 ) {
    cuHsize   = picHeight / 64;
    cuHmargin = 64;
  } else {
    cuHsize   = picHeight / 64 + 1;
    cuHmargin = picHeight % 64;
  }
}

void AllocateMen( int& HeightOrg, int& WidthOrg ) {
  //////////////////开辟空间的代码段//////////////////////////////////////////////////////

  //开辟空间存储原始的Jnd的值
  g_jnd = new double*[HeightOrg];
  if ( g_jnd == NULL ) {
    cout << "Failed to sert mem for ALL g_jnd" << endl;
    exit( 1 );
  }
  for ( int i = 0; i < HeightOrg; i++ ) {
    g_jnd[i] = new double[WidthOrg];
    if ( g_jnd[i] == NULL ) {
      cout << "Failed to sert mem for PART g_jnd" << endl;
      exit( 1 );
    }
  }
  //得到图像中有多少LCU
  g_getBasicImformation( HeightOrg, WidthOrg );
  //开辟空间存储每一个块的JND的值
  BlockJnd = new double*[cuHsize];
  if ( BlockJnd == NULL ) {
    cout << "Failed to get JND menory" << endl;
    exit( 1 );
  }
  for ( int j = 0; j < cuHsize; j++ ) {
    BlockJnd[j] = new double[cuWsize];
    if ( BlockJnd[j] == NULL ) {
      cout << "Failed to get JND menory" << endl;
      exit( 1 );
    }
    memset( BlockJnd[j], 0, cuWsize * sizeof( double ) );
  }
  //用于存储最终的JND
  g_OneDBlcokJnd = new double[cuWsize * cuHsize];
}
void DestroyMen( int& HeightOrg, int& WidthOrg ) {
  ////////////////////////////////////////////////////////////////////////////////////////
  //以下代码段是释放内存
  for ( int i = 0; i < HeightOrg; i++ )  //释放内存
  {
    delete g_jnd[i];
    g_jnd[i] = NULL;
  }
  delete g_jnd;
  g_jnd = NULL;
  for ( int j = 0; j < cuHsize; j++ )  //释放开辟的空间
  {
    delete[] BlockJnd[j];
  }
  delete BlockJnd;
  BlockJnd = NULL;
  delete[] g_OneDBlcokJnd;
  g_OneDBlcokJnd = NULL;
}
