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


    lava_kamera.release();
    prava_kamera.release();
    imwrite( "scenaL.png", obrazok_lavy );
    imwrite( "scenaR.png", obrazok_pravy );

    waitKey(0);
    return 0;
}
