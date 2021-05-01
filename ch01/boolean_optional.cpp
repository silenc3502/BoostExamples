#include <boost/optional.hpp>
#include <iostream>
#include <stdlib.h>

class locked_device {
    explicit locked_device(const char* /*param*/) {
        // We have unique access to device
        std::cout << "Device is locked\n";
    }

public:
    ~locked_device () {
        // Releasing device lock
    }

    void use() {
        std::cout << "Success!\n";
    }

	// Device에 대한 Lock을 얻기 위한 함수로 다양한 조건에 따라 성공할 수도 아닐 수도 있다.
    static boost::optional<locked_device> try_lock_device() {
        if (rand()%2) {
            // Failed to lock device
            return boost::none;
        }
        // Success!
        return locked_device("device name");
    }
};

int main() {
	// Boost에는 Random이라는 라이브러리가 있다.
	// stdlib.h에 rand () 함수가 있는데 왜 작성되었는지 궁금하다면
	// 'Random distribution'및 'Making a random generator'레시피를 참조하라.
    srandom(5);

    for (unsigned i = 0; i < 10; ++i) {
        boost::optional<locked_device> t = locked_device::try_lock_device();
        // optional is convertible to bool
        if (t) {
            t->use();
            return 0;
        } else {
            std::cout << "...trying again\n";
        }
    }

    std::cout << "Failure!\n";
    return -1;
}
