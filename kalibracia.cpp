#include<iostream>
using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;


enum
{
	capturing = 0, calibrated = 1
};


int main()
{
    const int CHESSBOARD_WIDTH = 10;
    const int CHESSBOARD_HEIGHT = 7;
    const float squareSize = 2.5;
    Size corner = Size(CHESSBOARD_WIDTH, CHESSBOARD_HEIGHT);
    int counter = 40;
    int nimages = 30;
    Size imageSize;
    int mode = capturing;
    char leftfilename[100], rightfilename[100];

    vector<Mat> imagePoints1;
    vector<Mat> imagePoints2;
    vector<vector<Point3f>> objectPoints;

    bool found1 = false, found2 = false;
    int counter2 = 0;
    Mat pointBuf1 = Mat::zeros(54, 2, CV_32FC1);
    Mat pointBuf2 = Mat::zeros(54, 2, CV_32FC1);
    
    for(int i = 1; i <= counter; i++)
    {   
		sprintf(leftfilename, "snimky/image_left_%d.png", i);
		sprintf(rightfilename, "snimky/image_right_%d.png", i);
		
		Mat imgleft_frame = imread(leftfilename);
		Mat imgright_frame = imread(rightfilename);
		
		imageSize = imgleft_frame.size();

		found1 = findChessboardCorners(imgleft_frame, corner, pointBuf1, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK | CALIB_CB_NORMALIZE_IMAGE);
		found2 = findChessboardCorners(imgright_frame, Size(CHESSBOARD_WIDTH,CHESSBOARD_HEIGHT), pointBuf2, CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_FAST_CHECK | CALIB_CB_NORMALIZE_IMAGE);
		
		if(found1)
		{       
			Mat gray_image1;
			cvtColor(imgleft_frame, gray_image1, COLOR_BGRA2GRAY);
			cornerSubPix(gray_image1, pointBuf1, Size(11, 11), Size(-1, -1), TermCriteria(TermCriteria::EPS+TermCriteria::MAX_ITER, 30, 0.1 ));
			drawChessboardCorners(imgleft_frame, Size(CHESSBOARD_WIDTH,CHESSBOARD_HEIGHT), pointBuf1, found1);
		}
		if(found2)
		{
			Mat gray_image2;
			cvtColor(imgright_frame, gray_image2, COLOR_BGRA2GRAY);
			cornerSubPix(gray_image2, pointBuf2, Size(11, 11), Size(-1, -1), TermCriteria(TermCriteria::EPS+TermCriteria::MAX_ITER, 30, 0.1 ));
			drawChessboardCorners(imgright_frame, Size(CHESSBOARD_WIDTH,CHESSBOARD_HEIGHT), pointBuf2, found2);
		}
		if(found1 && found2)
		{
			imagePoints1.push_back(pointBuf1);
			imagePoints2.push_back(pointBuf2);
			
			counter2++;
			cout << counter2 << '\n';
		}
		nimages = counter2;
		objectPoints.resize(nimages);
		cout << "countervalue" << i << '\n';
    }
    
    for(int i = 0; i < nimages; i++ )
    {
        for( int j = 0; j < CHESSBOARD_HEIGHT; j++ )
            for( int k = 0; k < CHESSBOARD_WIDTH; k++ )
                objectPoints[i].push_back(Point3f(j*squareSize, k*squareSize, 0));
    }
    
    cout << "check1\n";
    Mat cameraMatrix[2], distCoeffs[2];
    cameraMatrix[0] = Mat::eye(3, 3, CV_64F);
    cameraMatrix[1] = Mat::eye(3, 3, CV_64F);
    Mat R, T, E, F;
    cout << objectPoints.size() << '\n';
    cout << imagePoints1.size() << '\n';
    if(imagePoints1.size() == imagePoints2.size())
        cout<<"samesize\n";
    if(imagePoints1.size() >= nimages)
    {   cout<<"check2\n";     
    double rms = stereoCalibrate(objectPoints, imagePoints1, imagePoints2,cameraMatrix[0], distCoeffs[0],
        cameraMatrix[1], distCoeffs[1],imageSize, R, T, E, F,
        CALIB_FIX_ASPECT_RATIO +CALIB_ZERO_TANGENT_DIST +
        CALIB_SAME_FOCAL_LENGTH +CALIB_RATIONAL_MODEL +
        CALIB_FIX_K3 +CALIB_FIX_K4 + CALIB_FIX_K5,
        TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 100, 1e-5));

    cout << "check3\n";
    cout << "done with RMS error = " << rms << '\n';
    mode = calibrated;
    cout << "calibrated\n";
    }
    
    if(mode == calibrated)
    {
        vector<Vec3f> lines[2];
        for(int  i = 0; i < nimages; i++)
        {
            int npt = (int)imagePoints1[i].rows;
            cout << npt << '\n';
			Mat imgpt1;
            Mat imgpt2;

            imgpt1 = Mat(imagePoints1[i]);
            undistortPoints(imgpt1, imgpt1, cameraMatrix[0], distCoeffs[0], Mat(), cameraMatrix[0]);
            computeCorrespondEpilines(imgpt1, 1, F, lines[0]);


            imgpt2 = Mat(imagePoints2[i]);
            undistortPoints(imgpt2, imgpt2, cameraMatrix[1], distCoeffs[1], Mat(), cameraMatrix[1]);
            computeCorrespondEpilines(imgpt2, 2, F, lines[1]);
            cout << "checksdcdufb\n";

		
		
			cout << "check8\n";
			FileStorage fs("intrinsics.yml", FileStorage::WRITE);
			if(fs.isOpened())
			{
				fs << "M1" << cameraMatrix[0] << "D1" << distCoeffs[0] <<
					"M2" << cameraMatrix[1] << "D2" << distCoeffs[1];
				fs.release();
			}
			else
				 cout << "Error: can not save the intrinsic parameters\n";

			Mat R1, R2, P1, P2, Q;
			Rect validRoi[2];

			stereoRectify(cameraMatrix[0], distCoeffs[0],
				cameraMatrix[1], distCoeffs[1],
				imageSize, R, T, R1, R2, P1, P2, Q,
				CALIB_ZERO_DISPARITY, 1, imageSize, &validRoi[0], &validRoi[1]);

			fs.open("extrinsics.yml", FileStorage::WRITE);
			if(fs.isOpened())
			{
				fs << "CM1" << cameraMatrix[0];
				fs << "CM2" << cameraMatrix[1];
				fs << "D1" << distCoeffs[0];
				fs << "D2" << distCoeffs[1];
				fs << "R" << R;
				fs << "T" << T;
				fs << "E" << E;
				fs << "F" << F;
				fs << "R1" << R1;
				fs << "R2" << R2;
				fs << "P1" << P1;
				fs << "P2" << P2;
				fs << "Q" << Q;
				fs.release();
			}
			else
				cout << "Error: can not save the intrinsic parameters\n";    
		}
        
    }
    
	return 0;   
}
