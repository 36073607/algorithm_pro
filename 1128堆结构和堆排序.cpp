#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
using namespace std;
//一.堆结构
//前置知识：完全二叉树可以和数组前缀范围来对应，数组大小由单独的变量size控制
// （前缀是指从数组起始位置（下标0）开始，到某个位置结束的连续元素所组成的子数组）
//eg: [a,b,c,d,e,f,g]  size ==7
//     0 1 2 3 4 5 6
//   则其对应的完全二叉树如下：（用数组下标组建完全二叉树）
//           0
//          / \
//         1   2
//        /     \
//       3       4
//      /         \
//     5           6
//                      
//  [9 6 4 6 5 2 3 1]   size==8
//   0 1 2 3 4 5 6 7
// 按照大根堆排序结果如下：
//                   9(0)
//                /       \
//             6(1)       4(2)
//            /   \      /    \
//          6(3)  5(4)  2(5)  3(6) 
//         /
//        1(7)
// 
//具有n个结点的完全二叉树的深度为 log2(n) + 1 （向下取整）
//完全二叉树中某个结点i（不为根结点）的父亲节点：(i - 1) / 2
//                    i 的左孩子：i * 2 + 1
//                    i 的右孩子：i * 2 + 2
//若计算出的左后孩子的下标值>=size，则该孩子不存在


//堆结构完全由数组实现（堆的实际操控对象仍是数组，不过借助想象出来与这个数组对应的完全二叉树进行操作）
//
//堆是一种特殊的完全二叉树，是根据大根堆和小根堆的组织
//大根堆：这个结构中的任何一个子结构最大值一定在顶部，可能有多个最大值，但是你要保证最大值在顶部就可以
//本节以大根堆为例，小根堆同理


//二.堆的调整操作----向上调整、向下调整
//i位置的数，向上调整大根堆
// 当你在数组中插入一个新数字的时候，就不停与父结点比较，如果值大于父结点，则交换，持续这一过程，直到小于某一个父结点
//arr[i] = x , x是新来的！往上看，直到不比父结点大或者来到0位置（根结点）
void heapInsert(int* arr, int i)
{
	//i -->>父：(i-1)/2
	while (arr[i] > arr[(i - 1) / 2])
	{
		swap(arr[i], arr[(i - 1) / 2]);
		i = (i - 1) / 2;
	}
}

//i位置的数变小了，向下调整为大根堆，当前堆的大小为size
// 当你将数组中某一个元素的值改小之后，为了符合大根堆定义，该元素应该不断与两个子结点中较大的那个结点比较：如果小于最大孩子结点，则交换并继续下一轮比较，直到变为最后一层的结点或者两个子结点都比该结点小的时候停止
void heapify(int* arr, int i, int size)
{
	int l = 2 * i + 1;//左孩子
	while (l < size)//有左孩子
	{
		int r = l + 1;//右孩子
		int best_index = r<size && arr[r]>arr[l] ? r : l;//记录最大孩子结点对应的数组下标
		best_index = arr[best_index] > arr[i] ? best_index : i;//将最大孩子节点与该节点比较，记录两者之间最大元素对应的下标
		if (arr[best_index] == arr[i])
			break;
		swap(arr[i], arr[best_index]);
		i = best_index;
		l = 2 * i + 1;//更新左孩子对应的数组下标
	}
}

//heapInsert , heapify方法的单词调用，时间复杂度均为O(logn)，这是由完全二叉树的结构决定的
void pop(int* arr, int i, int& size)
{
	swap(arr[i], arr[size - 1]);
	heapify(arr, i, --size);
}


//三.堆排序
//法一：
//从顶到底建立大根堆，O(n*logn)
//依次弹出堆内最大值并排好序，O(n*logn)  -->>将堆顶（最大值）与未排序元素中下标最后一个元素交换，然后size-1,表示将该最大值断开(排好序)；然后将交换来的元素重新向下调整(heapify)；重复这一过程，直至size==0
//整体时间复杂度O(n*logn)
void heapSort1(int* arr, int length)
{
	for (int i = 0; i < length; i++)
		heapInsert(arr, i);
	int size = length;
	while (size > 1)
	{
		swap(arr[0], arr[--size]);
		heapify(arr, 0, size);
	}
}
//从顶到底建堆，时间复杂度O(n*logn), log1+log2+log3+...+logn -> O(n*logn)

//法二：
//从底到顶建立大根堆，O(n)
//依次弹出堆内最大值并排好序，O(n*logn) ，此过程与法一一致
//整体时间复杂度O(n*logn)，法二与法一差别仅在于建堆过程不一样
void heapSort2(int* arr, int length)
{
	for (int i = i - 1; i >= 0; i--)
		heapify(arr, i, length);
	int size = length;
	while (size > 1)
	{
		swap(arr[0], arr[--size]);
		heapify(arr, 0, size);
	}
}

//补充：堆排序空间复杂度是O(1),因为堆直接建立在了要排序的数组上，所以没有什么额外空间

//注意：堆结构比堆排序有用的多，尤其是和比较器结合之后

