/*!
 * \file
 * \brief
 * \author Maciej
 */

#include <memory>
#include <string>

#include "ShapeDetector.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace ShapeDetector {

ShapeDetector::ShapeDetector(const std::string & name) :
		Base::Component(name) , 
		shape1_name("shape1.name", std::string("shape_1")), 
		shape1_compactness("shape1.compactness", 1, "range"), 
		shape2_name("shape2.name", std::string("shape_2")), 
		shape2_compactness("shape2.compactness", 2, "range") {
	registerProperty(shape1_name);
	shape1_compactness.addConstraint("1");
	shape1_compactness.addConstraint("100");
	registerProperty(shape1_compactness);
	registerProperty(shape2_name);
	shape2_compactness.addConstraint("1");
	shape2_compactness.addConstraint("100");
	registerProperty(shape2_compactness);

}

ShapeDetector::~ShapeDetector() {
}

void ShapeDetector::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerStream("in_contours", &in_contours);
	registerStream("in_img", &in_img);
	registerStream("out_img", &out_img);
	// Register handlers
	registerHandler("onNewData", boost::bind(&ShapeDetector::onNewData, this));
	addDependency("onNewData", &in_contours);
	addDependency("onNewData", &in_img);

}

bool ShapeDetector::onInit() {

	return true;
}

bool ShapeDetector::onFinish() {
	return true;
}

bool ShapeDetector::onStop() {
	return true;
}

bool ShapeDetector::onStart() {
	return true;
}

void ShapeDetector::onNewData() {
	std::vector<std::vector<cv::Point> > contours = in_contours.read();
	cv::Mat img = in_img.read().clone();
	
	// Get the moments
	std::vector<cv::Moments> mu(contours.size() );
	for( int i = 0; i < contours.size(); i++ ) { 
		mu[i] = cv::moments( contours[i], false ); 
	}
	
	//  Get the mass centers:
	std::vector<cv::Point2f> mc( contours.size() );
	std::vector<float> cr( contours.size() );
	std::vector<std::string> names( contours.size() );
	for( int i = 0; i < contours.size(); i++ ) {
		mc[i] = cv::Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); 
		float a = cv::contourArea(contours[i]);
		float l = cv::arcLength(contours[i], true);
		cr[i] = l*l/a;
		if (fabs(cr[i] - shape1_compactness) < fabs(cr[i] - shape2_compactness))
			names[i] = shape1_name;
		else
			names[i] = shape2_name;
	}

	// Draw contours
	for( int i = 0; i< contours.size(); i++ ) {
		cv::Scalar color = cv::Scalar( 0, 255, 0 );
		cv::drawContours( img, contours, i, color, 2, 8, cv::noArray(), 0, cv::Point() );
		cv::circle( img, mc[i], 2, color, -1, 8, 0 );
		std::string txt = names[i] + ": " + std::to_string(round(cr[i]*10)/10);
		cv::putText(img, txt, mc[i], cv::FONT_HERSHEY_PLAIN, 1, color);
	}
	
	out_img.write(img);
}



} //: namespace ShapeDetector
} //: namespace Processors
