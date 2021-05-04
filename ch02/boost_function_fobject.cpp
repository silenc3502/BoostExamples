#include <functional>

typedef void (*func_t)(int);

// 함수 포인터를 받아 자신이 가진 각 정수에 대해
// 해당 함수를 호출하는 함수로 함수 객체는 받지 못함
void process_integers(func_t f);

// 함수 객체
class int_processor: public std::unary_function<int, void> {
   const int min_;
   const int max_;
   bool& triggered_;

public:
    int_processor(int min, int max, bool& triggered)
        : min_(min)
        , max_(max)
        , triggered_(triggered)
    {}

    void operator()(int i) const {
        if (i < min_ || i > max_) {
            triggered_ = true;
        }
    }
};

#include <boost/function.hpp>

// 위의 process_integers에 프로토타입을 가리지 않고 받고자 한다면
// boost::function을 사용해서 받을 수 있다.
typedef boost::function<void(int)> fobject_t;

// 이제 함수 객체를 받을 수 있다.
void process_integers(const fobject_t& f);

int main() {
    bool is_triggered = false;
    int_processor fo(0, 200, is_triggered);
    process_integers(fo);
    assert(is_triggered);
}

void foo(const fobject_t& f) {
	// boost::function 클래스는 기본 생성자를 가지며 빈 상태도 있다.
    if (f) {
        // we have value in 'f'
        // ...
    } else {
        // 'f' is empty
        // ...
    }
}

bool g_is_triggered = false;
void set_functional_object(fobject_t& f) {
    int_processor fo( 100, 200, g_is_triggered);
    f = fo;
    // fo leaves scope and will be destroyed,
    // but 'f' will be usable even in outer scope
}

#include <vector>
#include <algorithm>
#include <boost/bind.hpp>
void foo1() {
    std::vector<int> v;
    std::for_each(v.begin(), v.end(), boost::bind(std::plus<int>(), 10, _1));

    fobject_t f(boost::bind(std::plus<int>(), 10, _1));
    std::for_each(v.begin(), v.end(), f);
}

void process_integers(const fobject_t& f) {
    static const int data[] = {1, 2, 3, 4, 5, 250};
    std::for_each(data, data + sizeof(data) / sizeof(int), f);
}
