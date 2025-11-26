#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;

//归并分治原理：
//1.思考一个问题在大范围上的答案，是否等于：左部分的答案 + 右部分的答案 + 跨越左右产生的答案
//2.计算“跨越左右产生的答案时”，如果加上左、右各自有序这个设定，会不会获得计算的便利性-->>形成滑窗，不走回头路 （已经算完了才排序，不会对结果产生影响）
//3.如果以上两点都成立，那么该问题很可能被 归并分治 解决
//4.求解答案的过程中只需要加入归并排序的过程即可，因为要让左、右各自有序，来获得计算的便利性

//eg1: 小和问题
//在一个数组中，每一个数左边比当前数小的数累加起来，叫做这个数组的小和。求一个数组的小和。
//例子：
//[1, 3, 4, 2, 5]
//1左边比1小的数，没有；
//3左边比3小的数，1；
//4左边比4小的数，1、3；
//2左边比2小的数，1；
//5左边比5小的数，1、3、4、2；
//所以小和为1 + 1 + 3 + 1 + 1 + 3 + 4 + 2 = 16

#define MAXSIZE 10000
int arr[MAXSIZE];
int help[MAXSIZE];//辅助数组
int limit;//数据中元素具体个数
//当结果比较大时，用int会溢出
//返回arr[ l ... r ]范围上，小和的累加和，同时把arr[ l ... r ]变有序
long smallSum(int l, int r)
{
	if (l == r)//base case 只有一个数字，没有小和
		return 0;
	
	int m = (l + r) >> 1;
	return smallSum(l, m) + smallSum(m + 1, r) + merge(l, m, r);
}

//返回跨左右产生的小和累加和，
// 并让左侧有序、右侧有序，最终使左右两侧整体有序
// arr[l...m] arr[m+1...r]
long merge(int l, int m, int r)
{
	//统计部分
	long ans = 0;
	for (int i = l, j = m + 1, sum = 0; j <= r; j++)
	{
		while (i <= m && arr[i] <= arr[j])
			sum += arr[i++];
		ans += sum;
	}

	//正常merge排序
	int index_help = l;
	int a = l;
	int b = m + 1;
	while (a <= m && b <= r)
	{
		help[index_help++] = (arr[a] <= arr[b]) ? arr[a++] : arr[b++];
	}

	while (a <= m)
		help[index_help++] = arr[a++];
	while (b <= r)
		help[index_help++] = arr[b++];

	for (int i = l; i <= r; i++)
		arr[i] = help[i];

	return ans;
}



//eg2:翻转对数量
//给定一个数组nums,
//如果 i<j 且 nums[i] > 2*nums[j] ,我们就将 (i,j) 称作一个翻转对
//你需要返回给定数组中的翻转对的数量
#define MAXSIZE 10000
int arr[MAXSIZE];
int help[MAXSIZE];
int counts(int l, int r)
{
	if (l == r)//base case
		return 0;
	int m = (l + r) / 2;
	return counts(l, m) + counts(m + 1, r) + merge2(l, m, r);
}

int merge2(int l, int m, int r)
{
	int ans = 0;
	for (int i = l, j = m + 1; i <= m; i++)
	{
		while (j <= r && (long)arr[i] > (long)arr[j] * 2)
			j++;
		ans += j - m - 1;//由于j指向最后一个可以构成翻转对的下一个元素的下标，所以此处减一，代表j指向最后一个可以构成翻转对的下标
	}

	int help_index = l;
	int a = l;
	int b = m + 1;
	while (a <= m && b <= r)
		help[help_index++] = arr[a] <= arr[b] ? arr[a++] : arr[b++];

	while (a <= m)
		help[help_index++] = arr[a++];
	while (b <= r)
		help[help_index++] = arr[b++];

	for (int i = l; i <= r; i++)
		arr[i] = help[i];

	return ans;
}


//补充：
//1.一些用归并分治解决的问题，往往也可以用线段树、树状数组等解法，时间复杂度也都是最优解
//2.归并分治不仅可以解决简单问题，还可以解决很多较难的问题，只要符合上面说的特征
//3.后续更新“整块分治”
