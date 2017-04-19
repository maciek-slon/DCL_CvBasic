/*!
 * \file
 * \brief 
 * \author Maciej
 */

#ifndef BILATERALFILTER_HPP_
#define BILATERALFILTER_HPP_

#include "Base/Component_Aux.hpp"
#include "Base/Component.hpp"
#include "Base/DataStream.hpp"
#include "Base/Property.hpp"
#include "Base/EventHandler2.hpp"

#include <opencv2/opencv.hpp>


namespace Processors {
namespace BilateralFilter {

/*!
 * \class BilateralFilter
 * \brief BilateralFilter processor class.
 *
 * 
 */
class BilateralFilter: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	BilateralFilter(const std::string & name = "BilateralFilter");

	/*!
	 * Destructor
	 */
	virtual ~BilateralFilter();

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
	Base::Property<int> prop_d;
	Base::Property<int> prop_sigma_c;
	Base::Property<int> prop_sigma_s;
	
	// Handlers
	void onNewImage();

};

} //: namespace BilateralFilter
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("BilateralFilter", Processors::BilateralFilter::BilateralFilter)

#endif /* BILATERALFILTER_HPP_ */
