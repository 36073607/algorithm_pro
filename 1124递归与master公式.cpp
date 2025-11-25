#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cmath>
using namespace std;
//所有递归均可通过画 递归调用图 理解
//任何递归函数都一定可以改成非递归，不用系统帮你压栈（系统栈空间），自己压栈呗（内存空间）
//eg:
int maxValue(int* arr, int l, int r)
{
	if (l == r)
		return arr[l];
	int mid = (l + r) / 2;
	int lmax = maxValue(arr, l, mid);
	int rmax = maxValue(arr, mid + 1, r);
	return max(lmax, rmax);
}

//递归改成非递归的必要性：
//1.工程上几乎一定要改，除非确定数据量再大递归也一定不深，例如归并排序、快速排序、线段树、很多平衡树等
//2.算法笔试或者比赛中，能通过就不改



//master公式
//1.所有子问题规模相同的递归才能用master公式
//  T(n) = a * T(n/b) + O(n^c) ,  a,b,c都是常数 , n表示数据规模
//  a: 子过程调用几次
//  b: 调用一次减少的数据规模，例如eg中的递归，b=2
//  O(n^c)：除了子过程调用之外，代码中其他行为的时间复杂度
// 
// eg中的时间复杂度：a=2,b=2,c=0
// T(n) = 2 * T(n/2) + O(1)
//
//eg变式：在左侧1/3区域求最大值，再在右侧2/3区域求最大值，然后求出总的最大值
//此类问题不符合“所有子问题规模相同”，所以不能使用master公式

//2.使用master公式时不用管展开，只需看一层即可
//a.如果 log(b,a) < c ,复杂度为: O(n^c)
//b.如果 log(b,a) > c ,复杂度为: O(n^log(b,a))
//c.如果 log(b,a) == c ,复杂度为: O(n^c * logn)
//log(b,a) : log以b为底，a为真数
// 
// eg: T(n) = 2 * T(n/2) + O(1) , a=2,b=2,c=0
// log(b,a) = 1 > c = 0  ,  所以时间复杂度为：O(n^log(b,a))=O(n)

//3.补充：
// T(n) = 2*T(n/2) + O(n * logn) , 时间复杂度为：O( n * ((logn)的平方) )

