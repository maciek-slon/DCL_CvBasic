/*!
 * \file
 * \brief for grayscale images set; with FAST Detector
 * \author Jan Figat
 * \e-mail jan.figat@gmail.com
 */

#include <memory>
#include <string>

#include "CvFreak.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

#if (CV_MAJOR_VERSION == 3)
#include <opencv2/xfeatures2d.hpp>
#endif


namespace Processors {
namespace CvFreak {

CvFreak::CvFreak(const std::string & name) :
		Base::Component(name)
{
	// Register properties.

}

CvFreak::~CvFreak() {
}

void CvFreak::prepareInterface() {
	// Register handlers with their dependencies.
    registerHandler("onNewImage", boost::bind(&CvFreak::onNewImage, this));
    addDependency("onNewImage", &in_img);

	// Input and output data streams.
    registerStream("in_img", &in_img);
	registerStream("out_features", &out_features);
	registerStream("out_descriptors", &out_descriptors);
}

bool CvFreak::onInit() {

	return true;
}

bool CvFreak::onFinish() {
	return true;
}

bool CvFreak::onStop() {
	return true;
}

bool CvFreak::onStart() {
	return true;
}

void CvFreak::onNewImage()
{
	LOG(LTRACE) << "CvFreak::onNewImage\n";
	try {
		// Input: a grayscale image.
        cv::Mat img = in_img.read();
        cv::Mat gray;
        cvtColor(img, gray, COLOR_BGR2GRAY);

        std::vector<KeyPoint> keypoints;
        cv::Mat descriptors;
#if CV_VERSION_MAJOR==2
        //-- Step 1: Detect the keypoints using FAST Detector.
        //cv::FAST(gray,keypoints,10);
        cv::FastFeatureDetector detector(10);
        detector.detect( gray, keypoints );

		//-- Step 2: Calculate descriptors (feature vectors) using Freak descriptor.
        cv::FREAK extractor;
        extractor.compute( gray, keypoints, descriptors);
#elif CV_VERSION_MAJOR==3
		cv::Ptr<cv::FastFeatureDetector> fast = cv::FastFeatureDetector::create(10);
		fast->detect(gray, keypoints);
		cv::Ptr<cv::xfeatures2d::FREAK> freak = cv::xfeatures2d::FREAK::create();
		freak->compute(gray, keypoints, descriptors);
#endif

		// Write features to the output.
	    Types::Features features(keypoints);
		out_features.write(features);

		// Write descriptors to the output.
		out_descriptors.write(descriptors);
	} catch (...) {
		LOG(LERROR) << "CvFreak::onNewImage failed\n";
	}
}



} //: namespace CvFreak
} //: namespace Processors
