#pragma once

#include "Core/Core.h"
#include "Core/Event.h"

#define DEFINE_MODULE(type)															\
	private:																		\
		static RegisterModule<type> RegisterModule_##type;							\
	public:																			\
		static std::string StaticName() { return #type; }							\
		virtual std::string GetName() override { return #type; }					\
		virtual std::vector<std::string> GetNames() override { return { #type }; }	\
	private:

#define DEFINE_NAMED_MODULE(type, names, ...)																\
	private:																								\
		static RegisterModule<type> RegisterModule_##type;													\
	public:																									\
		static std::string StaticName() { return #type; }													\
		virtual std::string GetName() override { return #type; }											\
		virtual std::vector<std::string> GetNames() override { return { #type, #names ##__VA_ARGS__ }; }	\
	private:

#define REGISTER_MODULE(type)							\
	RegisterModule<type> type::RegisterModule_##type

class Module : public BaseClass
{
public:
	virtual void OnPreInitialize();
	virtual void OnInitialize();

	virtual void OnDestroy();

	virtual void OnEvent(BaseClass* sender, const EventArgs& args);
	virtual void OnTick(double DeltaTime);

	virtual std::string GetName() = 0;
	virtual std::vector<std::string> GetNames() = 0;
};

#include "Core/ModuleManager.h"

template<typename T>
class RegisterModule
{
public:
	RegisterModule()
	{
		ModuleManager::Initialize();
		ModuleManager::Register<T>();
	}
};
