#include "optional.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct my_struct
{
	~my_struct() noexcept
	{
		//cpp_course::out << "Destroyed" << std::endl;
		std::cout << "Destroyed" << std::endl;
	}

	int x = 1;
};

int main()
{
	using cpp_course::optional;
	using std::cout;
	using std::endl;
	using cpp_course::make_optional;

	my_struct m;
	optional<my_struct> opt(m);
	optional<int> opt_int;
	optional<int> opt_int_2(7);
	cout << *opt_int_2 << endl;
	*opt_int_2 = 42;
	cout << *opt_int_2 << endl;
	optional<int> opt_int_3(opt_int_2);
	cout << *opt_int_3 << endl;
	*opt_int_3 = 14;
	cout << *opt_int_3 << endl;
	optional<std::string> opt_string("hello");
	cout << opt_string->length() << endl;
	opt_string.emplace("new string");
	cout << *opt_string << endl;
	cout << (opt_int_3 <= opt_int_2) << endl;
	std::vector<int> vec(10, 1);
	optional<std::vector<int>> opt_vect = make_optional<std::vector<int>>(10, 1);
	for (auto it = opt_vect->begin(); it != opt_vect->end(); ++it)
	{
		cout << *it << " ";
	}
	cout << endl;
	cout << *opt_int_2 << endl;
	cout << *opt_int_3 << endl;
	opt_int_2.swap(opt_int_3);
	cout << *opt_int_2 << endl;
	cout << *opt_int_3 << endl;

	optional<std::string> opt_str1;
	optional<std::string> opt_str2("string_for_opt");
	cout << opt_str1.has_value() << " " << *opt_str2 << endl;
	opt_str1 = opt_str2;
	cout << *opt_str1 << " " << *opt_str2 << endl;
	std::string ret;
	optional<std::string> opt_str3(std::move(opt_str2));
	cout << *opt_str3 << endl;
	ret = opt_str3.value_or("default");
	cout << ret << endl;
	optional<std::string> opt_str4;
	ret = opt_str4.value_or("default");
	cout << ret << endl;
	system("pause");
	return 0;
}
