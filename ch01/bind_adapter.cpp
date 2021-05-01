#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <algorithm>
#include <functional>
#include <string>
#include <assert.h>

// 과거 C나 C++에서는 Adapter 패턴을 사용하여 함수 객체를 만들어 사용했다.
// 이것을 boost::bind가 보다 편리하게 해준다.
void test1() {
    boost::array<int, 12> v = {{1, 2, 3, 4, 5, 6, 7, 100, 99, 98, 97, 96}};

	// 5 보다 큰 갑싱 몇 개인지 판단한다.
    const std::size_t count0 = std::count_if(v.begin(), v.end(), [](int x) {
        return 5 < x ;
    });
    const std::size_t count1 = std::count_if(v.begin(), v.end(),
        boost::bind(std::less<int>(), 5, _1)
    );
    assert(count0 == count1);
}

void test2() {
    boost::array<std::string, 3> v = {{"We ", "are", " the champions!"}};

	// 빈 문자열이 몇 개인지 파악한다.
    const std::size_t count0 = std::count_if(v.begin(), v.end(), [](const std::string& s) {
        return s.empty();
    });
    const std::size_t count1 = std::count_if(v.begin(), v.end(),
        boost::bind(&std::string::empty, _1)
    );
    assert(count0 == count1);
}

void test3() {
    boost::array<std::string, 3> v = {{"We ", "are", " the champions!"}}; 

	// 길이가 5 보다 작은 문자열이 몇 개인지 파악한다.
    const std::size_t count0 = std::count_if(v.begin(), v.end(), 
        [](const std::string& s) {  return s.size() < 5; }
    ); 
    const std::size_t count1 = std::count_if(v.begin(), v.end(), 
        boost::bind(
            std::less<std::size_t>(),
            boost::bind(&std::string::size, _1),
            5
        )
    ); 
    assert(count0 == count1);  
}

void test4() {
    boost::array<std::string, 3> v = {{"We ", "are", " the champions!"}}; 
    std::string s("Expensive copy constructor is called when binding"); 

	// 문자열을 비교한다.
    const std::size_t count0 = std::count_if(v.begin(), v.end(),
        [&s](const std::string& x) {  return x < s; }
    ); 
    const std::size_t count1 = std::count_if(v.begin(), v.end(), 
        boost::bind(std::less<std::string>(), _1, s)
    ); 
    assert(count0 == count1);
}

// boost::bind()는 값을 묶을 때 함수 객체에 값이 복사된다.
// 복사 생성자의 복사를 발생시키고 싶지 않다면 어떻게 해야할까 ?
// boost::ref()를 통해 이 문제를 해결할 수 있다.
#include <boost/ref.hpp>

void test5() {
    boost::array<std::string, 3> v = {{"We ", "are", " the champions!"}}; 
    std::string s("Expensive copy constructor is NOT called when binding"); 

    const std::size_t count1 = std::count_if(v.begin(), v.end(), 
        boost::bind(std::less<std::string>(), _1, boost::cref(s))
    ); 
    const std::size_t count0 = std::count_if(v.begin(), v.end(), [&s](const std::string& x) {
        return x < s;
    });
    assert(count0 == count1);
};

void test6() {
	// boost::bind()를 통해 함수 파라미터의 순서를 바꾸거나 무시하거나 중복해서 사용이 가능하다.
    const auto twice = boost::bind(std::plus<int>(), _1, _1);
    assert(twice(2) == 4);

    const auto minus_from_second = boost::bind(std::minus<int>(), _2, _1);
    assert(minus_from_second(2, 4) == 2);

    const auto sum_second_and_third = boost::bind(std::plus<int>(), _2, _3);
    assert(sum_second_and_third(10, 20, 30) == 50);
}


int main () {
    test1();
    test2();
    test3();
    test4();
    test5();
    test6();
}



