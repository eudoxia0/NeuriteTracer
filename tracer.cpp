#include <opencv/cv.h>
#include <opencv/cv.hpp>
#include <opencv/highgui.h>
#include <string>
#include <sstream>
#include <iostream>

using namespace cv;
using namespace std;

std::string file = "/media/sda3/Code/NeuriteTracer/in.png";

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

Mat unsharp_mask(Mat in)
{
  Mat out;
  GaussianBlur(in,out,Size(0,0),5,0);
  addWeighted(in,1.5,out,-0.5,0,out);
  return out;
}

Mat algorithm(Mat img)
{
  Mat tmp, out, element;
  tmp = Mat::zeros(img.cols, img.rows, CV_8UC1);
  int cannyLower = 400, cannyUpper = 500;
  /*for(unsigned int i = 0; i < 6; i++)
    img = unsharp_mask(img);*/
  //bilateralFilter(img,tmp,10,30,10);
  //tmp = img;
  //morphologyEx(tmp,tmp,MORPH_OPEN,element,Point(-1,-1),2);*/
  //for(unsigned int i = 0; i < 0; i++)
  //  tmp = unsharp_mask(tmp);
  //Canny(tmp,tmp,cannyLower,cannyUpper);
  //tmp = showContours(tmp);
  return tmp;
}

struct Contour
{
  vector<Point> points;
  Point centroid;
  Contour* up;
  Contour* down;
  Scalar color;
};

struct Layer
{
  vector<Contour*> contours;
};

struct Stack
{
  vector<Layer*> layers;
  long xdim, ydim;
};

Point computeCentroid(vector<Point> in)
{
  int x, y;
  for(unsigned int i = 0; i < in.size(); i++)
  {
    x = x + in[i].x;
    y = y + in[i].y;
  }
  return Point(x/2,y/2);
}

Contour* makeContour(vector<Point> in)
{
  Contour* out;
  out->points = in;
  out->centroid = computeCentroid(in);
  out->color = Scalar(rand()&255, rand()&255, rand()&255 );
  return out;
}

Layer* makeLayer(Mat img)
{
  Layer* out;
  Mat dst = Mat::zeros(img.rows, img.cols, CV_8UC3);
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  findContours( img, contours, hierarchy,
                CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
  if(hierarchy.size() > 0)
  {
    for(int i = 0; i >= 0; i = hierarchy[i][0] )
    {
      out->contours.push_back(makeContour(contours[i]));
    }
  }
  return out;
}

bool classify(Contour* in)
{ }

Layer* eliminateContours(Layer* in)
{
  for(unsigned int i = 0; i < in->contours.size(); i++)
  {
    if(!classify(in->contours[i]))
      in->contours.erase(in->contours.begin()+i);
  }
  return in;
}

Stack* makeStack(vector<string> Images)
{
  Stack* out;
  Mat sample = imread(Images[i].c_str(),0)
  out->xdim = sample.cols;
  out->ydim = sample.rows;
  for(unsigned int i = 0; i < Images.size(); i++)
  {
    out->layers.push_back(makeLayer(imread(Images[i].c_str(),0)));
  }
}

void tieContours(Stack* input)
{
  
}

box
{
    <1,0,0>,  // Near lower left corner
    <2,1,1>   // Far upper right corner
    texture { pigment { color Red } } }

string exportData(Stack* data)
{
  string output =
  "#include \"shapes.inc\"\
#include \"colors.inc\"\
background { color Cyan };";
  long height = data->layers.size()-1;
  output += "camera { location <"
            + -(int)(to_string<long>(data->xdim)*2) + ","
            + (int)(to_string<long>(height)*2) + ","
            + -(int)(to_string<long>(data->ydim)*2) + ">\
  look_at <" + to_string<long>(data->xdim) + ","
             + to_string<long>(height)    + ","
             + to_string<long>(data->ydim) + ">}\
light_source { <" + -(int)(to_string<long>(data->xdim)*2) + ","
                  + (int)(to_string<long>(height)*2) + ","
                  + -(int)(to_string<long>(data->ydim)*2) + "> color White }\n";
  for(unsigned long i = 0; i < height; i++)
  {
    for(unsigned long j = 0; i < data->layers[i].contours.size()-1; j++)
    {
      for(unsigned long k = 0; i < data->layers[i].contours[j].points.size()-1; k++)
      {
        output += "box { <"
                  + to_string<long>(data->layers[i].contours[j].points[k].x) + ","
                  + to_string<long>(i) + ","
                  + to_string<long>(data->layers[i].contours[j].points[k].y) + ",>\n";
        output += "<"
                  + to_string<long>(1+data->layers[i].contours[j].points[k].x) + ","
                  + to_string<long>(1+i) + ","
                  + to_string<long>(1+data->layers[i].contours[j].points[k].y) + ",>\n";
        output += "texture { pigment { color ";
        
        output += " } } }\n";     
      }
    }
  }
}

int main()
{
  Mat img = imread(file.c_str(),0), out;
  out = algorithm(img);
  namedWindow("Derp",CV_WINDOW_NORMAL);
  cvResizeWindow("Derp",700,800);
  imshow("Derp", out);
  while(true)
  {
    if(waitKey(0) == 'q')
    {
      exit(0);
    }
  }
  return 0;
}