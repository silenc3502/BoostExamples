#include <boost/function.hpp>
typedef boost::function<void(int)> fobject_t;

void process_integers(const fobject_t& f);

#include <assert.h>
#include <deque>
int main() {
	// 파라미터를 받지 않고 아무것도 하지 않는 람다 함수
    process_integers([](int /*i*/){});

	// reference를 저장하는 람다 함수
    std::deque<int> ints;
    process_integers([&ints](int i){
        ints.push_back(i);
    });

	// 자신의 내용을 수정하는 람다 함수
    std::size_t match_count = 0;
    process_integers([ints, &match_count](int i) mutable {
        if (ints.front() == i) {
           ++ match_count;
        }
        ints.pop_front();
    });

    assert(match_count == 6);
}

void process_integers(const fobject_t& f) {
    static const int data[] = {1, 2, 3, 4, 5, 200, 0};
    // We'll be using only 6 elements in this example
    std::for_each(data, data + 6, f);
}
