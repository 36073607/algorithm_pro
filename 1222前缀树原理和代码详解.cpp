#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <utility>
#include <unordered_map>
using namespace std;

//前缀树又叫字典树（trie）
//每个样本 都从头结点开始 根据 前缀字符或者前缀数字 建出一颗大树，就是前缀树
//没有路就新建结点；已经有路了，就复用结点

//前缀树的使用场景：需要根据前缀信息来查询的场景
//前缀树的优点：根据前缀信息选择树上的分支，可以节省大量时间
//前缀树的缺点：比较浪费空间，和总字符数量有关，字符的种类有关
//前缀树的定制：pass , end等信息
//注：通常把字符信息放在树的边上，结点就维持p信息、e信息等


//实现前缀树Trie类：
//1.Trie() 初始化前缀树对象
//2.void insert(string word) 将字符串word插入前缀树中
//3.int search(string word) 返回前缀树中字符串word的实例个数
//4.int prefixNumber(string prefix) 返回前缀树中以prefix为前缀的字符串个数
//5.void delete(string word) 从前缀树中移除字符串word

//类描述的实现方式（动态结构）。不推荐！虽然最常用
class Trie1//路是数组实现：路的可能性范围较小，用 固定数组 实现路
{
private:
	class TrieNode
	{
	public:
		int pass;
		int end;
		TrieNode* nexts[26];

		TrieNode()
		{
			pass = 0;
			end = 0;
			for (int i = 0; i < 26; i++)
				nexts[i] = nullptr;
		}

		~TrieNode()
		{
			for (int i = 0; i < 26; i++)
			{
				if (nexts[i] != nullptr)
				{
					delete nexts[i];
					nexts[i] = nullptr;
				}
			}
		}
	};
	TrieNode* root;//根结点

public:
	Trie1()
	{
		root = new TrieNode();
	}

	~Trie1()
	{
		delete root;
	}

	void insert(string word)
	{
		TrieNode* node = root;
		node->pass++;
		for (int i = 0; i < word.size(); i++)//从左往右遍历字符串中的每个字符
		{
			int path = word[i] - 'a';//由字符，对应走哪条路
			if (node->nexts[path] == nullptr)
				node->nexts[path] = new TrieNode();
			node = node->nexts[path];
			node->pass++;
		}
		node->end++;
	}

	void erase(string word)
	{
		if (countWordsEqualTo(word) > 0)//前缀树中存有该字符串
		{
			TrieNode* node = root;
			node->pass--;
			stack<pair<TrieNode*, int>> s;
			for (int i = 0; i < word.size(); i++)
			{
				int path = word[i] - 'a';
				if (--node->nexts[path]->pass == 0)
				{
					s.push(make_pair(node, path));
				}

				node = node->nexts[path];
			}
			node->end--;

			while (!s.empty())
			{
				// 直接获取并删除栈顶元素
				TrieNode* parent = s.top().first;
				int path = s.top().second;
				s.pop();

				if (parent->nexts[path] != nullptr)
				{
					delete parent->nexts[path];
					parent->nexts[path] = nullptr;
				}
			}
		}
	}

	int countWordsEqualTo(string word)// 查询前缀树里，word单词出现了几次
	{
		TrieNode* node = root;
		for (int i = 0; i < word.size(); i++)
		{
			int path = word[i] - 'a';
			if (node->nexts[path] == nullptr)
				return 0;
			node = node->nexts[path];
		}
		return node->end;
	}

	int countWordsStartingWith(string pre)// 查询前缀树里，有多少单词以pre做前缀
	{
		TrieNode* node = root;
		for (int i = 0; i < pre.size(); i++)
		{
			int path = pre[i] - 'a';
			if (node->nexts[path] == nullptr)
				return 0;
			node = node->nexts[path];
		}
		return node->pass;
	}
};

class Trie2//路是哈希表实现：路的可能性范围较大，用 哈希表 实现路
{
private:
	class TrieNode
	{
	public:
		int pass;
		int end;
		unordered_map<int, TrieNode*> ump;

