#pragma once

#include "Property/BaseProperty.h"

//! TODO: If/when interfaces are implemented in JingleScript, this will be one to expose 'Editor_OnRender` and `Editor_OnPropertyChanged`
class ObjectProperty : public BaseProperty
{
private:
	std::map<std::string, BaseProperty*> m_Properties;

public:
	ObjectProperty() : BaseProperty()
	{
		JS_TRACE(Tracers::Property);
	}
	
	ObjectProperty(BaseProperty* parent, JingleScript::Type* type, Property* varProperty) : BaseProperty(parent, type, varProperty)
	{
		JS_TRACE(Tracers::Property);
	}

	//! 'data' must be dynamically cast to Object
	virtual bool OnSerialize(Config* cfg, void*& data) override;
	virtual bool OnDeserialize(Config* cfg, void*& data) override;

	virtual void Editor_OnRender(void*& data) override;

	bool Serialize(Config* cfg);
	bool Deserialize(Config* cfg);
	void Editor_Render();
};
