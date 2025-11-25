#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
//归并排序
// 左部分排好序、右部分排好序，利用merge过程让左右整体有序
// merge过程：谁小拷贝谁，直到左右两部分所有的数字耗尽，拷贝回原数组

//递归实现
//时间复杂度：O(n*logn)
//空间复杂度：O(n) -->>使用了辅助数组
#define MAXSIZE 100
int arr[MAXSIZE];//待排序的原数组
int help[MAXSIZE];//辅助数组

void merge(int, int, int);

void mergeSort1(int l, int r)//时间复杂度：T(n) = 2*T(n/2)+O(n) -->> O(n*logn)
{
	if (l == r)//base case
		return;
	int mid = (l + r) / 2;
	mergeSort1(l, mid);
	mergeSort1(mid + 1, r);
	merge(l, mid, r);
}

void merge(int l ,int mid ,int r)//时间复杂度：O(n)
{
	int i = l;//记录辅助数组的下标
	int a = l;//从子数组的左下标开始移动
	int b = mid + 1;//从子数组的中间下标+1开始移动
	while (a <= mid && b <= r)
		help[i++] = arr[a] <= arr[b] ? arr[a++] : arr[b++];

	//当其中一半的数组元素已经全部录入辅助数组时，用辅助数组记录另一半没记录完的全部元素
	//左侧下标和右侧下标必有一个越界，另一个不越界
	while (a <= mid)
		help[i++] = arr[a++];
	while (b <= r)
		help[i++] = arr[b++];

	for (int k = l; k <= r; k++)//将辅助数组中所有值覆盖原先数组	
		arr[k] = help[k];
}



//非递归实现
//时间复杂度：O(n*logn)
//空间复杂度：O(n) -->>使用了辅助数组
#include <cmath>
#define MAXSIZE 100
int arr[MAXSIZE];
int help[MAXSIZE];
void merge(int l, int mid, int r)//时间复杂度：O(n)
{
	int i = l;
	int a = l;
	int b = mid + 1;
	while (a <= mid && b <= r)
		help[i++] = arr[a] <= arr[b] ? arr[a++] : arr[b++];

	while (a <= mid)
		help[i++] = arr[a++];
	while (b <= r)
		help[i++] = arr[b++];

	for (int k = l; k <= r; k++)
		arr[k] = help[k];
}

void mergeSort2(int limit)//传入数组中具体有多少个元素
{
	for (int step = 1; step < limit; step <<= 1)//step是步长, O(logn)
	{
		int l, r, m;
		l = 0;
		while (l < limit)
		{
			m = l + step - 1;
			if (m + 1 > limit - 1)//已经没有右侧
				break;

			//有右侧，求右侧的右边界
			r = min(l + 2 * step - 1, limit - 1);
			//l ... m m+1 ... r
			merge(l, m, r);
			l = r + 1;
		}
	}
}


//为什么归并排序比三傻排序快？----因为比较行为没有被浪费
//利用归并排序的便利性可以解决很多问题----归并分治

//注意：
//有些资料说可以用原地归并排序，把额外空间复杂度变成O(1)，不要浪费时间去学
//因为原地归并排序确实可以省空间，但会让时间复杂度变成O(n^2)

