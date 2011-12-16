#pragma once

#include <list>

namespace Monocle
{
	template <class Handler> class EventEmitter;

	template <class Handler>
	class EventHandler : public Handler
	{
	public:
		typedef EventEmitter<Handler> EmitterType;
		typedef std::list< EmitterType *> EmitterList;
		EmitterList emitters;

		EventHandler::EventHandler()
		{

		}

		virtual void EventHandler::AddingTo(EmitterType *emitter)
		{
			emitters.push_back(emitter);
		}

		virtual EventHandler::~EventHandler()
		{
			for(EmitterList::iterator i = emitters.begin(); i != emitters.end(); i++)
			{
				(*i)->RemoveHandler(this);
			}
		}
	};

	template <class Handler>
	class EventEmitter
	{
	public:
		typedef EventHandler<Handler> HandlerType;
		typedef std::list< HandlerType* > HandlerList;
		void AddHandler(HandlerType* handler)
		{
			handlers.push_back(handler);
			handler->AddingTo(this);
		}

		void RemoveHandler(HandlerType* handler)
		{
			handlers.remove(handler);
		}

		template <typename ArgType, void(Handler::*MemberFunc)(ArgType)>
		void Emit(ArgType arg)
		{
			for(HandlerList::iterator i = handlers.begin(); i != handlers.end(); i++)
			{
				((*i)->*MemberFunc)(arg);
			}
		}
		
	private:
		HandlerList handlers;
	};
}