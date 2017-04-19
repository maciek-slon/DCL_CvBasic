/*!
 * \file
 * \brief 
 * \author Maciej,,,
 */

#ifndef CALCHIST_HPP_
#define CALCHIST_HPP_

#include "Base/Component_Aux.hpp"
#include "Base/Component.hpp"
#include "Base/DataStream.hpp"
#include "Base/Property.hpp"
#include "Base/EventHandler2.hpp"

#include <opencv2/opencv.hpp>

namespace Processors {
namespace CalcHist {

/*!
 * \class CalcHist
 * \brief CalcHist processor class.
 *
 * CalcHist processor.
 */
class CalcHist: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	CalcHist(const std::string & name = "CalcHist");

	/*!
	 * Destructor
	 */
	virtual ~CalcHist();

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

};

} //: namespace CalcHist
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("CalcHist", Processors::CalcHist::CalcHist)

#endif /* CALCHIST_HPP_ */
