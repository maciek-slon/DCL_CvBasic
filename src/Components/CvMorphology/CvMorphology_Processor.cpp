/*!
 * \file CvMorphology_Processor.cpp
 * \brief
 * \author mstefanc
 * \date 2010-07-05
 */

#include <memory>
#include <string>

#include "CvMorphology_Processor.hpp"
#include "Common/Logger.hpp"

namespace Processors {
namespace CvMorphology {

CvMorphology_Processor::CvMorphology_Processor(const std::string & name) : Base::Component(name),
		iterations("iterations", 1, "range"),
        type("type", MORPH_OPEN, "combo"),
        element("element", std::string("RECT"), "combo"),
        elem_size("element_size", 1, "range")
{
	LOG(LTRACE) << "Hello CvMorphology_Processor\n";

	iterations.addConstraint("0");
	iterations.addConstraint("15");
	registerProperty(iterations);

	type.addConstraint("MORPH_ERODE");
	type.addConstraint("MORPH_DILATE");
	type.addConstraint("MORPH_OPEN");
	type.addConstraint("MORPH_CLOSE");
	type.addConstraint("MORPH_GRADIENT");
	type.addConstraint("MORPH_TOPHAT");
	type.addConstraint("MORPH_BLACKHAT");
	registerProperty(type);
	
	element.addConstraint("RECT");
	element.addConstraint("ELLIPSE");
	element.addConstraint("CROSS");
	registerProperty(element);
	
	elem_size.addConstraint("1");
	elem_size.addConstraint("10");
	registerProperty(elem_size);
}

CvMorphology_Processor::~CvMorphology_Processor()
{
	LOG(LTRACE) << "Good bye CvMorphology_Processor\n";
}

void CvMorphology_Processor::prepareInterface() {
	registerHandler("onNewImage", boost::bind(&CvMorphology_Processor::onNewImage, this));
	addDependency("onNewImage", &in_img);

	registerStream("in_img", &in_img);

	registerStream("out_img", &out_img);
}

bool CvMorphology_Processor::onInit()
{
	LOG(LTRACE) << "CvMorphology_Processor::initialize\n";

	return true;
}

bool CvMorphology_Processor::onFinish()
{
	LOG(LTRACE) << "CvMorphology_Processor::finish\n";

	return true;
}

bool CvMorphology_Processor::onStep()
{
	LOG(LTRACE) << "CvMorphology_Processor::step\n";
	return true;
}

bool CvMorphology_Processor::onStop()
{
	return true;
}

bool CvMorphology_Processor::onStart()
{
	return true;
}

void CvMorphology_Processor::onNewImage()
{
	LOG(LTRACE) << "CvMorphology_Processor::onNewImage\n";
	try {
        cv::Mat in = in_img.read();
        cv::Mat out = in.clone();
        int elem_type = MORPH_RECT;
        if (element == "RECT") elem_type = MORPH_RECT;
        else if (element == "ELLIPSE") elem_type = MORPH_ELLIPSE;
        else if (element == "CROSS") elem_type = MORPH_CROSS;
        cv::Mat el = getStructuringElement(elem_type, cv::Size(elem_size*2+1, elem_size*2+1));
        if (type == MORPH_ERODE)
			cv::erode(in, out, el, cv::Point(-1, -1), iterations);
		else if (type == MORPH_DILATE)
			cv::dilate(in, out, el, cv::Point(-1, -1), iterations);
		else
			cv::morphologyEx(in, out, type, el, Point(-1, -1), iterations);
        out_img.write(out);
	} catch (...) {
		LOG(LERROR) << "CvMorphology_Processor::onNewImage failed\n";
	}
}

}//: namespace CvMorphology
}//: namespace Processors
