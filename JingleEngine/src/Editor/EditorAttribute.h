#pragma once

#include "Core/Core.h"

#include "Core/Logging.h"

class EditorAttribute : public JingleScript::Attribute
{
	DEFINE_CLASS(EditorAttribute, JingleScript::Attribute);

private:
	JingleScript::String m_Title;

public:
	EditorAttribute() {}
	EditorAttribute(std::string title) { m_Title = title; }

	std::string GetTitle() const { return m_Title; }

};