#pragma once

#include "Core/Core.h"

#include "Core/Logging.h"
#include "Core/Module.h"

//! TODO: Don't have panels as Modules
class EditorPanel : public Module
{
	DEFINE_VIRTUAL_MODULE(EditorPanel, Module);

private:
	bool m_IsOpen;

public:
	EditorPanel() {}

	virtual void OnRender(double DeltaTime) = 0;

	virtual void OnEvent(BaseClass* sender, const EventArgs& args) override;

	virtual std::string GetTitle() const;
	
	void Open();
	void Close();
	bool IsOpen() const;

};
