#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <utility>
#include <climits>
#include <algorithm>
using namespace std;



//邻接矩阵、邻接表建图详解
// 
//有向 vs 无向 、不带权 vs 带权
//入参一般为 ：节点数量n和所有的边 或者 直接给图
// 
// 建图的三种方式：
//1.邻接矩阵（适合点的数量不多的图）
//2.邻接表（最常用的方式）
//3.链式前向星（空间要求严苛的情况下使用。比赛必用，大厂笔试、面试不常用）


//题目一：
//三种方式建图的展示
// 
//邻接矩阵
const int MAXvertex = 11;//点的最大数量

vector<vector<int>> Adjmatrix(MAXvertex, vector<int>(MAXvertex, 0));//建图

void build_Adjmatrix(int n)//清空图结构
{
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			Adjmatrix[i][j] = INT_MAX;//根据具体情况设置
		}
	}
}

void directGraph_Adjmatrix(vector<vector<int>>& edges)//邻接矩阵建 有向带权图
{
	for (auto& edge : edges)
	{
		Adjmatrix[edge[0]][edge[1]] = edge[2];
	}
}

void undirectGraph_Adjmatrix(vector<vector<int>>& edges)//邻接矩阵建 无向带权图
{
	for (auto& edge : edges)
	{
		Adjmatrix[edge[0]][edge[1]] = edge[2];
		Adjmatrix[edge[1]][edge[0]] = edge[2];
	}
}

void traversal_Adjmatrix(int n)//邻接矩阵遍历
{
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			cout << Adjmatrix[i][j] << " ";
		}
		cout << endl;
	}
}


//邻接表
const int MAXvertex = 11;//点的最大数量

vector<vector<pair<int, int>>> AdjList(MAXvertex);//建图 （带权）
vector<vector<int>> AdjList_noweight(MAXvertex);//建图 （不带权）

void build_AdjList(int n)//清空图结构
{
	AdjList.clear();
	// 下标需要支持1~n，所以加入n+1个列表，0下标准备但不用
	for (int i = 0; i <= n; i++)
		AdjList.push_back(vector<pair<int, int>>());
}

void directGraph_AdjList(vector<vector<int>>& edges)//邻接表建 有向带权图
{
	for (auto& edge : edges)
	{
		AdjList[edge[0]].push_back(make_pair(edge[1], edge[2]));
	}
}
// 点1   点2 权值
//  1  { {2, 3} , {4, 7} } -->> (1->2)权值为3  ； (1->4)权值为7

void undirectGraph_AdjList(vector<vector<int>>& edges)//邻接表建 无向带权图
{
	for (auto& edge : edges)
	{
		AdjList[edge[0]].push_back(make_pair(edge[1], edge[2]));
		AdjList[edge[1]].push_back(make_pair(edge[0], edge[2]));
	}
}

void traversal_AdjList(int n)//邻接表遍历
{
	for (int i = 1; i <= n; i++)
	{
		cout << i << "（邻居、边权）：";
		for (auto& edge : AdjList[i])
		{
			cout << "(" << edge.first << " " << edge.second << ")";
		}
		cout << endl;
	}
}


//链式前向星
const int MAXvertex = 11;//点的最大数量
const int MAXedges = 21;//边的最大数量
// 只有链式前向星方式建图需要这个数量
// 注意如果无向图的最大数量是m条边，数量要准备m*2
// 因为一条无向边要加两条有向边

//建图
int head[MAXvertex];//下标：点的编号   值：最新的边
int next[MAXedges];//下标：边的编号   值：该顶点的另一条边 （一般来说，当新增一条边时，该新增的边的next[]值为该点的上一条增添的边的编号）
int to[MAXedges];//下标：边的编号   值：这条边上的另一个顶点
int weight[MAXedges];//下标：边的编号   值：这条边的权值
int edge_cnt = 1;//记录边的数量，可以充当边的编号

void build_LinkedForwardStar(int n) // 链式前向星清空
{
	edge_cnt = 1;
	for (int i = 1; i <= n; i++)
		head[i] = 0;
}

void addEdge(int u, int v, int w)
{
	// u -> v ，边权重为w
	to[edge_cnt] = v;
	::next[edge_cnt] = head[u];
	weight[edge_cnt] = w;
	head[u] = edge_cnt;
	edge_cnt++;
}

