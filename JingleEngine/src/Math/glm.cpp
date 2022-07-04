#include "Math/glm.h"

using namespace glm;

dvec1 js_dvec1::FromString(std::string value)
{
	if (!value.empty())
	{
		std::istringstream ss(value);

		dvec1 result;
		if ((ss >> result[0]))
		{
			return result;
		}
	}

	return dvec1(0);
}

std::string js_dvec1::ToString() const
{
	return fmt::format("{}", (*this)[0]);
}

BEGIN_GLM_LINK(dvec1);
END_STRUCTURE_LINK();

vec1 js_vec1::FromString(std::string value)
{
	if (!value.empty())
	{
		std::istringstream ss(value);

		vec1 result;
		if ((ss >> result[0]))
		{
			return result;
		}
	}

	return vec1(0);
}

std::string js_vec1::ToString() const
{
	return fmt::format("{}", (*this)[0]);
}

BEGIN_GLM_LINK(vec1);
END_STRUCTURE_LINK();

dvec2 js_dvec2::FromString(std::string value)
{
	if (!value.empty())
	{
		std::istringstream ss(value);

		dvec2 result;
		if ((ss >> result[0]) && (ss >> result[1]))
		{
			return result;
		}
	}

	return dvec2(0);
}

std::string js_dvec2::ToString() const
{
	return fmt::format("{} {}", (*this)[0], (*this)[1]);
}

BEGIN_GLM_LINK(dvec2);
END_STRUCTURE_LINK();

vec2 js_vec2::FromString(std::string value)
{
	if (!value.empty())
	{
		std::istringstream ss(value);

		vec2 result;
		if ((ss >> result[0]) && (ss >> result[1]))
		{
			return result;
		}
	}

	return vec2(0);
}

std::string js_vec2::ToString() const
{
	return fmt::format("{} {}", (*this)[0], (*this)[1]);
}

BEGIN_GLM_LINK(vec2);
END_STRUCTURE_LINK();

dvec3 js_dvec3::FromString(std::string value)
{
	if (!value.empty())
	{
		std::istringstream ss(value);

		dvec3 result;
		if ((ss >> result[0]) && (ss >> result[1]) && (ss >> result[2]))
		{
			return result;
		}
	}

	return dvec3(0);
}

std::string js_dvec3::ToString() const
{
	return fmt::format("{} {} {}", (*this)[0], (*this)[1], (*this)[2]);
}

BEGIN_GLM_LINK(dvec3);
END_STRUCTURE_LINK();

vec3 js_vec3::FromString(std::string value)
{
	if (!value.empty())
	{
		std::istringstream ss(value);

		vec3 result;
		if ((ss >> result[0]) && (ss >> result[1]) && (ss >> result[2]))
		{
			return result;
		}
	}

	return vec3(0);
}

std::string js_vec3::ToString() const
{
	return fmt::format("{} {} {}", (*this)[0], (*this)[1], (*this)[2]);
}

BEGIN_GLM_LINK(vec3);
END_STRUCTURE_LINK();

dvec4 js_dvec4::FromString(std::string value)
{
	if (!value.empty())
	{
		std::istringstream ss(value);

		dvec4 result;
		if ((ss >> result[0]) && (ss >> result[1]) && (ss >> result[2]) && (ss >> result[3]))
		{
			return result;
		}
	}

	return dvec4(0);
}

std::string js_dvec4::ToString() const
{
	return fmt::format("{} {} {} {}", (*this)[0], (*this)[1], (*this)[2], (*this)[3]);
}

BEGIN_GLM_LINK(dvec4);
END_STRUCTURE_LINK();

vec4 js_vec4::FromString(std::string value)
{
	if (!value.empty())
	{
		std::istringstream ss(value);

		vec4 result;
		if ((ss >> result[0]) && (ss >> result[1]) && (ss >> result[2]) && (ss >> result[3]))
		{
			return result;
		}
	}

	return vec4(0);
}

std::string js_vec4::ToString() const
{
	return fmt::format("{} {} {} {}", (*this)[0], (*this)[1], (*this)[2], (*this)[3]);
}

BEGIN_GLM_LINK(vec4);
END_STRUCTURE_LINK();

