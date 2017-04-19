/*!
 * \file CvThreshold_Processor.cpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#include <memory>
#include <string>

#include "CvThreshold_Processor.hpp"
#include "Common/Logger.hpp"

namespace Processors {
namespace CvThreshold {

CvThreshold_Processor::CvThreshold_Processor(const std::string & name) : Base::Component(name),
		m_type("type", THRESH_BINARY, "combo"),
		m_thresh("thresh", 128, "range"),
		m_maxval("maxval", 255, "range"),
		m_otsu("Otsu", false),
		m_pass("pass_through", false)
{
	LOG(LTRACE) << "Hello CvThreshold_Processor\n";

	m_type.setToolTip("Thresholding type");
	m_type.addConstraint("BINARY");
	m_type.addConstraint("BINARY_INV");
	m_type.addConstraint("TRUNC");
	m_type.addConstraint("TOZERO");
	m_type.addConstraint("TOZERO_INV");

	m_thresh.setToolTip("Threshold level");
	m_thresh.addConstraint("0");
	m_thresh.addConstraint("255");

	m_maxval.setToolTip("Maximum value to use with BINARY and BINARY_INV thresholding types");
	m_maxval.addConstraint("0");
	m_maxval.addConstraint("255");

	// Register properties.
	registerProperty(m_type);
	registerProperty(m_thresh);
	registerProperty(m_maxval);
	registerProperty(m_otsu);
	registerProperty(m_pass);
}

CvThreshold_Processor::~CvThreshold_Processor()
{
	LOG(LTRACE) << "Good bye CvThreshold_Processor\n";
}


void CvThreshold_Processor::prepareInterface() {
	CLOG(LTRACE) << "CvThreshold_Processor::prepareInterface\n";

	registerHandler("onNewImage", boost::bind(&CvThreshold_Processor::onNewImage, this));

	registerStream("in_img", &in_img);

	registerStream("out_img", &out_img);

	addDependency("onNewImage", &in_img);
}

bool CvThreshold_Processor::onInit()
{
	LOG(LTRACE) << "CvThreshold_Processor::initialize\n";

	return true;
}

bool CvThreshold_Processor::onFinish()
{
	LOG(LTRACE) << "CvThreshold_Processor::finish\n";

	return true;
}

bool CvThreshold_Processor::onStop()
{
	return true;
}

bool CvThreshold_Processor::onStart()
{
	return true;
}

void CvThreshold_Processor::onNewImage()
{
	LOG(LNOTICE) << "CvThreshold_Processor::onNewImage\n";
	try {
		double thr = m_thresh;
		cv::Mat img = in_img.read();
		cv::Mat out;
		if (m_pass)
			out = img.clone();
		else if (m_otsu)
			thr = cv::threshold(img, out, m_thresh, m_maxval, m_type + THRESH_OTSU);
		else
			cv::threshold(img, out, m_thresh, m_maxval, m_type);
			
		CLOG(LTRACE) << "Threshold " << thr;
		out_img.write(out);
	} catch (...) {
		LOG(LERROR) << "CvThreshold::onNewImage failed\n";
	}
}

}//: namespace CvThreshold
} //: namespace Processors