void directGraph_LinkedForwardStar(vector<vector<int>>& edges)//链式前向星建 有向带权图
{
	for (auto& edge : edges)
	{
		addEdge(edge[0], edge[1], edge[2]);
	}
}

void undirectGraph_LinkedForwardStar(vector<vector<int>>& edges)//链式前向星建 无向带权图
{
	for (auto& edge : edges)
	{
		addEdge(edge[0], edge[1], edge[2]);
		addEdge(edge[1], edge[0], edge[2]);
	}
}

void traversal_LinkedForwardStar(int n)//链式前向星遍历
{
	for (int i = 1; i <= n; i++)
	{
		cout << i << "(邻居、边权) : ";
		for (int ei = ::next[i]; ei > 0; ei = ::next[ei])
		{
			cout << "(" << to[ei] << " " << weight[ei] << ")";
		}
		cout << endl;
	}
}



//拓扑排序
//每个节点的前置节点都在这个结点之前
//要求：有向图、没有环
//拓扑排序的顺序可能不止一种。拓扑排序也可以用来判断有没有环
//1.在图中找到所有入度为0的点
//2.把所有入度为0的点在图中删掉，重点是删掉影响！继续找到入度为0的点并删掉影响
//3.直到所有点都被删掉，依次删除的顺序就是正确的拓扑排序结果
//4.如果无法把所有的点都删掉，说明有向图里有环

//注意：本节课讲解拓扑排序直接使用解决的题目，下节课会讲拓扑排序扩展技巧解决的题目


//题目二：
// 拓扑排序模版（Leetcode）
// 邻接表建图（动态方式）
// 课程表II
// 现在你总共有 numCourses 门课需要选，记为 0 到 numCourses - 1
// 给你一个数组 prerequisites ，其中 prerequisites[i] = [ai, bi]
// 表示在选修课程 ai 前 必须 先选修 bi
// 例如，想要学习课程 0 ，你需要先完成课程 1 ，我们用一个匹配来表示：[0,1]
// 返回你为了学完所有课程所安排的学习顺序。可能会有多个正确的顺序
// 你只要返回 任意一种 就可以了。如果不可能完成所有课程，返回 一个空数组
// 测试链接 : https://leetcode.cn/problems/course-schedule-ii/
class Solution
{
public:
	vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites)
	{
		vector<vector<int>> AdjList(numCourses);//不带权邻接表
		vector<int> indegree(numCourses);
		int l = 0, r = 0;//indegree的队头、队尾

		for (auto& ele : prerequisites)//建立有向图
		{
			AdjList[ele[1]].push_back(ele[0]);
			indegree[ele[0]]++;
		}

		vector<int> ans1(numCourses);
		vector<int> ans2;
		for (int i = 0; i < numCourses; i++)
		{
			if (indegree[i] == 0)
				ans1[r++] = i;
		}

		int cnt = 0;//记录收集到的顶点是否与题目给的顶点数目一致，如果不一致说明此图有环
		while (l < r)
		{
			int curvertex = ans1[l++];
			cnt++;
			for (auto& nextvertex : AdjList[curvertex])
			{
				if (--indegree[nextvertex] == 0)
					ans1[r++] = nextvertex;
			}
		}

		return ans1.size() == cnt ? ans1 : ans2;
	}
};



//题目三：
// 字典序最小的拓扑排序
// 要求返回所有正确的拓扑排序中 字典序最小 的结果
// 建图请使用链式前向星方式，因为比赛平台用其他建图方式会卡空间
// 测试链接 : https://www.luogu.com.cn/problem/U107394
//
//链式前向星 建图 + 小根堆维持“字典序最小”
//时间复杂度：O(n+m)+O(nlogn)
#include <iostream>
using namespace std;

const int MAXN = 100005;//最大点的数目
const int MAXM = 100005;//最大边的数目

int n, m;
int ans[MAXN];//最终答案

//链式前向星建图
int head[MAXN];
int next[MAXM];
int to[MAXM];
int cnt;

int indegree[MAXN];

int heap[MAXN];//小根堆
int heapsize;//小根堆的大小

bool isEmpty()
{
	return heapsize == 0;
}

void push(int i)
{
	if (heapsize == 0)
	{
		heap[heapsize++] = i;
		return;
	}

	int curpos = heapsize;
	heap[heapsize++] = i;
	while (heap[curpos] < heap[(curpos - 1) / 2])
	{
		swap(heap[curpos], heap[(curpos - 1) / 2]);
		curpos = (curpos - 1) / 2;
	}

}

