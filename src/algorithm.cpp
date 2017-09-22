/************************************************
 * @author Liu Yunfei
 * @stuID  BY1706126
 * @email  lyunfei@buaa.edu.cn
 * All rights reserved
 ***********************************************/
#include<iostream>
#include<cmath>
#include<vector>

using namespace std;

namespace test
{
	// Test function and modules here

	// The complexity of this divide-and-conquer function is 
	// O(lg n)
	template<typename T>
	T dncPower(T x, T n)
	{
		T rlt;
		// bounder condition
		if (n == 1) return x;
		// divide problem into sub-problems recursively
		if (n % 2 == 0) rlt = dncPower(x, n / 2) * dncPower(x, n / 2);
		else rlt = dncPower(x, (n + 1) / 2) * dncPower(x, (n - 1) / 2);
		
		return rlt;
	}

}

int main()
{
	// Main algorithm structure here
	int x = 5;
	int n = 3;
	cout << "Power(" << x << "," << n << ") = " << test::dncPower<int>(x, n) << endl;
	getchar();
}
