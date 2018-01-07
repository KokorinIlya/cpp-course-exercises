#pragma once
#include "any_exceptions.h"
#include <memory>
#include <iostream>

namespace cpp_course
{
	struct any
	{
	public:
		any() noexcept :
			ptr(nullptr)
		{}

		template <typename T>
		any(T value) :
			ptr(new derived<T>(std::move(value)))
		{}

		any(any const& other) : ptr(other.ptr ? other.ptr->clone() : nullptr) {}

		any& operator=(any const& other)
		{
			any temp(other);
			swap(temp);
			return *this;
		}

		void swap(any& other) noexcept
		{
			ptr.swap(other.ptr);
		}

		const type_info& get_type() const
		{
			if (!ptr)
			{
				throw empty_any("Any doesn't have current type because it is empty");
			}
			return ptr->get_type();
		}

		friend void swap(any& a, any& b) noexcept;

		template <typename T>
		friend const T& any_cast(any const& from);

		template <typename T>
		friend T& any_cast(any& from);

	private:
		//polymorph class with virtual destructor.
		//it will delete derived class correctly
		struct base
		{
			virtual ~base() noexcept = default;
			virtual base* clone() = 0;
			virtual const type_info& get_type() noexcept = 0;
		};

		//stores data. Will be deleted correctly by virtual base::~base
		template<typename T>
		struct  derived : public base
		{
			T value;

			derived(T _value): value(std::move(_value)) {}

			derived* clone() override
			{
				return new derived(value);
			}

			const type_info& get_type() noexcept override
			{
				return typeid(value);
			}
		};

		std::unique_ptr<base> ptr;

	public:
		any(any&& other) noexcept : ptr(std::move(other.ptr)) {}

		any& operator=(any&& other) noexcept
		{
			swap(other);
			return *this;
		}
	};

	template <typename T>
	const T& any_cast(any const& from)
	{
		const any::derived<T>* T_ptr = dynamic_cast<any::derived<T>*>(from.ptr.get());
		if (T_ptr == nullptr)
		{
			throw bad_any_cast("any can't be casted to this type");
		}
		return T_ptr->value;
	}

	template <typename T>
	T& any_cast(any& from)
	{
		const any& const_from = static_cast<const any&>(from);
		return const_cast<T&>(any_cast<T>(const_from));
	}

	inline void swap(any& a, any& b) noexcept
	{
		a.swap(b);
	}
}
