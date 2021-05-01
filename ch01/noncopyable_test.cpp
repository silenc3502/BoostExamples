// C++ 프로그램을 만들다 보면 복사 생성자나
// 이동 할당 연산자를 지원하기 위해
// 많은 작업을 해야하거나 여러 가지 기술적 이유로
// 복사할 수 없는 자원을 갖고 있는 상황이 발생하게 된다.
class descriptor_owner {
    void* descriptor_;

public:
    explicit descriptor_owner(const char* params);

    ~descriptor_owner() {
        // system_api_free_descriptor(descriptor_);
    }
};

descriptor_owner::descriptor_owner(const char* ){}

// 복사 생성자와 할당 연산자를 생성하므로
// descriptor_owner 클래스의 사용자가 아래같이 대형 사고를 치게 된다.
void foo() {
    descriptor_owner d1("O_o");
    descriptor_owner d2("^_^");

	// d2가 가진 descriptor_owner가 올바르게 해제되지 않는다.
    d2 = d1;

	// d2의 소멸자가 descriptor_owner를 해제한다.
	// d1의 소멸자가 이미 해제된 descriptor_owner를 해제하려고 한다.
}

#include <boost/noncopyable.hpp>

// 위와 같은 대참사를 방지하기 위해 boost::noncopyable 클래스를 활용한다.
// 이 클래스에서 자신의 클래스를 파생시키면 복사생성자와 할당연산자를 생성하지 않는다.
class descriptor_owner_fixed : private boost::noncopyable {
    // ...
public:
    descriptor_owner_fixed(const char* params){ (void)params; }
};

void foo2() {
    descriptor_owner_fixed d1("O_o");
    descriptor_owner_fixed d2("^_^");

	// 아래와 같은 코드는 컴파일 자체가 막힌다.
    //d2 = d1;
    //descriptor_owner_fixed d3(d1);
}

// 결국 이 방식의 좋은 점은 실수를 방지할 수 있게 해주면서
// 동시에 복사 생성자 정의는 다른데 있는가 ? 표준이 아닌 복사 생성자를 갖는가 ?
// 와 같은 별도의 질문을 받지 않아도 작업을 할 수 있게 된다.
// (이를 설명하기 위한 별도의 문서가 없어도 된다는 의미)

struct noncopyable_or_not {
private:
    noncopyable_or_not(const noncopyable_or_not&);
    noncopyable_or_not& operator=(const noncopyable_or_not&);

public:
    noncopyable_or_not(){}
    noncopyable_or_not(noncopyable_or_not&){}
    noncopyable_or_not& operator=(noncopyable_or_not&){ return *this; }
};

struct noncopyable_or_not2 {
private:
    noncopyable_or_not2(noncopyable_or_not2&);
    noncopyable_or_not2& operator=(noncopyable_or_not2&);

public:
    noncopyable_or_not2(){}
    noncopyable_or_not2(const noncopyable_or_not2&){}
    noncopyable_or_not2& operator=(const noncopyable_or_not2&) { return *this; }
};

int main()
{
    foo();
    foo2();

    noncopyable_or_not non1;
    noncopyable_or_not non2 = non1;

    const noncopyable_or_not2 n1;
    noncopyable_or_not2 n2 = n1;

    // Supressing warnings about unused variables
    (void)non2;
    (void) n2;

    return 0;
}

