/************************************************
 * @author Liu Yunfei
 * @stuID  BY1706126
 * @email  lyunfei@buaa.edu.cn
 * All rights reserved
 ***********************************************/
#include<iostream>
#include<cmath>
#include<vector>
#include <algorithm>

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

	// Function name: Find the K'st Minimum Number in a List
	// Realized by quick sort method 
	// The complexity of this function is 
	template<typename T>
	inline void mySwap(T& a, T&b)
	{
		T c = a;
		a = b;
		b = c;
	}
	template<typename T>
	T findTheKstMinNum(vector<T> a, int l, int r, int k)
	{
		if (l < r)
		{
			T item = a[l];
			int i = l + 1, j = r;
			while (true)
			{
				while (a[i] < item) i++;
				while (a[j] > item) j--;
				if (i > j)
					break;
				mySwap<T>(a[i++], a[j--]);
			}
			mySwap<T>(a[j], a[l]);
			if (j < k)
				findTheKstMinNum<T>(a, j + 1, r, k);
			else if (j > k)
				findTheKstMinNum<T>(a, l, j - 1, k);
			else
				return a[k];
		}
		return -3444444;
	}
	// Realized by k-min heap method 
	// The complexity of this function is 
	template<typename T>
	T findTheKstMinNum(vector<T> a, int k)
	{
		T b = a[0];
		return b;
	}

}

int main()
{
	// Main algorithm structure here
	int x = 5;
	int n = 3;
	cout << "Power(" << x << "," << n << ") = " << test::dncPower<int>(x, n) << endl;
	// Find the k'st minimal number by the quick sort
	vector<int> a = { 2, 5, 6, 8, 3, 0, 6, 2, 7, 8, 34, 23,-1 };
	int rlt = test::findTheKstMinNum<int>(a, 0, a.size() - 1, 3);
	sort(a.begin(), a.end(), [](int x, int y) {return x < y; });
	for (auto elem : a)
	{
		cout << "  " << elem;
	}
	cout << "\nThe " << 3 << "'s result is " << rlt << endl;
	getchar();
}
