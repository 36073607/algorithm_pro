#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
using namespace std;

//求最大公约数、最小公倍数
//1.辗转相除法证明过程：  直接记忆即可
// 证明辗转相除法就是证明如下关系：
	// gcd(a, b) = gcd(b, a % b)
	// 假设a % b = r，即需要证明的关系为：gcd(a, b) = gcd(b, r)
	// 证明过程：
	// 因为a % b = r，所以如下两个等式必然成立
	// 1) a = b * q + r，q为0、1、2、3....中的一个整数
	// 2) r = a − b * q，q为0、1、2、3....中的一个整数
	// 假设u是a和b的公因子，则有: a = s * u, b = t * u
	// 把a和b带入2)得到，r = s * u - t * u * q = (s - t * q) * u
	// 这说明 : u如果是a和b的公因子，那么u也是r的因子
	// 假设v是b和r的公因子，则有: b = x * v, r = y * v
	// 把b和r带入1)得到，a = x * v * q + y * v = (x * q + y) * v
	// 这说明 : v如果是b和r的公因子，那么v也是a的公因子
	// 综上，a和b的每一个公因子 也是 b和r的一个公因子，反之亦然
	// 所以，a和b的全体公因子集合 = b和r的全体公因子集合
	// 即gcd(a, b) = gcd(b, r)
	// 证明结束
//2.求gcd(a,b)，其中a>b,时间复杂度为O((log a)的3次方)，时间复杂度证明略，这个复杂度足够好了
//3.简单转化即可求最小公倍数
//4.更高效求最大公约数的Stein算法、由最大公约数扩展出的“裴蜀定理”，比赛同学有兴趣可以继续研究
//5.不比赛的同学，掌握这一行函数即可！！
class GcdAndLcm
{
	long gcd(long a, long b)
	{
		return b == 0 ? a : gcd(b, a % b);
	}
		
	long lcm(int a, int b)
	{
		return a / gcd(a, b) * b;// 先除后乘，避免溢出
	}
};



//第N个神奇数字
class NthMagicalNumber
{
	int nthMagicalNumber(int n, int a, int b)
	{
		long long ans = 0;
		long long Lcm = lcm(a, b);//a,b的最小公倍数
		//n*min(a,b)是答案的大致范围：因为n*(a,b)中任意一个都满足第n个神奇数字(这里以n*a为例，a<b)，加上另一个数b的限制，最终答案肯定比n*a小
		for (long long l = 0, r = (long long)n * min(a,b); l <= r;)//二分答案法,注意边界检查l==r
		{
			long m = (l + r) / 2;
			if (m / a + m / b - m / Lcm >= n)
			{
				ans = m;
				r = m - 1;
			}
			else
				l = m + 1;
		}

		return (int)(ans % 1000000007);
	}

	long long gcd(long long a, long long b)
	{
		return b == 0 ? a : gcd(b, a % b);
	}
	
	long long lcm(long long a, long long b)
	{
		return a / gcd(a, b) * b;
	}

};



//同余原理
//1.加法、乘法每一步计算完后直接取模，减法则为(a-b+m)%m
//2.要确保过程中不溢出，所以往往乘法运算时用long类型1做中间变量
//3.除法的同余需要求逆元
