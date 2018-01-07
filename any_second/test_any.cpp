#include "any.h"
#include <iostream>
#include "any_exceptions.h"

struct my_type
{
	~my_type()
	{
		std::cout << "Deleted\n";
	}
};

int main()
{
	using cpp_course::any;
	using std::cout;
	using std::endl;

	any any1(std::string("Hello"));
	cout << cpp_course::any_cast<std::string>(any1).length() << "\n";
	try
	{
		cout << cpp_course::any_cast<int>(any1) << "\n";
	}
	catch (cpp_course::bad_any_cast e)
	{
		cout << e.cause() << endl;
	}
	my_type m;
	any any_string2(std::string("Hi there"));
	cout << cpp_course::any_cast<std::string>(any_string2) << "\n";
	any_string2 = any1;
	cout << cpp_course::any_cast<std::string>(any_string2) << "\n";
	cpp_course::any_cast<std::string>(any_string2) = "Fuck you and your tests";
	cout << cpp_course::any_cast<std::string>(any_string2) << "\n";
	cout << any_string2.get_type().name() << std::endl;
	any any_int(42);
	cout << any_int.get_type().name() << std::endl;
	any any2(m);
	any any_emp;
	try
	{
		any_emp.get_type();
	}
	catch(cpp_course::empty_any e)
	{
		cout << e.what() << endl;
	}

	system("pause");
	return 0;
}
