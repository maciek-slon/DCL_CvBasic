/*!
 * \file
 * \brief
 * \author Maciej,,,
 */

#include <memory>
#include <string>

#include "ColorVis.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace ColorVis {

ColorVis::ColorVis(const std::string & name) :
		Base::Component(name),
		m_type("type", std::string("RGB"), "combo")  {
	m_type.addConstraint("RGB");
	m_type.addConstraint("HSV");
	registerProperty(m_type);
}

ColorVis::~ColorVis() {
}

void ColorVis::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerHandler("onNewImage", boost::bind(&ColorVis::onNewImage, this));
	addDependency("onNewImage", &in_img);
	
	registerStream("in_img", &in_img);
	registerStream("out_plane0", &out_plane0);
	registerStream("out_plane1", &out_plane1);
	registerStream("out_plane2", &out_plane2);
}

bool ColorVis::onInit() {

	return true;
}

bool ColorVis::onFinish() {
	return true;
}

bool ColorVis::onStop() {
	return true;
}

bool ColorVis::onStart() {
	return true;
}

void ColorVis::onNewImage() {
	CLOG(LTRACE) << "ColorVis::onNewImage\n";
	try {
		cv::Mat img = in_img.read();

		std::vector<cv::Mat> channels;
		cv::split(img, channels);
		
		std::vector<cv::Mat> outputs(3);
		cv::Mat zeros = cv::Mat::zeros(img.size(), CV_8UC1);
		
		if (m_type == "RGB") {
			cv::Mat out_r = img.clone();
			cv::Mat out_g = img.clone();
			cv::Mat out_b = img.clone();
			
			cv::Mat in_arr[] = {img, zeros};
			
			cv::Mat out_arr_r[] = {out_r};
			int mix_r[] = {3,0, 3,1, 2,2};
			cv::mixChannels(in_arr, 2, out_arr_r, 1, mix_r, 3);
			
			cv::Mat out_arr_g[] = {out_g};
			int mix_g[] = {3,0, 1,1, 3,2};
			cv::mixChannels(in_arr, 2, out_arr_g, 1, mix_g, 3);
			
			cv::Mat out_arr_b[] = {out_b};
			int mix_b[] = {0,0, 3,1, 3,2};
			cv::mixChannels(in_arr, 2, out_arr_b, 1, mix_b, 3);
							
			// Write output to streams.
			out_plane0.write(out_b);
			out_plane1.write(out_g);
			out_plane2.write(out_r);
		} else 
		if (m_type == "HSV") {
		}
		
	}
	catch (const char * ex) {
		LOG(LERROR) << ex;
	}
	catch (...) {
		LOG(LERROR) << "RGBLUT::onNewImage failed\n";
	}
}

} //: namespace ColorVis
} //: namespace Processors