		TrieNode()
		{
			pass = 0;
			end = 0;
		}

	};
	TrieNode* root;

public:
	//...与Trie1实现类似,只不过把数组换成表，能存储的字符更多
};




//静态数组的实现方式。推荐！不仅笔试，就连比赛也能保证使用
// 测试链接：https://www.nowcoder.com/practice/7f8a8553ddbf4eaab749ec988726702b
//1.一切都是静态数组来实现，提交准备好够用的空间
//2.如果路的可能性范围较大，就用每一位的信息建树。-->>在 前缀树的题目 有展示
class Trie
{
private://这里用静态成员变量，所有Trie类的实例都共享同一个数组、cnt等。对于这道题效率高，但要记得：1.静态成员必须在类外初始化  2.clear()
	static const int MAXSIZE = 200005;//作用域为类的常量
	static int cnt;//当前节点数量，记录来到了第几层: 二维数组Tree的第cnt行
	static int Tree[MAXSIZE][26];
	static int pass[MAXSIZE];// 经过该节点的单词数量
	static int end[MAXSIZE];// 以该节点结尾的单词数量
	
public:
	Trie()
	{
		cnt = 1;//第0层置空，默认从第1层开始
	}

	void insert(string word)
	{
		int cur = 1;//从根结点开始，指向当前层数
		pass[cur]++;
		for (int i = 0; i < word.size(); i++)
		{
			int path = word[i] - 'a';
			if (Tree[cur][path] == 0)
			{
				Tree[cur][path] = ++cnt;//创建新结点
			}
			cur = Tree[cur][path];//移动到子结点
			pass[cur]++;
		}
		end[cur]++;
	}

	int search(string word)
	{
		int cur = 1;
		for (int i = 0; i < word.size(); i++)
		{
			int path = word[i] - 'a';
			if (Tree[cur][path] == 0)
				return 0;
			cur = Tree[cur][path];
		}
		return end[cur];
	}

	int prefixNumber(string word)
	{
		int cur = 1;
		for (int i = 0; i < word.size(); i++)
		{
			int path = word[i] - 'a';
			if (Tree[cur][path] == 0)
				return 0;
			cur = Tree[cur][path];
		}
		return pass[cur];
	}

	void erase (string word)
	{
		if (search(word) == 0)//前缀树中没有该字符串
			return;

		//前缀树中有该字符串
		int cur = 1;
		pass[cur]--;
		for (int i = 0; i < word.size(); i++)
		{
			int path = word[i] - 'a';
			if (--pass[Tree[cur][path]] == 0)
			{
				Tree[cur][path] = 0;//调整父结点
				return;
			}
			cur = Tree[cur][path];
		}
		end[cur]--;
	}

	void clear()
	{
		for (int i = 1; i <= cnt; i++)
		{
			for (int j = 0; j < 26; j++)
			{
				Tree[i][j] = 0;
			}
			pass[i] = 0;
			end[i] = 0;
		}
		cnt = 1;
	}

};

// 静态成员初始化 ( 静态成员必须在类外初始化 )
int Trie::cnt = 1;
int Trie::Tree[MAXSIZE][26] = { 0 };
int Trie::pass[MAXSIZE] = { 0 };
int Trie::end[MAXSIZE] = { 0 };

int main() {
	int m;
	cin >> m;
	Trie trie;
	for (int i = 0; i < m; i++) {
		int op;
		string str;
		cin >> op >> str;

		switch (op) {
		case 1:
			trie.insert(str);
			break;
		case 2:
			trie.erase(str);
			break;
		case 3:
			if (trie.search(str))
				cout << "YES" << endl;
			else
				cout << "NO" << endl;
			break;
		case 4:
			cout << trie.prefixNumber(str) << endl;
		}
	}
	trie.clear();
	return 0;
}
//关于使用静态数组实现字符种类很多的情况，可以把path按位打散，依次往下扎
//此内容在 前缀树相关题目 中可见
