#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <utility>
#include <queue>
#include <climits>
#include <string>
#include <algorithm>
using namespace std;
//题目一：二叉树的层序遍历
struct TreeNode
{
	int val;
	TreeNode* left;
	TreeNode* right;
	TreeNode() : val(0), left(nullptr), right(nullptr) {}
	TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
	TreeNode(int x, TreeNode* left, TreeNode* right) : val(x), left(left), right(right) {}
};

//法一：普通bfs: 通过哈希表记录自己在哪一层，一个一个处理
class Solution1
{
public:
	vector<vector<int>> levelOrder(TreeNode* root)
	{
		vector<vector<int>> ans;
		if (root == nullptr)
			return ans;

		unordered_map<TreeNode*, int> ump;
		queue<TreeNode*> q;
		q.push(root);
		ump[root] = 0;
		while (!q.empty())
		{
			TreeNode* cur = q.front();
			q.pop();
			int theCurLevel = ump[cur];
			if (ans.size() <= theCurLevel)
			{
				ans.push_back(vector<int>());
				//ans.emplace_back();
			}
			ans[theCurLevel].push_back(cur->val);
			//ans.back().push_back(cur->val);

			if (cur->left != nullptr)
			{
				q.push(cur->left);
				ump[cur->left] = theCurLevel + 1;
			}
			if (cur->right != nullptr)
			{
				q.push(cur->right);
				ump[cur->right] = theCurLevel + 1;
			}
		}
		return ans;
	}
};

//法二：每次处理一层的优化bfs
// 使用queue容器实现
class Solution2
{
public:
	vector<vector<int>> levelOrder(TreeNode* root)
	{
		vector<vector<int>> ans;
		if (root == nullptr)
			return ans;

		queue<TreeNode*> que;
		que.push(root);
		while (!que.empty())
		{
			int currLevelSize = que.size();
			vector<int> currLevelans;
			for (int i = 0; i < currLevelSize; i++)
			{
				TreeNode* cur = que.front();
				que.pop();
				currLevelans.push_back(cur->val);
				if (cur->left != nullptr)
				{
					que.push(cur->left);
				}

				if (cur->right != nullptr)
				{
					que.push(cur->right);
				}

			}
			ans.push_back(currLevelans);
		}
		return ans;
	}
};

//用数组模拟队列实现
//其一：
vector<vector<int>> levelOrder(TreeNode* root)
{
	vector<vector<int>> ans;
	if (root == nullptr)
		return ans;

	vector<TreeNode*> que;
	que.push_back(root);
	int l = 0, r = 1;
	while (l < r)
	{
		vector<int> curLevel;
		for (int i = l; i < r; i++)
		{
			TreeNode* cur = que[l++];
			curLevel.push_back(cur->val);

			if (cur->left != nullptr)
			{
				que.push_back(cur->left);
			}
			if (cur->right != nullptr)
			{
				que.push_back(cur->right);
			}
		}
		ans.push_back(curLevel);
		r = que.size();//不要在for循环内更改r，这样会导致输出结果都在同一层
	}
	return ans;
}
//其二：
vector<vector<int>> levelOrder(TreeNode* root)
{
	vector<vector<int>> ans;
	if (root == nullptr)
		return ans;

	vector<TreeNode*> que(2007);//预先分配空间
	que[0] = root;
	int l = 0, r = 1;
	while (l < r)
	{
		int curLevelSize = r - l;
		vector<int> curLevel;
		for (int i = 0; i < curLevelSize; i++)
		{
			TreeNode* cur = que[l++];
			curLevel.push_back(cur->val);

			if (cur->left != nullptr)
				que[r++] = cur->left;
			if (cur->right != nullptr)
				que[r++] = cur->right;
		}
		ans.push_back(curLevel);
	}
	return ans;
}



