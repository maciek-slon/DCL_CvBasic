/*!
 * \file
 * \brief 
 * \author Maciej
 */

#ifndef SCHARR_HPP_
#define SCHARR_HPP_

#include "Base/Component_Aux.hpp"
#include "Base/Component.hpp"
#include "Base/DataStream.hpp"
#include "Base/Property.hpp"
#include "Base/EventHandler2.hpp"

#include <opencv2/opencv.hpp>


namespace Processors {
namespace Scharr {

/*!
 * \class Scharr
 * \brief Scharr processor class.
 *
 * 
 */
class Scharr: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	Scharr(const std::string & name = "Scharr");

	/*!
	 * Destructor
	 */
	virtual ~Scharr();

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

	
	// Handlers
	void onNewImage();

};

} //: namespace Scharr
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("Scharr", Processors::Scharr::Scharr)

#endif /* SCHARR_HPP_ */
