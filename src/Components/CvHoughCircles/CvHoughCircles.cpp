/*!
 * \file
 * \brief
 */

#include <memory>
#include <string>

#include "CvHoughCircles.hpp"
#include "Common/Logger.hpp"

#include <opencv2/imgproc/imgproc.hpp>

namespace Processors {
namespace CvHoughCircles {

using namespace cv;
using namespace std;
using Types::Circles;

CvHoughCircles_Processor::CvHoughCircles_Processor(const std::string & name) :
	Base::Component(name), 
	inverseRatioOfAccumulatorResolution("inverseRatioOfAccumulatorResolution",2,"range"),
	minDist("minDist", 1, "range"),
	cannyHigherThreshold("cannyHigherThreshold", 100, "range"),
	accumulatorThreshold("accumulatorThreshold", 100, "range"),
	minCircleRadius("minCircleRadius", 10, "range"),
	maxCircleRadius("maxCircleRadius", 20, "range")
{
	inverseRatioOfAccumulatorResolution.addConstraint("1");
	inverseRatioOfAccumulatorResolution.addConstraint("10");
	registerProperty(inverseRatioOfAccumulatorResolution);
	
	minDist.addConstraint("1");
	minDist.addConstraint("250");
	registerProperty(minDist);
	
	cannyHigherThreshold.addConstraint("30");
	cannyHigherThreshold.addConstraint("250");
	registerProperty(cannyHigherThreshold);
	
	accumulatorThreshold.addConstraint("1");
	accumulatorThreshold.addConstraint("1000");
	registerProperty(accumulatorThreshold);
	
	minCircleRadius.addConstraint("3");
	minCircleRadius.addConstraint("300");
	registerProperty(minCircleRadius);
	
	maxCircleRadius.addConstraint("3");
	maxCircleRadius.addConstraint("300");
	registerProperty(maxCircleRadius);
}

CvHoughCircles_Processor::~CvHoughCircles_Processor()
{
	LOG(LTRACE) << "Good bye CvHoughCircles_Processor\n";
}

void CvHoughCircles_Processor::prepareInterface() {
	// Register data streams, events and event handlers HERE!

	registerHandler("onNewImage", boost::bind(&CvHoughCircles_Processor::onNewImage, this));
	addDependency("onNewImage", &in_img);

	registerStream("in_img", &in_img);
	registerStream("out_circles", &out_circles);
}

bool CvHoughCircles_Processor::onInit()
{
	LOG(LTRACE) << "CvHoughCircles_Processor::initialize\n";

	

	return true;
}

bool CvHoughCircles_Processor::onFinish()
{
	return true;
}

bool CvHoughCircles_Processor::onStop()
{
	return true;
}

bool CvHoughCircles_Processor::onStart()
{
	return true;
}

void CvHoughCircles_Processor::onNewImage()
{
	if (in_img.empty()) {
		LOG(LFATAL) << "Component " << name() << " " << "in_img input stream is empty.";
		return;
	}
	Mat image = in_img.read();

	if (image.channels() != 1) {
		LOG(LWARNING) << "Component " << name() << " " << "in_img received image must have only one channel.";
		cv::cvtColor(image,image, CV_BGR2GRAY);
	}

	Circles c;

	HoughCircles(image, c.circles, CV_HOUGH_GRADIENT, inverseRatioOfAccumulatorResolution, minDist, cannyHigherThreshold, accumulatorThreshold, minCircleRadius, maxCircleRadius);

	out_circles.write(c);
}

}//: namespace CvHoughCircles
}//: namespace Processors
