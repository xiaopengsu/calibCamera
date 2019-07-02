#include "opencv2/core.hpp"
#include <opencv2/core/utility.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/highgui.hpp"

#include <cctype>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <iostream>
using namespace cv;
using namespace std;

const char * usage =
" \nexample command line for calibration from a live feed.\n"
"   ./myUndisRectifyMap  -ints=../data/camera.yml ../data/image_list.xml [../undistImages/]\n"
" \n";

static void help()
{
    printf("\n%s",usage);
}

static bool readStringList( const string& filename, vector<string>& l )
{
    l.resize(0);
    FileStorage fs(filename, FileStorage::READ);
    if( !fs.isOpened() )
        return false;

    FileNode n = fs.getFirstTopLevelNode();
    if( n.type() != FileNode::SEQ )
        return false;
    FileNodeIterator it = n.begin(), it_end = n.end();
    for( ; it != it_end; ++it )
        l.push_back((string)*it);
    return true;
}
vector<string> split(string str, string pattern)
{
    vector<string> ret;
    if(pattern.empty()) return ret;
    size_t start=0,index=str.find_first_of(pattern,0);
    while(index!=str.npos)
    {
         if(start!=index)
             ret.push_back(str.substr(start,index-start));
         start=index+1;
         index=str.find_first_of(pattern,start);
}
if(!str.substr(start).empty())
ret.push_back(str.substr(start));
return ret;
}
int main( int argc, char** argv )
{
    if(argc< 2)
    {
	help();
	return 0;
    }
    cv::CommandLineParser parser(argc, argv,
	"{help||}"
	"{ints|camera.yml|}"
    "{@input_data|0|}"
    "{@output_data|../undistorImage/|}");
    if (parser.has("help"))
    {
        help();
        return 0;
    }
    vector<string> imageList;
    string intrinsic_filename;
    string inputFilename;
    string outputFileDir;
    //read imagelist
    if(parser.has("ints"))
    {
        intrinsic_filename = parser.get<string>("ints");
    }
    inputFilename = parser.get<string>("@input_data");
    outputFileDir = parser.get<string>("@output_data");
    
    cout << intrinsic_filename <<endl;
    cout << inputFilename <<endl;
    readStringList(inputFilename, imageList);
   
    FileStorage fs(intrinsic_filename, FileStorage::READ);
    if(!fs.isOpened())
    {
        printf("Failed to open file %s\n", intrinsic_filename.c_str());
	return -1;
    } 
    
    Mat cameraMatrix, distCoeffs;
    fs["camera_matrix"] >> cameraMatrix;
    fs["distortion_coefficients"] >> distCoeffs;
    //read camera instrisic
    Size imageSize;
    Mat view, rview, map1, map2;
    view = imread(imageList[0],1);
    imageSize = view.size();
   
#if   1 
    double alpha=1;//0 kuo, 1 suo
    Size rectificationSize = imageSize;
    Rect validPixROI;
    cv::Mat P;
    P= getOptimalNewCameraMatrix(cameraMatrix,distCoeffs,
                                 imageSize,
                                 alpha,//0 huo, 1 suo
                                 rectificationSize,//undistorted image size
                                 &validPixROI//undistorted image rectangle in source image
                                 );//new camera matirx

    cout << "P: "<<P<<endl;
    initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(),
                            P,//undistort image camera instrinsic matirx
                            rectificationSize,//undistorted image size
                            CV_16SC2, map1, map2);
   
#else
    initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(),
                            Mat(),
                            imageSize, CV_16SC2, map1, map2);

#endif    
    for(int i = 0; i < (int)imageList.size(); i++ )
    {
        view = imread(imageList[i], 1);
        if(view.empty())
            continue;
//        undistort( view, rview, cameraMatrix, distCoeffs,
//                   getOptimalNewCameraMatrix(cameraMatrix,distCoeffs,
//                                             imageSize,
//                                             alpha,//0 huo, 1 suo
//                                             rectificationSize,//undistorted image size
//                                             &validPixROI//undistorted image rectangle in source image
//                                             )//cameraMatrix
//                   );
        remap(view, rview, map1, map2, INTER_LINEAR);
//         cout << "UndistortImage " << imageList[i] << endl;
        vector<string> vImageSubstr = split(imageList[i],"/");
//         string imageWritename = outputFileDir + vImageSubstr[3];
	string imageWritename = outputFileDir + vImageSubstr[2];
        imwrite(imageWritename, rview);
	cout << "imageWritename " << imageWritename << endl;
//         char c = (char)waitKey();
//         if( c == 27 || c == 'q' || c == 'Q' )
//            break;
    }
    fs.release();

    return 0;
}
