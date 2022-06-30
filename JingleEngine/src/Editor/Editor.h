#pragma once

#include "Core/Core.h"

#include "Math/Vector3.h"

#include <imgui.h>

template<typename T>
class ScopedIncrement
{
	T* m_Value;

public:
	ScopedIncrement(T& value)
	{
		m_Value = &value;

		++(*m_Value);
	}

	~ScopedIncrement()
	{
		--(*m_Value);
	}
};

namespace Editor
{
	struct PropertyRenderContext
	{
		uint64_t Depth = 0;

		float GetDepthIndent() const { return Depth * 4.0f; }
	};

	extern PropertyRenderContext Context;

	void ResetContext();

	void Render_CellHeader(std::string name, bool isHeader = false, bool isRightIndent = false);

	bool Render_CellInputText(std::string& value);

	bool Render_Vector3(Vector3& value);
};
