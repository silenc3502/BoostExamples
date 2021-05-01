#include <boost/any.hpp>
#include <vector>
#include <string>
#include <typeinfo>
#include <algorithm>
#include <iostream>

// 컴파일시 boost::variant는 거대한 switch문을 만들고
// switch 조건의 개별 값에 대한 case문에 적합한 값을
// 가변형 변수의 형식 목록에서 찾기 위한 visitor를 호출한다.
typedef boost::any cell_t;
typedef std::vector<cell_t> db_row_t;

db_row_t get_row(const char* /*query*/) {

    db_row_t row;
    row.push_back(10);
    row.push_back(10.1f);
    row.push_back(std::string("hello again"));

    return row;
}

// visitor가 아무것도 변환하지 않는다면
// boost::static_visitor<>에 템플릿 파라미터를 제공할 수 없다.
struct db_sum: public std::unary_function<boost::any, void> {
private:
    double& sum_;

public:
	// explicit을 사용하여 형변환을 위한 생성자가 자동으로 호출되지 않도록 한다.
    explicit db_sum(double& sum)
        : sum_(sum)
    {}

	// 함수 호출을 Overloading 한 것으로 for_each를 돌면서 호출된다.
    void operator()(const cell_t& value) {
        const std::type_info& ti = value.type();
        if (ti == typeid(int)) {
            sum_ += boost::any_cast<int>(value);
        } else if (ti == typeid(float)) {
            sum_ += boost::any_cast<float>(value);
        }
    }
};

int main() {
    db_row_t row = get_row("Query: Give me some row, please.");
    double res = 0.0;
    std::for_each(row.begin(), row.end(), db_sum(res));
    std::cout << "Sum of arithmetic types in database row is: " << res << std::endl;
}
