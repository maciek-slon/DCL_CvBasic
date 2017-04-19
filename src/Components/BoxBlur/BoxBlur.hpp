/*!
 * \file
 * \brief 
 * \author Maciej
 */

#ifndef BOXBLUR_HPP_
#define BOXBLUR_HPP_

#include "Base/Component_Aux.hpp"
#include "Base/Component.hpp"
#include "Base/DataStream.hpp"
#include "Base/Property.hpp"
#include "Base/EventHandler2.hpp"

#include <opencv2/opencv.hpp>


namespace Processors {
namespace BoxBlur {

/*!
 * \class BoxBlur
 * \brief BoxBlur processor class.
 *
 * 
 */
class BoxBlur: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	BoxBlur(const std::string & name = "BoxBlur");

	/*!
	 * Destructor
	 */
	virtual ~BoxBlur();

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

	/// Size of the kernel - height.
	Base::Property<int> kernel_height;

	
	// Handlers
	void onNewImage();

};

} //: namespace BoxBlur
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("BoxBlur", Processors::BoxBlur::BoxBlur)

#endif /* BOXBLUR_HPP_ */
