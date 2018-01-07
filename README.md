# cpp-course-exercises
Excercises for c++ course

Including:

1) any - my implementation of std::any or boost::any.

http://en.cppreference.com/w/cpp/utility/any

Implemented using void* with type_info (any_first) and polymorph classes (any_second)

2) call_once_function - function, identical to std::function, that can store and call call_once functional objects (like objects, created by call_once_bind)

Implemented using Small Object Optimization (SOO). The data structure is fully reentrancy-safe

3) optional - my implementation of std::optional or boost::optional

http://en.cppreference.com/w/cpp/utility/optional

Stores objects in the optional, does't allocate memory for storing on heap.

4) template-list - implementation of linked list, using c++-17 template metaprogramming. All functions are calculated in compile-time.

