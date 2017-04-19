/*!
 * \file
 * \brief
 * \author Maciej,,,
 */

#include <memory>
#include <string>

#include "BrightnessContrast.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace BrightnessContrast {

BrightnessContrast::BrightnessContrast(const std::string & name) :
		Base::Component(name),
		prop_brightness("brightness", 0, "range"),
		prop_contrast("contrast", 0, "range"),
		prop_equalize("equalize_histogram", false)  {

	prop_brightness.addConstraint("-256");
	prop_brightness.addConstraint("256");
	registerProperty(prop_brightness);

	prop_contrast.addConstraint("-256");
	prop_contrast.addConstraint("256");
	registerProperty(prop_contrast);

	registerProperty(prop_equalize);
}

BrightnessContrast::~BrightnessContrast() {
}

void BrightnessContrast::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerHandler("onNewImage", boost::bind(&BrightnessContrast::onNewImage, this));
	addDependency("onNewImage", &in_img);
	
	registerStream("in_img", &in_img);
	registerStream("out_img", &out_img);
}

bool BrightnessContrast::onInit() {

	return true;
}

bool BrightnessContrast::onFinish() {
	return true;
}

bool BrightnessContrast::onStop() {
	return true;
}

bool BrightnessContrast::onStart() {
	return true;
}

void BrightnessContrast::onNewImage() {
	cv::Mat img = in_img.read();
	if (img.channels() != 1) {
		CLOG(LWARNING) << "in_img.channels() != 1";
		cv::cvtColor(img, img, CV_BGR2GRAY);
	}
	cv::Mat out;
	if (prop_equalize) {
		/// Apply Histogram Equalization
		equalizeHist( img, out );
	} else {	
		img.convertTo(img, CV_32F);
		out = img - 128 + prop_brightness();
		out = out * (1./128 * (prop_contrast+128));
		out = out + 128;
		out.convertTo(out, CV_8U);
	}
	
	out_img.write(out);
}


} //: namespace BrightnessContrast
} //: namespace Processors
