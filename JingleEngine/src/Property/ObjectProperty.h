#pragma once

#include "Property/BaseProperty.h"

class ArrayProperty;

//! TODO: If/when interfaces are implemented in JingleScript, this will be the one to expose 'Editor_OnRender` and `Editor_OnPropertyChanged`
class ObjectProperty : public BaseProperty
{
	friend ArrayProperty;

private:
	std::map<std::string, BaseProperty*> m_Properties;

	ConfigAsset* m_BaseConfig = nullptr;
	bool m_HasBase = false;
	std::string m_Name;

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

	ConfigAsset* GetBaseConfig() const;
	void SetBaseConfig(ConfigAsset* asset);

	bool HasBase() const;
	void SetHasBase(bool has);

	const std::string& GetName() const;
	void SetName(const std::string& name);

	Config* Serialize();
	bool Deserialize(Config* cfg);
	void Editor_Render();
	
};
