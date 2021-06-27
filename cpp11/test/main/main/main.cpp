#include <iostream>
#include <array>
using namespace std;
class Test
{
public:
	int ival_;
};
int main()
{
	Test t;
	t.ival_ = 200;
	int & i = t.ival_;
	i = 100;
	cout << t.ival_ << endl;
	return 0;
	std::array<int, 5> first = { 10, 20, 30, 40, 50 };
	std::array<int, 5> second = { 11, 22, 33, 44, 55 };

	first.swap(second);

	std::cout << "first:";
	for (int& x : first) std::cout << ' ' << x;
	std::cout << '\n';

	std::cout << "second:";
	for (int& x : second) std::cout << ' ' << x;
	std::cout << '\n';

	return 0;
}