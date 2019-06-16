#include <iostream>
using namespace std;

int main()
{
	int a;
	int* pa;

	a=5;
	pa=&a;

	cout << "Value: " << a <<endl;
    cout << "Address: " << pa <<endl;
    cout << "Value from address: " << *pa <<endl;
    cout << "Address from variable: " << &a <<endl;
    return 0;
}
