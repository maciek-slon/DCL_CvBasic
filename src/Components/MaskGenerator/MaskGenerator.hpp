/*!
 * \file
 * \brief 
 * \author Maciej
 */

#ifndef MASKGENERATOR_HPP_
#define MASKGENERATOR_HPP_

#include "Base/Component_Aux.hpp"
#include "Base/Component.hpp"
#include "Base/DataStream.hpp"
#include "Base/Property.hpp"
#include "Base/EventHandler2.hpp"

#include "Types/Rectangle.hpp"

#include <opencv2/opencv.hpp>


namespace Processors {
namespace MaskGenerator {

/*!
 * \class MaskGenerator
 * \brief MaskGenerator processor class.
 *
 * 
 */
class MaskGenerator: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	MaskGenerator(const std::string & name = "MaskGenerator");

	/*!
	 * Destructor
	 */
	virtual ~MaskGenerator();

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

	// Output data streams
	Base::DataStreamOut<cv::Mat> out_mask;
	Base::DataStreamOut<Types::Rectangle> out_draw;

	// Handlers

	// Properties
	Base::Property<int> cx;
	Base::Property<int> cy;
	Base::Property<int> sx;
	Base::Property<int> sy;

	
	// Handlers
	void generate();

};

} //: namespace MaskGenerator
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("MaskGenerator", Processors::MaskGenerator::MaskGenerator)

#endif /* MASKGENERATOR_HPP_ */
