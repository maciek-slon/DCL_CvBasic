/*!
 * \file
 * \brief 
 * \author Maciej
 */

#ifndef MEDIAN_HPP_
#define MEDIAN_HPP_

#include "Base/Component_Aux.hpp"
#include "Base/Component.hpp"
#include "Base/DataStream.hpp"
#include "Base/Property.hpp"
#include "Base/EventHandler2.hpp"

#include <opencv2/opencv.hpp>


namespace Processors {
namespace Median {

/*!
 * \class Median
 * \brief Median processor class.
 *
 * 
 */
class Median: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	Median(const std::string & name = "Median");

	/*!
	 * Destructor
	 */
	virtual ~Median();

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
	Base::DataStreamIn<cv::Mat> in_img;

	// Output data streams
	Base::DataStreamOut<cv::Mat> out_img;

	// Handlers

	// Properties
	/// Size of the kernel - width.
	Base::Property<int> kernel_width;


	
	// Handlers
	void onNewImage();

};

} //: namespace Median
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("Median", Processors::Median::Median)

#endif /* MEDIAN_HPP_ */
