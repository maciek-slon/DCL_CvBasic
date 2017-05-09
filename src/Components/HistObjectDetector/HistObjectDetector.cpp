/*!
 * \file
 * \brief
 * \author Maciej
 */

#include <memory>
#include <string>

#include "HistObjectDetector.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace HistObjectDetector {

HistObjectDetector::HistObjectDetector(const std::string & name) :
		Base::Component(name) , 
		object_name("object_name", std::string("object_1")) {
	registerProperty(object_name);
	clear_flag = false;

}

HistObjectDetector::~HistObjectDetector() {
}

void HistObjectDetector::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerStream("in_hist", &in_hist);
	registerStream("out_result", &out_result);
	// Register handlers
	registerHandler("onNewData", boost::bind(&HistObjectDetector::onNewData, this));
	addDependency("onNewData", &in_hist);
	registerHandler("updateObject", boost::bind(&HistObjectDetector::updateObject, this));
	registerHandler("clearObjects", boost::bind(&HistObjectDetector::clearObjects, this));

}

bool HistObjectDetector::onInit() {

	return true;
}

bool HistObjectDetector::onFinish() {
	return true;
}

bool HistObjectDetector::onStop() {
	return true;
}

bool HistObjectDetector::onStart() {
	return true;
}

void HistObjectDetector::onNewData() {
	cv::Mat hist = in_hist.read();
	
	if (clear_flag) {
		clear_flag = false;
		known_objects.clear();
	}
	
	if (!object_to_update.empty()) {
		known_objects[object_to_update] = hist;
		object_to_update = "";
	}
	
	Types::DetectionResult result;
	
	int compare_method = CV_COMP_CORREL;
	double best_val = 0;
	std::string best_obj = "";
	for (auto const & kv : known_objects) {
		double val = cv::compareHist(hist, kv.second, compare_method);
		if (val < 0) val = 0;
		//std::cout << kv.first << ": " << val << std::endl;
		result.add(kv.first, val);
		if (val > best_val) {
			best_val = val;
			best_obj = kv.first;
		}
	}
	out_result.write(result);
}

void HistObjectDetector::updateObject() {
	object_to_update = object_name;
}

void HistObjectDetector::clearObjects() {
	clear_flag = true;
}



} //: namespace HistObjectDetector
} //: namespace Processors
