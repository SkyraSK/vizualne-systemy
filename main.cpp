#include <iostream>
using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
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
    int vstup;
    while (true)
    {
        lava_kamera >> obrazok_lavy;
        prava_kamera >> obrazok_pravy;
        imshow("Lava snimka", obrazok_lavy);
        imshow("Prava snimka", obrazok_pravy);
        vstup = waitKey(1);
        if (vstup == 27)
			break;
    }
	lava_kamera.release();
	prava_kamera.release();

	return 0;
}
