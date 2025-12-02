#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdlib>
#include <algorithm>
using namespace std;
//无序数组中寻找第K大的数
//给定整数数组nums和整数k，返回数组中第k个最大的元素
//请注意，你需要找的是数组排序后的第K个最大的元素（返回该元素的值即可），而不是第K个最大的元素（并不是该元素的下标）
//要求时间复杂度为O(n)


//利用改写快排的方法，时间复杂度O(n),额外空间复杂度O(1)
// 区别：快排两边都要进行，该算法两边至多只选一侧搜
//上面问题的解法就是随机选择算法，是常考内容！
//详细时间复杂度证明见算法导论-9.2
//随机快速排序、随机选择算法，时间复杂度的证明理解起来很困难，只需记住结论，并不会对后续的算法学习造成什么影响
int first, last;
void portition(int* arr, int l, int r, int x)
{
	first = l, last = r;
	int i = l;
	while (i <= r)
	{
		if (arr[i] < x)
		{
			swap(arr[i], arr[first]);
			first++;
			i++;
		}
		else if (arr[i] > x)
		{
			swap(arr[i], arr[last]);
			last--;
		}
		else
			i++;
	}
}

int randomizedSelect(int* arr, int i, int length)
{
	int ans = -1;
	//0 ~ n-1
	//l   first  last   r
	//  i       i     i
	for (int l = 0, r = length - 1;l <= r;)
	{
		portition(arr, l, r, arr[l + rand() % (r - l + 1)]);
		if (first > i)
			r = first - 1;
		else if (last < i)
			l = last + 1;
		else
		{
			ans = arr[i];
			break;
		}
	}
	return ans;
}

int findKthLargest(int* arr, int k, int length)
{
	return randomizedSelect(arr, length - k, length);//由于数组按照升序走，故length-k表示第k大且作为数组下标
}


//补充：算法导论第9章，还有一个BFPRT算法，不用随机选择一个数的方式，也能做到时间复杂度O(n)，额外空间复杂度O(log n)

//特别提醒：记得在主函数中用srand函数来初始化随机数种子
