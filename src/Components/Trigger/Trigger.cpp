/*!
 * \file
 * \brief
 * \author Micha Laszkowski
 */

#include <memory>
#include <string>

#include "Trigger.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>


namespace Processors {
namespace Trigger {

Trigger::Trigger(const std::string & name) :
	Base::Component(name),
	prop_auto_trigger("auto_trigger", false),
	prop_triggers("triggers",std::string("trigger"))
{
	//registerProperty(prop_auto_trigger);
	registerProperty(prop_triggers);
}

Trigger::~Trigger() {
}

void Trigger::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	/*registerStream("out_trigger", &out_trigger);
	registerStream("in_stop_auto_trigger", &in_stop_auto_trigger);*/

	// Register handlers
	/*registerHandler("Send Trigger", boost::bind(&Trigger::onTriggerButtonPressed, this));

	registerHandler("onAutoTrigger", boost::bind(&Trigger::onAutoTrigger, this));
	addDependency("onAutoTrigger", NULL);

	registerHandler("onStopAutoTrigger", boost::bind(&Trigger::onStopAutoTrigger, this));
	addDependency("onStopAutoTrigger", &in_stop_auto_trigger);*/
	
	std::string t = prop_triggers;
	std::vector<std::string> trigs;
	boost::split(trigs, t, boost::is_any_of(","));
	for (int i = 0; i < trigs.size(); ++i) {
		registerHandler(std::string("Trigger - ") + trigs[i], boost::bind(&Trigger::onTriggerN, this, i));
		Base::DataStreamOut<Base::UnitType> * stream =
				new Base::DataStreamOut<Base::UnitType>;
		out_triggers.push_back(stream);
		registerStream(std::string("out_") + trigs[i], out_triggers[i]);
	}
}

bool Trigger::onInit() {
	// Reset flag.
	triggered_flag = false;
	return true;
}

bool Trigger::onFinish() {
	return true;
}

bool Trigger::onStop() {
	return true;
}

bool Trigger::onStart() {
	return true;
}


void Trigger::onTriggerButtonPressed() {
	CLOG(LDEBUG) << "Trigger::onTriggerButtonPressed";
	triggered_flag = true;
}

void Trigger::onTriggerN(int id) {
	CLOG(LDEBUG) << "Trigger::onTrigger" << id;
	Base::UnitType t;
	out_triggers[id]->write(t);
}


void Trigger::onAutoTrigger() {
	CLOG(LDEBUG) << "Trigger::onAutoTrigger";
	if (prop_auto_trigger || triggered_flag){
		triggered_flag = false;
		Base::UnitType t;
		out_trigger.write(t);
	}//: if
}


void Trigger::onStopAutoTrigger() {
	CLOG(LDEBUG) << "Trigger::onStopAutoTrigger";
	in_stop_auto_trigger.read();
	prop_auto_trigger = false;
	// TODO: DisCODe GUI fix is required to notice this. As a result turning autotrigger once again requires checking off and on.
}


} //: namespace Trigger
} //: namespace Processors