//题目二：二叉树的锯齿形层序遍历: 从左往右->从右往左->从左往右
//可以用优化后的bfs做
vector<vector<int>> zigzagLevelOrder(TreeNode* root)
{
	vector<vector<int>> ans;
	if (root == nullptr)
		return ans;

	vector<TreeNode*> que(2007);//用数组模拟队列,题目要求节点数目至多2000，这里需提前确定大小，否则无法用que[r/l++]
	int l = 0, r = 0;//队头与队尾
	que[r++] = root;
	bool flag = true;//翻转标志
	while (l < r)
	{
		int curLevelSize = r - l;
		vector<int> curLevel(curLevelSize);//提前确定大小
		for (int i = 0; i < curLevelSize; i++)
		{
			TreeNode* cur = que[l++];

			int index = flag ? i : curLevelSize - 1 - i;
			curLevel[index] = cur->val;
			
			if (cur->left != nullptr)
				que[r++] = cur->left;
			if (cur->right != nullptr)
				que[r++] = cur->right;
		}
		flag = !flag;
		ans.push_back(curLevel);
	}
	return ans;
}
//ps:这里不能通过改变入队顺序来改变输出顺序，因为一旦某处入队顺序改变会影响接下来所有结点的入队



//题目三：二叉树的最大特殊宽度
//可以把它看成完全二叉树，利用编号计算宽度
//依旧借助优化bfs
typedef unsigned long long ull;
int widthOfBinaryTree(TreeNode* root)
{
	int ans = 0;
	if (root == nullptr)
		return ans;

	queue<pair<TreeNode*, int>> q;//结点  对于编号
	q.push(make_pair(root, 1));
	//令根结点编号为1，则左孩子2*i 右孩子2*i+1
	while (!q.empty())
	{
		ull lcode = q.front().second;
		ull rcode = q.back().second;
		ans = max(ans, (int)(rcode - lcode + 1));
		int curLevelSize = q.size();

		for (int i = 0; i < curLevelSize; i++)
		{
			TreeNode* cur = q.front().first;
			ull curCode = q.front().second;
			q.pop();

			if (cur->left != nullptr)
				q.push(make_pair(cur->left, curCode * 2));
			if (cur->right != nullptr)
				q.push(make_pair(cur->right, curCode * 2 + 1));
		}
	}
	return ans;
}



//题目四：求二叉树的最大深度、最小深度
//最大深度：
int maxDepth(TreeNode* root)
{
	if (root == nullptr)
		return 0;
	return max(maxDepth(root->left), maxDepth(root->right)) + 1;
}
//最小深度：
//最小深度是从根节点到最近叶子节点的最短路径上的节点数量。
//小心孩子为空对答案的干扰
int minDepth(TreeNode* root)
{
	if (root == nullptr)//空树
		return 0;

	if (root->left == nullptr && root->right == nullptr)//叶子结点
		return 1;

	int ldepth = INT_MAX;
	int rdepth = INT_MAX;
	if (root->left != nullptr)//防止空结点干扰
		ldepth = minDepth(root->left);
	if (root->right != nullptr)//防止空结点干扰
		rdepth = minDepth(root->right);

	return min(ldepth, rdepth) + 1;
}



//题目五到题目七为序列化内容
// 不存在中序的序列化，因为序列化的结果不唯一

//题目五：二叉树先序序列化和反序列化
//这里以先序为例，反序同理
class Codec 
{
public:
	void serialize_operator(string& s,TreeNode* root)
	{
		if (root == nullptr)
			s += "null,";
		else
		{
			s += to_string(root->val) + ",";
			serialize_operator(s, root->left);
			serialize_operator(s, root->right);
		}
	}

	string serialize(TreeNode* root)//序列化
	{
		string ans;
		serialize_operator(ans, root);
		return ans;
	}

	TreeNode* deserialize_operator(list<string>& lst)
	{
		if (lst.front() == "null")
		{
			lst.pop_front();
			return nullptr;
		}
		TreeNode* newNode = new(TreeNode);
		newNode->val = stoi(lst.front());
		lst.pop_front();
		newNode->left = deserialize_operator(lst);
		newNode->right = deserialize_operator(lst);

		return newNode;
	}

	TreeNode* deserialize(string data) 
	{
		string tmp;//临时存储
		list<string> dataArray;
		for (int i = 0; i < data.size(); i++)
		{
			if (data[i] == ',')
			{
				dataArray.push_back(tmp);
				tmp.clear();
			}
			else
			{
				tmp += data[i];
			}
		}

		if (!tmp.empty())//处理最后一次循环的结果
		{
			dataArray.push_back(tmp);
			tmp.clear();
		}

		return deserialize_operator(dataArray);
	}
};



