#include <boost/program_options.hpp>
#include <iostream>

namespace opt = boost::program_options;

int main(int argc, char *argv[])
{
    opt::options_description desc("All options");

    int oranges_var = 0;
    desc.add_options()
		// ProgramOptions 라이브러리는 포인터로 전달받은 변수에 옵션 값을 저장한다.
		// --oranges 옵션은 oranges_var에 저장된다.
        ("oranges,o", opt::value<int>(&oranges_var)->required(), "oranges that you have")

		// name 옵션은 required()가 표시되지 않았기 때문에
		// 반드시 이 값을 꼭 설정하지 않아도 된다.
        ("name", opt::value<std::string>(), "your name")

		// 'a'는 apple에 대한 약어이므로 -a 33 형태로 적어도 된다.
		// 또한 디폴트 값이 설정되었으므로 별도의 값이 없다면 기본값으로 사용된다.
        ("apples,a", opt::value<int>()->default_value(10), "apples that you have")
        ("help", "produce help message")
    ;

    opt::variables_map vm;

	// 커맨드 옵션을 파싱해 vm에 값을 저장한다.
    opt::store(opt::parse_command_line(argc, argv, desc), vm);

	// opt::store와 함께 parse_environment 메서드를 사용하여 환경 변수를 파싱할 수도 있다.
    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }

	// apples_oranges.cfg 구성 파일에서 빠진 옵션을 추가한다.
    try {
        opt::store(opt::parse_config_file<char>("apples_oranges.cfg", desc), vm);
    } catch (const opt::reading_file& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    try {
		// 필수 옵션이 빠져 있다면 opt::required_option 예외를 던진다.
        opt::notify(vm);

    } catch (const opt::required_option& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return 2;
    }

    if (vm.count("name")) {
        std::cout << "Hi," << vm["name"].as<std::string>() << "!\n";
    }
    std::cout << "Fruits count: "
        << vm["apples"].as<int>() + vm["oranges"].as<int>()
        << std::endl;
}
