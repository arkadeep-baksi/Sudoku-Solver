#include <iostream>

using namespace std;

int main() {
	int num;
	while (cin >> num) {
		if (num ==0) {
			cout << ".";
		}else {
			cout << num;
		}
	}
	return 0;
}