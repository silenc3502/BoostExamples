#include <iostream>
#include <typeinfo>

// 그다지 이식성이 좋지 못하다.
// 혹은 플랫폼마다 출력되는 정보가 i 혹은 d 정도로만 출력될 수도 있다.
template <class T>
void do_something(const T& x) {
    std::cout << "T is " << typeid(T).name() << std::endl;

    (void)x;
}

void test1() {
    auto&& x = 42;
    std::cout << "x is "
              << typeid(decltype(x)).name()
              << std::endl;

    (void)x;
}

#include <iostream>
#include <boost/type_index.hpp>

// boost::typeindex는 type 이름을 사람이 읽기 편하게 만드는 효율적인 방법을 알고 있다.
namespace bti = boost::typeindex;

// boost::typeindex::type_id 함수는 boost::typeindex::type_index를 반환한다.
template <class T>
void do_something_again(const T& x) {
    std::cout << "T is " << boost::typeindex::type_id<T>() << std::endl;

    (void)x;
}

// boost::typeindex::type_id_with_cvr에 cvr은 const, volatile, reference를 의미한다.
// 이를 사용하면 type이 소실되지 않는다.
void test2() {
    auto&& x = 42;
    std::cout << "x is "
              << boost::typeindex::type_id_with_cvr<decltype(x)>()
              << std::endl;

    (void)x;
}

int main () {
    do_something(1.0);
    test1();
    do_something_again(1.0);
    test2();
}



