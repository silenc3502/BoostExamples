void may_throw1(const char* buffer);
void may_throw2(const char* buffer);

void foo() {
	// 10 MB에 이르는 메모리는 Stack에 할당할 수 없으므로 Heap에 할당한다.
    char* buffer = new char[1024 * 1024 * 10];

	// 예외를 던질 수도 있다.
	// 포인터를 사용한다면 Memory Leak이 발생할 것이다.
    may_throw1(buffer);
    may_throw2(buffer);

    delete[] buffer;
}

#include <boost/scoped_array.hpp>

void foo_fixed() {
	// 배열을 Heap에 할당한다.
    boost::scoped_array<char> buffer(new char[1024 * 1024 * 10]);

	// 예외를 던질 수 있으나 이번에는 Memory Leak이 발생하지 않는다.
    may_throw1(buffer.get());
    may_throw2(buffer.get());

	// buffer의 소멸자가 delete[]를 호출한다.
}

// scoped_array는 scoped_ptr와 거의 비슷하게 동작하지만
// 소멸자에서 delete 대신 delete[]를 호출한다.
// unique_ptr<T[]>도 동일하게 동작한다.
#include <stdexcept>
#include <assert.h>

int main() {
    // foo(); // Leaks memory
    try {
        foo_fixed();
    } catch (...){}

    return 0;
}


void may_throw1(const char* /*buffer*/) {
    // Do nothing
}

void may_throw2(const char* /*buffer*/) {
    throw std::exception();
}
