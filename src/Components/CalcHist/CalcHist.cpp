/*!
 * \file
 * \brief
 * \author Maciej,,,
 */

#include <memory>
#include <string>

#include "CalcHist.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace CalcHist {

CalcHist::CalcHist(const std::string & name) :
		Base::Component(name)  {

}

CalcHist::~CalcHist() {
}

void CalcHist::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerHandler("onNewImage", boost::bind(&CalcHist::onNewImage, this));
	addDependency("onNewImage", &in_img);
	
	registerStream("in_img", &in_img);
}

bool CalcHist::onInit() {

	return true;
}

bool CalcHist::onFinish() {
	return true;
}

bool CalcHist::onStop() {
	return true;
}

bool CalcHist::onStart() {
	return true;
}

void CalcHist::onNewImage() {
	CLOG(LTRACE) << "CalcHist::onNewImage";
	try {
		cv::Mat in = in_img.read();
		cv::Mat gray;
		if (in.channels() == 3)
			cv::cvtColor(in, gray, cv::COLOR_BGR2GRAY);
		else
			gray = in;

		/// Establish the number of bins
		int histSize = 256;

		/// Set the ranges ( for B,G,R) )
		float range[] = { 0, 256 } ;
		const float* histRange = { range };

		bool uniform = true; bool accumulate = false;

	CLOG(LTRACE) << "CalcHist::onNewImage";
		cv::Mat hist;

		/// Compute the histograms:
		cv::calcHist( &gray, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange, uniform, accumulate );

	CLOG(LTRACE) << "CalcHist::onNewImage";
		// Draw the histogram
		int hist_w = 256; int hist_h = 200;
		int bin_w = cvRound( (double) hist_w/histSize );

		cv::Mat histImage( hist_h, hist_w, CV_8UC3, cv::Scalar( 0,0,0) );

	CLOG(LTRACE) << "CalcHist::onNewImage";
		/// Normalize the result to [ 0, histImage.rows ]
		cv::normalize(hist, hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat() );

	CLOG(LTRACE) << "CalcHist::onNewImage";
		/// Draw for each channel
		/*for( int i = 1; i < histSize; i++ )
		{
			cv::line( histImage, cv::Point( bin_w*(i-1), hist_h - cvRound(hist.at<float>(i-1)) ) ,
								 cv::Point( bin_w*(i), hist_h - cvRound(hist.at<float>(i)) ),
								 cv::Scalar( 255, 255, 255), 2, 8, 0  );
		}*/

		float hist_sum = 0;
		for( int i = 0; i < histSize; i++ )
		{
			cv::line( histImage, cv::Point( i, hist_h - cvRound(hist.at<float>(i)) ) ,
								 cv::Point( i, hist_h ),
								 cv::Scalar( 255, 255, 255), 1, 8, 0  );
			hist_sum = hist_sum + hist.at<float>(i);
		}
		
		float hist_cum = hist.at<float>(0), hist_old;
		for( int i = 1; i < histSize; i++ )
		{
			hist_old = hist_cum;
			hist_cum = hist_cum + hist.at<float>(i);
			cv::line( histImage, cv::Point( i, hist_h - cvRound(hist_old * hist_h / hist_sum) ), 
			                     cv::Point( i, hist_h - cvRound(hist_cum * hist_h / hist_sum) ), 
			                     cv::Scalar( 255, 80, 80), 1, 8, 0 );
		}
		

	CLOG(LTRACE) << "CalcHist::onNewImage";
		/// Display
		cv::imshow("calcHist Demo", histImage );
		cv::waitKey(10);

	CLOG(LTRACE) << "CalcHist::onNewImage done";
	} catch (...) {
		LOG(LERROR) << "CvCanny::onNewImage failed\n";
	}
}

} //: namespace CalcHist
} //: namespace Processors
