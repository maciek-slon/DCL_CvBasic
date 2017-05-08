/*!
 * \file
 * \brief
 * \author Maciej
 */

#include <memory>
#include <string>

#include "MaskGenerator.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace MaskGenerator {

MaskGenerator::MaskGenerator(const std::string & name) :
		Base::Component(name) , 
		cx("cx", 320, "range"), 
		cy("cy", 240, "range"), 
		sx("sx", 200, "range"), 
		sy("sy", 200, "range") {
	cx.addConstraint("0");
	cx.addConstraint("640");
	registerProperty(cx);
	
	cy.addConstraint("0");
	cy.addConstraint("640");
	registerProperty(cy);
	
	sx.addConstraint("0");
	sx.addConstraint("640");
	registerProperty(sx);
	
	sy.addConstraint("0");
	sy.addConstraint("640");
	registerProperty(sy);

}

MaskGenerator::~MaskGenerator() {
}

void MaskGenerator::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerStream("in_img", &in_img);
	registerStream("out_mask", &out_mask);
	registerStream("out_draw", &out_draw);
	registerStream("out_roi", &out_roi);
	// Register handlers
	registerHandler("generate", boost::bind(&MaskGenerator::generate, this));
	addDependency("generate", &in_img);

}

bool MaskGenerator::onInit() {

	return true;
}

bool MaskGenerator::onFinish() {
	return true;
}

bool MaskGenerator::onStop() {
	return true;
}

bool MaskGenerator::onStart() {
	return true;
}

void MaskGenerator::generate() {
	cv::Mat img = in_img.read();
	
	cv::Rect cr(cx-sx/2, cy-sy/2, sx, sy);
	Types::Rectangle rect(cx-sx/2, cy-sy/2, sx, sy);
	cv::Mat mask = cv::Mat::zeros(img.size(), CV_8UC1);
	cv::rectangle(mask, cv::Point(cx-sx/2, cy-sy/2), cv::Point(cx+sx/2, cy+sy/2), cv::Scalar::all(255), CV_FILLED);
	out_mask.write(mask);
	out_draw.write(rect);
	out_roi.write(img(cr));
}



} //: namespace MaskGenerator
} //: namespace Processors
