#include <iostream>

using namespace std;

struct nullopt_t
{};

nullopt_t nullopt;

struct in_place_t
{};

in_place_t in_place;

template <typename T>
struct optional
{
	optional():
		valid(false)
	{}
	
	optional(T rhs):
		valid(true)
	{
		*(reinterpret_cast <T*> (&data)) = std::move(rhs);
	}
	
	optional(nullopt_t):
		valid(false)
	{}
	
	template <typename... Args>
	optional(in_place_t, Args&&... args):
		valid(true)
	{
		new(&data) T(std::forward <Args> (args)...);
	}

	optional(optional const& rhs):
		valid(rhs.valid)
	{
		if (rhs.valid)
		{
			*(reinterpret_cast <T*> (&data)) = *(reinterpret_cast <const T*> (&rhs.data));
		}
	}

	optional(optional&& rhs):
		valid(rhs.valid)
	{
		if (rhs.valid)
		{
			*(reinterpret_cast <T*> (&data)) = std::move(*(reinterpret_cast <T*> (&rhs.data)));
		}
	}

	optional& operator=(optional const& rhs)
	{
		for_copy(*(reinterpret_cast <const T*> (&rhs.data)), rhs.valid);
		return *this;
	}
	
	optional& operator=(optional&& rhs)
	{
		for_copy(std::move(reinterpret_cast <T*> (&rhs.data)), rhs.valid);
		return *this;
	}

	optional& operator=(nullopt_t)
	{
		reset();
	}
	
	const T& operator*() const&
	{
		return *(reinterpret_cast <const T*> (&data));
	}
	
	T& operator*() &
	{
		return *(reinterpret_cast <T*> (&data));
	}

	const T&& operator*() const&&
	{
		return *(reinterpret_cast <const T*> (&data));
	}

	T&& operator*() &&
	{
		return *(reinterpret_cast <T*> (&data));
	}

	const T* operator->() const
	{
		return reinterpret_cast <const T*> (&data);
	}

	T* operator->()
	{
		return reinterpret_cast <T*> (&data);
	}
	
	explicit operator bool()
	{
		return valid;
	}
	
	template <typename U>
	T value_or(U&& val)
	{
		if (valid)
		{
			return *(reinterpret_cast <T*> (&data));
		}
		return std::forward <U> (val);
	}

	template <typename... Args>
	void emplace(Args&&... args)
	{
		reinterpret_cast <T*> (&data)->~T();
		new (&data) T(std::forward <Args> (args)...);
		valid = true;
	}

	void reset()
	{
		if (valid)
		{
			reinterpret_cast <T*> (&data)->~T();
		}
		valid = false;
	}

	void swap(optional& other)
	{
		if (other.valid && valid)
		{
			swap(*(reinterpret_cast <T*> (&data)), *(reinterpret_cast <T*> (&other.data)));
		}
		if (other.valid && !valid)
		{
			*(reinterpret_cast <T*> (&data)) = *(reinterpret_cast <T*> (&other.data));
			reinterpret_cast <T*> (&other.data)->~T();
		}
		if (valid && !other.valid)
		{
			*(reinterpret_cast <T*> (&other.data)) = *(reinterpret_cast <T*> (&data));
			reinterpret_cast <T*> (&data)->~T();
		}
		swap(valid, other.valid);
	}

	~optional()
	{
		reinterpret_cast <T*> (&data)->~T();
	}

	private:

	template <typename U>
	void for_copy(U&& rhs, bool rvalid)
	{
		if (!rvalid)
		{
			if (valid)
			{
				reinterpret_cast <T*> (&data)->~T();
				valid = false;
			}
		}
		else
		{
			*(reinterpret_cast <T*> (&data)) = std::forward <U> (rhs);
			valid = true;
		}
		valid = rvalid;
	}
	
	typename std::aligned_storage<sizeof(T), alignof(T)>::type data;
	bool valid;
};

template <typename T>
bool operator ==(const optional<T>& lhs, const optional<T>& rhs)
{
	return *lhs == *rhs;
}
template <typename T>
bool operator !=(const optional<T>& lhs, const optional<T>& rhs)
{
	return *lhs != *rhs;
}
template <typename T>
bool operator >(const optional<T>& lhs, const optional<T>& rhs)
{
	return *lhs > *rhs;
}
template <typename T>
bool operator <(const optional<T>& lhs, const optional<T>& rhs)
{
	return *lhs < *rhs;
}
template <typename T>
bool operator >=(const optional<T>& lhs, const optional<T>& rhs)
{
	return *lhs >= *rhs;
}
template <typename T>
bool operator <=(const optional<T>& lhs, const optional<T>& rhs)
{
	return *lhs <= *rhs;
}

struct my_big_type
{
	int a, b, c, d;
	my_big_type()
	{}

	my_big_type(int _a, int _b, int _c, int _d):
		a(_a), b(_b), c(_c), d(_d)
	{}
};

int main()
{
	optional <int> opt;
	optional <my_big_type> opt1(in_place, 1, 2, 3, 4);
	opt1->a = 3;
	opt.emplace(1);
	optional <double> opt2;
	*opt1 = my_big_type(4, 3, 2, 1);
	cout << opt1->a << ' ' << opt1->b << ' ' << opt1->c << ' ' << opt1->d << endl;
	opt1.reset();
	my_big_type tmp = opt1.value_or(my_big_type(0, 0, 0, 0));
	cout << tmp.a << ' ' << tmp.b << ' ' << tmp.c << ' ' << tmp.d << endl;
	if (opt)
	{
		cout << "Opt" << endl;
	}
	else
	{
		cout << "No opt" << endl;
	}

	if (opt1)
	{
		cout << "Opt1" << endl;
	}
	else
	{
		cout << "No opt1" << endl;
	}

	if (opt2)
	{
		cout << "Opt2" << endl;
	}
	else
	{
		cout << "No opt2" << endl;
	}

	optional <int> smth(in_place, 4);
	smth = opt;
	optional <int> opt5(opt);
	cout << (smth <= opt) << endl;
	return 0;
}
