#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <stack>
using namespace std;
//注意：
//1.如果笔试中空间要求不严格，直接使用容器来解决链表问题
//2.如果笔试中空间要求严格、或者在面试中面试官强调空间的优化，需要使用额外空间复杂度O(1)的方法
//3.最常用的技巧--快慢指针
//4.链表类题目往往都是很简单的算法问题，核心考察点也并不是算法设计，是coding能力
//5.这一类问题除了多写多练没有别的方法
// 
//个人建议：
//链表类题目既然练的就是coding，那么不要采取空间上讨巧的方式来练习

//题目一：返回两个无环链表相交的第一个结点
typedef int ElemType;
struct ListNode
{
	ElemType data;
	ListNode* next;
};

ListNode* getIntersectionNode(ListNode* L1, ListNode* L2)
{
	ListNode* a = L1;
	ListNode* b = L2;
	int diff = 0;
	while (a->next != NULL)
	{
		a = a->next;
		diff++;
	}
	while (b->next != NULL)
	{
		b = b->next;
		diff--;
	}

	if (a != b)
		return NULL;

	ListNode* fast = NULL;
	ListNode* low = NULL;
	if (diff >= 0)
	{
		fast = L1;
		low = L2;
	}
	else
	{
		fast = L2;
		low = L1;
	}
	diff = abs(diff);

	for (int i = 0; i < diff; i++)
	{
		fast = fast->next;
	}
	while (fast != low)
	{
		fast = fast->next;
		low = low->next;
	}

	return fast;
}



//题目二：每k个结点一组翻转链表
//使用容器的实现方法：将链表放在数组里，在数组里swap即可
typedef int ElemType;
struct ListNode
{
	ElemType data;
	ListNode* next;
};

ListNode* reverseKGroup1(ListNode* L, int k)
{
	if (!L || k <= 1)
		return L;

	vector<ListNode*> nodes;
	while (L)
	{
		nodes.push_back(L);
		L = L->next;
	}

	for (int i = 0; i + k <= nodes.size(); i += k)
	{
		reverse(nodes.begin() + i, nodes.begin() + i + k);
	}
	for (int i = 0; i <= nodes.size() - 2; i++)
		nodes[i]->next = nodes[i + 1];

	nodes[nodes.size() - 1]->next = NULL;

	return nodes[0];
}

//不使用容器实现
ListNode* getEnd(ListNode* start, int k)
{
	while (--k && start)
		start = start->next;
	return start;
}

void reverseList(ListNode* start, ListNode* end)
{
	ListNode* nextGroupStart = end->next;//指向下一段链表的头结点
	ListNode* prev = NULL;
	ListNode* curr = start;
	ListNode* next = NULL;
	while (curr != nextGroupStart)
	{
		next = curr->next;
		curr->next = prev;
		prev = curr;
		curr = next;
	}

	start->next = nextGroupStart;//反转后的尾节点指向下一段链表的头结点

}

ListNode* reverseKGroup2(ListNode* head, int k)
{
	if (!head || k <= 1)
		return head;

	ListNode* start = head;
	ListNode* end = getEnd(start, k);
	if (!end)
		return head;

	//第一组很特殊因为牵扯到换头的问题 ：
	head = end;//    start变尾 end变头 
	reverseList(start, end);
	ListNode* lastTeamEnd = start;// 翻转之后start变成了上一组的结尾节点  
	//其他组：
	while (lastTeamEnd->next != NULL)
	{
		start = lastTeamEnd->next;
		end = getEnd(start, k);
		if (!end)
			return head;

		reverseList(start, end);
		lastTeamEnd->next = end;//        上一个的尾巴，指向反转后的头（end）
		lastTeamEnd = start;//        更新尾巴变成下一个尾巴  
	}
	return head;
}
//经验总结：
//一定要认清楚我们是用栈区临时变量操作堆区的链表，不要被变量名迷惑了
//由于传入的参数为指针类型，所以要注意不要破坏传入的参数：使用临时变量解决



//题目三：复制带随机指针的链表
//使用容器实现：利用map
struct Node
{
	int data;
	Node* next;
	Node* random;
};

