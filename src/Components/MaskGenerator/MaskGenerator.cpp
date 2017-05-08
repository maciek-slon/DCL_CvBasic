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
	registerStream("out_mask", &out_mask);
	registerStream("out_draw", &out_draw);
	// Register handlers
	registerHandler("generate", boost::bind(&MaskGenerator::generate, this));
	addDependency("generate", NULL);

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
	Types::Rectangle rect(cx-sx/2, cy-sx/2, sx, sy);
	cv::Mat mask = cv::Mat::zeros(cv::Size(640, 480), CV_8UC1);
	cv::rectangle(mask, cv::Point(cx-sx/2, cy-sy/2), cv::Point(cx+sx/2, cy+sy/2), cv::Scalar::all(255), CV_FILLED);
	out_mask.write(mask);
	out_draw.write(rect);
}



} //: namespace MaskGenerator
} //: namespace Processors