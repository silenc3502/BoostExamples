# How to compile manually

gcc *.cpp -lboost_program_options  

# How to execute  

./a.out --apple=3 --orange=7  

# If need some config file ?

# Flow
1. main_arg_options  
2. main_options_config_file  
3. any_object  
4. variant_union  
5. explicit_any_sum  
6. boolean_optional  
7. array_return  
8. tuple_multitype_pair  
9. bind_adapter  
10. type_index  
11. rvalue_boost_move  
12. noncopyable_test  
13. boost_algorithm_test  

# boost::any type erasure
```cpp
template<typename ValueType>
struct holder : public placeholder {
	virtual const std::type_info& type() const {
		return typeid(ValueType);
	}
	ValueType held;
};

struct placeholder {
	virtual ~placeholder();
	virtual const std::type_info& type() const = 0;
};

형식이 T인 변수를 할당하면 Boost::Any는 내부 기본형 placeholder에서 파생됐고
명시된 형식 T를 저장할 수 있는 형식인 holder<T>를 생성한다.

placeholder형은 저장된 형식 T의 std::type_info를 얻어오거나
저장된 형식을 복사할 수 있는 인터페이스를 갖는다.

boost::any는 placeholder에 대한 포인터인 ptr을 갖고 있다.
any_cast<T>()가 호출되면 boost::any는 ptr->type()을 호출해
std::type_info가 typeid(T)와 같은지 검사한 후 static_cast<holder<T>*>(ptr)->held를 반환한다.

인터페이스가 많기 때문에 성능에 민감하다면
C 스타일의 인라인 어셈이 적용된 매크로 함수를 사용하는 것이 가장 베스트하지만 생산성은 저하될 것이다.
```
