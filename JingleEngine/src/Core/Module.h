#pragma once

#include "Core/Core.h"
#include "Core/Event.h"

#define MODULE(type)												\
	public:															\
		static std::string StaticName() { return #type; }			\
		virtual std::string GetName() override { return #type; }	\
	private:

class Module : public BaseClass
{
public:
	virtual void OnCreate();
	virtual void OnDestroy();

	virtual void OnEvent(BaseClass* sender, const EventArgs& args);
	virtual void OnTick(double DeltaTime);

	virtual std::string GetName() = 0;
};
