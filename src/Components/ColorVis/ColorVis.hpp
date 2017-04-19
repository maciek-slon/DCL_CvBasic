/*!
 * \file
 * \brief 
 * \author Maciej,,,
 */

#ifndef COLORVIS_HPP_
#define COLORVIS_HPP_

#include "Base/Component_Aux.hpp"
#include "Base/Component.hpp"
#include "Base/DataStream.hpp"
#include "Base/Property.hpp"
#include "Base/EventHandler2.hpp"

#include <opencv2/opencv.hpp>

namespace Processors {
namespace ColorVis {

/*!
 * \class ColorVis
 * \brief ColorVis processor class.
 *
 * ColorVis processor.
 */
class ColorVis: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	ColorVis(const std::string & name = "ColorVis");

	/*!
	 * Destructor
	 */
	virtual ~ColorVis();

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
	Base::DataStreamOut<cv::Mat> out_plane0, out_plane1, out_plane2;
	
	Base::Property<std::string> m_type;
	

};

} //: namespace ColorVis
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("ColorVis", Processors::ColorVis::ColorVis)

#endif /* COLORVIS_HPP_ */
