#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <ctime>
using namespace std;

//特别提醒：记得在主函数内添加srand函数来初始化随机数种子
//srand((unsigned int)time(NULL));
//srand(time(0));

//随机快排经典版（不推荐）
#define MAXSIZE 100
int arr[MAXSIZE];
//已知arr[l...r]范围上一定有x这个值
//划分数组 <=x放左边，>x放右边，并且确保划分完成后<=x区域的最后一个数字是x
int partition1(int l, int r, int x)
{
	int a = l, xi = 0;
	//a：arr[l...a-1]范围是<=x的区域
	//xi：记录在<=x的区域上任何一个x的位置，哪一个都可以
	for (int i = l; i <= r; i++)
	{
		if (arr[i] <= x)
		{
			swap(arr[i], arr[a]);
			if (arr[i] == arr[x])
				xi = i;
			a++;
		}
	}
	//<=x        >x
	//l.....a-1  a.....r
	swap(arr[xi], arr[a - 1]);
	return a - 1;
}

void quickSort(int l, int r)
{
	if (l >= r)//base case
		return;
	//随机这一下，常数时间比较大
	//但只有这一下随机，才能在概率上把快速排序的时间复杂度收敛到O(n*logn)
	//l.....r 随机选一个位置，x这个值，做划分
	int x = arr[r + rand() % (r - l + 1)];
	int m = partition1(l, r, x);
	quickSort(l, m - 1);
	quickSort(m + 1, r);
}




//partition函数优化 （荷兰国旗问题）
static int first, last;//作为优化后的partition函数返回值，因为函数只能返回一个返回值，故用全局变量充当函数返回值
//已知arr[l...r]范围上一定有x这个值
//划分数组 <x放左边，==x放中间，>x放右边
//把全局变量first,last，更新成==x区域的左右边界
void partition2(int l, int r, int x)
{
	first = l;
	last = r;
	int i = l;//充当游标
	while (i <= last)
	{
		if (arr[i] < x)
		{
			swap(arr[first], arr[i]);
			first++;
			i++;
			//这里i++是因为i从左至右过一遍，换过来的数据已经看过了
		}
		else if (arr[i] > x)
		{
			swap(arr[last], arr[i]);
			last--;
			//这里i不变是因为新换过来的数据还没看，需进一步判断
		}
		else if (arr[i] == x)
			i++;
	}
}

void quickSort_pro(int l, int r)
{
	if (l >= r)
		return;
	int x = arr[l + rand() % (r - l + 1)];
	partition2(l, r, x);
	//为了防止底层的递归过程覆盖全局变量
	//这里用临时变量记录first,last
	int left = first;
	int right = last;
	quickSort_pro(l, left - 1);
	quickSort_pro(right + 1, r);
}
//荷兰国旗问题的优化点：选出一个数字x，数组在划分时会搞定所有值是x的数字
//而经典的随机快排每次只搞定一个位置的数



//复杂度分析：
//核心点：怎么选择数字？
//选择的数字是当前范围上的固定位置，比如范围上的最右数字，那么就是普通快速排序
//选择的数字是当前范围上的随机位置，那么就是随机快速排序

//普通快速排序，时间复杂度O(n^2)，额外空间复杂度O(n) -->>压了n层栈
//随机快速排序，时间复杂度O(n*logn)，额外空间复杂度O(logn)

//详细证明可见 算法导论-7.4.2



//补充：
#include <random>

// 创建随机数生成器
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> distrib(l, r);

// 使用
int randomIndex = distrib(gen);  // 获取[l, r]范围内的随机数
