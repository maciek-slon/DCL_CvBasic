/*!
 * \file
 * \brief
 * \author Maciej
 */

#include <memory>
#include <string>

#include "Hist2D.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace Hist2D {

Hist2D::Hist2D(const std::string & name) :
		Base::Component(name)  {

}

Hist2D::~Hist2D() {
}

void Hist2D::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerStream("in_img", &in_img);
	registerStream("in_mask", &in_mask);
	registerStream("out_img", &out_img);
	registerStream("out_hist", &out_hist);
	registerStream("out_hist_img", &out_hist_img);
	
	// Register handlers
	registerHandler("onNewImage", boost::bind(&Hist2D::onNewImage, this));
	addDependency("onNewImage", &in_img);

}

bool Hist2D::onInit() {

	return true;
}

bool Hist2D::onFinish() {
	return true;
}

bool Hist2D::onStop() {
	return true;
}

bool Hist2D::onStart() {
	return true;
}

void Hist2D::onNewImage() {
	cv::Mat img = in_img.read();
	cv::Mat mask, hsv;
	if (!in_mask.empty()) mask = in_mask.read();
	
	
	cv::cvtColor(img, hsv, CV_BGR2HSV);

	// Quantize the hue to 30 levels
	// and the saturation to 32 levels
	int hbins = 15, sbins = 16;
	int histSize[] = {hbins, sbins};
	// hue varies from 0 to 179, see cvtColor
	float hranges[] = { 0, 180 };
	// saturation varies from 0 (black-gray-white) to
	// 255 (pure spectrum color)
	float sranges[] = { 0, 256 };
	const float* ranges[] = { hranges, sranges };
	cv::Mat hist;
	// we compute the histogram from the 0-th and 1-st channels
	int channels[] = {0, 1};

	cv::calcHist( &hsv, 1, channels, mask, // do not use mask
			 hist, 2, histSize, ranges,
			 true, // the histogram is uniform
			 false );
	double maxVal=0;
	cv::minMaxLoc(hist, 0, &maxVal, 0, 0);
	
	cv::Mat back;
	cv::calcBackProject( &hsv, 1, channels, hist, back, ranges, 255./maxVal, true); 
	
	int scale = 20;
	cv::Mat histImg = cv::Mat::zeros(sbins*scale, hbins*scale, CV_8UC3);

	for( int h = 0; h < hbins; h++ )
		for( int s = 0; s < sbins; s++ )
		{
			float binVal = hist.at<float>(h, s);
			int intensity = cvRound(binVal*255/maxVal);
			cv::rectangle( histImg, cv::Point(h*scale, s*scale),
						cv::Point( (h+1)*scale - 1, (s+1)*scale - 1),
						cv::Scalar::all(intensity),
						CV_FILLED );
		}

	out_img.write(back);
	out_hist.write(hist);
	out_hist_img.write(histImg);
}



} //: namespace Hist2D
} //: namespace Processors
