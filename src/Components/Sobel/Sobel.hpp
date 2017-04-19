/*!
 * \file
 * \brief 
 * \author Maciej
 */

#ifndef SOBEL_HPP_
#define SOBEL_HPP_

#include "Base/Component_Aux.hpp"
#include "Base/Component.hpp"
#include "Base/DataStream.hpp"
#include "Base/Property.hpp"
#include "Base/EventHandler2.hpp"

#include <opencv2/opencv.hpp>


namespace Processors {
namespace Sobel {

/*!
 * \class Sobel
 * \brief Sobel processor class.
 *
 * 
 */
class Sobel: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	Sobel(const std::string & name = "Sobel");

	/*!
	 * Destructor
	 */
	virtual ~Sobel();

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
	Base::Property<int> kernel_size;

	
	// Handlers
	void onNewImage();

};

} //: namespace Sobel
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("Sobel", Processors::Sobel::Sobel)

#endif /* SOBEL_HPP_ */
