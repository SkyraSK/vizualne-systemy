#include <iostream>
using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;

int main()
{
	Mat obrazok = imread("test.png", 1);
	
    namedWindow("Okno", CV_WINDOW_AUTOSIZE);
    imshow("Obrazok", obrazok);
    waitKey(0);

	return 0;
}
