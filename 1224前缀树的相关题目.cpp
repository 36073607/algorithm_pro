#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
using namespace std;


//题目一：
// 牛牛和他的朋友们约定了一套接头密匙系统，用于确认彼此身份
// 密匙由一组数字序列表示，两个密匙被认为是一致的，如果满足以下条件：
// 密匙 b 的长度不超过密匙 a 的长度。
// 对于任意 0 <= i < length(b)，有b[i+1] - b[i] == a[i+1] - a[i]
// 现在给定了m个密匙 b 的数组，以及n个密匙 a 的数组
// 请你返回一个长度为 m 的结果数组 ans，表示每个密匙b都有多少一致的密匙
// 数组 a 和数组 b 中的元素个数均不超过 10^5
// 1 <= m, n <= 1000
// 测试链接 : https://www.nowcoder.com/practice/c552d3b4dfda49ccb883a6371d9a6932
//
//值得注意的点：
//1.为了避免起义，转化为字符串时，数组中每一个数后面加 # 表示隔断
//2.为了避免元素之间差值过大导致内存浪费，eg:[3,1000,10000]-> 997，9000
//  可以在前缀树中设置为：9->9->7->#  ;   9->0->0->0->#

class CountConsistentKeys
{
private:
	class Trie
	{
	private:
		static const int MAXSIZE = 2000001;
		static int cnt;
		static int Tree[MAXSIZE][12];
		static int pass[MAXSIZE];
		static int end[MAXSIZE];

	public:
		Trie()
		{
			cnt = 1;
		}

		int path(char ch)
		{
			if (ch == '-')
				return 10;
			else if (ch == '#')
				return 11;
			else
				return ch - '0';
		}

		void insert(string word)
		{
			int cur = 1;
			pass[cur]++;
			for (int i = 0; i < word.size(); i++)
			{
				int _path = path(word[i]);
				if (Tree[cur][_path] == 0)
					Tree[cur][_path] = ++cnt;

				cur = cnt;
				pass[cur]++;
			}
			end[cur]++;
		}

		void erase(string word)
		{
			if (search(word) == 0)
				return;

			int cur = 1;
			pass[cur]--;
			for (int i = 0; i < word.size(); i++)
			{
				int _path = path(word[i]);
				if (--pass[Tree[cur][_path]] == 0)
				{
					Tree[cur][_path] = 0;
					return;
				}
				cur = Tree[cur][_path];
			}
			end[cur]--;
		}

		int search(string word)
		{
			int cur = 1;
			for (int i = 0; i < word.size(); i++)
			{
				int _path = path(word[i]);
				if (Tree[cur][_path] == 0)
					return 0;
				cur = Tree[cur][_path];
			}
			return end[cur];
		}

		int prefixNumber(string word)
		{
			int cur = 1;
			for (int i = 0; i < word.size(); i++)
			{
				int _path = path(word[i]);
				if (Tree[cur][_path] == 0)
					return 0;
				cur = Tree[cur][_path];
			}
			return pass[cur];
		}

		void clear()
		{
			for (int i = 1; i <= cnt; i++)
			{
				for (int j = 0; j < 12; j++)
				{
					Tree[i][j] = 0;
				}
				pass[i] = 0;
				end[i] = 0;
			}
			cnt = 1;
		}

	};
	

public:
	vector<int> countConsistentKeys(vector<vector<int> >& b, vector<vector<int> >& a)
	{
		vector<int> ans;
		Trie trie;
		for (int i = 0; i < a.size(); i++)
		{
			string tmp;
			for (int j = 1; j < a[i].size(); j++)
			{
				int diff = a[i][j] - a[i][j - 1];
				tmp += to_string(diff) + '#';
			}
			trie.insert(tmp);
		}

		for (int i = 0; i < b.size(); i++)
		{
			string tmp;
			for (int j = 1; j < b[i].size(); j++)
			{
				int diff = a[i][j] - a[i][j - 1];
				tmp += to_string(diff) + '#';
			}
			ans.push_back(trie.prefixNumber(tmp));
		}
		trie.clear();
		return ans;
	}

};
// 静态成员初始化
int CountConsistentKeys::Trie::cnt = 1;
int CountConsistentKeys::Trie::Tree[2000001][12] = { 0 };
int CountConsistentKeys::Trie::pass[2000001] = { 0 };
int CountConsistentKeys::Trie::end[2000001] = { 0 };




//题目二：
// 数组中两个数的最大异或值
// 给你一个整数数组 nums ，返回 nums[i] XOR nums[j] 的最大运算结果，其中 0<=i<=j<=n
// 1 <= nums.length <= 2 * 10^5
// 0 <= nums[i] <= 2^31 - 1
// 测试链接 : https://leetcode.cn/problems/maximum-xor-of-two-numbers-in-an-array/

//法一：
//Trie树中每个从根节点到叶节点的路径都对应数组中的一个数字。
class TwoNumbersMaximumXor1//前缀树做法
{
private:
	class Trie
	{
	private:
		const static int MAXSIZE = 3000001;
		static int Tree[3000001][2];//此题不需要pass数组、end数组
		static int cnt;// 前缀树目前使用了多少空间
		static int high;//记录数组中最大元素的最高位，数字只需要从哪一位开始考虑

