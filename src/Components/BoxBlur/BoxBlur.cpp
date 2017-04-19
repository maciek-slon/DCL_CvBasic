/*!
 * \file
 * \brief
 * \author Maciej
 */

#include <memory>
#include <string>

#include "BoxBlur.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace BoxBlur {

BoxBlur::BoxBlur(const std::string & name) :
		Base::Component(name),
		kernel_width("kernel.width", 1, "range"),
		kernel_height("kernel.height", 1, "range")  {

	kernel_width.addConstraint("0");
	kernel_width.addConstraint("20");
	kernel_height.addConstraint("0");
	kernel_height.addConstraint("20");
	registerProperty(kernel_height);
	registerProperty(kernel_width);
}

BoxBlur::~BoxBlur() {
}

void BoxBlur::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerStream("in_img", &in_img);
	registerStream("out_img", &out_img);
	// Register handlers
	registerHandler("onNewImage", boost::bind(&BoxBlur::onNewImage, this));
	addDependency("onNewImage", &in_img);

}

bool BoxBlur::onInit() {

	return true;
}

bool BoxBlur::onFinish() {
	return true;
}

bool BoxBlur::onStop() {
	return true;
}

bool BoxBlur::onStart() {
	return true;
}

void BoxBlur::onNewImage() {
	CLOG(LTRACE) << "BoxBlur::onNewImage\n";
	try {
		cv::Mat img = in_img.read().clone();
		cv::blur(img, img, cv::Size(kernel_width*2+1, kernel_height*2+1));
		out_img.write(img);
	} catch (...) {
		CLOG(LERROR) << "BoxBlur::onNewImage failed\n";
	}
}



} //: namespace BoxBlur
} //: namespace Processors
