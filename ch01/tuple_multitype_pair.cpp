#include <boost/tuple/tuple.hpp>
#include <string>
#include <iostream>

#include <boost/tuple/tuple_comparison.hpp>
#include <assert.h>
#include <set>

void foo1() {
	int k;

	// Tuple을 사용하기 위한 변수들이며
	// 아래와 같이 여러 타입들을 한 묶음으로 다룰 수 있다.
    boost::tuple<int, std::string> almost_a_pair(10, "Hello");
    boost::tuple<int, float, double, int> quad(10, 1.0f, 10.0, 1);

	// boost::get<N>을 통해 특정 인덱스 N에 있는 값을 얻어올 수 있다.
    int i = boost::get<0>(almost_a_pair);
    const std::string& str = boost::get<1>(almost_a_pair);
    double d = boost::get<2>(quad);

    (void) i;
    (void) str;
    (void) d;

	std::cout << i << ' ' << str << ' ' << d << std::endl;

	// 튜플 비교 연산자는 "boost/tuple/tuple_comparison.hpp"헤더에 정의되어 있다.
	// boost::make_tuple() 함수를 사용하면
	// 형식을 일일히 나열하지 않아도 되기 때문에 코드가 더 짧아진다.
    std::set<boost::tuple<int, double, int> > s;
    s.insert(boost::make_tuple(1, 1.0, 2));
    s.insert(boost::make_tuple(2, 10.0, 2));
    s.insert(boost::make_tuple(3, 100.0, 2));

	for (auto it = s.begin(); it != s.end(); ++it)
	{
		std::cout << ' ' << boost::get<0>(*it) << std::endl;
		std::cout << ' ' << boost::get<1>(*it) << std::endl;
		std::cout << ' ' << boost::get<2>(*it) << std::endl;
	}
}

void foo2() {
#ifndef BOOST_NO_CXX11_AUTO_DECLARATIONS
    // Requires C++11
    const auto t = boost::make_tuple(0, -1.0, 2);
    assert(2 == boost::get<2>(t));
    // We can make a compile time assert for type
    // of t. Interested? See chapter 'Compile time tricks'
#endif
}

void foo3() {
    boost::tuple<int, float, double, int> quad(10, 1.0f, 10.0, 1);
    int i;
    float f;
    double d;
    int i2;

	// quad 변수에 있는 내용을 i, f, d, i2에 각각 전달한다.
    boost::tie(i, f, d, i2) = quad;

    assert(i == 10);
    assert(i2 == 1);
}

// 구조체가 아닌 튜플 라이브러리를 사용하는 이유는 결국 템플릿 프로그래밍을 쉽게 하기 위함이다.
struct id_name_pair {
    int id;
    std::string name;
};

int main () {
    foo1();
    foo2();
    foo3();
}
