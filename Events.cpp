/*
 * Events.cpp
 *
 *  Created on: 5.5.2012
 *      Author: Morphe
 */

#include "Events.h"

EventGenBase::~EventGenBase() {
	/*
	 * Do not remove this declaration. We want EventGenBase to
	 * be abstract, but C++ won't allow us pure-virtual destructor.
	 * So this is to bypass it - there IS destructor for linker and
	 * EventGenBase is still abstract
	 */
}

EventCallerBase::EventCallerBase() {
}

EventCallerBase& EventCallerBase::operator =(EventGenBase* event) {
	event_ = std::shared_ptr<EventGenBase>(event);
	return *this;
}

EventCallerBase::~EventCallerBase() {
}
