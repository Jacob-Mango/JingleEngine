#pragma once

#ifdef JE_EDITOR

#include "Core/Core.h"

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

#endif
