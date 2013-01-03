#include <opencv/cv.h>
#include <opencv/cv.hpp>
#include <opencv/highgui.h>
#include <string>
#include <sstream>
#include <string>
#include <cstdio>

using namespace cv;

Mat img, tmp, element;

std::string file = "/media/sda3/Science/Uploads/TrakEM2/180-220-int/180-220-int-00.tif";

template <typename T>
inline std::string to_string(T datum)
{
  std::stringstream output;
  output << datum;
  return output.str();
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

int sigmaX = 15;

Mat unsharp_mask(Mat img)
{
  Mat in = img;
  Mat out;
  GaussianBlur(in,out,Size(0,0),sigmaX,0);
  addWeighted(in,1.5,out,-0.5,0,out);
  return out;
}

int main()
{
  img = imread(file,0);
  for(unsigned int cannyLower = 150; cannyLower < 700; cannyLower=cannyLower+20)
  {
    for(unsigned int cannyUpper = cannyLower; cannyUpper < 700; cannyUpper=cannyUpper+20)
    {
      /*bilateralFilter(img,tmp,10,20,5);
      morphologyEx(img,img,MORPH_OPEN,element,Point(-1,-1),1);
      for(unsigned int i = 0; i < 4; i++)
        tmp = unsharp_mask(tmp);*/
      Canny(img,tmp,cannyLower,cannyUpper);
      tmp = showContours(tmp);
      //FindContours();
      vector<int> params(2);
      params[0] = CV_IMWRITE_JPEG_QUALITY;
      params[1] = 100;
      imwrite(("Output/"+to_string(cannyLower)+","+to_string(cannyUpper)+".jpg").c_str(),tmp,params);      
    }
  }
  return 0;
}