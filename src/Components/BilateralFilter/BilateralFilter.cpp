/*!
 * \file
 * \brief
 * \author Maciej
 */

#include <memory>
#include <string>

#include "BilateralFilter.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace BilateralFilter {

BilateralFilter::BilateralFilter(const std::string & name) :
		Base::Component(name),
		prop_d("d", 3, "range"),
		prop_sigma_c("sigma_color", 10, "range"),
		prop_sigma_s("sigma_space", 10, "range")  {

	prop_d.addConstraint("1");
	prop_d.addConstraint("20");
	prop_sigma_c.addConstraint("1");
	prop_sigma_c.addConstraint("255");
	prop_sigma_s.addConstraint("1");
	prop_sigma_s.addConstraint("255");
	registerProperty(prop_d);
	registerProperty(prop_sigma_c);
	registerProperty(prop_sigma_s);
}

BilateralFilter::~BilateralFilter() {
}

void BilateralFilter::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerStream("in_img", &in_img);
	registerStream("out_img", &out_img);
	// Register handlers
	registerHandler("onNewImage", boost::bind(&BilateralFilter::onNewImage, this));
	addDependency("onNewImage", &in_img);

}

bool BilateralFilter::onInit() {

	return true;
}

bool BilateralFilter::onFinish() {
	return true;
}

bool BilateralFilter::onStop() {
	return true;
}

bool BilateralFilter::onStart() {
	return true;
}

void BilateralFilter::onNewImage() {
	CLOG(LTRACE) << "BilateralFilter::onNewImage\n";
	try {
		cv::Mat img = in_img.read();
		cv::Mat out;
		cv::bilateralFilter(img, out, prop_d, prop_sigma_c, prop_sigma_s);
		out_img.write(out);
	} catch (...) {
		CLOG(LERROR) << "BilateralFilter::onNewImage failed\n";
	}
}



} //: namespace BilateralFilter
} //: namespace Processors
