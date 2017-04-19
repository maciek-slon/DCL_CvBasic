/*!
 * \file
 * \brief
 * \author Maciej
 */

#include <memory>
#include <string>

#include "Median.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace Median {

Median::Median(const std::string & name) :
		Base::Component(name),
		kernel_width("kernel", 1, "range")  {

	kernel_width.addConstraint("1");
	kernel_width.addConstraint("20");
	registerProperty(kernel_width);
}

Median::~Median() {
}

void Median::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerStream("in_img", &in_img);
	registerStream("out_img", &out_img);
	// Register handlers
	registerHandler("onNewImage", boost::bind(&Median::onNewImage, this));
	addDependency("onNewImage", &in_img);

}

bool Median::onInit() {

	return true;
}

bool Median::onFinish() {
	return true;
}

bool Median::onStop() {
	return true;
}

bool Median::onStart() {
	return true;
}

void Median::onNewImage() {
	CLOG(LTRACE) << "Median::onNewImage\n";
	try {
		cv::Mat img = in_img.read().clone();
		cv::medianBlur(img, img, kernel_width*2+1);
		out_img.write(img);
	} catch (...) {
		CLOG(LERROR) << "Median::onNewImage failed\n";
	}
}



} //: namespace Median
} //: namespace Processors
