/*!
 * \file
 * \brief 
 * \author Maciej
 */

#ifndef HISTOBJECTDETECTOR_HPP_
#define HISTOBJECTDETECTOR_HPP_

#include "Base/Component_Aux.hpp"
#include "Base/Component.hpp"
#include "Base/DataStream.hpp"
#include "Base/Property.hpp"
#include "Base/EventHandler2.hpp"

#include "Types/DetectionResult.hpp"

#include <opencv2/opencv.hpp>


namespace Processors {
namespace HistObjectDetector {

/*!
 * \class HistObjectDetector
 * \brief HistObjectDetector processor class.
 *
 * 
 */
class HistObjectDetector: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	HistObjectDetector(const std::string & name = "HistObjectDetector");

	/*!
	 * Destructor
	 */
	virtual ~HistObjectDetector();

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
	Base::DataStreamIn<cv::Mat> in_hist;

	// Output data streams
	Base::DataStreamOut<Types::DetectionResult> out_result;

	// Handlers

	// Properties
	Base::Property<std::string> object_name;

	
	// Handlers
	void onNewData();
	void updateObject();


	std::string object_to_update;
	
	std::map<std::string, cv::Mat> known_objects;
};

} //: namespace HistObjectDetector
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("HistObjectDetector", Processors::HistObjectDetector::HistObjectDetector)

#endif /* HISTOBJECTDETECTOR_HPP_ */