void heapify(int i)
{
	int l = 2 * i + 1;
	while (l < heapsize)
	{
		int r = l + 1;
		int minidx = (r < heapsize && heap[r] < heap[l]) ? r : l;
		minidx = heap[i] < heap[minidx] ? i : minidx;
		if (minidx == i)
			return;
		swap(heap[i], heap[minidx]);
		i = minidx;
		l = 2 * i + 1;
	}
}

int pop()
{
	if (heapsize <= 0)
		return -1;

	int front = heap[0];
	swap(heap[0], heap[--heapsize]);
	heapify(0);
	return front;
}

void build(int n)
{
	cnt = 1;
	heapsize = 0;
	for (int i = 0; i <= n; i++)
	{
		head[i] = 0;
		indegree[i] = 0;
	}
}

void addEdge(int u, int v)
{
	to[cnt] = v;
	::next[cnt] = head[u];
	head[u] = cnt;
	cnt++;
}

void topoSort()
{
	for (int i = 1; i <= n; i++)
	{
		if (indegree[i] == 0)
			push(i);
	}

	int fill = 0;
	while (!isEmpty())
	{
		int cur = pop();
		ans[fill++] = cur;
		for (int ei = head[cur]; ei > 0; ei = ::next[ei])
		{
			if (--indegree[to[ei]] == 0)
				push(to[ei]);
		}
	}
}

int main()
{
	cin >> n >> m;
	build(n);
	for (int i = 0; i < m; i++)
	{
		int u, v;
		cin >> u >> v;
		addEdge(u, v);
		indegree[v]++;
	}

	topoSort();

	for (int i = 0; i < n; i++)
		cout << ans[i] << " ";

	return 0;
}



//题目四：
// 火星词典
// 现有一种使用英语字母的火星语言
// 这门语言的字母顺序对你来说是未知的。
// 给你一个来自这种外星语言字典的字符串列表 words
// words 中的字符串已经 按这门新语言的字母顺序进行了排序 。
// 如果这种说法是错误的，并且给出的 words 不能对应任何字母的顺序，则返回 ""
// 否则，返回一个按新语言规则的 字典递增顺序 排序的独特字符串
// 如果有多个解决方案，则返回其中任意一个
// words中的单词一定都是小写英文字母组成的
// 测试链接 : https://leetcode.cn/problems/alien-dictionary/
// 测试链接(不需要会员) : https://leetcode.cn/problems/Jf1JuT/
class Solution 
{
public:
	string alienOrder(vector<string>& words) 
	{
		vector<int> indegree(26, -1);//入度表
		for (auto& word : words)//将可能参与建图的字母入度设置为零
		{
			for (int i = 0; i < word.size(); i++)
				indegree[word[i] - 'a'] = 0;
		}

		vector<vector<int>> AdjList(26);
		for (int i = 0; i < words.size() - 1; i++)
		{
			string cur = words[i];
			string next = words[i + 1];
			int checkLength = min(cur.size(), next.size());//要检查的字符长度
			int pos = 0;//当前检查的下标
			for (; pos < checkLength; pos++)
			{
				if (cur[pos] != next[pos])
				{
					AdjList[cur[pos] - 'a'].push_back(next[pos] - 'a');//建图加边
					indegree[next[pos] - 'a']++;
					break;
				}

				//特判：两个字符串要检查的部分完全一致且cur字符串长度比next长
				//不符合题目定义
				if (pos == next.size() - 1 && cur.size() > next.size())
					return "";
			}
		}

		vector<int> deq(26);
		int l = 0, r = 0;
		string ans;
		int kind = 0;//字符的种类，用于最终比较：该图是否成环

		for (int i = 0; i < 26; i++)
		{
			if (indegree[i] == 0)
				deq[r++] = i;
			if (indegree[i] != -1)
				kind++;
		}

		while (l < r)
		{
			int cur = deq[l++];
            ans.push_back(static_cast<char>(cur + 'a'));
			for (auto& word : AdjList[cur])
			{
				if (--indegree[word] == 0)
					deq[r++] = word;
			}
		}

		return ans.size() == kind ? ans : "";
	}
};