Node* copyRandomList(Node* head)
{
	if (head == NULL)
		return NULL;

	map<Node*, Node*> mp;
	Node* curr = head;
	while (curr != NULL)
	{
		mp[curr] = new Node;
		mp[curr]->data = curr->data;
		//mp[curr] = new Node(curr->data);
		curr = curr->next;
	}

	curr = head;
	while (curr != NULL)
	{
		//map的operator[]会为不存在的key创建默认值（NULL）
		mp[curr]->next = mp[curr->next];
		mp[curr]->random = mp[curr->random];

		//mp[curr]->next = curr->next ? mp[curr->next] : NULL;
		//mp[curr]->random = curr->random ? mp[curr->random] : NULL;

		curr = curr->next;
	}
	
	return mp[head];
}

//不使用容器实现：
//主要就是找两个节点长度的差异，这样就可以对两个链表对齐，对齐之后一起走就可以找到相同的点。
Node* copyRandomList_(Node* head)
{
	if (head == NULL)
		return NULL;

	//1 -> 2 -> 3 -> ...
	//变成：1 -> 1' -> 2 -> 2' -> 3 -> 3' -> ...
	Node* cur = head;
	while (cur != NULL)//插入白绝结点
	{
		Node* tmp = new Node;
		tmp->data = cur->data;
		//Node* tmp = new Node(cur->data);
		tmp->next = cur->next;
		cur->next = tmp;

		cur = cur->next->next;
	}

	//搞定random结点
	cur = head;
	while (cur != NULL)
	{
		cur->next->random = cur->random ? cur->random->next : NULL;
		cur = cur->next->next;
	}
	//新老链表分离
	Node* ans = head->next;
	cur = head;
	while (cur != NULL)
	{
		Node* tmp = cur->next;
		cur->next = tmp->next;
		if (tmp->next != NULL)
			tmp->next = tmp->next->next;

		cur = cur->next;
	}

	return ans;
}



//题目四：判断链表是否是回文结构。这个题的流程设计甚至是考研常考。快慢指针找中点
//用容器实现：放在栈里
struct ListNode
{
	ElemType val;
	ListNode* next;
};

bool isPalindrome(ListNode* head)
{
	if (head == NULL || head->next == NULL)
		return true;

	bool ans = true;
	stack<ListNode*> s;
	ListNode* cur = head;
	while (cur != NULL)
	{
		s.push(cur);
		cur = cur->next;
	}

	cur = head;
	while (cur != NULL)
	{
		ListNode* tmp = s.top();
		s.pop();
		if (cur->val == tmp->val)
			cur = cur->next;
		else
		{
			ans = false;
			break;
		}
	}

	return ans;
}

//非容器实现：
bool isPalindrome(ListNode* head)
{
	if (!head || !head->next)
		return true;

	ListNode* fast = head;//快指针速度为慢指针两倍
	ListNode* slow = head;//慢指针即为链表中间结点
	while (fast->next != NULL && fast->next->next != NULL)
	{
		fast = fast->next->next;
		slow = slow->next;
	}

	//将右半部分链表翻转
	ListNode* prev = NULL;
	ListNode* cur = slow->next;
	ListNode* after = NULL;
	while (cur != NULL)
	{
		after = cur->next;
		cur->next = prev;
		prev = cur;
		cur = after;
	}

	//将左右两半进行比对
	bool ans = true;
	ListNode* left = head;
	ListNode* right = prev;
	while (left != NULL && right != NULL)
	{
		if (left->val != right->val)
		{
			ans = false;
			break;
		}

		left = left->next;
		right = right->next;
	}

	//将链表恢复最初状态
	cur = prev;
	prev = NULL;
	while (cur != NULL)
	{
		after = cur->next;
		cur->next = prev;
		prev = cur;
		cur = after;
	}
	slow->next = prev;

	return ans;
}



//题目五：返回链表的第一个入环节点。 快慢指针找中点
//容器实现： set
struct ListNode
{
	ElemType val;
	ListNode* next;
};

ListNode* detectCycle(ListNode* head)
{
	if (head == NULL || head->next == NULL || head->next->next == NULL)
		return NULL;

	set<ListNode*> st;
	ListNode* cur = head;
	ListNode* ans = NULL;
	while (cur != NULL)
	{
		if (st.find(cur) != st.end())// 如果节点已经在集合中，说明找到了环的起点
		{
			ans = cur;
			break;
		}
		else// 节点不在集合中，加入集合并继续
		{
			st.insert(cur);
			cur = cur->next;
		}	
	}
	return ans;
}

