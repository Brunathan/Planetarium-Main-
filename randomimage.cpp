/*
 * main.cpp
 *
 *  Created on: 30 oct. 2013
 *      Author: lyang
 */
#include <iostream>
#include "basictype.h"
#include "bouleversement.h"
#include "nvcalgorithm.h"
using namespace std;

vector<PtArray> referencepoint, scenepoint;
vector<pair<RDM_Point,RDM_Point> > correspdencelist;
nvcalgorithm::NVCAlgorithm nvcalg(6,0,0.2f,0.01f,4,0,10.0f,1.3f);
cv::Mat_<float> homo;
cv::Mat_<cv::Vec3b> whitebackground;

#if (DEBUG >= 1)
	ofstream debugoutput;
#endif

void terminateMain() //...

cv::Mat drawOpenCV //...

template <typename PointType>
bool readFromCoordinate(string filename, vector<PointType> &borderpts, vector<PointType> &pointarray)
{
    borderpts.resize(4);
    ifstream file(filename.c_str());
    if (!file.is_open())         //Check if the corresponding file exists
    {
        cout<<"Fail to open file "<< filename << endl;
        return false;
    }
    // Deal with all the coordinates

    int totalpoints;
    PointType tmppoint;
    file >> borderpts[0].x >> borderpts[0].y;       //read left lower corner coordinate
    file >> borderpts[2].x >> borderpts[2].y;       //read right higher corner coordinate

    borderpts[1].x = borderpts[0].x; borderpts[1].y = borderpts[2].y;
    borderpts[3].x = borderpts[2].x; borderpts[3].y = borderpts[0].y;
    file >> totalpoints;

    for(int i=0;i<totalpoints;i++)
    {
        file >> tmppoint.x >> tmppoint.y;
        pointarray.push_back(tmppoint);
    }

    file.close();

    return true;
}

// main function
int main(int argc, char **argv)
{

#if (DEBUG >= 1)
	debugoutput.open("DebugOutput.txt");
	if(!debugoutput.is_open())
	{
		cout<<"Can't open debug file correctly !"<<endl;
	}
	debugoutput<<"Open?"<<endl;
#endif


	if (setenv ("DISPLAY", ":0", 0) == -1) return -1;

	float eta = 0.08;

	//Define the background size
	cv::Size sz(1280, 720);
	whitebackground = cv::Mat::ones(sz, CV_8UC3);
	whitebackground.setTo(cv::Scalar(255,255,255));

	//Initialize for all
	if (eta == 0)
	{
		nvcalg.setParam(6,0,0.2f,0.01,4,0,10.0f,1.3f);
	}
	else
	{
		nvcalg.setParam(6,0,0.2f,eta,4,0,10.0f,1.3f);
    }

    PtArray refborderpts,refpointarray,imgborderpts,imgpointarray;

    readFromCoordinate("/home/brunathan/Téléchargements/rrdm/ref.txt",refborderpts,refpointarray);
    readFromCoordinate("/home/brunathan/Téléchargements/rrdm/img.txt",imgborderpts,imgpointarray);

    referencepoint.push_back(refborderpts);          //border line , in red (matching result)
    referencepoint.push_back(refpointarray);   //reference points, in red
    scenepoint.push_back(imgborderpts);          //border line, in blue (real one)
    scenepoint.push_back(imgpointarray);         //scene points, in blue cross

	cv::Mat resultimg = drawOpenCV(&whitebackground, &scenepoint , NULL, &referencepoint, &correspdencelist);

	cv::imshow("Result window", resultimg);
	cv::waitKey(-1);
}

