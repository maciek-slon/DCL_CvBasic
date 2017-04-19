/*!
 * \file
 * \brief 
 * \author Maciej
 */

#ifndef SHAPEDETECTOR_HPP_
#define SHAPEDETECTOR_HPP_

#include "Base/Component_Aux.hpp"
#include "Base/Component.hpp"
#include "Base/DataStream.hpp"
#include "Base/Property.hpp"
#include "Base/EventHandler2.hpp"

#include <opencv2/opencv.hpp>


namespace Processors {
namespace ShapeDetector {

/*!
 * \class ShapeDetector
 * \brief ShapeDetector processor class.
 *
 * 
 */
class ShapeDetector: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	ShapeDetector(const std::string & name = "ShapeDetector");

	/*!
	 * Destructor
	 */
	virtual ~ShapeDetector();

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
	Base::DataStreamIn<std::vector<std::vector<cv::Point> > > in_contours;
	Base::DataStreamIn<cv::Mat> in_img;

	// Output data streams
	Base::DataStreamOut<cv::Mat> out_img;

	// Handlers

	// Properties
	Base::Property<std::string> shape1_name;
	Base::Property<double> shape1_compactness;
	Base::Property<std::string> shape2_name;
	Base::Property<double> shape2_compactness;

	
	// Handlers
	void onNewData();

};

} //: namespace ShapeDetector
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("ShapeDetector", Processors::ShapeDetector::ShapeDetector)

#endif /* SHAPEDETECTOR_HPP_ */