//题目六：二叉树按层序列化和反序列化
//优化bfs层序遍历:
string serialize(TreeNode* root)
{
	string ans;
	if (root == nullptr)
		return ans;

	queue<TreeNode*> q;
	q.push(root);
	while (!q.empty())
	{
		int curLevelSize = q.size();
		for (int i = 0; i < curLevelSize; i++)
		{
			TreeNode* cur = q.front();
			q.pop();
			if (cur != nullptr)
				ans += to_string(cur->val) + ",";
			else
			{
				ans += "null,";
				continue;
			}
			
			//空结点也入队
			q.push(cur->left);
			q.push(cur->right);
		}
	}
	
	return ans;
}

TreeNode* deserialize(string data)
{
	if (data.empty())
		return nullptr;

	string tmp;
	list<string> lst;
	for (int i = 0; i < data.size(); i++)
	{
		if (data[i] == ',')
		{
			lst.push_back(tmp);
			tmp.clear();
		}
		else
			tmp += data[i];
	}
	if (!tmp.empty())//处理最后一次
	{
		lst.push_back(tmp);
		tmp.clear();
	}

	queue<TreeNode*> q;
	TreeNode* root = new(TreeNode);
	root->val = stoi(lst.front());
	q.push(root);
	lst.pop_front();
	while (!q.empty() && !lst.empty())
	{
		int curLevelSize = q.size();
		for (int i = 0; i < curLevelSize; i++)
		{
			TreeNode* cur = q.front();
			q.pop();

			if (!lst.empty())
			{
				if (lst.front() != "null")
				{
					TreeNode* newNode = new(TreeNode);
					newNode->val = stoi(lst.front());
					lst.pop_front();
					cur->left = newNode;
					q.push(newNode);
				}
				else
				{
					cur->left = nullptr;
					lst.pop_front();
				}
			}
			
			if (!lst.empty())
			{
				if (lst.front() != "null")
				{
					TreeNode* newNode = new(TreeNode);
					newNode->val = stoi(lst.front());
					lst.pop_front();
					cur->right = newNode;
					q.push(newNode);
				}
				else
				{
					cur->right = nullptr;
					lst.pop_front();
				}
			}
		}
	}

	return root;
}



//题目七：利用先序与中序遍历序列构造二叉树
TreeNode* operate(vector<int>& preorder, int l1, int r1,
				  vector<int>& inorder, int l2, int r2,
				  unordered_map<int, int>& ump)
{
	if (l1 > r1 || l2 > r2)
		return nullptr;

	TreeNode* root = new TreeNode(preorder[l1]); // 先序：根左右
	int k = ump[preorder[l1]]; // 根节点在中序数组中的下标

	// 计算左子树节点数
	int leftSubtreeSize = k - l2;

	if (l1 == r1)//或者 l2==r2
		return root;

	// 递归构建左右子树
	root->left = operate(preorder, l1 + 1, l1 + leftSubtreeSize,inorder, l2, k - 1, ump);
	root->right = operate(preorder, l1 + leftSubtreeSize + 1, r1,inorder, k + 1, r2, ump);

	return root;
}

TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) 
{
	if (preorder.empty() || inorder.empty() || preorder.size() != inorder.size())
		return nullptr;

	// 构建中序遍历的值到索引的映射
	unordered_map<int, int> ump;
	for (int i = 0; i < inorder.size(); i++)
	{
		ump[inorder[i]] = i; 
	}

	return operate(preorder, 0, preorder.size() - 1,inorder, 0, inorder.size() - 1, ump);
}



//题目八：验证完全二叉树
//判断要点：
//1.有右孩子但没左孩子 -->> 不是完全二叉树
//2.一旦发现孩子不全的结点，接下来必须全是叶结点
bool isCompleteTree(TreeNode* root)
{
	if (root == nullptr)
		return true;

	vector<TreeNode*> que;
	int l = 0, r = 0;
	bool leap_falg = false;//记录是否遇到过左右两个孩子不双全的结点
	que.push_back(root);
	r = 1;
	while (l < r)
	{
		TreeNode* cur = que[l++];
		if (cur->right != nullptr && cur->left == nullptr
			||
			leap_falg && (cur->left != nullptr || cur->right != nullptr))
			return false;

		if (cur->left != nullptr)
		{
			que.push_back(cur->left);
			r++;
		}
		if (cur->right != nullptr)
		{
			que.push_back(cur->right);
			r++;
		}

		if (cur->left == nullptr || cur->right == nullptr)
			leap_falg = true;
	}
	return true;
}



