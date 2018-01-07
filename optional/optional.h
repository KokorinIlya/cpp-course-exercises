#pragma once
#include <type_traits>
#include <iostream>
#include <fstream>
#include "optional_exceptions.h"

namespace cpp_course
{
	//std::ofstream out("file.txt");
	template <typename T>
	struct optional
	{
	public:
		using storage_type = typename std::aligned_storage<sizeof(T), alignof(T)>::type;

		friend bool operator==(const optional& lhs, const optional& rhs) noexcept
		{
			if (!lhs && !rhs)
			{
				return true;
			}
			if (lhs && rhs && *lhs == *rhs)
			{
				return true;
			}
			return false;
		}

		friend bool operator!=(const optional& lhs, const optional& rhs) noexcept
		{
			return !(lhs == rhs);
		}

		friend bool operator<(const optional& lhs, const optional& rhs) noexcept
		{
			if (lhs && rhs && *lhs < *rhs)
			{
				return true;
			}
			return false;
		}

		friend bool operator<=(const optional& lhs, const optional& rhs) noexcept
		{
			return (lhs < rhs || lhs == rhs);
		}

		friend bool operator>(const optional& lhs, const optional& rhs) noexcept
		{
			return !(lhs <= rhs);
		}

		friend bool operator>=(const optional& lhs, const optional& rhs) noexcept
		{
			return !(lhs < rhs);
		}

		friend void swap(optional& lhs, optional& rhs) noexcept;

	private:
		static void my_dtor(T& t) noexcept (noexcept(t.T::~T()))
		{
			//out << "deleting " << &t << ": ";
			t.~T();
		}

		void copy_data(storage_type const& other)
		{
			new (&storage) T(reinterpret_cast<T const&>(other));
			valid = true;
		}

		storage_type storage;
		bool valid;
	public:

		optional() noexcept: valid(false) {}

		optional(T value) : valid(true)
		{
			new (&storage) T(std::move(value));
		}

		void reset() noexcept(noexcept(my_dtor(reinterpret_cast<T&>(storage))))
		{
			if (valid)
			{
				my_dtor(reinterpret_cast<T&>(storage));
			}
			valid = false;
		}

		~optional() noexcept(noexcept(reset()))
		{
			reset();
		}

		optional(optional const& other) : valid(other.valid)
		{
			if (other.valid)
			{
				new (&storage) T(*other);
			}
		}

		bool has_value() const noexcept
		{
			return valid;
		}

		explicit operator bool() const noexcept
		{
			return has_value();
		}

		const T& operator*() const
		{
			if (!valid)
			{
				throw bad_optional_access("Optional doesn't store a value");
			}
			return reinterpret_cast<const T&>(storage);
		}

		T& operator*()
		{
			const T& const_res = static_cast<const optional*>(this)->operator*();
			return const_cast<T&>(const_res);
		}

		T const* operator->() const
		{
			return &(this->operator*());
		}

		T* operator->()
		{
			return &(this->operator*());
		}

		template <typename ...Args>
		void emplace(Args&& ...args)
		{
			reset();
			new (&storage) T(std::forward<Args>(args)...);
			valid = true;
		}

		void swap(optional& other) noexcept
		{
			if (this == &other)
			{
				return;
			}
			if (!valid && !other)
			{
				return;
			}
			if (!valid && other)
			{
				copy_data(other.storage);
				other.reset();
				return;
			}
			if (valid && !other)
			{
				other.copy_data(storage);
				reset();
				return;
			}
			using std::swap;
			swap(storage, other.storage);
		}

		optional& operator=(optional const& other)
		{
			optional temp(other);
			swap(temp);
			return *this;
		}

		optional& operator=(optional&& other) noexcept
		{
			swap(other);
			return *this;
		}

		optional(optional&& other) noexcept : valid(other.valid)
		{
			if (other.valid)
			{
				new (&storage) T(std::move(reinterpret_cast<T&>(other.storage)));
				other.reset();
			}
		}

		T value() const
		{
			if (!valid)
			{
				throw bad_optional_access("Optional doesn't store a value");
			}
			return reinterpret_cast<const T&>(storage);
		}

		T value_or(T default_value) const
		{
			if (!valid)
			{
				return std::move(default_value);
			}
			return value();
		}
	};

	template <typename T, typename ...Args>
	optional<T> make_optional(Args&& ...args)
	{
		optional<T> tmp;
		tmp.emplace(std::forward<Args>(args)...);
		return tmp;
	}

	template <typename T>
	void swap(optional<T>& lhs, optional<T>& rhs) noexcept
	{
		lhs.swap(rhs);
	}
}
