#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
//栈和队列相互实现
// 栈实现队列
#include <stack>
stack<int> in;
stack<int> out;
//倒数字：
//从in栈中，把数据倒入out栈
//1.out栈空了才能倒数据
//2.如果倒数据，in栈必须倒完
void inToOut()
{
	if (out.empty())
	{
		while (!in.empty())
		{
			out.push(in.top());
			in.pop();
		}
	}
}

// 队列实现栈
#include <queue>
queue<int> q;
void myStack_push(int value)
{
	// 先将新元素入队
	q.push(value);

	// 将之前的元素全部移到新元素后面
	for (int i = 0; i < q.size() - 1; i++)
	{
		q.push(q.front());
		q.pop();
	}
}



//最小栈----求栈中最小元素
//创建一个栈存储每种情况下原先栈中的最小值
//1.当原先栈新存入的数据<=min,最小栈压入这个新数据
//2.当原先栈新存入的数据>min,最小栈依旧压入min
//3.当原先栈出栈时，最小栈也同步出栈
//4.当最小栈为空时，说明原先栈也为空
stack<int> init;
stack<int> min_;
int mins;//记录原先栈中最小值
void myPush(int x)
{
	init.push(x);
	if (min_.empty() || x <= min_.top())
		min_.push(x);
	else//!min_.empty()&&x>min_.top()
		min_.push(min_.top());
}

void myPop()
{
	init.pop();
	min_.pop();
}

int getMin()
{
	return min_.top();
}

