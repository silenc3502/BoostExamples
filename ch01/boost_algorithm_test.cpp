#include <boost/array.hpp>

// 65 ~ 125사이의 코드에 해당하는 문자를 출력하고 싶다면
// C++11 이전의 컴파일러에서는 아래와 같이 만들어야 한다.
boost::array<unsigned char, 60> chars_65_125_pre11() {
    boost::array<unsigned char, 60> res;

    const unsigned char offset = 65;
    for (std::size_t i = 0; i < res.size(); ++i) {
        res[i] = i + offset;
    }

    return res;
}

#include <boost/algorithm/cxx11/iota.hpp>
#include <boost/array.hpp>

// boost::algorithm에는 C++11, C++14에 추가된 모든 새 알고리즘이 들어있다.
boost::array<unsigned char, 60> chars_65_125() {
    boost::array<unsigned char, 60> res;
    boost::algorithm::iota(res.begin(), res.end(), 65);
    return res;
}

#include <boost/algorithm/hex.hpp>
#include <iterator>
#include <iostream>

void to_hex_test1() {
    const std::string data = "Hello word";
    boost::algorithm::hex(
        data.begin(), data.end(),
        std::ostream_iterator<char>(std::cout)
    );

    std::cout << '\n';
}

void to_hex_test2() {
    const std::string data = "Hello word";
    boost::algorithm::hex(
        data,
        std::ostream_iterator<char>(std::cout)
    );

    std::cout << '\n';
}

#include <assert.h>
int main() {
    assert(chars_65_125_pre11() == chars_65_125());
    to_hex_test1();
    to_hex_test2();
}
