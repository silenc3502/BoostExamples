#include <boost/variant.hpp>
#include <iostream>
#include <vector>
#include <string>

void example_func(void);
void example_func1(void);

int main() {
    typedef boost::variant<int, const char*, std::string> my_var_t;
    std::vector<my_var_t> some_values;
    some_values.push_back(10);
    some_values.push_back("Hello there!");
    some_values.push_back(std::string("Wow!"));

    std::string& s = boost::get<std::string>(some_values.back());
    s += " That is great!\n";
    std::cout << s;

	example_func();
	example_func1();
}

void example_func(void) {
	// 기본 생성자는 boost::blank 인스턴스를 만든다.
    boost::variant<boost::blank /*empty state*/, int, const char*, std::string> var;

	// which() 매서드는 variant가 소유한 형식의 인덱스를 반환한다.
    assert(var.which() == 0); // Empty state

    var = "Hello, dear reader";
    assert(var.which() != 0);

	std::cout << var << std::endl;
}

void example_func1(void) {
    boost::variant<int, std::string> variable(0);

	// 변수의 실제값이 int가 아니라면 다음 매서드가 boost::bad_get 예외를 던질 수도 있다.
    int s1 = boost::get<int>(variable);

	// 변수의 실제 값이 int가 아니라면 NULL 포인터를 반환한다.
    int* s2 = boost::get<int>(&variable);

    (void)s1;
    (void)s2;

	std::cout << s1 << std::endl;
	std::cout << s2 << std::endl;
}
