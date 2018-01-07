#pragma once

namespace cpp_course
{
	struct nil
	{};

	 //nil ~ || (empty node)

	template <typename Data, typename Next>
	struct node
	{
		using data = Data;
		using next = Next;
	};

	//node<Data, Next> ~ |Data| -> Next

	/*
	 * node <int, nil> ~ |int| -> nil ~ [int]
	 * node <int, node <float, nil>> ~ |int| -> |float| -> nil ~ [int, float] 
	 */

	/*
	 * numbers can be hidden inside types
	 * like this:
	 */

	template <int N>
	struct my_int
	{
		static constexpr int value = N;
	};

	//my_int is a type, containing int
	//my_int<1>::value == 1

	template <typename T>
	struct size;

	template<>
	struct size<nil>
	{
		static constexpr size_t value = 0;
	};

	template <typename List>
	struct size
	{
		static size_t constexpr value = 1 +
			size<typename List::next>::value;
	};

	 template <typename T>
	 static constexpr size_t size_v = size<T>::value;

	 //Other implementation of size, that does the same
	
	template <typename T>
	struct other_size;

	template <>
	struct other_size<nil>
	{
		static constexpr size_t value = 0;
	};

	template <typename Data, typename Next>
	struct other_size<node<Data, Next>>
	{
		static constexpr size_t value = 1 +
			other_size<Next>::value;
	};

	template <typename T>
	static constexpr size_t other_size_v = other_size<T>::value;

	template <typename T, int N>
	struct at;

	template <int N>
	struct at<nil, N>
	{
		static_assert(N == 0 && "Index out of range");
		using type = nil;
	};
	/*
	
	|| => return nil
	^
	|

	|| => error
		^
		|

	 */

	template <typename Data, typename Next>
	struct at<node<Data, Next>, 0>
	{
		using type = Data;
	};
	/*
	
	|Data| -> Next => return Data
	  ^
	  |

	 */

	template <typename Data, typename Next, int N>
	struct at<node<Data, Next>, N>
	{
		using type = typename at<Next, N - 1>::type;
	};

	template <typename T, int N>
	using at_t = typename at<T, N>::type;

	template<typename List, typename Elem>
	struct push_front
	{
		using type = node <Elem, List>;
	};
	/*
	|n1| -> |n2| -> ... + |elem| => |elem| -> |n1| -> |n2| -> ...
	 */

	template <typename List, typename Elem>
	using push_front_t = typename push_front<List, Elem>::type;

	template<typename List, typename Elem>
	struct push_back;

	template<typename Elem>
	struct push_back<nil, Elem>
	{
		using type = node<Elem, nil>;
	};

	/*
	|| + |elem| => |elem| -> nil
	*/

	template <typename Data, typename Next, typename Elem>
	struct push_back<node<Data, Next>, Elem>
	{
		using type = node<Data, typename push_back<Next, Elem>::type>;
	};

	/*
	|data| -> next + |elem| => |data| -> |next| -> ... -> |elem|
	 */

	template<typename T, typename Elem>
	using push_back_t = typename push_back<T, Elem>::type;

	template<typename T>
	struct pop_front;

	template <typename Data, typename Next>
	struct pop_front<node<Data, Next>>
	{
		using result = Data;
		using remainder = Next;
	};

	/*
	|data| -> |next| -> ... => |data| + |next| -> ... 
	 */

	template <typename T>
	using pop_front_res = typename  pop_front<T>::result;

	template <typename T>
	using pop_front_rem = typename  pop_front<T>::remainder;

	template <typename T>
	struct pop_back;

	template <typename Data>
	struct pop_back<node<Data, nil>>
	{
		using result = Data;
		using remainder = nil;
	};

	/*
	|data| -> nil => |data| + nil
	 */

	template<typename Data, typename Next>
	struct pop_back<node<Data, Next>>
	{
		using result = typename pop_back<Next>::result;
		using remainder = node<Data, typename pop_back<Next>::remainder>;
	};

	/*
	|n1| -> |n2| -> ... -> |n_m| -> nil => |n_m| + |n1| -> |n2| -> ... -> |n_m-1| -> nil
	 */

	template<typename T>
	using pop_back_res = typename pop_back<T>::result;

	template<typename T>
	using pop_back_rem = typename pop_back<T>::remainder;

	template <typename T>
	struct is_nil
	{
		static constexpr bool value = false;
	};

	template<>
	struct is_nil<nil>
	{
		static constexpr bool value = true;
	};

	template<typename T>
	static constexpr bool is_nil_v = is_nil<T>::value;
}

