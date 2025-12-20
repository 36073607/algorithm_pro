#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
//题目一：判断一个整数是不是2的幂
//二的幂在他的二进制里只有一个1
//0001 -- 2^0
//0010 -- 2^1
//0100 -- 2^2
//思路：利用Brian Kernighan算法，提取出二进制里最右侧的1
//      判断这个状态是否与原数相等
bool isPowerOfTwo(int n)
{
	return n > 0 && n == (n & -n);
	//return n > 0 && (n & (n - 1)) == 0;
}



//题目二：判断一个整数是不是3的幂
//如果一个数字是3的某次幂，那么这个数一定只含有3这个质数因子
//1162261467是int型范围内，最大的3的幂，它是3的19次方
//这个1162261467只含有3这个质数因子，如果n也是只含有3这个质数因子，
//那么：1162261467 % n == 0
//反之，如果1162261467 % n != 0 ,说明n一定含有其他因子
bool isPowerOfThree(int n)
{
	return n > 0 && 1162261467 % n == 0;
}



//题目三：已知n是非负数，返回大于等于n的最小的2的幂
//c++无符号右移，要先正常无符号整型(unsigned)  （通过将有符号整数强制转换为无符号类型来实现无符号右移）
int near2Power(int n)
{
	if (n <= 0)
		return 1;

	n--;//这里减一防一手n本身就是2的幂
	//下面这些操作可以将n的右侧都刷成1
	//eg:
	//n  00010010
	//-->00011111
	n |= (unsigned)n >> 1;
	n |= (unsigned)n >> 2;
	n |= (unsigned)n >> 4;
	n |= (unsigned)n >> 8;
	n |= (unsigned)n >> 16;

	return n + 1;   //00100000
}
//如果int范围内不存在这样的数（例如n为int范围最大值），则返回int范围内最小值



//题目四：区间[left, right]内所有数字 & 的结果
//暴力遍历太慢了
int rangeBitwiseAnd(int left, int right)
{
	while (left < right)//如果left比right小，那么right最右侧的1一定留不下来
		right -= right & -right;//right不断减去二进制状态下最右侧的1

	return right;
}



//题目五：逆序反转一个二进制状态
unsigned int ReverseBits1(unsigned int n)
{
	unsigned int ans = 0;
	for (int i = 0; i < 32; i++)
	{
		unsigned int bit = (n >> i) & 1;//取出n当前最低位
		ans |= bit << (31 - i);//放到ans的(31-i)位
	}
	return ans;
}

unsigned int ReverseBits2(unsigned int n)
{
	unsigned int ans = 0;
	for (int i = 31; i >= 0; i--)
	{
		ans |= (n & 1) << i;//取n的当前最低位，放到ans的对应高位
		n >>= 1;
	}

	return ans;
}

//eg:
// abcdefgh
// ba dc fe hg
// dc ba  hg fe
// hg fe dc ba
unsigned int ReverseBits_Pro(int n)//（类似于归并排序的递归写法）
{
	n = ((n & 0xaaaaaaaa) >> 1) | ((n & 0x55555555) << 1);
	n = ((n & 0xcccccccc) >> 2) | ((n & 0x33333333) << 2);
	n = ((n & 0xf0f0f0f0) >> 4) | ((n & 0x0f0f0f0f) << 4);
	n = ((n & 0xff00ff00) >> 8) | ((n & 0x00ff00ff) << 8);
	n = (n >> 16) | (n << 16);

	return n;
}



//题目六：返回一个数二进制状态下有几个1
int countOne(unsigned int n)
{
	int ans = 0;
	for (int i = 0; i < 32; i++)
	{
		if (((n >> i) & 1) == 1)
			ans++;
	}

	return ans;
}

int countOne_Pro(int n)
{
	n = (n & 0x55555555) + (((unsigned)n >> 1) & 0x55555555);
	n = (n & 0x33333333) + (((unsigned)n >> 2) & 0x33333333);
	n = (n & 0x0f0f0f0f) + (((unsigned)n >> 4) & 0x0f0f0f0f);
	n = (n & 0x00ff00ff) + (((unsigned)n >> 8) & 0x00ff00ff);
	n = (n & 0x0000ffff) + ((unsigned)(n >> 16) & 0x0000ffff);
	return n;
}
//补充：
//两个整数之间的 汉明距离 指的是这两个数字对应二进制位不同的位置的数目。  
//给你两个整数 x 和 y，计算并返回它们之间的汉明距离  
int hammingDistance(int x, int y)
{
	return countOne_Pro(x ^ y);
}


//条件判断相比于赋值、位运算、算术运算是稍慢的。所以题目五、六的pro解法有现实意义（直接当模板用即可）



