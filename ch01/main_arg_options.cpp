#include <boost/program_options.hpp>
#include <iostream>

namespace opt = boost::program_options;

int main(int argc, char *argv[])
{
	// 옵션 변수를 만들고 "All options" 조건을 생성한다.
    opt::options_description desc("All options");

	// 옵션 추가시
	// 첫번째 파라미터는 옵션 이름
	// 두번째는 옵션 형식으로 value<>로 감싼다.
	// 세번째는 이 옵션에 대한 설명문을 넣도록 한다.
    desc.add_options()
        ("apples", opt::value<int>(), "how many apples do you have")
        ("oranges", opt::value<int>(), "how many oranges do you have")
        ("help", "produce help message")
    ;

	// 옵션을 저장할 변수
    opt::variables_map vm;

	// 인자를 파싱하여 저장한다.
    opt::store(opt::parse_command_line(argc, argv, desc), vm);

	// 옵션의 최종값을 설정한다.
    opt::notify(vm);

    if (vm.count("help")) {
		std::cout << "exe --apples='number' --oranges='number'" << std::endl;
        std::cout << desc << "\n";
        return 1;
    }

    std::cout << "Fruits count: "
        << vm["apples"].as<int>() + vm["oranges"].as<int>()
        << std::endl;
}