//题目五：
// 戳印序列
// 你想最终得到"abcbc"，认为初始序列为"?????"。印章是"abc"
// 那么可以先用印章盖出"??abc"的状态，
// 然后用印章最左字符和序列的0位置对齐，就盖出了"abcbc"
// 这个过程中，"??abc"中的a字符，被印章中的c字符覆盖了
// 每次盖章的时候，印章必须完全盖在序列内
// 给定一个字符串target是最终的目标，长度为n，认为初始序列为n个'?'
// 给定一个印章字符串stamp，目标是最终盖出target
// 但是印章的使用次数必须在10*n次以内
// 返回一个数组，该数组由每个回合中被印下的最左边字母的索引组成
// 上面的例子返回[2,0]，表示印章最左字符依次和序列2位置、序列0位置对齐盖下去，就得到了target
// 如果不能在10*n次内印出序列，就返回一个空数组
// 测试链接 : https://leetcode.cn/problems/stamping-the-sequence/
//
//法一：
//新盖的章会影响之前盖的章 -->> 拓扑排序
class Solution
{
public:
	vector<int> movesToStamp(string stamp, string target) 
	{
		//stamp放置的位置即为开头
		int slength = stamp.size();
		int tlength = target.size();
		vector<int> indegree(tlength - slength + 1, slength);//入度数组下标为：所有在target中可以盖章的下标(所有可能放置stamp的开头)
		//与前面题目不同，这里需要把所有可以盖章的下标的入度都设置为 满入度(slength)

		vector<vector<int>> AdjList(tlength);
		//stamp依次与target比较，与stamp不符的target下标  --  哪个开头造成的(影响哪个开头)
		//indgree==0清楚影响时，依次把下标删除，该下标影响的开头的入度减一
		vector<int> deq(tlength - slength + 1);
		int l = 0, r = 0;
		vector<int> ans;

		for (int i = 0; i <= tlength - slength; i++)//可以在target中盖章的下标
		{
			for (int j = 0; j < slength; j++)//与stamp一一比对
			{
				if (target[i + j] == stamp[j])
				{
					if (--indegree[i] == 0)
					{
						deq[r++] = i;
					}
				}
				else
				{
					AdjList[i + j].push_back(i);//建图加边
				}
			}
		}

		// 同一个位置取消错误不要重复统计
		vector<int> visited(tlength, false);
		while (l < r)
		{
			int cur = deq[l++];
			ans.push_back(cur);
			for (int i = cur; i <= cur + slength - 1; i++)//将入度为零的章所产生的所有影响消除
			{
				if (!visited[i])
				{
					visited[i] = true;
					for (auto& ele : AdjList[i])
					{
						if (--indegree[ele] == 0)
							deq[r++] = ele;
					}
				}
			}
		}
		reverse(ans.begin(), ans.end());

		for (bool v : visited)
		{
			if (!v)
				return{};
		}

		return ans;
	}
};



//法二：
class Solution 
{
public:
	vector<int> movesToStamp(string stamp, string target)
	{
		int k = stamp.length();
		int n = target.length();
		vector<int> result;
		bool changed;

		while (true) {
			changed = false;

			for (int i = 0; i <= n - k; i++) 
			{
				if (canStamp(stamp, target, i)) 
				{
					result.push_back(i);
					// 将这部分替换为问号
					for (int j = 0; j < k; j++)
					{
						target[i + j] = '?';
					}
					changed = true;
				}
			}

			// 如果一轮中没有找到可以盖章的位置，退出循环
			if (!changed) 
			{
				break;
			}
		}

		// 检查是否所有字符都变成了'?'
		for (char c : target)
		{
			if (c != '?') 
			{
				return {};
			}
		}

		// 反转结果，因为我们是从后向前构建的
		reverse(result.begin(), result.end());
		return result;
	}

private:
	// 判断在位置i是否可以盖章
	bool canStamp(const string& stamp, const string& target, int i)
	{
		int k = stamp.length();
		bool hasLetter = false;

		for (int j = 0; j < k; j++) 
		{
			// 如果已经盖过章（问号），匹配任何字符
			if (target[i + j] == '?') 
			{
				continue;
			}

			// 如果字符不匹配，不能盖章
			if (stamp[j] != target[i + j]) 
			{
				return false;
			}

			hasLetter = true;
		}

		// 至少需要有一个字符与原始字符匹配
		return hasLetter;
	}
};

