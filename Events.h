/*
 * Events.h
 *
 *  Created on: 4.5.2012
 *      Author: Morphe
 */

#ifndef EVENTS_H_
#define EVENTS_H_
#include <memory>

class EventGenBase {
public:
	virtual ~EventGenBase() = 0;
};

template<class _SenderT> class EventBase : public EventGenBase {
public:
	EventBase() {}
	~EventBase() {}
	virtual int operator()(_SenderT &sentBy) {return 0;}
};

template<class _SenderT, class _ParentType> class Event : public EventBase<_SenderT> {
	typedef int (_ParentType::*_FuncPtr_t)(_SenderT &o);
	_ParentType &parent;
	_FuncPtr_t f_;
public:
	Event(_ParentType &x, _FuncPtr_t f) : parent(x), f_(f) {};
	int operator() (_SenderT &sender) {
		return (parent.*f_)(sender);
	}
};

class EventCallerBase {
protected:
	std::shared_ptr<EventGenBase> event_;
public:
	EventCallerBase();
	~EventCallerBase();
	EventCallerBase& operator =(EventGenBase *event);
};

template<class _SenderT> class EventCaller : protected EventCallerBase {
public:
	EventCaller() {
		 event_ = std::shared_ptr<EventGenBase>(new EventBase<_SenderT>());
	}
	int operator() (_SenderT &sentBy) {
		return (*std::dynamic_pointer_cast<EventBase<_SenderT>>(event_))(sentBy);
	}
	using EventCallerBase::operator=;
};

template<class _SenderT, class _ParentT> EventBase<_SenderT> * NewEvent
	(_ParentT &parent, int (_ParentT::*f)(_SenderT &w)) {
	return new Event<_SenderT, _ParentT> (parent, f);
}

#endif /* EVENTS_H_ */
