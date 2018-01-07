#include "call_once_function.h"
#include <iostream>
#include <functional>

struct func
{
	int operator()(int) &&
	{
		return 42;
	}
};

int main()
{
	using std::cout;
	using std::endl;
	using my_f = cpp_course::call_once_function<int(int)>;
	my_f rval(func{});
	cout << std::move(rval)(5) << endl;
	system("pause");
	return 0;
}