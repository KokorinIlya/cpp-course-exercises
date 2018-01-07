#pragma once
#include <stddef.h>
#include <memory>
#include <string>
#include "any_exceptions.h"

namespace cpp_course
{
	struct any
	{
	public:
		any() noexcept: ptr(nullptr), deleter(nullptr), copier(nullptr), cur_type(nullptr) {}

		~any() noexcept
		{
			if (deleter != nullptr)
			{
				deleter(ptr);
			}
		}

		template<typename T>
		any(T val) : 
			ptr(new T(std::move(val))), 
			deleter(&delete_impl<T>),
			copier(&copy_impl<T>),
			cur_type(&typeid(val))
			//create copy and delete functions for type of val
			//and remember them
			//also, remember type of stored value
		{}

		any(any const& other) :
			ptr(other.copier != nullptr ? other.copier(other.ptr) : nullptr),
			deleter(other.deleter),
			copier(other.copier),
			cur_type(other.cur_type)
		{}

		void swap(any& other) noexcept
		{
			using std::swap;
			swap(ptr, other.ptr);
			swap(deleter, other.deleter);
			swap(copier, other.copier);
			swap(cur_type, other.cur_type);
		}

		any& operator=(any const& other)
		{
			any temp(other);
			swap(temp);
			return *this;
		}

		const type_info& get_type() const
		{
			if (cur_type == nullptr)
			{
				throw empty_any("Any doesn't have cuttent type, because it is empty");
			}
			return *cur_type;
		}

		friend void swap(any& a, any& b) noexcept;

		template <typename T>
		friend const T& any_cast(const any& from);

		template <typename T>
		friend T& any_cast(any& from);

	private:
		using deleter_type = void(*)(void*);
		using copier_type = void*(*)(void*);

		//ptr is a pointer to any's data, allocated on heap
		//ptr has type void*, so it can't be copied or deleted.
		//that's why we use deleter and copier, that remember real ptr type

		void* ptr;
		deleter_type deleter;
		copier_type copier;

		const type_info* cur_type;

		//functions, that remember ptr type and cast to it
		template <typename T>
		static void delete_impl(void* ptr)
		{
			delete static_cast<T*>(ptr);
		}

		template <typename T>
		static void* copy_impl(void* ptr)
		{
			return new T(*(static_cast<T*>(ptr)));
		}

	public:
		any(any&& other) noexcept :
			ptr(other.ptr),
			deleter(other.deleter),
			copier(other.copier),
			cur_type(other.cur_type)
		{
			other.ptr = nullptr;
			other.deleter = nullptr;
			other.copier = nullptr;
			other.cur_type = nullptr;
		}
		
		any& operator=(any&& other) noexcept
		{
			swap(other);
			return *this;
		}
	};

	inline void swap(any& a, any& b) noexcept
	{
		a.swap(b);
	}

	template <typename T>
	const T& any_cast(const any& from)
	{
		//check if cur_type and type of T are equal
		if (typeid(T) != from.get_type())
		{
			//if not equal, we can't cast
			throw bad_any_cast("any can't be casted to this type");
		}
		//type-safe cast, if type are equals
		const T* T_ptr = static_cast<T*>(from.ptr);
		return *T_ptr;
	}

	template <typename T>
	T& any_cast(any& from)
	{
		const any& const_from = static_cast<const any&>(from);
		return const_cast<T&>(any_cast<T>(const_from));
	}
}