//题目九：已知该树为完全二叉树，求完全二叉树的节点个数，要求时间复杂度低于O(n)
//充分利用完全二叉树性质，时间复杂度O(logn)
int getLeftDepth(TreeNode* node, int level)// 计算从当前节点向左子节点走的最大深度
{
	while (node != nullptr) 
	{
		level++;
		node = node->left;
	}
	return level - 1;
}

int calculateNodes(TreeNode* node, int level, int maxDepth)// 递归计算节点数量
{
	if (node == nullptr) return 0;
	if (level == maxDepth) return 1;

	// 计算右子树的最左深度
	int rightLeftDepth = getLeftDepth(node->right, level + 1);

	if (rightLeftDepth == maxDepth) 
	{
		// 右子树的最左节点到达最深层，说明左子树是满二叉树
		return (1 << (maxDepth - level)) + calculateNodes(node->right, level + 1, maxDepth);
	}
	else
	{
		// 右子树的最左节点未到达最深层，说明右子树是满二叉树（比左子树少一层）
		return (1 << (maxDepth - level - 1)) + calculateNodes(node->left, level + 1, maxDepth);
	}
}

int countNodes(TreeNode* root)
{
	if (root == nullptr) return 0;

	// 计算整棵树的最大深度
	int maxDepth = getLeftDepth(root, 1);

	// 计算节点总数
	return calculateNodes(root, 1, maxDepth);
}


//普通前序遍历递归求法，时间复杂度O(n)
int n = 0;
void preOrder(TreeNode* cur)
{
	if (cur == nullptr)
		return;
	n++;
	preOrder(cur->left);
	preOrder(cur->right);
}

int countNodes(TreeNode* root)
{
	if (root == nullptr)
		return 0;
	preOrder(root);
	return n;
}



//题目十：普通二叉树上寻找两个结点的最近公共祖先
//lca问题，非常重要
//要整体理解，才可以记下来
//核心想法是：遇到要找的节点，就返回不空（true）。然后一路返回合并，
//如果返回的路上，没有遇到两个都是不空的情况，说明自己就是两个点的共同祖先（走最后一句的return）
//如果都为不空，说明当前节点就是公共祖先，会返回当前节点。 有且仅切且一个节点会存在都不为空的情况
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q)
{
	if (root == nullptr || root == p || root == q)//base case: 遇到空，或者p，或者q，直接返回  
		return root;

	TreeNode* l = lowestCommonAncestor(root->left, p, q);// 在左子树中找 p 或 q
	TreeNode* r = lowestCommonAncestor(root->right, p, q);// 在右子树中找 p 或 q

	if (l != nullptr && r != nullptr)// 左树也搜到，右树也搜到，返回root  → 当前 root 就是最近公共祖先
		return root;

	if (l == nullptr && r == nullptr)// 都没搜到返回空  
		return nullptr;

	//只找到一个 → 把那个往上返回
	return l == nullptr ? r : l;
}



//题目十一：搜索二叉树上寻找两个节点的最近公共祖先
//二叉搜索树：每一个结点的左数都小于他自己，右数都大于他自己
//思路：根据二叉搜索树的性质：
//1.如果root在p~q左侧，那么root往右移动
//2.如果root在p~q右侧，那么root往左移动
//3.如果root在p~q之间，那么root就是p,q最近公共祖先
//4.如果先遇到p，则p是答案
//5.如果先遇到q，则q是答案
//
//递归
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q)
{
	if (root == nullptr || root == p || root == q)
		return root;

	if (root->val < min(p->val, q->val))
		return lowestCommonAncestor(root->right, p, q);
	if (root->val > max(p->val, q->val))
		return lowestCommonAncestor(root->left, p, q);

	if (root->val > min(p->val, q->val) && root->val < max(p->val, q->val))
		return root;

	return root == p ? p : q;
}
//迭代
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q)
{
	while (root->val != p->val && root->val != q->val)
	{
		if (root->val > min(p->val, q->val) && root->val < max(p->val, q->val))
			break;
		root = root->val < min(p->val, q->val) ? root->right : root->left;
	}
	return root;
}



//题目十二：收集累加和等于aim的所有路径 （递归恢复现场）
vector<int> path;//历史路径
vector<vector<int>> ans;//最终答案

