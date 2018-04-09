#include <opencv2/core/utility.hpp>
#include <opencv2/tracking.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <cstring>
#include <ctime>

using namespace std;
using namespace cv;

Ptr<Tracker> createTrackerByName(string trackerType) {

        if (trackerType == "BOOSTING")
            return TrackerBoosting::create();
        if (trackerType == "MIL")
            return TrackerMIL::create();
        if (trackerType == "KCF")
            return TrackerKCF::create();
        if (trackerType == "TLD")
            return TrackerTLD::create();
        if (trackerType == "MEDIANFLOW")
            return TrackerMedianFlow::create();
        if (trackerType == "GOTURN")
            return TrackerGOTURN::create();
}

int main( int argc, char** argv ){
	if(argc<2){
	cout<<
	" Usage: example_tracking_multitracker <video_name> [algorithm]\n"
	" examples:\n"
	" example_tracking_multitracker Bolt/img/%04d.jpg\n"
	" example_tracking_multitracker faceocc2.webm MEDIANFLOW\n"
	<< endl;
	return 0;
	}

	std::string trackingAlg = "KCF";

	if(argc>2)
	trackingAlg = argv[2];

	MultiTracker trackers;

	vector<Rect2d> objects;

	std::string video = argv[1];
	VideoCapture cap(video);

	Mat frame;

	// get bounding box
	cap >> frame;
	vector<Rect> rois;
	selectROIs("tracker",frame, rois);

	//quit when the tracked object(s) is not provided
	if(rois.size()<1)
		return 0;

	std::vector<Ptr<Tracker> > algorithms;
	for (size_t i = 0; i < rois.size(); i++)
	{
		algorithms.push_back(createTrackerByName(trackingAlg));
		objects.push_back(rois[i]);
	}

	// initialize the tracker
	trackers.add(algorithms, frame, objects);

	// do the tracking
	printf("Start the tracking process, press ESC to quit.\n");
	for (;;) {
		// get frame from the video
		cap >> frame;

		// stop the program if no more images
		if(frame.rows==0 || frame.cols==0)
		break;

		//update the tracking result
		trackers.update(frame);

		// draw the tracked object
		for(unsigned i=0; i<trackers.getObjects().size(); i++)
			rectangle( frame, trackers.getObjects()[i], Scalar( 255, 0, 0 ), 2, 1 );

		// show image with the tracked object
		imshow("tracker",frame);

		//quit on ESC button
		if(waitKey(1)==27)break;
	}

}
