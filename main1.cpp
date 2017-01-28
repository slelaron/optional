#include "Optional.h"

using namespace std;

int get()
{
	return 1;
}

struct my_type
{
	my_type();
	private:
	optional <int> t;
};

my_type::my_type():
	t(in_place, 1)
{
	cout << &in_place << endl;
}
