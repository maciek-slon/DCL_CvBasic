/*!
 * \file
 * \brief 
 * \author Maciej Stefanczyk
 */

#ifndef FINDHOMOGRAPHY_HPP_
#define FINDHOMOGRAPHY_HPP_

#include "Base/Component_Aux.hpp"
#include "Base/Component.hpp"
#include "Base/DataStream.hpp"
#include "Base/Property.hpp"
#include "Base/EventHandler2.hpp"

#include "Types/Features.hpp"

#include <opencv2/opencv.hpp>


namespace Processors {
namespace FindHomography {

/*!
 * \class FindHomography
 * \brief FindHomography processor class.
 *
 * 
 */
class FindHomography: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	FindHomography(const std::string & name = "FindHomography");

	/*!
	 * Destructor
	 */
	virtual ~FindHomography();

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
	Base::DataStreamIn<std::vector<cv::DMatch> > in_matches;
	Base::DataStreamIn<Types::Features> in_features0;
	Base::DataStreamIn<Types::Features> in_features1;

	// Output data streams
	Base::DataStreamOut<cv::Mat> out_homography;

	// Handlers

	// Properties

	
	// Handlers
	void calculate();

};

} //: namespace FindHomography
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("FindHomography", Processors::FindHomography::FindHomography)

#endif /* FINDHOMOGRAPHY_HPP_ */
