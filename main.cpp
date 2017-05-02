#include <iostream>
using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

int main()
{
	VideoCapture lava_kamera(0);
    if (!lava_kamera.isOpened())
		cout << "Lava kamera nefunguje.\n";
    
    VideoCapture prava_kamera(1);
    if (!prava_kamera.isOpened())
		cout << "Prava kamera nefunguje.\n";
		

    Mat obrazok_lavy, obrazok_pravy;
	lava_kamera >> obrazok_lavy; 
	prava_kamera >> obrazok_pravy;
	imshow("lavy", obrazok_lavy);
	imshow("pravy", obrazok_pravy);
	waitKey(0);
		
	lava_kamera.release();
	prava_kamera.release();
	
	Mat CM1, CM2, D1, D2, R1, R2, P1, P2;
	FileStorage fs("extrinsics.yml", FileStorage::READ);
	fs["CM1"] >> CM1;
	fs["CM2"] >> CM2;
	fs["D1"] >> D1;
	fs["D2"] >> D2;
	fs["R1"] >> R1;
	fs["R2"] >> R2;
	fs["P1"] >> P1;
	fs["P2"] >> P2;
	fs.release();
	
	Mat map1x, map1y, map2x, map2y, obrazok_lavy_U, obrazok_pravy_U;
	initUndistortRectifyMap(CM1, D1, R1, P1, obrazok_lavy.size(), CV_32FC1, map1x, map1y);
    initUndistortRectifyMap(CM2, D2, R2, P2, obrazok_pravy.size(), CV_32FC1, map2x, map2y);
    remap(obrazok_lavy, obrazok_lavy_U, map1x, map1y, INTER_LINEAR, BORDER_CONSTANT, Scalar());
    remap(obrazok_pravy, obrazok_pravy_U, map2x, map2y, INTER_LINEAR, BORDER_CONSTANT, Scalar());
    
    Mat obrL_grey, obrP_grey;
    cvtColor(obrazok_lavy, obrL_grey, CV_BGR2GRAY);
	cvtColor(obrazok_pravy, obrP_grey, CV_BGR2GRAY);
	imshow("lavy", obrL_grey);
	imshow("pravy", obrP_grey);
	waitKey(0);
	
	Ptr<StereoBM> sbm = StereoBM::create(16, 21);
	
	Mat disp, disp8;
	sbm->compute(obrL_grey, obrP_grey, disp);
	normalize(disp, disp8, 0, 255, CV_MINMAX, CV_8U);

	imshow("lavy", obrazok_lavy_U);
	imshow("pravy", obrazok_pravy_U);
	imshow("disparity map", disp8);
	
	waitKey(0);

	return 0;
}
