/*!
 * \file
 * \brief 
 * \author Maciej,,,
 */

#ifndef BRIGHTNESSCONTRAST_HPP_
#define BRIGHTNESSCONTRAST_HPP_

#include "Base/Component_Aux.hpp"
#include "Base/Component.hpp"
#include "Base/DataStream.hpp"
#include "Base/Property.hpp"
#include "Base/EventHandler2.hpp"

#include <opencv2/opencv.hpp>

namespace Processors {
namespace BrightnessContrast {

/*!
 * \class BrightnessContrast
 * \brief BrightnessContrast processor class.
 *
 * BrightnessContrast processor.
 */
class BrightnessContrast: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	BrightnessContrast(const std::string & name = "BrightnessContrast");

	/*!
	 * Destructor
	 */
	virtual ~BrightnessContrast();

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


	void onNewImage();
	
	Base::DataStreamIn<cv::Mat> in_img;
	Base::DataStreamOut<cv::Mat> out_img;
	
	Base::Property<int> prop_brightness;
	Base::Property<int> prop_contrast;
	Base::Property<bool> prop_equalize;

};

} //: namespace BrightnessContrast
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("BrightnessContrast", Processors::BrightnessContrast::BrightnessContrast)

#endif /* BRIGHTNESSCONTRAST_HPP_ */
