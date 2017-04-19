/*!
 * \file
 * \brief
 * \author Maciej
 */

#include <memory>
#include <string>

#include "Sobel.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace Sobel {

Sobel::Sobel(const std::string & name) :
		Base::Component(name) , 
		kernel_size("kernel_size", 1, "range") {
	kernel_size.addConstraint("0");
	kernel_size.addConstraint("3");
	registerProperty(kernel_size);

}

Sobel::~Sobel() {
}

void Sobel::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerStream("in_img", &in_img);
	registerStream("out_img", &out_img);
	// Register handlers
	registerHandler("onNewImage", boost::bind(&Sobel::onNewImage, this));
	addDependency("onNewImage", &in_img);

}

bool Sobel::onInit() {

	return true;
}

bool Sobel::onFinish() {
	return true;
}

bool Sobel::onStop() {
	return true;
}

bool Sobel::onStart() {
	return true;
}

void Sobel::onNewImage() {
	CLOG(LTRACE) << "Sobel::onNewImage\n";
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
		cv::Sobel( img, grad_x, ddepth, 1, 0, kernel_size * 2 + 1, scale, delta, cv::BORDER_DEFAULT );
		cv::convertScaleAbs( grad_x, abs_grad_x );

		/// Gradient Y
		cv::Sobel( img, grad_y, ddepth, 0, 1, kernel_size * 2 + 1, scale, delta, cv::BORDER_DEFAULT );
		cv::convertScaleAbs( grad_y, abs_grad_y );

		/// Total Gradient (approximate)
		cv::addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );
		
		out_img.write(grad);
	} catch (...) {
		CLOG(LERROR) << "Sobel::onNewImage failed\n";
	}
}



} //: namespace Sobel
} //: namespace Processors
