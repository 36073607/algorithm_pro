#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cmath>
using namespace std;
//基于比较的排序
// 只需要定义好两个对象之间怎么比较即可，对象的数据特征并不关心，很通用
// 例如：三傻排序、归并排序、快速排序、堆排序

//不基于比较的排序
// 和比较无关的排序，对于对象的数据类型有要求，并不通用
// 例如：计数排序
// 假设一个数组中所有元素都在[0~60]，数据量在10^6
// 我们只需遍历一下数组，记录每个数字出现的次数，然后从小到大直接输出即可
// eg:1个0，3个1，3个2......      输出：0111222


//基数排序
// 思路：按照位数不断对代码排序，从个位数，十位数等等开始排序，依次排好序之后整体就有序了
//基层排序核心代码：
//arr内要保证没有负数
//bits是arr中最大值在BASE进制下有几位
const int MAXSIZE = 50001;
const int BASE = 10;//代表几进制，理解的时候假设BASE为10
int cnt[BASE];//桶
int help[MAXSIZE];//临时数组，存储原数组某一次排好序的结果

int bits(long x)
{
	int ans = 0;
	while (x)
	{
		x /= 10;
		ans++;
	}
	return ans;
}

void radixSort(int* arr, int length, int bits)
{
	for (int offset = 1; bits > 0; offset *= BASE, bits--)//一共执行次数
	{
		for (int i = 0; i < length; i++)//提取每一位数字
			cnt[(arr[i] / offset) % BASE]++;

		for (int i = 1; i < BASE; i++)//处理成前缀次数累加的形式
			cnt[i] += cnt[i - 1];

		for (int i = length - 1; i >= 0; i--)//前缀分区
			help[--cnt[(arr[i] / offset) % BASE]] = arr[i];

		for (int i = 0; i < length; i++)
			arr[i] = help[i];
	}
}

//若arr内有负数：数组中每个数都减去数组中的最小值（不溢出的话）
int* sortArray(int* arr, int length)
{
	if (length > 1)
	{
		int mins = arr[0];
		for (int i = 1; i < length; i++)
		{
			mins = min(mins, arr[i]);
		}

		int maxs = 0;
		for (int i = 0; i < length; i++)
		{
			arr[i] -= mins;
			maxs = max(maxs, arr[i]);
		}

		radixSort(arr, length, bits(maxs));

		for (int i = 0; i < length; i++)
			arr[i] += mins;
	}

	return arr;
}


//补充：
//进制大了，运行时间变快-->>执行不了几轮；但占用空间增加
//根据具体情况具体判断

//时间复杂度O(n)，额外空间复杂度O(m)，需要辅助空间做类似桶的作用，来不停装入、弹出数字
//一旦比较的对象不再是常规数字，那么改写代价的增加是显而易见的，所以不基于比较的排序并不通用
