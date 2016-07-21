/*!
 * \file
 * \brief
 * \author Tomek Kornuta,,,
 */

#include <memory>
#include <string>

#include "CvSIFT.hpp"
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
namespace CvSIFT {

CvSIFT::CvSIFT(const std::string & name) :
		Base::Component(name),
		prop_scale("scale", 1)  {

	registerProperty(prop_scale);

}

CvSIFT::~CvSIFT() {
}

void CvSIFT::prepareInterface() {
	// Register handlers with their dependencies.
	registerHandler("onNewImage", boost::bind(&CvSIFT::onNewImage, this));
	addDependency("onNewImage", &in_img);

	// Input and output data streams.
	registerStream("in_img", &in_img);
	registerStream("out_features", &out_features);
	registerStream("out_descriptors", &out_descriptors);
}

bool CvSIFT::onInit() {

	return true;
}

bool CvSIFT::onFinish() {
	return true;
}

bool CvSIFT::onStop() {
	return true;
}

bool CvSIFT::onStart() {
	return true;
}

void CvSIFT::onNewImage()
{
	LOG(LTRACE) << "CvSIFT::onNewImage\n";
	try {
		// Input: a grayscale image.
		cv::Mat input = in_img.read();

	    std::vector<cv::KeyPoint> keypoints;
		cv::Mat descriptors;

#if CV_MAJOR_VERSION==2
		//-- Step 1: Detect the keypoints.
	    cv::SiftFeatureDetector detector;
	    detector.detect(input, keypoints);

		//-- Step 2: Calculate descriptors (feature vectors).
		cv::SiftDescriptorExtractor extractor;
		extractor.compute( input, keypoints, descriptors);


#elif CV_MAJOR_VERSION==3
		cv::Ptr<cv::xfeatures2d::SIFT> sift= cv::xfeatures2d::SIFT::create();
		sift->detectAndCompute(input, cv::Mat(), keypoints, descriptors);
#endif
		if (prop_scale != 1) {
			for (int i = 0 ; i < keypoints.size(); ++i)
				keypoints[i].pt = keypoints[i].pt * (1.0 / prop_scale);
		}
		// Write results to outputs.
	    Types::Features features(keypoints);
		out_features.write(features);
		out_descriptors.write(descriptors);
	} catch (...) {
		LOG(LERROR) << "CvSIFT::onNewImage failed\n";
	}
}


} //: namespace CvSIFT
} //: namespace Processors
