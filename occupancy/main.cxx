#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    if( argc != 2)
    {
     cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
     return -1;
    }

    Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file

    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
	//prepare data
	std::vector<std::vector<int> > data(100);
	for(int i= 0; i<100; i++)
	{
		std::vector<int> pixel(2);
		pixel[0]= i*5;
		pixel[1]= i*5;
		data[i] = pixel;
		
	}
    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
	
	int height = image.rows;
	int width = image.cols;
	cv::Vec3b colour;
	colour.val[0] = 255;
	colour.val[1] = 0;
	colour.val[2] = 0;
	for(int i = 0; i<data.size(); i++)
	{	
			image.at<cv::Vec3b>(data[i][0],data[i][1]) = colour;
			image.at<cv::Vec3b>(data[i][0]+1,data[i][1]+1) = colour;
			image.at<cv::Vec3b>(data[i][0]+1,data[i][1]+1) = colour;
			image.at<cv::Vec3b>(data[i][0]+1,data[i][1]+1) = colour;		
	}

	imshow( "Display window", image );                   // Show our image inside it.
	
    waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}