void dfs(TreeNode* cur, int preSum, int targetSum)
{
	if (cur->left == nullptr && cur->right == nullptr)//叶子结点
	{
		if (preSum + cur->val == targetSum)//符合要求
		{
			path.push_back(cur->val);
			ans.push_back(path);

			path.pop_back();//回溯
		}
	}
	else//非叶结点
	{
		path.push_back(cur->val);
		if (cur->left != nullptr)
		{
			dfs(cur->left, preSum + cur->val, targetSum);
		}
		if (cur->right != nullptr)
		{
			dfs(cur->right, preSum + cur->val, targetSum);
		}
		path.pop_back();//回溯
	}

}

vector<vector<int>> pathSum(TreeNode* root, int targetSum)
{
	if (root == nullptr)
		return ans;

	dfs(root, 0, targetSum);

	return ans;
}



//题目十三：验证平衡二叉树(树形dp沾边)
bool balence;

int height(TreeNode* cur)
{
	if (cur == nullptr || !balence)
	{
		return 0;
	}

	int lheight = height(cur->left);
	int rheight = height(cur->right);
	if (abs(lheight - rheight) >= 2)//已经不平衡了，返回啥无所谓
	{
		balence = false;
		return 0;
	}
	return max(lheight, rheight) + 1;
}

bool isBalanced(TreeNode* root) 
{
	balence = true;
	height(root);

	return balence;
}



//题目十四：验证搜索二叉树(树形dp沾边)
//中序遍历的结果为升序，则一定是搜索二叉树
//非递归
bool isValidBST(TreeNode* root)
{
	if (root == nullptr)
		return true;

	vector<TreeNode*> stk;
	TreeNode* prev = nullptr;
	TreeNode* cur = root;  

	while (cur != nullptr || !stk.empty())
	{
		while (cur != nullptr)
		{
			stk.push_back(cur);
			cur = cur->left;
		}

		TreeNode* node = stk.back();
		stk.pop_back();

		if (prev != nullptr && prev->val >= node->val)
		{
			return false;
		}

		prev = node;
		cur = node->right;  // 处理右子树
	}
	return true;
}
//递归:全局变量min和max
long mins, maxs;

bool isValidBST(TreeNode* node)
{
	if (node == nullptr)
	{
		mins = LONG_MAX;
		maxs = LONG_MIN;
		return true;
	}

	bool lok = isValidBST(node->left);
	long lmax = maxs;
	long lmin = mins;
	bool rok = isValidBST(node->right);
	long rmax = maxs;
	long rmin = mins;

	if (lok && rok && lmax < node->val && node->val < rmin)
	{
		maxs = max(max(lmax, rmax), (long)node->val);
		mins = min(min(lmin, rmin), (long)node->val);
		return true;
	}
	return false;
}



//题目十五：修剪搜索二叉树
TreeNode* trimBST(TreeNode* cur, int low, int high)
{
	if (cur == nullptr)
		return nullptr;

	if (cur->val < low)
		return trimBST(cur->right, low, high);
	if (cur->val > high)
		return trimBST(cur->left, low, high);

	//cur->val介于low与high之间
	cur->left = trimBST(cur->left, low, high);
	cur->right = trimBST(cur->right, low, high);
	return cur;
}



//题目十六：二叉树打家劫舍问题（树形dp沾边）
//全局变量，完成了 X子树 的遍历，返回之后
//Yes变成，X子树在偷头结点的情况下，最大的收益
int Yes;

//全局变量，完成了 X子树 的遍历，返回之后
//No变成，X子树在不偷头结点的情况下，最大的收益
int No;

void dp(TreeNode* cur)
{
	if (cur == nullptr)
	{
		Yes = 0;
		No = 0;
	}
	else
	{//局部的yes与no：针对于是否选了当前结点
		int cur_yes = cur->val;
		int cur_no = 0;

		dp(cur->left);
		cur_yes += No;//如果选了当前节点，就加上子树的No
		cur_no += max(Yes, No);// 如果没选自己，就比较自己和自己相连的节点，选大还是不选大，选比较大的那个 
		//此时全局变量Yes与No都表示子树的Yes与No的结果
		dp(cur->right);
		cur_yes += No;
		cur_no += max(Yes, No);

		//把全局变量Yes与No更新至当前结点
		Yes = cur_yes;
		No = cur_no;
	}

}

int rob(TreeNode* root)
{
	dp(root);
	return max(Yes, No);
}
