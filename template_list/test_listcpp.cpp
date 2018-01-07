#include "list.h"
#include <iostream>

int main()
{
	using namespace cpp_course;
	using std::cout;
	using std::endl;

	using _1 = my_int<1>;
	using _2 = my_int<2>;
	using _3 = my_int<3>;
	using _4 = my_int<4>;
	using _5 = my_int<5>;
	using _6 = my_int<6>;
	using _7 = my_int<7>;
	using _8 = my_int<8>;
	using _9 = my_int<9>;
	using _10 = my_int<10>;

	using list1 = node<_1, node<_3, node<_7, nil>>>;
	cout << size_v<list1> << " " << other_size_v<list1> << endl;

	cout << at_t<list1, 0>::value << " " << at_t<list1, 1>::value << " " <<
		at_t<list1, 2>::value << " " << typeid(at_t<list1, 3>).name() << endl;

	using list2 = push_back_t<list1, _2>;
	using list3 = push_front_t<list2, _4>;
	
	cout << at_t<list3, 0>::value << " " << at_t<list3, 1>::value << " " <<
		at_t<list3, 2>::value << " " << at_t<list3, 3>::value << " "  <<
		at_t<list3, 4>::value << " " << typeid(at_t<list3, 5>).name() << endl;

	using rem_front = pop_front_rem<list3>;
	using res_front = pop_front_res<list3>;

	cout << res_front::value << endl;

	cout << at_t<rem_front, 0>::value << " " << at_t<rem_front, 1>::value << " " <<
		at_t<rem_front, 2>::value << " " << at_t<rem_front, 3>::value << " " <<
		typeid(at_t<rem_front, 4>).name() << endl;

	using rem_back = pop_back_rem<rem_front>;
	using res_back = pop_back_res<rem_front>;

	cout << res_back::value << endl;

	cout << at_t<rem_back, 0>::value << " " << at_t<rem_back, 1>::value << " " <<
		at_t<rem_back, 2>::value << " " << typeid(at_t<rem_back, 3>).name() << endl;
	
	system("pause");
	return 0;
}
