#include "Optional.h"

using namespace std;

struct my_big_type
{
	int a, b, c, d;
	my_big_type()
	{}

	my_big_type(int _a, int _b, int _c, int _d):
		a(_a), b(_b), c(_c), d(_d)
	{}
};

struct my_type
{
	my_type();
	private:
	optional <int> t;
};

int get();

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

	cout << &in_place << endl;

	int a = get();
	my_type c;
	return 0;
}
