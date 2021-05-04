#include <boost/scope_exit.hpp>
#include <cstdlib>
#include <cstdio>
#include <cassert>

int main() {
    std::FILE* f = std::fopen("example_file.txt", "w");
    assert(f);
    BOOST_SCOPE_EXIT(f) {
		// BOOST_SCOPE_EXIT 위에서 무슨 일이 일어나든 이 코드는 무조건 실행된다.
		// 그러므로 open한 파일 디스크립터 f를 안전하게 닫을 수 있다.
        std::fclose(f);
    } BOOST_SCOPE_EXIT_END

	// 예외 발생을 유발할 수 있는 코드를 여기에 뱇한다.
    // ...
}

class theres_more_example {
public:
    void close(std::FILE*);

    void theres_more_example_func() {
        std::FILE* f = 0;
        BOOST_SCOPE_EXIT(f, this_) { // Capture object `this_`.
            this_->close(f);
        } BOOST_SCOPE_EXIT_END
    }
};
