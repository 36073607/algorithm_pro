#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stack>
using namespace std;
typedef int ElemType;
struct TreeNode
{
	ElemType data;
	TreeNode* lchild;
	TreeNode* rchild;
};
typedef TreeNode* BiTree;
//先序打印所有结点，非递归版
stack<BiTree> s1;
void preOrder(BiTree T)
{
	if (T != NULL)
	{
		s1.push(T);
		while (!s1.empty())
		{
			BiTree curr = s1.top();
			s1.pop();
			cout << curr->data;
			if (curr->rchild != NULL)
				s1.push(curr->rchild);
			if (curr->lchild != NULL)
				s1.push(curr->lchild);
		}
	}
}

//中序打印所有结点，非递归版
stack<BiTree> s2;
void inOrder(BiTree T)
{
	if (T != NULL)
	{
		while (!s2.empty() || T != NULL)
		{
			if (T != NULL)
			{
				s2.push(T);
				T = T->lchild;
			}
			else
			{
				T = s2.top();
				s2.pop();
				cout << T->data;
				T = T->rchild;
			}
		}
	}
}

void inOrderOperate(Stack* s, BiTree T)
{
    BiTree curr = T;
    while (curr!=NULL || !isEmpty(s))
    {
        while (curr!=NULL)  // 将当前节点及其所有左子节点入栈
        {
            push(s, curr);
            curr = curr->lchild;
        }

        curr = pop(s);  // 弹出栈顶节点
        printf("%c ", curr->data);  // 访问该节点
        curr = curr->rchild;  // 处理右子树
    }
}

//后序打印所有结点，非递归版
//法一：两个栈实现
//先序：根 左 右
//先序'：根 右 左
//后序：左 右 根  -->>先序'的结果反转得到
stack<BiTree> s3;
stack<ElemType> collect;
void postOrder(BiTree T)
{
	if (T != NULL)
	{
		s3.push(T);
		while (!s3.empty())
		{
			BiTree curr = s3.top();
			collect.push(curr->data);
			s3.pop();
			if (curr->lchild != NULL)
				s3.push(curr->lchild);
			if (curr->rchild != NULL)
				s3.push(curr->rchild);
		}
	}
	while (!collect.empty())
	{
		cout << collect.top();
		collect.pop();
	}
}

//法二：一个栈实现
stack<BiTree> s4;
void postOrder_pro(BiTree T)
{
	if (T != NULL)
	{
		s4.push(T);
		BiTree prev = T;
		//用prev记录上一个打印输出的结点;如果没有,就是根结点
		while (!s4.empty())
		{
			BiTree curr = s4.top();
			if (curr->lchild != NULL && curr->lchild != prev && curr->rchild != prev)//如果左子树存在且未被处理
			{
				s4.push(curr->lchild);
			}
			else if (curr->rchild != NULL && curr->rchild != prev)//如果右子树存在且未被处理
			{
				s4.push(curr->rchild);
			}
			else//左右子树均被处理 或 左右子树均为空
			{
				cout << s4.top()->data;
				prev = s4.top();
				s4.pop();
			}
		}
	}
}
//注：当条件满足多个if语句时，执行第一个满足该条件的if语句


//遍历二叉树复杂度分析：
//a.时间复杂度O(n),递归和非递归都是每个结点操作有限几次，当然O(n)
//b.额外空间复杂度O(h),递归和非递归都需要二叉树高度h的空间来保存路径，方便回到上级去

//c.存在时间复杂度O(n),额外空间复杂度O(1)的遍历方式：Morris遍历
