#include <opencv/cv.h>
#include <opencv/cv.hpp>
#include <opencv/highgui.h>
#include <string>
#include <sstream>
#include <string>
#include <cstdio>

std::string file = //"test.jpg";
                //"test2.jpg";
                "/media/sda3/Science/Uploads/TrakEM2/180-220-int/180-220-int-00.tif";
                //"in.png";

using namespace cv;

Mat img, tmp, element;

int cannyLower, cannyUpper, dilation, erosion, opening, closing,
    gradient, tophat, blackhat, sigmaX, sigmaY,
    alpha_factor, beta_factor, gamma_factor = 0;

void unsharp_mask()
{
  Mat in = img;
  Mat out;
  GaussianBlur(in,out,Size(0,0),sigmaX,sigmaY);
  addWeighted(in,1.5,out,-0.5,0,out);
  img = out;
}

void manualThreshold(Mat* result) {
    for(int i = 0; i < result->rows; i++) {
        uchar* rowPtr = result->ptr(i);
        for(int j=0; j < result->cols; j++) {
            rowPtr[j] = (rowPtr[j] > 0) ? 255 : 0;
        }
    }
}

Mat showContours(Mat src)
{
  Mat dst = Mat::zeros(src.rows, src.cols, CV_8UC3);
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  findContours( src, contours, hierarchy,
                CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
  srand(time(NULL));
  if(hierarchy.size() > 0)
  {
    int idx = 0;
    for( ; idx >= 0; idx = hierarchy[idx][0] )
    {
      Scalar color( rand()&255, rand()&255, rand()&255 );
      drawContours( dst, contours, idx, color, CV_FILLED, 8, hierarchy,std::abs(idx));
    }
  }
  return dst;
}

void load()
{
  tmp = imread(file,0);
  //bilateralFilter(tmp,img,20,20,4);
  //equalizeHist(tmp,img);
  //threshold(img,img,200,255,THRESH_TRUNC);
  img = tmp;
  //bilateralFilter(tmp,img,10,20,5);
  //medianBlur(img,img,3);
  //ALGORITHM
  
  /*sigmaX = 2;
  unsharp_mask();
  unsharp_mask();
  erode(img,img,Mat(),Point(-1,-1),1);
  element = Mat();
  morphologyEx(img,img,MORPH_OPEN,element,Point(-1,-1),1);*/
}


void reset()
{
  load();
  setTrackbarPos("Dilation","Controls",0);
  dilation = 0;
  setTrackbarPos("Erosion","Controls",0);
  erosion = 0;
  setTrackbarPos("Opening","Controls",0);
  opening = 0;
  setTrackbarPos("Closing","Controls",0);
  closing = 0;
  setTrackbarPos("Gradient","Controls",0);
  gradient = 0;
  setTrackbarPos("TopHat","Controls",0);
  tophat = 0;
  setTrackbarPos("BlackHat","Controls",0);
  blackhat = 0;
  setTrackbarPos("cannyLower","Controls",0);
  cannyLower = 0;
  setTrackbarPos("cannyUpper","Controls",0);
  cannyUpper = 0;
  setTrackbarPos("SigmaX","Controls",0);
  sigmaX = 0;
  /*setTrackbarPos("SigmaY","Controls",0);
  sigmaY = 0;
  setTrackbarPos("Alpha","Controls",0);
  alpha_factor = 0;
  setTrackbarPos("Beta","Controls",0);
  beta_factor = 0;
  setTrackbarPos("Gamma","Controls",0);
  gamma_factor = 0;*/
}

void do_operations()
{
  if(dilate != 0)
    dilate(img,img,Mat(),Point(-1,-1),dilation);
  if(erode != 0)
    erode(img,img,Mat(),Point(-1,-1),erosion);
  if(opening != 0)
    morphologyEx(img,img,MORPH_OPEN,element,Point(-1,-1),opening);
  if(closing != 0)
    morphologyEx(img,img,MORPH_CLOSE,element,Point(-1,-1),closing);
  if(gradient != 0)
    morphologyEx(img,img,MORPH_GRADIENT,element,Point(-1,-1),gradient);
  if(tophat != 0)
    morphologyEx(img,img,MORPH_TOPHAT,element,Point(-1,-1),tophat);
  if(blackhat != 0)
    morphologyEx(img,img,MORPH_BLACKHAT,element,Point(-1,-1),blackhat);
  if(sigmaX != 0)
    unsharp_mask();
}

int main()
{
  load();
  element = Mat();
  namedWindow("Sandbox",CV_WINDOW_NORMAL);
  namedWindow("Controls",CV_WINDOW_NORMAL);
  imshow("Sandbox",img);
  createTrackbar("Dilation","Controls",&dilation,10,NULL);
  createTrackbar("Erosion","Controls",&erosion,10,NULL);
  createTrackbar("Opening","Controls",&opening,10,NULL);
  createTrackbar("Closing","Controls",&closing,10,NULL);
  createTrackbar("Gradient","Controls",&gradient,10,NULL);
  createTrackbar("TopHat","Controls",&tophat,10,NULL);
  createTrackbar("BlackHat","Controls",&blackhat,10,NULL);
  createTrackbar("cannyLower","Controls",&cannyLower,1000,NULL);
  createTrackbar("cannyUpper","Controls",&cannyUpper,1000,NULL);
  createTrackbar("SigmaX","Controls",&sigmaX,100,NULL);
  /*createTrackbar("SigmaY","Controls",&sigmaY,100,NULL);
  createTrackbar("Alpha","Controls",&alpha_factor,100,NULL);
  createTrackbar("Beta","Controls",&beta_factor,100,NULL);
  createTrackbar("Gamma","Controls",&gamma_factor,100,NULL);*/
  derp:
  char key = cvWaitKey(0);
  if(key == 'r')
  {
    reset();
    imshow("Sandbox",img);
  }
  else if(key == 'd')
  {
    do_operations();
    imshow("Sandbox",img);
  }
  else if(key == 'c')
  {
    Canny(img,img,cannyLower,cannyUpper);
    imshow("Sandbox",img);
  }  
  else if(key == 's')
  {
    img = showContours(img);
    imshow("Sandbox",img);
  }
  else if(key == 't')
  {
    manualThreshold(&img);
    imshow("Sandbox",img);
  }
  goto derp;
  return 0;  
}