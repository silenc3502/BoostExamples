#include <stdexcept>
class foo_class {
public:
    char data[100000000];

    explicit foo_class(const char* /*param*/){}
};

bool g_exit_on_first_function = true;
bool some_function1(foo_class* /*param*/) {
    return g_exit_on_first_function;
}

void some_function2(foo_class* /*param*/) {
    throw std::exception();
}

// 동적 메모리를 할당한 후 해당 메모리에 클래스를 만들어야 할 때가 있다.
// 문제는 바로 이 작업에서 발생한다.
bool foo1() {
    foo_class* p = new foo_class("Some initialization data");

	// some_function1 혹은 some_function2가 예외를 던진다면 p는 삭제되지 않는다.
    const bool something_else_happened = some_function1(p);
    if (something_else_happened) {
        delete p;
        return false;
    }

    some_function2(p);
    delete p;
    return true;
}

// 위에서 발생하던 문제를 해결하였다.
// 간결하면서도 이와 같이 문제를 해결할 수는 없을까 ?
bool foo2() {
    foo_class* p = new foo_class("Some initialization data");
    try {
        const bool something_else_happened = some_function1(p);
        if (something_else_happened) {
            delete p;
            return false;
        }

       some_function2(p);
    } catch (...) {
        delete p;
        throw;
    }

    delete p;
    return true;
}


#include <boost/scoped_ptr.hpp>

// boost::scoped_ptr은 전형적인 RAII 클래스로
// 예외가 던져지거나 변수가 자신의 영역을 벗어나게 되면 스택이 풀리고 소멸자가 호출된다.
bool foo3() {
    const boost::scoped_ptr<foo_class> p(new foo_class("Some initialization data"));

    const bool something_else_happened = some_function1(p.get());
    if (something_else_happened) {
       return false;
    }

    some_function2(p.get());
    return true;
}


#include <boost/move/make_unique.hpp>

// 소멸자에서 boost::scoped_ptr과 boost::unique_ptr은
// 자신이 저장하고 있는 포인터에 대한 delete를 호출한다.
// 이들 클래스는 기본적으로 delete를 호출하기 때문에
// 기본 클래스의 소멸자가 가상일 경우
// 파생 클래스는 기본 클래스에 대한 포인터로 저장하는 것이 안전하다.
bool foo3_1() {
    const boost::movelib::unique_ptr<foo_class> p
        = boost::movelib::make_unique<foo_class>("Some initialization data");

    const bool something_else_happened = some_function1(p.get());
    if (something_else_happened) {
       return false;
    }
    some_function2(p.get());
    return true;
}

#include <assert.h>

// boost::scoped_ptr 클래스 템플릿은 복사할 수도 있고 이동할 수도 있다.
// boost::unique_ptr은 이동만 가능하며 C++11 이전이라면 이를 에뮬레이션 한다.
// 두 클래스 모두 자신이 소유한 자원에 대한 포인터를 저장한다.
int main() {
    try { foo2(); } catch(...){ assert(false); }
    try { foo3(); } catch(...){ assert(false); }
    try { foo3_1(); } catch(...){ assert(false); }

    g_exit_on_first_function = false;
    try { foo2(); assert(false); } catch(...){}
    try { foo3(); assert(false); } catch(...){}
    try { foo3_1(); assert(false); } catch(...){}
}

