#pragma once
#include <stdexcept>

namespace cpp_course
{
	struct bad_optional_access : public std::runtime_error
	{
		bad_optional_access(const std::string& message) : runtime_error(message) {}
	};
}
