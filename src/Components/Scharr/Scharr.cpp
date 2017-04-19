/*!
 * \file
 * \brief
 * \author Maciej
 */

#include <memory>
#include <string>

#include "Scharr.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace Scharr {

Scharr::Scharr(const std::string & name) :
		Base::Component(name)  {

}

Scharr::~Scharr() {
}

void Scharr::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerStream("in_img", &in_img);
	registerStream("out_img", &out_img);
	// Register handlers
	registerHandler("onNewImage", boost::bind(&Scharr::onNewImage, this));
	addDependency("onNewImage", &in_img);

}

bool Scharr::onInit() {

	return true;
}

bool Scharr::onFinish() {
	return true;
}

bool Scharr::onStop() {
	return true;
}

bool Scharr::onStart() {
	return true;
}

void Scharr::onNewImage() {
	
	CLOG(LTRACE) << "Scharr::onNewImage\n";
	try {
		cv::Mat img = in_img.read().clone();
		
		if (img.channels() ==  3)
			cv::cvtColor(img, img, CV_BGR2GRAY);
		
		/// Generate grad_x and grad_y
		cv::Mat grad, grad_x, grad_y;
		cv::Mat abs_grad_x, abs_grad_y;
					
		int scale = 1;
		int delta = 0;
		int ddepth = CV_16S;

		/// Gradient X
		cv::Scharr( img, grad_x, ddepth, 1, 0, scale, delta, cv::BORDER_DEFAULT );
		cv::convertScaleAbs( grad_x, abs_grad_x );

		/// Gradient Y
		cv::Sobel( img, grad_y, ddepth, 0, 1, scale, delta, cv::BORDER_DEFAULT );
		cv::convertScaleAbs( grad_y, abs_grad_y );

		/// Total Gradient (approximate)
		cv::addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );
		
		out_img.write(grad);
	} catch (...) {
		CLOG(LERROR) << "Sobel::onNewImage failed\n";
	}
}



} //: namespace Scharr
} //: namespace Processors
