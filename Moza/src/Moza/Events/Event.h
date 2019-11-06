#pragma once

#include "mzpch.h"
#include "Moza/Core.h"

namespace Moza {

	// Events in Hazel are currently blocking, meaning when an event occurs it
	// immediately gets dispatched and must be dealt with right then an there.
	// For the future, a better strategy might be to buffer events in an event
	// bus and process them during the "event" part of the update stage.

	enum class MOZA_API EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

//allows the implementing classes of Event to not need to rewrite all this code
//need to have an virtual GetEventType because be able to check polymorphic Event types ie. Event*
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class MOZA_API Event
	{
	public:
		bool Handled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	};

	//A way to dispatch events based on type
	//Because we get events at Event& (Event handlers), we create an instance of this class to handle this
	class MOZA_API EventDispatcher
	{
		template<typename T>
		//implement a function that takes in a Event and returns a bool, implemented somewhere in the Engine
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			: m_Event(event) {}

		//if the type of event giveb from EventFn matches the function, run it
		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};

	//exists for the logging library
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}