#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
using namespace std;
//选择排序
//总结：i~n-1范围上，找到最小值并放在i位置，然后i+1~n-1范围上继续
void selectionSort(int* arr, int sz)//这里为升序排列
{
	if (sz < 2)
		return;

	for (int i = 0; i < sz - 1; i++)
	{
		int index = i;
		for (int j = i + 1; j < sz; j++)
		{
			if (arr[j] < arr[index])
			{
				index = j;
			}
		}
		swap(arr[i], arr[index]);
	}
}

//冒泡排序
//总结：0~i范围上，相邻位置较大的数滚下去，最大值最终来到i位置，然后0~i-1范围上继续
void bubbleSort(int* arr, int sz)//这里为升序排列
{
	if (sz < 2)
		return;

	for (int i = 0; i < sz - 1; i++)//一共sz-1轮
	{
		for (int j = 0; j < sz - i - 1; j++)
		{
			if (arr[j] > arr[j + 1])
				swap(arr[j], arr[j + 1]);
		}
	}

}

//插入排序
//总结：0~i范围上已经有序，新来的数从右向左滑到不再小的位置插入，然后继续
void insertSort(int* arr, int sz)//这里为升序排列
{
	if (sz < 2)
		return;

	for (int i = 1; i < sz; i++)
	{
		for (int j = i - 1; j >= 0 && arr[j] > arr[j + 1]; j--)
			swap(arr[i], arr[j]);
	}
}

int main()
{
	int a[10] = { 2,1,4,5,7,8,3,9 };
	int sz = sizeof(a) / sizeof(a[0]);
	selectionSort(a, sz);
	for (int i = 0; i < sz; i++)
		cout << a[i] << " ";
	cout << endl;

	int b[10] = { 2,1,4,5,7,8,3,9 };
	int sz2 = sizeof(b) / sizeof(b[0]);
	bubbleSort(b, sz2);
	for (int i = 0; i < sz; i++)
		cout << a[i] << " ";
	cout << endl;

	int c[10] = { 2,1,4,5,7,8,3,9 };
	int sz3 = sizeof(c) / sizeof(c[0]);
	insertSort(c, sz3);
	for (int i = 0; i < sz; i++)
		cout << a[i] << " ";
	cout << endl;

	return 0;

}
