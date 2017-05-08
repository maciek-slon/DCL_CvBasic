/*!
 * \file Text.hpp
 * \brief File containing Text type
 */

#ifndef DETECTION_RESULT_HPP_
#define DETECTION_RESULT_HPP_

#include "Drawable.hpp"

#include <opencv2/opencv.hpp>

#include <map>
#include <sstream>

namespace Types {

class DetectionResult : public Drawable {
public:
	DetectionResult() {};

	DetectionResult(float x, float y) {
		m_pos = cv::Point2f(x, y);
	}

	DetectionResult(const DetectionResult & rhs) : m_pos(rhs.m_pos), m_results(rhs.m_results) {};

	virtual ~DetectionResult() {}

	virtual void draw(cv::Mat & image, cv::Scalar color, int offsetX = 0, int offsetY = 0) {
		int max_w = 0;
		int pos_x = 0;
		int pos_y = 15;
		int baseline;
		for(auto const & kv : m_results) {
			cv::Size ts = cv::getTextSize(kv.first, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseline);
			cv::putText(image, kv.first, cv::Point2f(pos_x+1, pos_y+1), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar::all(0));
			cv::putText(image, kv.first, cv::Point2f(pos_x, pos_y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar::all(255));
			pos_y += 20;
			if (ts.width > max_w) max_w = ts.width;
		}
		
		pos_y = 15;
		
		cv::Size ts = cv::getTextSize(std::to_string(0.99), cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseline);
		for(auto const & kv : m_results) {
			cv::putText(image, std::to_string(kv.second), cv::Point2f(pos_x+max_w+10+1, pos_y+1), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar::all(0));
			cv::putText(image, std::to_string(kv.second), cv::Point2f(pos_x+max_w+10, pos_y), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar::all(255));
			cv::rectangle(image, cv::Rect(pos_x + max_w + 10 + ts.width + 4, pos_y-12, kv.second*100, 14), cv::Scalar::all(255), CV_FILLED);
			cv::rectangle(image, cv::Rect(pos_x + max_w + 10 + ts.width + 4, pos_y-12, 100, 14), cv::Scalar::all(255), 1);
			pos_y += 20;
		}
		
		
	}

	virtual Drawable * clone() {
		return new DetectionResult(*this);
	}
	
	void add(const std::string & name, double val) {
		m_results[name] = val;
	}

private:
	cv::Point2f m_pos;
	
	std::map<std::string, double> m_results;
};

} //: namespace Types

#endif /* DETECTION_RESULT_HPP_ */
