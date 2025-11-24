#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
//数组实现双端队列
#define MAXSIZE 100
int* arr = new int[MAXSIZE];
int left, right;
int limit;//最大存储数量
int size = 0;//记录当前数组中元素个数

bool isEmpty()
{
	if (::size == 0)
		return true;
	return false;
}

bool isFull()
{
	if (::size == ::limit)
		return true;
	return false;
}

bool insertFront(int val)
{
	if (isFull())
		return false;
	else
	{
		if (isEmpty())
		{
			::left = ::right = 0;
			arr[::left] = val;
		}
		else
		{
			::left = ::left == 0 ? (limit - 1) : (::left - 1);
			arr[::left] = val;
		}
	}
	::size++;
	return true;
}

bool insertRear(int val)
{
	if (isFull())
		return false;
	else
	{
		if (isEmpty())
		{
			::left = ::right = 0;
			arr[::right] = val;
		}
		else
		{
			::right = ::right == (limit - 1) ? 0 : (::right + 1);
			arr[::right] = val;
		}
	}
	::size++;
	return true;
}

bool deleteFront()
{
	if (isEmpty())
		return false;
	else
	{
		::left = ::left == (::limit - 1) ? 0 : (::left + 1);
		::size--;
	}
	return true;
}

bool deleteRear()
{
	if (isEmpty())
		return false;
	else
	{
		::right = ::right == 0 ? (::limit - 1) : (::right - 1);
		::size--;
	}
	return true;
}

int getFront()
{
	if (isEmpty())
		return -1;
	return arr[::left];
}

int getRear()
{
	if (isEmpty())
		return -1;
	return arr[::right];
}
