#include <boost/array.hpp>
#include <algorithm>
#include <iostream>

typedef boost::array<char, 4> array4_t;

// char (&vector_advance(char (&val)[4]))[4]
array4_t& vector_advance(array4_t& val) {
	// C++ 람다 함수(1 증가)
    const auto inc = [](char& c){ ++c; };

	// boost::array는 begin(), cbegin(), end, cend(), rbegin(), size(), emtpy() 등을 가지고 있다.
    std::for_each(val.begin(), val.end(), inc);
    return val;
}

void vector_print(array4_t& val) {
	const auto print = [](char& c) { std::cout << (int)c << std::endl; };

	std::for_each(val.begin(), val.end(), print);
}

// Boost Array의 가장 큰 장점은 동적으로 메모리를 할당하지 않으며
// 일반 C 배열과 완전히 똑같은 성능을 보인다는 점이다.
int main() {
	
	// C++11에서는 {0, 1, 2, 3}으로 처리할 수 있지만 C++03에서는 {{0, 1, 2, 3}}으로 처리해야함
    array4_t val = {{0, 1, 2, 3}};

    array4_t val_res;               // it is default constructible
    val_res = vector_advance(val);  // it is assignable

    assert(val.size() == 4);
    assert(val[0] == 1);
    /*val[4];*/ // 인덱스의 최댓값이 3이므로 가정에 어긋남

	vector_print(val);
	
    assert(sizeof(val) == sizeof(char) * array4_t::static_size);
}
