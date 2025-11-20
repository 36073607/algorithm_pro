#define _CRT_SECURE_NO_WARNINGS
//在有序数组中找>=num的最左位置
int findleft(int* arr, int length, int value)//传入的数组已经升序排列
{
	int pos = -1;
	int low = 0;
	int high = length - 1;
	while (low <= high)
	{
		int mid = low + ((high - low) >> 1);// low + (high - low)/2
		if (arr[mid] < value)
			low = mid + 1;
		else if (arr[mid] >= value)
		{
			pos = mid;
			high = mid - 1;
		}
	}
	return pos;
}

//注：找有序数组中<=num的最左位置没有意义，因为只需比较第一个数组元素即可

//在有序数组中找<=num的最右位置
int findright(int* arr, int length, int value)
{
	int pos = -1;
	int low = 0;
	int high = length - 1;
	while (low <= high)
	{
		int mid = low + ((high - low) >> 1);
		if (arr[mid] > value)
			high = mid - 1;
		else if (arr[mid] <= value)
		{
			pos = mid;
			low = mid + 1;
		}
	}
	return pos;
}


//二分搜索不一定发生在有序数组上（比如寻找峰值问题）
//峰值元素是指其值严格大于左右相邻值的元素
//给你一个整数数组arr，已知任何两个相邻的值都不相等
//找到峰值元素并返回其索引
//数组可能包含多个峰值，在这种情况下，返回 任何一个峰值 所在的位置即可
//你可以假设arr[-1] = arr[n] = 无穷小
//你必须实现时间复杂度为O(log n) 的算法来解决此问题
int findPeakElement(int* arr, int length)
{
	if (length == 1)//此时数组中只有一个元素
		return 0;

	if (arr[0] > arr[1])//单独验证0位置是不是峰值点
		return 0;
	if (arr[length - 1] > arr[length - 2])//单独验证length-1位置是不是峰值点
		return length - 1;

	//X        中间一定有峰值点         X
	//0                              length-1
	//中间： 1~~length-2,一定有峰值点
	int pos = -1;
	int left = 1;
	int right = length - 2;
	while (left <= right)
	{
		int mid = (left + right) / 2;
		
		if (arr[mid - 1] > arr[mid])
			right = mid - 1;
		else if (arr[mid] < arr[mid + 1])
			left = mid + 1;
		else
		{
			pos = mid;
			break;
		}
	}
	return pos;
}

//二分精髓：即使整体是无序的，但是你只要确定哪侧必有、哪侧必没有，就可以去二分
//此题就是某一侧必有，就可以去二分