	public:
		Trie(vector<int>& nums)//构造函数
		{
			cnt = 1;
			int maxs = 0;
			for (int i = 0; i < nums.size(); i++)
			{
				if (nums[i] > maxs)
					maxs = nums[i];
			}

			high = findHigh(maxs);
			for (int i = 0; i < nums.size(); i++)
				insert(nums[i]);
		}

		int findHigh(int num)
		{
			int i = 31;
			for (; i >= 0; i--)
			{
				if ((((unsigned)num >> i) & 1) != 0)
					return i;
			}
			return 0;
		}

		void insert(int num)
		{
			int cur = 1;
			for (int i = high; i >= 0; i--)
			{
				int path = ((unsigned)num >> i) & 1;
				if (Tree[cur][path] == 0)
					Tree[cur][path] = ++cnt;
				cur = Tree[cur][path];
			}
		}

		void clear()
		{
			for (int i = 1; i <= cnt; i++)
			{
				for (int j = 0; j < 2; j++)
					Tree[i][j] = 0;
			}
			cnt = 1;
			high = 0;
		}

		int findMAX(int num)
		{
			int ans = 0;
			int cur = 1;
			for (int i = high; i >= 0; i--)
			{
				int status = (unsigned)num >> i & 1; //num第i位的状态
				int want = status ^ 1;//num第i位希望遇到的状态
				if (Tree[cur][want] == 0)//没有路
				{
					want ^= 1;//没有想要的路，只能选择另一条路
				}
				//此时want为即将要选择的路
				ans |= (status ^ want) << i;
				cur = Tree[cur][want];
			}
			return ans;
		}

	};

	
public:
	int findMaximumXOR(vector<int>& nums)
	{
		int ans = 0;
		Trie trie(nums);
		for (int i = 0; i < nums.size(); i++)
		{
			ans = max(ans, trie.findMAX(nums[i]));
			//由于每个元素要从前缀树挨个找，即使数组中最大的数maxs也有可能 因为没有第二个high位有1的数而导致maxs的high异或完为0	
		}
		trie.clear();
		return ans;
	}

};

//类外初始化静态成员
int TwoNumbersMaximumXor1::Trie::Tree[3000001][2] = { 0 };
int TwoNumbersMaximumXor1::Trie::cnt = 1;
int TwoNumbersMaximumXor1::Trie::high = 0;



//法二：
//有点贪心的思想，如果答案有高位是1，那么这个1是一定要得，后面低位哪怕是全1也没有这个高位的1给结果的影响大
//所以可以直接从高位慢慢去低位找1。
//
//技巧：如何只保留一个数字的第i~j(j>i)位，其他位全清空？
// ( x>>i ) << i  ,先右移后左移，这样j~i位保留，其他位全为0

class TwoNumbersMaximumXor2//哈希表做法
{
private:
	int findHigh(int num)
	{
		int i = 31;
		for (; i >= 0; i--)
		{
			if ((((unsigned)num >> i) & 1) != 0)
				return i;
		}
		return 0;
	}

public:
	int findMaximumXOR(vector<int>& nums)
	{
		int ans = 0;// ans : 31....i+1 已经达成的目标
		int maxs = 0;
		for (int i = 0; i < nums.size(); i++)
		{
			if (nums[i] > maxs)
				maxs = nums[i];
		}
		int high = findHigh(maxs);
		
		unordered_set<int> st;
		for (int i = high; i >= 0; i--)
		{
			int want = ans | (1 << i);//想要的状态：最高位为1
			st.clear();//只使用一个哈希表，每次都清空，这会使空间消耗更低

			for (int j = 0; j < nums.size(); j++)// 将所有数字的前i位（高位部分）存入集合
			{
				int num = (nums[j] >> i) << i;// num : 31.....i 这些状态保留，剩下全成0
				st.insert(num);

				if (st.find(num ^ want) != st.end())// num ^ 某状态 是否能 达成want目标，就在set中找 某状态 : want ^ num
				{
					ans = want;
					break;
				}
			}

		}
		return ans;
	}

};




//题目三：
// 在二维字符数组中搜索可能的单词
// 给定一个 m x n 二维字符网格 board 和一个单词（字符串）列表 words
// 返回所有二维网格上的单词。单词必须按照字母顺序，通过 相邻的单元格 内的字母构成
// 其中“相邻”单元格是那些水平相邻或垂直相邻的单元格
// 同一个单元格内的字母在一个单词中不允许被重复使用
// 1 <= m, n <= 12
// 1 <= words.length <= 3 * 10^4
// 1 <= words[i].length <= 10
// 测试链接 : https://leetcode.cn/problems/word-search-ii/

//时间复杂度，O(m*n* 4^10)
///不管用不用前缀树都是这个复杂度1，只不过通过使用前缀树可以大量剪枝，优化常数时间
//空间复杂度，O(words中所有字符串的全部字符数量)
const int MAXSIZE = 10001;
int Tree[MAXSIZE][26];
int cnt = 1;
int pass[MAXSIZE];
string end_[MAXSIZE];

