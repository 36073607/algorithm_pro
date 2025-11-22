#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
//反转链表
// 单链表
typedef int ElemType;
struct Node
{
	ElemType data;
	Node* next;
};

Node* reverseList(Node* L)
{
    if (L == nullptr || L->next == nullptr)
        return;

    Node* prev = nullptr;
    Node* curr = L->next;
    Node* next = nullptr;

    while (curr != nullptr)
    {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }

    L->next = prev;

    return L;
}

//双向链表
typedef int ElemType;
struct doubleNode
{
    ElemType data;
    doubleNode* prev;
    doubleNode* next;
};

doubleNode* reverseList(doubleNode* L)
{
    if (L == nullptr || L->next == nullptr) 
    {
        return L;  // 空链表或只有头节点
    }

    doubleNode* current = L->next;
    doubleNode* prevNode = nullptr;
    doubleNode* nextNode = nullptr;

    // 反转实际节点
    while (current != nullptr)
    {
        nextNode = current->next;

        // 交换prev和next指针
        current->next = prevNode;
        current->prev = nextNode;

        prevNode = current;
        current = nextNode;
    }

    // 重新连接头节点
    L->next = prevNode;  // prevNode现在是新的第一个节点
    if (prevNode != nullptr) 
    {
        prevNode->prev = L;
    }

    return L;
}



//合并两个有序链表
typedef int ElemType;
struct Node
{
    ElemType data;
    Node* next;
};

Node* mergeTwoLists(Node* L1, Node* L2)//传来的两个链表都是升序排列，现在把两个链表升序合并
{
    if (L1 == NULL || L2 == NULL)
        return L1 == NULL ? L2 : L1;

    Node* head = (L1->next->data > L2->next->data) ? L2 : L1;//返回两个链表中第一个结点较小的那个链表的头结点
    Node* curr1 = head->next;//被选择的链表的第一个结点
    Node* curr2 = head == L1 ? L2->next : L1->next;//记录另一个链表的第一个结点
    Node* prev = head;//记录新链表最后一个结点，从头结点开始

    while (curr1 != NULL && curr2 != NULL)
    {
        if (curr1->data <= curr2->data)
        {
            prev->next = curr1;
            curr1 = curr1->next;
        }
        else if(curr1 -> data > curr2 -> data)
        {
            prev->next = curr2;
            curr2 = curr2->next;
        }
        prev = prev->next;
    }
    prev->next = curr1 != NULL ? curr1 : curr2;//当一个链表为空时，prev继承另一个链表的剩余内容

    return head;
}



//两个链表相加   （类似于用链表写高精度）
typedef int ElemType;
struct Node
{
    ElemType data;
    Node* next;
};

Node* addTwoNumbers(Node* h1, Node* h2)
{
    Node* ans = NULL;
    Node* cur = NULL;
    int carry = 0;//进位
    for (int sum, val; h1 != NULL || h2 != NULL; h1 = h1 == NULL ? NULL : h1->next, h2 = h2 == NULL ? NULL : h2->next)
    {
        sum = (h1 == NULL ? 0 : h1->data) + (h2 == NULL ? 0 : h2->data) + carry;

        val = sum % 10;
        carry = sum / 10;

        if (ans == NULL)
        {
            ans = new(Node);
            ans->data = val;
            ans->next = NULL;
            cur = ans;
        }
        else
        {
            cur->next = new(Node);
            cur->next->data = val;
            cur->next->next = NULL;
            cur = cur->next;
        }
    }
    if (carry == 1)
    {
        cur->next = new(Node);
        cur->next->data = carry;
    }

    return ans;
}



//划分链表
typedef int ElemType;
struct Node
{
    ElemType data;
    Node* next;
};

Node* partition(Node* L, int x)
{
    // <x的区域
    Node* leftHead = NULL;
    Node* leftTail = NULL;
    // >x的区域
    Node* rightHead = NULL;
    Node* rightTail = NULL;

    Node* next;//用于记录原链表结点
    Node* realNode = L->next;
    while (realNode != NULL)
    {
        next = realNode->next;
        realNode->next = NULL;//断链
        if (realNode->data >= x)
        {
            if (rightHead == NULL)
            {
                rightHead = realNode;
                rightTail = rightHead;
            }
            else
            {
                rightTail->next = realNode;
                rightTail = rightTail->next;
            }
        }
        else
        {
            if (leftHead == NULL)
            {
                leftHead = leftTail = realNode;
            }
            else
            {
                leftTail->next = realNode;
                leftTail = leftTail->next;
            }
        }
        realNode = next;
    }
    if (leftHead == NULL)//没有小于x的
        return rightHead;

    //小于x的区域有内容
    leftTail->next = rightHead;
    return leftHead;
}