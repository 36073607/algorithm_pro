#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
using namespace std;
//一.异或运算性质
//1.异或运算就是无进位相加
// eg:
//    A: 01101110
//    B: 10011101
//  A+B: 11110011
// 
//2.异或运算满足交换律、结合律，也就是同一批数字，不管异或顺序是什么，最终结果都是一个
//3. 0^n = n , n^n = 0
//4.整体异或和如果是x，整体中某个部分的异或和如果是y，那么剩下部分的异或和就是x^y
//  a^b=c -->> a = c^b , b = c^a
// 
//这些结论最重要就是结论1，其他所有结论都可以由这个结论推论得到
//其中结论4相关的题目最多，利用区间上异或和的性质


//题目一：交换两个数
void my_swap(int a, int b)
{
	a = a ^ b;
	b = a ^ b;
	a = a ^ b;
}
//特别提示：
//这么写的前提是a,b都有自己的内存空间
//例如:
void my_swap(int* arr, int i, int j)
{
	arr[i] = arr[i] ^ arr[j];
	arr[j] = arr[i] ^ arr[j];
	arr[i] = arr[i] ^ arr[j];
}
//当i!=j，没问题，会完成交换功能
//当i==j，会出错，因为arr[i]和arr[j]指向同一块内存空间，n^n=0
//所以知道这种写法即可，并不推荐



//题目二：不用任何判断语句和比较操作，返回两个数的最大值
int flip(int n)//必须保证n一定是0或者1
{
	return n ^ 1;//0变1，1变0
}

//非负数返回1
//负数返回0
int sign(int n)
{
	return flip((unsigned)n >> 31);
}

int	getMax(int a, int b)//有溢出风险
{
	int c = a - b;
	//c非负，returnA-->>1
	//c为负，returnA-->>0
	int returnA = sign(c);
	int returnB = flip(returnA);//对returnA取反，returnA和returnB互斥
	//c非负，returnB-->>0
	//c为负，returnB-->>1

	return a * returnA + b * returnB;
}


//改进版本，无溢出风险
int getMax_pro(int a, int b)
{
	int c = a - b;//c有可能溢出
	int sa = sign(a);//a的符号
	int sb = sign(b);//b的符号
	int sc = sign(c);//c的符号

	int diffAB = sa ^ sb;//判断a和b符号是不是不一样：如果不一样diffAB=1；如果一样diffAB=0
	int sameAB = flip(diffAB);//判断a和b是不是一样：如果一样sameAB=1；如果不一样sameAB=0

	int returnA = sameAB * sc + diffAB * sa;//如果a和b符号一样且c非负 或者 如果a和b符号不一样且a非负
	//这里乘号相当于两个事件取交集
	int returnB = flip(returnA);//returnA和returnB互斥

	return a * returnA + b * returnB;
}



//题目三：找到缺失的数字
// 例如：一个长度为10的数组，存储0~10中的10个数，求0~10中未被存储的那个数字
//思路：不缺那个数字的异或和 ^ 缺那个数字的异或和 == 缺少的那个数字 (结论4)
int missingNumber(int* arr, int length)
{
	int eorAll = 0, eorHas = 0;
	for (int i = 0; i < length; i++)
	{
		eorAll ^= i;
		eorHas ^= arr[i];
	}
	eorAll ^= length;

	return eorAll ^ eorHas;
}



//题目四：数组中一种数出现了奇数次，其他的数都出现了偶数次，返回出现了奇数次的数
int SingleNumber(int* arr, int length)
{
	int eor = 0;
	for (int i = 0; i < length; i++)
		eor ^= arr[i];

	return eor;
}



//Brian Kernighan算法 - 提取出二进制状态中只剩最右侧的1的状态  (注意是这个状态)
// 方法：n & ( (~n) + 1 ) -->>n & (-n)
// eg:
// n:01101000 , 想取n二进制状态中最右侧的1：00001000
// step1: ~n:       10010111
// step2: ~n+1:     10011000
// step3: (~n+1)&n: 00001000


//题目五：数组中有2种数出现了奇数次，其他的数都出现了偶数次，返回这2种出现了奇数次的数
//两个数一定不一样。所以所有数全部相与了之后，得到的数最右边的1，一定就是第一位这两个数不同的地方。 
//通过这个最右边不一样的1就可以分开两个数。两个数在那一位，一个=1，一个=0。
//与得到的最右侧的1相与，其中一个数一定等于0，这样就可以分隔出他们了 
//取反的时候要转换一下long，不然会溢出
vector<int> SingleNumber2(vector<int>& arr)
{
	vector<int> ans;
	int eor1 = 0;
	for (int i = 0; i < arr.size(); i++)
		eor1 ^= arr[i];
	//eor1: a ^ b

	int rightOne = eor1 & (-(long)eor1);// Brian Kernighan算法，提取出二进制里最右侧的1
	int eor2 = 0;
	for (int i = 0; i < arr.size(); i++)
	{
		if ((arr[i] & rightOne) == 0)
			eor2 ^= arr[i];
		//eg:
		//rightOne: 0000 0100
		//arr[i]:   0011 0110
		//          0000 0100 -->> 4
	}

	ans.push_back(eor2);
	ans.push_back(eor1 ^ eor2);
	return ans;
}



//题目六：数组中只有1种数出现次数少于m次(比方说k次)，其他数都出现了m次，返回出现次数小于m次的那种数
//准备一个桶，存储二进制下每个位1的数目
//遍历数组，如果一个数二进制形式下的某位是1，则存入桶中
//如果桶中的某个数x是m的倍数（%m==0）,则要求的数在该位（x的下标）为0；同理，不是m的倍数，在该位是1
int find(vector<int>& arr, int m)
{
	vector<int> cnt(32, 0);//桶
	//cnt[0]: 0位上有多少个1
	//cnt[i]: i位上有多少个1
	//cnt[31]: 31位上有多少个1
	for (int i = 0; i < arr.size(); i++)
	{
		for (int j = 0; j < 32; j++)
		{
			cnt[i] += (arr[i] >> i) & 1;
		}
	}

	int ans = 0;
	for (int i = 0; i < 32; i++)
	{
		if (cnt[i] % m != 0)
			ans = ans | (1 << i);
	}
	return ans;
}