class WordSearchII
{
public:
	void buildTrie(vector<string>& words)
	{
		for (auto& word : words)
		{
			int cur = 1;
			pass[cur]++;
			for (int i = 0; i < word.size(); i++)
			{
				int path = word[i] - 'a';
				if (Tree[cur][path] == 0)
					Tree[cur][path] = ++cnt;
				cur = Tree[cur][path];
				pass[cur]++;
			}
			end_[cur] = word;
		}
	}

	void clear()
	{
		for (int i = 1; i <= cnt; i++)
		{
			for (int j = 0; j < 26; j++)
				Tree[i][j] = 0;
			pass[i] = 0;
			end_[i] = "";
		}
		cnt = 1;
	}


	vector<string> findWords(vector<vector<char>>& board, vector<string>& words)
	{
		vector<string> ans;
		buildTrie(words);
		for (int i = 0; i < board.size(); i++)
		{
			for (int j = 0; j < board[i].size(); j++)
			{
				dfs(board, i, j, 1, ans);
				//i,j : 第i行第j列
				//1 ：每次从前缀树开头（编号为1）进行剪枝
			}
		}
		clear();
		return ans;
	}

	//子过程：来到一个位置后，先判断该位置能不能来；
	//        若能来，判断该位置能否收集到一个字符串：若能，则把该字符串收集；若不能，则上下左右四个位置依次判断（记得回溯）
	//
	//剪枝条件：对于方阵(i,j)位置上的元素：
	//1.不越界且在前缀树上
	//2.该元素未被访问过
	//3.该元素对应的前缀树层级t，是否已经把字符串收集完全了 -> pass[t]
	int dfs(vector<vector<char>>& board, int i, int j, int t, vector<string>& ans)
	{
		if (i<0 || i>=board.size() || j<0 || j>=board[0].size() || board[i][j] == 0)//越界 或 该元素已经访问过
			return 0;

		char cur = board[i][j];//取出当前的board数组元素
		int road = board[i][j] - 'a';//path
		//road:
		//a -> 0
		//b -> 1
		//c -> 2
		// ...
		//z -> 25
		t = Tree[t][road];//更新 新到达的前缀树层数
		if (pass[t] == 0)//等价于: t==0||pass[t]==0  ,但当pass[t]==0时，t一定等于0
			return 0;
		//pass[t]==0说明之前的过程已经把字符串收集完了

		//当前i,j位置有必要来
		int fix = 0;//记录一共收集到几个字符串
		if (end_[t] != "")//到达前缀树末端
		{
			fix++;
			ans.push_back(end_[t]);
			end_[t] = "";
		}

		board[i][j] = 0;//i,j位置已经来过，重置为0
		fix += dfs(board, i + 1, j, t, ans);
		fix += dfs(board, i - 1, j, t, ans);
		fix += dfs(board, i, j + 1, t, ans);
		fix += dfs(board, i, j - 1, t, ans);
		pass[t] -= fix;//更新pass信息
		board[i][j] = cur;
		return fix;
	}

};

//封装一下：
class Solution 
{
private:
	static const int MAXSIZE = 10001;
	int Tree[MAXSIZE][26];
	int pass[MAXSIZE];
	string end_[MAXSIZE];
	int cnt = 1;

	void buildTrie(vector<string>& words) 
	{
		for (auto& word : words) 
		{
			int cur = 1;
			pass[cur]++;
			for (int i = 0; i < word.size(); i++) 
			{
				int path = word[i] - 'a';
				if (Tree[cur][path] == 0) 
				{
					Tree[cur][path] = ++cnt;
				}
				cur = Tree[cur][path];
				pass[cur]++;
			}
			end_[cur] = word;
		}
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
			end_[i].clear();
		}
		cnt = 1;
	}

	int dfs(vector<vector<char>>& board, int i, int j, int t, vector<string>& ans)
	{
		if (i < 0 || i >= board.size() || j < 0 || j >= board[0].size() || board[i][j] == 0)
		{
			return 0;
		}

		char tmp = board[i][j];
		int road = tmp - 'a';
		t = Tree[t][road];

		if (t == 0 || pass[t] == 0)
		{
			return 0;
		}

		int fix = 0;
		if (!end_[t].empty()) 
		{
			fix++;
			ans.push_back(end_[t]);
			end_[t].clear();
		}

		board[i][j] = 0;
		fix += dfs(board, i - 1, j, t, ans);
		fix += dfs(board, i + 1, j, t, ans);
		fix += dfs(board, i, j - 1, t, ans);
		fix += dfs(board, i, j + 1, t, ans);

		pass[t] -= fix;
		board[i][j] = tmp;
		return fix;
	}

public:
	vector<string> findWords(vector<vector<char>>& board, vector<string>& words)
	{
		buildTrie(words);
		vector<string> ans;

		for (int i = 0; i < board.size(); i++) 
		{
			for (int j = 0; j < board[0].size(); j++) 
			{
				dfs(board, i, j, 1, ans);
			}
		}

		clear();
		return ans;
	}
};