//非容器实现
//快慢指针流程：
//1.快指针一次走两步，慢指针一次走一步，直至快慢指针相遇（不相遇则不为循环链表）
//2.慢指针原地呆着，快指针回到开头，然后快慢指针每次都走一步，相遇的结点即为环口
ListNode* detectCycle_(ListNode* head)
{
	if (head == NULL || head->next == NULL || head->next->next == NULL)
		return NULL;

	ListNode* slow = head->next;
	ListNode* fast = head->next->next;//先走一步
	while (fast != slow)
	{
		if (fast->next == NULL || fast->next->next == NULL)
			return NULL;

		fast = fast->next->next;
		slow = slow->next;
	}

	fast = head;
	while (fast != slow)
	{
		fast = fast->next;
		slow = slow->next;
	}

	return fast;
}



//题目六：在链表上排序。要求时间复杂度O(n*logn)，额外空间复杂度O(1)，还要求排序有稳定性
// 注意为了额外空间复杂度O(1)，所以不能使用递归  
// 因为mergeSort递归需要O(log n)的额外空间  
//用链表实现非递归版本的归并排序

ListNode* start;
ListNode* end;
ListNode* findEnd(ListNode* s, int k)
{
	while (s->next != nullptr && --k != 0)
	{
		s = s->next;
	}
	return s;
}

// l1...r1 -> null : 有序的左部分  
// l2...r2 -> null : 有序的右部分  
// 整体merge在一起，保证有序  
// 并且把全局变量start设置为整体的头，全局变量end设置为整体的尾  
void mergeList(ListNode* l1, ListNode* r1, ListNode* l2, ListNode* r2) 
{
	// 需要一个pre变量，这样遇到谁小的问题的时候才可以串list，  
	// start 用来确认整个链表的头  
	ListNode* pre;
	// 先判断用谁做头  
	if (l1->val <= l2->val) 
	{
		start = l1;
		pre = l1;
		l1 = l1->next;
	}
	else 
	{
		start = l2;
		pre = l2;
		l2 = l2->next;
	}
	while (l1 != nullptr && l2 != nullptr) 
	{
		if (l1->val <= l2->val) 
		{
			pre->next = l1;
			l1 = l1->next;
			pre = pre->next;
		}
		else 
		{
			pre->next = l2;
			l2 = l2->next;
			pre = pre->next;
		}
	}
	if (l1 == nullptr) 
	{
		// 连起来  
		pre->next = l2;
		::end = r2;
	}
	else {
		pre->next = l1;
		::end = r1;
	}
}

// 时间复杂度O(n*logn)，额外空间复杂度O(1)，有稳定性  
// 注意为了额外空间复杂度O(1)，所以不能使用递归  
// 因为mergeSort递归需要O(log n)的额外空间  
ListNode* sortList(ListNode* head) 
{
	ListNode* cur = head;
	int n = 0;
	while (cur != nullptr) 
	{
		n++;
		cur = cur->next;
	}

	ListNode* l1, * r1, * l2, * r2, * next, * lastTeamEnd;
	for (int step = 1; step < n; step <<= 1) 
	{
		// 第一组很特殊，因为要决定整个链表的头，所以单独处理  
		l1 = head;
		r1 = findEnd(l1, step);
		l2 = r1->next;
		r2 = findEnd(l2, step);
		next = r2->next;
		// 全部置空，断开两个list方便合并  
		r1->next = nullptr;
		r2->next = nullptr;
		mergeList(l1, r1, l2, r2);
		// 更新后的头  
		head = start;
		lastTeamEnd = ::end;
		while (next != nullptr) 
		{
			l1 = next;
			r1 = findEnd(l1, step);
			l2 = r1->next;
			if (l2 == nullptr) 
			{
				lastTeamEnd->next = l1;
				break;
			}
			r2 = findEnd(l2, step);
			next = r2->next;
			r1->next = nullptr;
			r2->next = nullptr;
			mergeList(l1, r1, l2, r2);
			lastTeamEnd->next = start;
			lastTeamEnd = ::end;
		}
	}
	return head;
}

//容器实现
struct cmp
{
	bool operator()(ListNode* l1, ListNode* l2)
	{
		return l1->val < l2->val;
	}
};

ListNode* sortList(ListNode* head)
{
	if (head == nullptr)
		return NULL;

	vector<ListNode*> arr;
	ListNode* cur = head;
	while (cur != NULL)
	{
		arr.push_back(cur);
		cur = cur->next;
	}
	sort(arr.begin(), arr.end(), cmp());

	for (int i = 0; i < arr.size() - 1; i++)
	{
		arr[i]->next = arr[i + 1];
	}
	
	arr.back()->next = NULL;
	return arr.front();
}