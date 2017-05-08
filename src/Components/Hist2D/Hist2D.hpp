/*!
 * \file
 * \brief 
 * \author Maciej
 */

#ifndef HIST2D_HPP_
#define HIST2D_HPP_

#include "Base/Component_Aux.hpp"
#include "Base/Component.hpp"
#include "Base/DataStream.hpp"
#include "Base/Property.hpp"
#include "Base/EventHandler2.hpp"

#include <opencv2/opencv.hpp>


namespace Processors {
namespace Hist2D {

/*!
 * \class Hist2D
 * \brief Hist2D processor class.
 *
 * 
 */
class Hist2D: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	Hist2D(const std::string & name = "Hist2D");

	/*!
	 * Destructor
	 */
	virtual ~Hist2D();

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
	Base::DataStreamIn<cv::Mat, Base::DataStreamBuffer::Newest> in_mask;

	// Output data streams
	Base::DataStreamOut<cv::Mat> out_img;
	Base::DataStreamOut<cv::Mat> out_hist;
	Base::DataStreamOut<cv::Mat> out_hist_img;

	// Handlers

	// Properties

	
	// Handlers
	void onNewImage();

};

} //: namespace Hist2D
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("Hist2D", Processors::Hist2D::Hist2D)

#endif /* HIST2D_HPP_ */
