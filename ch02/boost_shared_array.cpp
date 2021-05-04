#include <cstring>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

void do_process(const char* data, std::size_t size);

void do_process_in_background(const char* data, std::size_t size) {
	// caller가 언제 데이터를 할당 해제할지 알 수 없으므로 데이터를 복사해야 한다.
    char* data_cpy = new char[size];
    std::memcpy(data_cpy, data, size);

	// 데이터를 처리하기 위해 Thread를 시작한다.
    boost::thread(boost::bind(&do_process, data_cpy, size));

	// do_process가 data_cpy를 처리하고 있을지도 모르므로
	// delete[] data_cpy를 사용할 수 없다.
}

#include <boost/shared_array.hpp>
void do_process_shared_array(const boost::shared_array<char>& data, std::size_t size) {
    do_process(data.get(), size);
}

void do_process_in_background_v1(const char* data, std::size_t size) {
	// caller에 의해 언제 할당 해제 될지 모르므로 데이터를 복사할 필요가 있다.
    boost::shared_array<char> data_cpy(new char[size]);
    std::memcpy(data_cpy.get(), data, size);

	// 데이터를 처리하기 위해 Thread를 시작한다.
    boost::thread(boost::bind(&do_process_shared_array, data_cpy, size))
            .detach();

	// data_cpy에 대해 delete []를 호출 할 필요가 없다.
	// data_cpy 소멸자는 참조 횟수가 0 일 때 데이터를 할당 해제하기 때문이다.
}


#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

void do_process_shared_ptr(
        const boost::shared_ptr<char[]>& data,
        std::size_t size)
{
    do_process(data.get(), size);
}

void do_process_in_background_v2(const char* data, std::size_t size) {
	// 첫번째 솔루션보다 빠름
    boost::shared_ptr<char[]> data_cpy = boost::make_shared<char[]>(size);
    std::memcpy(data_cpy.get(), data, size);

	// 데이터 처리를 위한 스레드 실행
    boost::thread(boost::bind(&do_process_shared_ptr, data_cpy, size))
            .detach();

	// data_cpy 소멸자는 참조 횟수가 0 일 때 데이터 할당을 해제합니다.
}

void do_process_shared_ptr2(
        const boost::shared_ptr<char>& data,
        std::size_t size)
{
    do_process(data.get(), size);
}

void do_process_in_background_v3(const char* data, std::size_t size) {
	// 첫번째 솔루션과 속도는 동일하다.
    boost::shared_ptr<char> data_cpy(
                new char[size],
                boost::checked_array_deleter<char>()
    );
    std::memcpy(data_cpy.get(), data, size);

    boost::thread(boost::bind(&do_process_shared_ptr2, data_cpy, size))
            .detach();
}

#include <boost/chrono/duration.hpp>
int main () {
    // do_process_in_background(); // Will cause a memory leak
    char ch[] = "Hello dear reader.";
    do_process_in_background_v1(ch, sizeof(ch));
    do_process_in_background_v2(ch, sizeof(ch));
    do_process_in_background_v3(ch, sizeof(ch));

    // Give all the threads a chance to finish
    // Note: It is an awfull design, but it is OK
    // for example
    boost::this_thread::sleep_for(boost::chrono::seconds(2));
    return 0;
}

void do_process(const char* data, std::size_t size) {
    assert(size);
    assert(data);
}
