/*!
 * \file
 * \brief
 * \author Tomek Kornuta,,,
 */

#include <memory>
#include <string>

#include "CvSURF.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

#if (CV_MAJOR_VERSION == 2)
#if (CV_MINOR_VERSION > 3)
#include <opencv2/nonfree/features2d.hpp>
#endif
#elif (CV_MAJOR_VERSION == 3)
#include <opencv2/xfeatures2d.hpp>
#endif


namespace Processors {
namespace CvSURF {

CvSURF::CvSURF(const std::string & name) :
		Base::Component(name),
        minHessian("minHessian", 400, "minHessian")
{
	// Register properties.
	registerProperty(minHessian);
}

CvSURF::~CvSURF() {
}

void CvSURF::prepareInterface() {
	// Register handlers with their dependencies.
	registerHandler("onNewImage", boost::bind(&CvSURF::onNewImage, this));
	addDependency("onNewImage", &in_img);

	// Input and output data streams.
	registerStream("in_img", &in_img);
	registerStream("out_features", &out_features);
	registerStream("out_descriptors", &out_descriptors);
}

bool CvSURF::onInit() {

	return true;
}

bool CvSURF::onFinish() {
	return true;
}

bool CvSURF::onStop() {
	return true;
}

bool CvSURF::onStart() {
	return true;
}

void CvSURF::onNewImage()
{
	LOG(LTRACE) << "CvSURF::onNewImage\n";
	try {
		// Input: a grayscale image.
		cv::Mat input = in_img.read();


		std::vector<KeyPoint> keypoints;
		Mat descriptors;

#if CV_VERSION_MAJOR==2
		//-- Step 1: Detect the keypoints using SURF Detector.
		SurfFeatureDetector detector( minHessian );
		detector.detect( input, keypoints );


		//-- Step 2: Calculate descriptors (feature vector).
        SurfDescriptorExtractor extractor;
		extractor.compute( input, keypoints, descriptors);
#elif CV_VERSION_MAJOR==3
		cv::Ptr<cv::xfeatures2d::SURF> surf = cv::xfeatures2d::SURF::create(minHessian);
		surf->detectAndCompute(input, cv::Mat(), keypoints, descriptors);
#endif
		// Write features to the output.
	    Types::Features features(keypoints);
		out_features.write(features);

		// Write descriptors to the output.
		out_descriptors.write(descriptors);
	} catch (...) {
		LOG(LERROR) << "CvSURF::onNewImage failed\n";
	}
}



} //: namespace CvSURF
} //: namespace Processors
