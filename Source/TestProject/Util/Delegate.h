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
		// std::function::target ���ø� ��� �Լ��� ����� ��,
		// ���ø� Ŭ������ ���ø� ��� �Լ��� ȣ���ϰ� �Ǹ� �����Ϸ��� Ÿ���� �߷����� ����
		// �����Ϸ����� ���ø� �Լ���� ���� ��������� �˷��ֱ� ���� template Ű���� ���
		void (* const* func_ptr)(ARGS...) = func.template target<void(*)(ARGS...)>();
		const std::size_t func_hash = func.target_type().hash_code();

		// Ŭ���� ��� �Լ��� ������ ��� �ؽ� �ڵ带 ��
		// <void(*)(ARGS...)> Ÿ���� �ƴϱ� ������ target�� �̿��� �Լ� �����͸� ������ nullptr�� ��ȯ��
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
		// �� �̿� �Ϲ� �Լ��� ��� ���� ������ �Լ��� ���� �ؽ� �ڵ带 ��ȯ��
		// <void(*)(ARGS...)> Ÿ���̱� ������ �Լ� �����ͷ� �� ����
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