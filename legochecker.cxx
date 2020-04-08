#include <stdio.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

string DetectShape( vector<Point> &shape )
{
    //Check perimeter of closed Shape
    double perimeter = arcLength( shape, true);
    //Approximate shape for better recognition
    vector<Point> approx;
    approxPolyDP(shape, approx, 0.04 * perimeter, true);
    if(approx.size() == 4)
    {
        if(perimeter > 540 && perimeter < 590)
        {
            return "Plate 2x4";
        }
        else if (perimeter <520 && perimeter > 470)
        {
            return "Plate 1x4";
        }
        Rect r = boundingRect(approx);
        float ar = r.width / r.height;
        cout <<"Es un rectangulo jeje";
        return "Rectangle" + to_string(perimeter);
    }
    else
    {
        return "Plate 1x1 Round";
    }
    
    return "NO";

}

int main(int argc, char** argv )
{
  // Get command line arguments
  if ( argc < 2 )
  {
  	std::cerr << "Usage: " << argv[ 0 ] << " image_file" << std::endl;
    return( -1 );

  } // fi

  // Review given command line arguments
  std::cout << "-------------------------" << std::endl;
  for( int a = 0; a < argc; a++ )
    std::cout << argv[ a ] << std::endl;
  std::cout << "-------------------------" << std::endl;

  // Read an image
  Mat image;
  image = imread( argv[1], 1 );
  
  if ( !image.data )
  {
    std::cerr << "Error: No image data" << std::endl;
    return( -1);
  
  } // fi
  Mat resized;
  pyrDown(image, resized, Size(image.cols/2, image.rows/2));
  float ratio = float(image.size().height / resized.size().height);
  Mat grayScale;
  cvtColor(image, grayScale, COLOR_BGR2GRAY);
  Mat bImage;
  GaussianBlur(grayScale, bImage, Size(5, 5), 0);
  threshold(bImage, bImage,70, 255, THRESH_BINARY);
  imwrite("threshold.png", bImage);
  vector<vector<Point>> contours;
  cout << "Finding Cont\n";
  findContours(bImage, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
  cout << "Success Finding Cont\n";
  Moments m;
  string response = "";
  int cx, cy;
  cout << "Gathering Moments :')\n";
  for(auto i = 0; i < contours.size(); i++)
  {
     m = moments(contours[i]);
     cx = int((m.m10 / m.m00) /** ratio*/);
     cy = int((m.m01 / m.m00) /** ratio*/);
     cout << "Detecting Shape\n";
     response = DetectShape(contours[i]);
     cout << "Shaoe Detected: " + response << endl;
     if(response != "NO")
     {
         vector<Point> pointCopy(contours[i]);
        /*for(auto j = 0; j < contours[i].size(); j++)
        {
        pointCopy[j].x *= ratio;
        pointCopy[j].y *= ratio;
     }*/
     cout << "Drawing Contours\n";
     drawContours(image, contours, i, Scalar(0,255,0), 2);
     cout << "Success Drawing Cont\n";
     putText(image, response, Point(cx -80, cy), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,255,255), 2);
     cout << "Success Drawing Text\n";
     }
     
     
  }
  imwrite("response.png", image);

  //Write something
  return( 0 );
}

// eof - example.cxx