dmat3 js_dmat3::FromString(std::string value)
{
	if (!value.empty())
	{
		std::istringstream ss(value);

		dmat3 result;
		if ((ss >> result[0][0]) && (ss >> result[0][1]) && (ss >> result[0][2]) && 
			(ss >> result[1][0]) && (ss >> result[1][1]) && (ss >> result[1][2]) && 
			(ss >> result[2][0]) && (ss >> result[2][1]) && (ss >> result[2][2]))
		{
			return result;
		}
	}

	return dmat3(0);
}

std::string js_dmat3::ToString() const
{
	return fmt::format("{} {} {}", (*this)[0], (*this)[1], (*this)[2]);
}

BEGIN_GLM_LINK(dmat3);
END_STRUCTURE_LINK();

mat3 js_mat3::FromString(std::string value)
{
	if (!value.empty())
	{
		std::istringstream ss(value);

		mat3 result;
		if ((ss >> result[0][0]) && (ss >> result[0][1]) && (ss >> result[0][2]) && 
			(ss >> result[1][0]) && (ss >> result[1][1]) && (ss >> result[1][2]) && 
			(ss >> result[2][0]) && (ss >> result[2][1]) && (ss >> result[2][2]))
		{
			return result;
		}
	}

	return mat3(0);
}

std::string js_mat3::ToString() const
{
	return fmt::format("{} {} {}", (*this)[0], (*this)[1], (*this)[2]);
}

BEGIN_GLM_LINK(mat3);
END_STRUCTURE_LINK();

dmat4 js_dmat4::FromString(std::string value)
{
	if (!value.empty())
	{
		std::istringstream ss(value);

		dmat4 result;
		if ((ss >> result[0][0]) && (ss >> result[0][1]) && (ss >> result[0][2]) && (ss >> result[0][3]) && 
			(ss >> result[1][0]) && (ss >> result[1][1]) && (ss >> result[1][2]) && (ss >> result[1][3]) && 
			(ss >> result[2][0]) && (ss >> result[2][1]) && (ss >> result[2][2]) && (ss >> result[2][3]) && 
			(ss >> result[3][0]) && (ss >> result[3][1]) && (ss >> result[3][2]) && (ss >> result[3][3]))
		{
			return result;
		}
	}

	return dmat4(0);
}

std::string js_dmat4::ToString() const
{
	return fmt::format("{} {} {} {}", (*this)[0], (*this)[1], (*this)[2], (*this)[3]);
}

BEGIN_GLM_LINK(dmat4);
END_STRUCTURE_LINK();

mat4 js_mat4::FromString(std::string value)
{
	if (!value.empty())
	{
		std::istringstream ss(value);

		mat4 result;
		if ((ss >> result[0][0]) && (ss >> result[0][1]) && (ss >> result[0][2]) && (ss >> result[0][3]) && 
			(ss >> result[1][0]) && (ss >> result[1][1]) && (ss >> result[1][2]) && (ss >> result[1][3]) && 
			(ss >> result[2][0]) && (ss >> result[2][1]) && (ss >> result[2][2]) && (ss >> result[2][3]) && 
			(ss >> result[3][0]) && (ss >> result[3][1]) && (ss >> result[3][2]) && (ss >> result[3][3]))
		{
			return result;
		}
	}

	return mat4(0);
}

std::string js_mat4::ToString() const
{
	return fmt::format("{} {} {} {}", (*this)[0], (*this)[1], (*this)[2], (*this)[3]);
}

BEGIN_GLM_LINK(mat4);
END_STRUCTURE_LINK();

quat js_quat::FromString(std::string value)
{
	if (!value.empty())
	{
		std::istringstream ss(value);

		quat result;
		if ((ss >> result[0]) && (ss >> result[1]) && (ss >> result[2]) && (ss >> result[3]))
		{
			return result;
		}
	}

	return quat();
}

std::string js_quat::ToString() const
{
	return fmt::format("{} {} {} {}", (*this)[0], (*this)[1], (*this)[2], (*this)[3]);
}

BEGIN_GLM_LINK(quat);
END_STRUCTURE_LINK();

dquat js_dquat::FromString(std::string value)
{
	if (!value.empty())
	{
		std::istringstream ss(value);

		dquat result;
		if ((ss >> result[0]) && (ss >> result[1]) && (ss >> result[2]) && (ss >> result[3]))
		{
			return result;
		}
	}

	return dquat();
}

std::string js_dquat::ToString() const
{
	return fmt::format("{} {} {} {}", (*this)[0], (*this)[1], (*this)[2], (*this)[3]);
}

BEGIN_GLM_LINK(dquat);
END_STRUCTURE_LINK();
