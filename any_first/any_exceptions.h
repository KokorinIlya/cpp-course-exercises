#pragma once
#include <typeinfo>
#include <string>

namespace  cpp_course
{
	struct bad_any_cast : public std::bad_cast
	{
	public:
		bad_any_cast(std::string const& _message) : bad_cast(), message(_message) {}

		const std::string& cause() const noexcept
		{
			return message;
		}

	private:
		std::string message;
	};

	struct empty_any : public std::runtime_error
	{
	public:
		empty_any(std::string const& _message) : runtime_error(_message) {}
	};
}