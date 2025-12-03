#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
//所谓位图(bitmap)，就是用每一位来存放某种状态  -->>有点类似于集合set
//适用于大规模数据，但数据状态又不是很多的情况。
//通常是用来判断某个数据存不存在的。在STL中有一个bitset容器，其实就是位图法。

//位图原理：
//其实就是用bit组成的数组用来存放值，用bit状态1、0代表存在、不存在，取值和存值操作都用位运算
//限制是必须为连续范围且不能过大
//好处是极大的节省空间，因为一个数字只占用1个bit的空间

//把位图想象成一个整型数组int arr[]
//arr[0]这个整数可以表示0~31这些数字是否出现
//arr[1]表示32~63
//arr[2]表示64~95
//arr[3]表示96~127
//
//定位一个数字x:
// x/32 找到它来自于哪个整数
// x%32 找到它来自于第几位

class bitmap
{
private:
	int* set;
public:
	bitmap(int n = 0)//构造函数
	{
		this->set = new int[(n + 31) / 32];//初始化位图的大小，只支持0~n-1所有数字的增删改查
		// a/b如果结果想向上取整，可以写成 : (a+b-1)/b         前提是a和b都是非负数
		//理解：相当于a + (b-1) , b-1是整数内最接近b的余项，只要a=k*b+x,x>0,则进位
	}
	~bitmap()//析构函数
	{
		delete[] set;
	}

	void add(int num)//把num加入到位图
	{
		set[num / 32] |= 1 << (num % 32);
	}

	void remove(int num)//把num从位图中删除
	{
		set[num / 32] &= ~(1 << (num % 32));
	}

	void flip(int num)//如果位图中没有num，就加入；如果位图中有num，就删除
	//反转，如果该位是1，则变为0；如果该位是0，则变为1
	{
		set[num / 32] ^= 1 << (num % 32);
	}

	bool contain(int num)//查询num是否在位图中
	{
		return (((unsigned int)set[num / 32] >> (num % 32)) & 1) == 1;
	}

};


//更多操作：
//Bitset(int n):初始化n个位，所有位都是0
//void fix(int i):将下标i的位上的值更新为存在
//void unfix(int i):将下标i的位上的值更新为不存在
//void flip():翻转所有位的值
//bool all():是否所有位都是1
//bool one():是否至少有一位是1
//int count():返回所有位中1的数量
//string toString():返回所有位的状态
class Bitset
{
private:
	vector<int> set;
	int size;
	int ones;
	int zeros;
	bool reverse;
public:
	Bitset(int n = 0)
	{
		set.resize((n + 31) / 32, 0);
		size = n;
		zeros = n;
		ones = 0;
		reverse = false;
	}
	~Bitset()
	{

	}

	void fix(int i)
	{
		int index = i / 32;
		int bit = i % 32;
		if (!reverse)//位图所有位的状态，维持原始含义：0 -- 不存在；1 -- 存在
		{
			if ((set[index] & 1 << bit) == 0)
			{
				zeros--;
				ones++;
				set[index] |= 1 << bit;
			}
		}
		else//位图所有位的状态，翻转了：0 -- 存在；1 -- 不存在
		{
			if ((set[index] & 1 << bit) != 0)
			{
				zeros--;
				ones++;
				set[index] ^= 1 << bit;
			}
		}
	}

	void unfix(int i)
	{
		int index = i / 32;
		int bit = i % 32;
		if (!reverse)
		{
			if ((set[index] & 1 << bit) != 0)
			{
				zeros++;
				ones--;
				set[index] ^= 1 << bit;
			}
		}
		else
		{
			if ((set[index] & 1 << bit) == 0)
			{
				zeros++;
				ones--;
				set[index] |= 1 << bit;
			}
		}
	}

	void flip()
	{
		reverse = !reverse;
		swap(zeros, ones);
	}

	bool all()
	{
		return ones == size;
	}

	bool one()
	{
		return ones > 0;
	}

	int count()
	{
		return ones;
	}

	string toString()
	{
		string ans;
		ans.reserve(size);  // 预分配空间，提高性能

		//逐个位处理（最直接）
		for (int i = 0; i < size; i++)
		{
			int index = i / 32;
			int bit = i % 32;
			int status = (set[index] >> bit) & 1;

			// 处理翻转
			if (reverse)
			{
				status = status ? 0 : 1;
			}

			ans.push_back('0' + status);
		}

		return ans;
	}

};
