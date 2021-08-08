#pragma once

#include <list>
#include <functional>

template <class... ARGS>
class Delegate
{
public:
	typedef typename std::list<std::function<void(ARGS...)>>::iterator iterator;

	void operator () (const ARGS... args)
	{
		for (auto& func : functions)
		{
			func(args...);
		}
	}

	Delegate& operator += (const std::function<void(ARGS...)>& func)
	{
		functions.push_back(func);
		return *this;
	}

	Delegate& operator -= (const std::function<void(ARGS...)>& func)
	{		
		// std::function::target 템플릿 멤버 함수를 사용할 때,
		// 템플릿 클래스의 템플릿 멤버 함수를 호출하게 되면 컴파일러가 타입을 추론하지 못함
		// 컴파일러에게 템플릿 함수라는 것을 명시적으로 알려주기 위해 template 키워드 사용
		void (* const* func_ptr)(ARGS...) = func.template target<void(*)(ARGS...)>();
		const std::size_t func_hash = func.target_type().hash_code();

		// 클래스 멤버 함수나 람다의 경우 해쉬 코드를 비교
		// <void(*)(ARGS...)> 타입이 아니기 때문에 target을 이용해 함수 포인터를 얻으면 nullptr를 반환함
		if (nullptr == func_ptr)
		{
			for (auto itr = functions.begin(); itr != functions.end(); itr++)
			{
				if (func_hash == (*itr).target_type().hash_code())
				{
					functions.erase(itr);
					return *this;
				}
			}
		}
		// 그 이외 일반 함수의 경우 같은 포맷의 함수는 같은 해쉬 코드를 반환함
		// <void(*)(ARGS...)> 타입이기 때문에 함수 포인터로 비교 가능
		else
		{
			for (auto itr = functions.begin(); itr != functions.end(); itr++)
			{
				void (* const* delegate_ptr)(ARGS...) = (*itr).template target<void(*)(ARGS...)>();
				if (nullptr != delegate_ptr && *func_ptr == *delegate_ptr)
				{
					functions.erase(itr);
					return *this;
				}
			}
		}
		return *this;
	}

	iterator begin() noexcept
	{
		return functions.begin();
	}
	iterator end() noexcept
	{
		return functions.end();
	}
	void clear()
	{
		functions.clear();
	}

private:
	std::list<std::function<void(ARGS...)>> functions;
};