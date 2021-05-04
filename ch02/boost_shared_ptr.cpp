// 데이터를 담고 있는 구조체를 동적으로 하당하고
// 여러가지 실행 스레드에서 이 데이터를 처리하는 상황을 고려한다.

class foo_class {
public:
    char data[10000000];

    explicit foo_class(const char* /*param*/){}
};

foo_class* get_data() {
    static int i = 0;
    ++ i;
    if (i % 2) {
        return new foo_class("Just some string");
    } else {
        return 0;
    }
}


#include <boost/thread.hpp>
#include <boost/bind.hpp>

void process1(const foo_class* p);
void process2(const foo_class* p);
void process3(const foo_class* p);

void foo1() {
    while (foo_class* p = get_data()) // C way
    {
		// 스레드가 너무 많다면 스레드가 제어할 수 없을 정도로 생성되는 것을 막기 위해
		// 'Parallel execution of different tasks'를 읽어보도록 한다.
        boost::thread(boost::bind(&process1, p))
                .detach();
        boost::thread(boost::bind(&process2, p))
                .detach();
        boost::thread(boost::bind(&process3, p))
                .detach();
        // delete p; Oops!!!!
    }
}

// 처리 함수를 실행하는 스레드가 있는 한 while 루프가 끝나더라도 p를 지울 수 없다.
// 그런데 처리 함수들은 다른 스레드들이
// 더 이상 p를 쓰지 않는다는 것을 알 수 없기 때문에 p를 지울 수 없다.
#include <boost/shared_ptr.hpp>

void process_sp1(const boost::shared_ptr<foo_class>& p);
void process_sp2(const boost::shared_ptr<foo_class>& p);
void process_sp3(const boost::shared_ptr<foo_class>& p);

// boost::shared_ptr은 atomic reference counter를 갖고 있다.
// 이것을 복사하면 reference counter가 증가하며 소멸자가 호출되면 참조 카운터가 줄어든다.
// 참조 카운터가 0이 되면 shared_ptr이 가리키는 객체에 대해 delete를 호출한다.

// boost::thread(boost::bind(&process_sp1, p))에서
// process_sp1 함수는 파라미터를 reference 형식으로 받는다.
// while 루프를 빠져나갈 때 이 값을 삭제하지 않을까 ?
// bind()가 반환하는 함수 객체는 공유 포인터에 대한 복사본을 갖고 있기 때문에
// 함수 객체가 소멸되지 않는 한 p가 가리키는 데이터도 삭제되지 않는 것이다.
// 함수 객체는 스레드에 복사됐고 스레드가 실행되는 동안 살아있다.
void foo2() {
    typedef boost::shared_ptr<foo_class> ptr_t;
    ptr_t p;
    while (p = ptr_t(get_data())) // C way
    {
        boost::thread(boost::bind(&process_sp1, p))
                .detach();
        boost::thread(boost::bind(&process_sp2, p))
                .detach();
        boost::thread(boost::bind(&process_sp3, p))
                .detach();
        // no need to anything
    }
}

#include <string>
#include <boost/smart_ptr/make_shared.hpp>

void process_str1(boost::shared_ptr<std::string> p);
void process_str2(const boost::shared_ptr<std::string>& p);

// boost::make_shared로 되돌아가서 shared_ptr<std::string> ps(new int(0))을 살펴보면
// new int(0)으로 정수에 대한 포인터를 구성하는 경우
// Heap에 할당된 shared_ptr 클래스 내부 reference counter를 구성하는 경우
// 위와 같이 두 번 new를 호출한다.
void foo3() {
	// make_shared를 써서 shared_ptr을 만들면 단 한번만 new를 호출한다.
	// make_shared는 메모리 한 조각을 할당하고
	// 그 조각 위에 atomic counter와 템플릿 객체를 만든다.
    boost::shared_ptr<std::string> ps = boost::make_shared<std::string>(
        "Guess why make_shared<std::string> "
        "is faster than shared_ptr<std::string> "
        "ps(new std::string('this string'))"
    );

    boost::thread(boost::bind(&process_str1, ps))
            .detach();
    boost::thread(boost::bind(&process_str2, ps))
            .detach();
}

// shared_ptr을 사용해서 데이터 무결성을 지킬 수 있지만
// 원자 연산은 비원자 연산만큼 빠르지 않다.
// C++11을 지원한다면 std::move를 사용해 카운터에 드는 원자 연산량을 줄일 수도 있다.
// 즉 boost::shared_ptr<T> p1(std::move(p))
#include <boost/chrono/duration.hpp>
int main() {
    // foo1(); // Will cause a memory leak
    foo2();
    foo3();

    // Give all the threads a chance to finish
    // Note: It is an awfull design, but it is OK
    // for example
    boost::this_thread::sleep_for(boost::chrono::seconds(2));
    return 0;
}

#include <assert.h>


void process1(const foo_class* p) {
    assert(p);
}

void process2(const foo_class* p) {
    assert(p);
}

void process3(const foo_class* p) {
    assert(p);
}

void process_str1(boost::shared_ptr<std::string> p) {
    assert(p);
}

void process_str2(const boost::shared_ptr<std::string>& p) {
    assert(p);
}
void process_sp1(const boost::shared_ptr<foo_class>& p) {
    assert(!!p);
}

void process_sp2(const boost::shared_ptr<foo_class>& p) {
    assert(!!p);
}

void process_sp3(const boost::shared_ptr<foo_class>& p) {
    assert(!!p);
}

