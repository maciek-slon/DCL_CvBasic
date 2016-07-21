/*!
 * \file
 * \brief
 * \author Jan Figat,
 * \e-mail jan.figat@gmail.com
 */

#include <memory>
#include <string>

#include "CvBRISK.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace CvBRISK {

CvBRISK::CvBRISK(const std::string & name) :
        Base::Component(name),
        thresh("thresh",60,"thresh")
        //octaves("octaves",3,"octaves"),
        //patternScale("patternScale",1.0f,"patternScale")
{
	// Register properties.
    registerProperty(thresh);
}

CvBRISK::~CvBRISK() {
}

void CvBRISK::prepareInterface() {
	// Register handlers with their dependencies.
	registerHandler("onNewImage", boost::bind(&CvBRISK::onNewImage, this));
	addDependency("onNewImage", &in_img);

	// objectImg and output data streams.
	registerStream("in_img", &in_img);
	registerStream("out_features", &out_features);
	registerStream("out_descriptors", &out_descriptors);

}

bool CvBRISK::onInit() {

	return true;
}

bool CvBRISK::onFinish() {
	return true;
}

bool CvBRISK::onStop() {
	return true;
}

bool CvBRISK::onStart() {
	return true;
}

void CvBRISK::onNewImage()
{
    LOG(LTRACE) << "CvBRISK::onNewImage\n";
	try {
		// objectImg: a grayscale image.
		cv::Mat objectImg = in_img.read();

        //-- Step 1: Detect the keypoints using Brisk Detector.
        //cv::BriskFeatureDetector detector;//(thresh,3,1.0f);

		std::vector<cv::KeyPoint> keypoints;
		cv::Mat descriptors;

#if CV_VERSION_MAJOR==2
		cv::BRISK brisk(thresh, 3, 1.0f);
		brisk.detect( objectImg, keypoints );

		//-- Step 2: Calculate descriptors (feature vectors).
        //cv::BriskDescriptorExtractor extractor;
		//cv::DescriptorExtractor * extractor = new cv::BRISK();
		brisk.compute( objectImg, keypoints, descriptors);
#elif CV_VERSION_MAJOR==3
		cv::Ptr<cv::BRISK> brisk = cv::BRISK::create(thresh, 3, 1.0f);
		brisk->detectAndCompute(objectImg, cv::Mat(), keypoints, descriptors);
#endif

		// Write features to the output.
	    Types::Features features(keypoints);
		out_features.write(features);

		// Write descriptors to the output.
		out_descriptors.write(descriptors);
	} catch (...) {
        LOG(LERROR) << "CvBRISK::onNewImage failed\n";
	}
}



} //: namespace CvBRISK
} //: namespace Processors
