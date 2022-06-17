#pragma once

#include "Core/Core.h"

#include <functional>

#define EVENT_DEFAULT(type, value)												\
	static const EventType EventType##type = value;								\
	class type##EventArgs : public EventArgs									\
	{																			\
	public:																		\
		static EventType StaticTypeID() { return EventType##type; }				\
		virtual EventType GetType() const override { return StaticTypeID(); }	\
		virtual std::string GetName() const override { return #type; }

#define EVENT(type, base, value)												\
	static const EventType EventType##type = value;								\
	class type##EventArgs : public base##EventArgs								\
	{																			\
	public:																		\
		static EventType StaticTypeID() { return EventType##type; }				\
		virtual EventType GetType() const override { return StaticTypeID(); }	\
		virtual std::string GetName() const override { return #type; }

typedef unsigned int EventType;

class EventArgs : public BaseClass
{
public:
	virtual EventType GetType() const = 0;
	virtual std::string GetName() const = 0;

	virtual std::string ToString() const override
	{
		std::string str = BaseClass::ToString();

		str += ", ";
		str += "Name=" + GetName();

		return str;
	}

	template<typename T>
	const T& As() const
	{
		return *((const T*)this);
	}
};

typedef std::function<void(BaseClass*, EventArgs&)> Event;

template <typename T>
class EventHandler
{
private:
	std::vector<std::function<void(BaseClass*, T&)>> m_Events;

public:
	EventHandler& operator+=(const std::function<void(BaseClass*, T&)> &event)
	{
		m_Events.push_back(event);
		return *this;
	}

	void Invoke(BaseClass* sender, T& args)
	{
		for (auto event : m_Events)
		{
			event(sender, args);
		}
	}
};

EVENT_DEFAULT(Key, 1)
	int Key;
};

EVENT(KeyPress, Key, 2)
};

EVENT(KeyRelease, Key, 3)
};

EVENT_DEFAULT(Mouse, 4)
};

EVENT(MouseButton, Mouse, 5)
	int Button;
};

EVENT(MouseButtonPress, MouseButton, 6)
};

EVENT(MouseButtonRelease, MouseButton, 7)
};

EVENT(MouseScroll, Mouse, 8)
	int Direction;
};

EVENT(MouseMove, Mouse, 9)
	int X;
	int Y;
};

EVENT_DEFAULT(Window, 10)
};

EVENT(WindowResize, Window, 11)
	unsigned int Width;
	unsigned int Height;
};

EVENT(WindowClose, Window, 12)
};

EVENT_DEFAULT(ImGuiRender, 13)
	double DeltaTime;
};
