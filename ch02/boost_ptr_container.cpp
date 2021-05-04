#include <set>
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <cassert>

// 컨테이너에 포인터를 저장해야 할 때가 있다.
// 예로 컨테이너에 다형적 데이터를 저장, 컨테이너 내에 있는 데이터를 빠르게 복사,
// 컨테이너 내의 데이터로 작업할 때 예외 처리를 적용하는 경우등이 있다.
template <class T>
struct ptr_cmp: public std::binary_function<T, T, bool> {
    template <class T1>
    bool operator()(const T1& v1, const T1& v2) const {
        return operator ()(*v1, *v2);
    }

    bool operator()(const T& v1, const T& v2) const {
        return std::less<T>()(v1, v2);
    }
};

template <class T>
struct ptr_deleter: public std::unary_function<T, void> {
    void operator()(T* ptr) {
        delete ptr;
    }
};

void example1() {

    std::set<int*, ptr_cmp<int> > s;
    s.insert(new int(1));
    s.insert(new int(0));

    // ...
    assert(**s.begin() == 0);

    // ...
    // Deallocating resources
    // Any exception in this code will lead to
    // memory leak
    std::for_each(s.begin(), s.end(), ptr_deleter<int>());
}

//void example2_a() {
//    typedef std::auto_ptr<int> int_aptr_t;
//    std::set<int_aptr_t, ptr_cmp<int> > s;
//    s.insert(int_aptr_t(new int(1)));
//    s.insert(int_aptr_t(new int(0)));
//    // ...
//    assert(**s.begin() == 0);
//    // ...
//    // resources will be deallocated by auto_ptr<>
//}

// 위와 같이 작성하면 실수가 발생할 수 있으므로 스마트 포인터를 저장하도록 한다.
// 나쁘지 않은 방법이지만 C++03등에서는 사용이 불가능하다.
// 또 비교자 함수 객체를 만들어야 한다.
void example2_b() {
    typedef std::unique_ptr<int> int_uptr_t;
    std::set<int_uptr_t, ptr_cmp<int> > s;
    s.insert(int_uptr_t(new int(1)));
    s.insert(int_uptr_t(new int(0)));
    // ...
    assert(**s.begin() == 0);
    // ...
    // resources will be deallocated by unique_ptr<>
}

#include <boost/shared_ptr.hpp>

// 아까보다 이식성을 높였지만 여전히 비교자를 만들어야 하며 성능도 떨어진다.
void example3() {
    typedef boost::shared_ptr<int> int_sptr_t;
    std::set<int_sptr_t, ptr_cmp<int> > s;
    s.insert(int_sptr_t(new int(1)));
    s.insert(int_sptr_t(new int(0)));
    // ...
    assert(**s.begin() == 0);
    // ...
    // resources will be deallocated by shared_ptr<>
}

#include <boost/ptr_container/ptr_set.hpp>

// Boost.PointerContainer 라이브러리는 ptr_array, ptr_vector, ptr_set, ptr_multimap 등을 갖는다.
// 이 클래스들은 포인터를 할당 해제하며 포인터가 가리키는 데이터로 접근도 용이하다.
void correct_impl() {
    boost::ptr_set<int> s;
    s.insert(new int(1));
    s.insert(new int(0));
    // ...
    assert(*s.begin() == 0);
    // ...
    // resources will be deallocated by container itself
}

#include <boost/ptr_container/clone_allocator.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
void theres_more_example() {
    // Creating vector of 10 elements with values 100
    boost::ptr_vector<int> v;
    int value = 100;
    v.resize(10, &value); // Beware! No ownership of pointer!
    assert(v.size() == 10);
    assert(v.back() == 100);
}

int main() {
    example1();
//    example2_a();
    example2_b();
    example3();
    correct_impl();
    theres_more_example();
}
