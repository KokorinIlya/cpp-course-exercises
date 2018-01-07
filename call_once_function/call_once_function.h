#pragma once
#pragma once
#include <type_traits>
#include <iostream>
#include <functional>

namespace cpp_course
{
	static constexpr size_t SMALL_SIZE = 16;
	static constexpr size_t SMALL_ALIGN = 8;

	template <typename F>
	static constexpr bool is_small_type =
		(sizeof(F) <= SMALL_SIZE && alignof(F) <= SMALL_ALIGN &&
			std::is_nothrow_move_constructible_v<F>);

	template<typename T>
	struct call_once_function;

	template <typename R, typename ... Args>
	struct call_once_function<R(Args...)>
	{
	public:
		call_once_function() noexcept : ops(get_empty_ops()) {}

		template <typename F>
		call_once_function(F f) noexcept(is_small_type<F>) : ops(get_func_ops<F>())
		{
			if constexpr (is_small_type<F>)
			{
				new (&storage) F(std::move(f));
			}
			else
			{
				new (&storage) F*(new F(std::move(f)));
			}
		}

		call_once_function(call_once_function&& other) noexcept :
		ops(other.ops)
		{
			other.ops->mover(other.storage, storage);
			other.ops = get_empty_ops();
		}

		void clear() noexcept
		{
			ops->deleter(storage);
			ops = get_empty_ops();
		}

		~call_once_function() noexcept
		{
			clear();
		}

		explicit operator bool() const noexcept
		{
			return exists();
		}

		bool exists() const noexcept
		{
			return ops != get_empty_ops();
		}

		R operator()(Args ...args) &&
		{
			call_once_function temp(std::move(*this));
			return temp.ops->caller(temp.storage, std::move(args)...);
		}

		void swap(call_once_function& other) noexcept
		{
			storage_type first;
			storage_type second;

			ops->mover(storage, first);
			ops->deleter(storage);

			other.ops->mover(other.storage, second);
			other.ops->deleter(other.storage);

			ops->mover(first, other.storage);
			ops->deleter(first);

			other.ops->mover(second, storage);
			other.ops->deleter(second);

			std::swap(ops, other.ops);
		}

		call_once_function& operator=(call_once_function&& other) noexcept
		{
			swap(other);
			return *this;
		}

	private:
		using storage_type = typename std::aligned_storage<SMALL_SIZE, SMALL_ALIGN>::type;
		using deleter_type = void(*)(storage_type& storage);
		using caller_type = R(*)(storage_type& storage, Args... args);
		using mover_type = void(*)(storage_type& from, storage_type& to);

		template <typename F>
		static void delete_impl_big(storage_type& storage)
		{
			delete reinterpret_cast<F*&>(storage);
		}

		template <typename F>
		static void delete_impl_small(storage_type& storage)
		{
			reinterpret_cast<F&>(storage).F::~F();
		}

		static void delete_impl_empty(storage_type& storage) {}

		template <typename F>
		static R call_impl_big(storage_type& storage, Args... args)
		{
			return (std::move(*reinterpret_cast<F*&>(storage)))(std::move(args)...);
		}

		template <typename F>
		static R call_impl_small(storage_type& storage, Args... args)
		{
			return (std::move(reinterpret_cast<F&>(storage)))(std::move(args)...);
		}

		static R call_impl_empty(storage_type& storage, Args... args)
		{
			throw std::bad_function_call();
		}

		template<typename F>
		static void move_impl_small(storage_type& from, storage_type& to)
		{
			F& f = reinterpret_cast<F&>(from);
			new (&to) F(std::move(f));
		}

		template <typename F>
		static void move_impl_big(storage_type& from, storage_type& to)
		{
			F*& fp = reinterpret_cast<F*&>(from);
			new (&to) F*(std::move(fp));
		}

		static void move_impl_empty(storage_type& from, storage_type& to) {}

		struct func_ops
		{
			deleter_type deleter;
			caller_type caller;
			mover_type mover;

			constexpr func_ops(deleter_type _deleter, caller_type _caller, mover_type _mover) :
				deleter(_deleter),
				caller(_caller),
				mover(_mover)
			{}
		};

		template <typename F>
		static const func_ops* get_func_ops()
		{
			static constexpr func_ops instance = is_small_type<F> ?
				func_ops{ delete_impl_small<F>, call_impl_small<F>, move_impl_small<F> } :
				func_ops{ delete_impl_big<F>, call_impl_big<F>, move_impl_big<F> };
			return &instance;
		}

		static const func_ops* get_empty_ops()
		{
			static constexpr func_ops empty_ops =
				func_ops{ delete_impl_empty, call_impl_empty, move_impl_empty };
			return &empty_ops;
		}

		const func_ops* ops;
		storage_type storage;
	};
}
