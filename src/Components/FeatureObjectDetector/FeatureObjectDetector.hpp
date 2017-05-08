/*!
 * \file
 * \brief 
 * \author Maciej Stefanczyk
 */

#ifndef FEATUREOBJECTDETECTOR_HPP_
#define FEATUREOBJECTDETECTOR_HPP_

#include "Base/Component_Aux.hpp"
#include "Base/Component.hpp"
#include "Base/DataStream.hpp"
#include "Base/Property.hpp"
#include "Base/EventHandler2.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>

#include <map>
#include <mutex>

namespace Processors {
namespace FeatureObjectDetector {

/*!
 * \class FeatureObjectDetector
 * \brief FeatureObjectDetector processor class.
 *
 * 
 */
class FeatureObjectDetector: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	FeatureObjectDetector(const std::string & name = "FeatureObjectDetector");

	/*!
	 * Destructor
	 */
	virtual ~FeatureObjectDetector();

	/*!
	 * Prepare components interface (register streams and handlers).
	 * At this point, all properties are already initialized and loaded to 
	 * values set in config file.
	 */
	void prepareInterface();

protected:

	/*!
	 * Connects source to given device.
	 */
	bool onInit();

	/*!
	 * Disconnect source from device, closes streams, etc.
	 */
	bool onFinish();

	/*!
	 * Start component
	 */
	bool onStart();

	/*!
	 * Stop component
	 */
	bool onStop();


	// Input data streams
	Base::DataStreamIn<cv::Mat, Base::DataStreamBuffer::Newest> in_img;
	Base::DataStreamIn<cv::Mat, Base::DataStreamBuffer::Newest> in_roi;

	// Output data streams
	Base::DataStreamOut<cv::Mat> out_img;

	// Handlers

	// Properties
	Base::Property<std::string> object_name;
	Base::Property<int> threshold;

	
	// Handlers
	void onNewImage();
	void clearObjects();
	void updateObject();

	std::map<std::string, cv::Mat> known_objects;
	
	
	std::mutex map_mutex;
};

} //: namespace FeatureObjectDetector
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("FeatureObjectDetector", Processors::FeatureObjectDetector::FeatureObjectDetector)

#endif /* FEATUREOBJECTDETECTOR_HPP_ */
