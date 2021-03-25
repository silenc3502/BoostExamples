//#include <iostream>
//#include <vector>
//#include <string>
//#include <auto_ptr.h>

//int main()
//{

//    typedef std::auto_ptr<Object> object_ptr;
//    std::vector<object_ptr> some_values;
//    some_values.push_back(new Object(10));
//    some_values.push_back(new Object("Hello there"));
//    some_values.push_back(new Object(std::string("Wow!")));

//    std::string* p = dynamic_cast<std::string*>(some_values.back().get());
//    assert(p);
//    (*p) += " That is great!\n";
//    std::cout << *p;

//    return 0;
//}

#include <boost/any.hpp>
#include <iostream>
#include <vector>
#include <string>

void example_func(void);

int main() {
    std::vector<boost::any> some_values;
    some_values.push_back(10);
    const char* c_str = "Hello there!";
    some_values.push_back(c_str);
    some_values.push_back(std::string("Wow!"));

    std::string& s = boost::any_cast<std::string&>(some_values.back());
    s += " That is great!\n";
    std::cout << s;

	example_func();
}

void example_func(void) {
    boost::any variable(std::string("Hello world!"));

	// 아래 방식에서 변수의 실제값이 std::string이 아니라면
	// boost::bad_any_cast 예외를 던질 수도 있다.
    std::string s1 = boost::any_cast<std::string>(variable);

	// 절대 예외를 던지지 않으며
	// 변수의 실제값이 std::string이 아니라면 NULL 포인터를 반환한다.
    std::string* s2 = boost::any_cast<std::string>(&variable);

    (void)s2;
    (void)s1;

	std::cout << s1 << std::endl;
	std::cout << s2 << std::endl;
}
