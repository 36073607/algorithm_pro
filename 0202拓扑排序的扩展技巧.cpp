#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
using namespace std;


//回顾拓扑排序：
//拓扑排序的精髓：
//以BFS为基础，将端点（入度为0的点）加入到队列，然后每遇到一个节点，就将它的入度减一，如果入度此时入度为0，说明它变成了端点，再把其加入队列，如此往复直到所有结点遍历完。类似于“抽丝剥茧”。
//节点总数为n，边的个数为m，所以总的时间复杂度和空间复杂度是O(n + m)。此外，因为是单向图，也不需要记录节点是否已经遍历，入度数组已经替我们完成了这一切。

//在拓扑排序中，我们记录出队的节点个数，如果最后出队的节点个数小于总节点数，说明存在环。

//重要技巧：
//利用拓扑排序的过程，上游节点逐渐推送消息给下游节点的技巧

//注意：
//这个技巧已经是树形dp的内容了，不过即便不会动态规划，本节课也能听懂



//题目一：
// 最大食物链计数
// a -> b，代表a在食物链中被b捕食
// 给定一个有向无环图，返回
// 这个图中从最初级动物到最顶级捕食者的食物链有几条
// 测试链接 : https://www.luogu.com.cn/problem/P4017
#include <iostream>
#include <vector>
using namespace std;

const int MAXN = 5005;
const int MAXM = 500005;
const int MOD = 80112002;
//链式前向星
vector<int> head(MAXN, 0);
vector<int> next(MAXM, 0);
vector<int> to(MAXM, 0);
int edgecnt = 1;

vector<int> indegree(MAXN,0);
vector<int> lines(MAXN, 0);//每个点的食物链条数     上游节点逐渐推送消息给下游节点

vector<int> deq(MAXN);
int l, r;
int n, m;

void build(int n)
{
	edgecnt = 1;
	for (int i = 1; i <= n; i++)
	{
		indegree[i] = 0;
		head[i] = 0;
		lines[i] = 0;
	}
}

void addEdge(int a, int b)
{
	to[edgecnt] = b;
	::next[edgecnt] = head[a];
	head[a] = edgecnt;
	edgecnt++;
}

int main()
{
	cin >> n >> m;
	build(n);
	for (int i = 0; i < m; i++)
	{
		int a, b;
		cin >> a >> b;
		addEdge(a, b);
		indegree[b]++;
	}

	l = r = 0;
	int ans = 0;
	for (int i = 1; i <= n; i++)
	{
		if (indegree[i] == 0)
		{
			deq[r++] = i;
			lines[i] = 1;
		}
	}

	while (l < r)
	{
		int cur = deq[l++];
		if (head[cur] == 0)//到达cur节点没有后续邻居，说明已经到达食物链最顶端
			ans = (ans + lines[cur]) % MOD;
		else
		{
			for (int ei = head[cur]; ei > 0; ei = ::next[ei])
			{
				lines[to[ei]] = (lines[cur] + lines[to[ei]]) % MOD;
				if (--indegree[to[ei]] == 0)
					deq[r++] = to[ei];
			}
		}
	}

	//题目保证无环
	cout << ans;

	return 0;
}



//题目二：
// 喧闹和富有
// 从 0 到 n - 1 编号，其中每个人都有不同数目的钱，以及不同程度的安静值
// 给你一个数组richer，其中richer[i] = [ai, bi] 表示 
// person ai 比 person bi 更有钱
// 还有一个整数数组 quiet ，其中 quiet[i] 是 person i 的安静值
// richer 中所给出的数据 逻辑自洽
// 也就是说，在 person x 比 person y 更有钱的同时，不会出现
// person y 比 person x 更有钱的情况
// 现在，返回一个整数数组 answer 作为答案，其中 answer[x] = y 的前提是,
// 在所有拥有的钱肯定不少于 person x 的人中，
// person y 是最安静的人（也就是安静值 quiet[y] 最小的人）。
// 测试链接 : https://leetcode.cn/problems/loud-and-rich/
class Solution 
{
public:
	vector<int> loudAndRich(vector<vector<int>>& richer, vector<int>& quiet) 
	{
		int length = quiet.size();
		vector<int> ans(length);
		for (int i = 0; i < length; i++)
		{
			//一开始认为自己是“安静的”，在拓扑排序的过程中逐渐修改正确
			ans[i] = i;
		}

		//建图
		//细节：假设a比b有钱，究竟让a指向b还是让b指向a呢？
		// a指向b
		//由于题目要求答案的设置是起码比本人有钱，所以让有钱的a指向没钱的b，这样可以使得有钱的入度较低，尽快出答案并将信息传递给后面的元素
		vector<vector<int>> AdjList(length);
		vector<int> indegree(length, 0);
		for (auto& ele : richer)//加边
		{
			AdjList[ele[0]].push_back(ele[1]);
			indegree[ele[1]]++;
		}

		vector<int> deq(length);
		int l = 0, r = 0;
		for (int i = 0; i < length; i++)
		{
			if (indegree[i] == 0)
				deq[r++] = i;
		}

		while (l < r)
		{
			int cur = deq[l++];
			for (auto& nxt : AdjList[cur])
			{
				// 关键：每次都要尝试更新
				// 如果cur的答案比nxt当前的答案更安静，则更新nxt的答案
				if (quiet[ans[nxt]] > quiet[ans[cur]])
					ans[nxt] = ans[cur];

				if (--indegree[nxt] == 0)
					deq[r++] = nxt;
			}
		}
		return ans;//题目保证没有环
	}
};



//题目三：
// 并行课程 III
// 给你一个整数 n ，表示有 n 节课，课程编号从 1 到 n
// 同时给你一个二维整数数组 relations ，
// 其中 relations[j] = [prevCoursej, nextCoursej]
// 表示课程 prevCoursej 必须在课程 nextCoursej 之前 完成（先修课的关系）
// 同时给你一个下标从 0 开始的整数数组 time
// 其中 time[i] 表示完成第 (i+1) 门课程需要花费的 月份 数。
// 请你根据以下规则算出完成所有课程所需要的 最少 月份数：
// 如果一门课的所有先修课都已经完成，你可以在 任意 时间开始这门课程。
// 你可以 同时 上 任意门课程 。请你返回完成所有课程所需要的 最少 月份数。
// 注意：测试数据保证一定可以完成所有课程（也就是先修课的关系构成一个有向无环图）
// 测试链接 : https://leetcode.cn/problems/parallel-courses-iii/
//
//跟题目二一样
class Solution 
{
public:
	int minimumTime(int n, vector<vector<int>>& relations, vector<int>& time) 
	{
		int ans = 0;
		vector<vector<int>> AdjList(n+1);
		vector<int> indegree(n+1, 0);
		vector<int> timecost(n + 1, 0);
		vector<int> deq(n);
		int l = 0, r = 0;

		for (auto& ele : relations)
		{
			AdjList[ele[0]].push_back(ele[1]);
			indegree[ele[1]]++;
		}

		for (int i = 1; i <= n; i++)
		{
			if (indegree[i] == 0)
			{
				deq[r++] = i;
				timecost[i] = time[i - 1];
			}
		}

		while (l < r)
		{
			int cur = deq[l++];
			ans = max(ans, timecost[cur]);
			for (auto& nxt : AdjList[cur])
			{
				timecost[nxt] = max(timecost[nxt], timecost[cur]);
				if (--indegree[nxt] == 0)
				{
					timecost[nxt] += time[nxt - 1];
					deq[r++] = nxt;
				}
			}
		}
		return ans;
	}
};



//题目四：
// 参加会议的最多员工数
// 一个公司准备组织一场会议，邀请名单上有 n 位员工
// 公司准备了一张 圆形 的桌子，可以坐下 任意数目 的员工
// 员工编号为 0 到 n - 1 。每位员工都有一位 喜欢 的员工
// 每位员工 当且仅当 他被安排在喜欢员工的旁边，他才会参加会议
// 每位员工喜欢的员工 不会 是他自己。给你一个下标从 0 开始的整数数组 favorite
// 其中 favorite[i] 表示第 i 位员工喜欢的员工。请你返回参加会议的 最多员工数目
// 测试链接 : https://leetcode.cn/problems/maximum-employees-to-be-invited-to-a-meeting/
//
//题目分析：
//答案只会从两个地方出：
//1.当两个节点形成环时，分别找到与两个节点相连的最长链，最后相加
//2.当三个及以上节点形成环时，只能让所有成环的节点坐一块，即答案为环中节点数目
//
//若环的长度大于2，最大邀请数就是这个环的长度（因为环上的人可以全邀请）。
//若环的长度等于2（A喜欢B，B喜欢A），则可以邀请这两人，再加上他们各自链上能延伸的最多人（两条链的最长长度之和）。
// 
//两个互相暗恋的人能稳定各自吊着一群舔狗不崩盘.
//三个和三个以上火药味就没法隐藏, 路人只能看戏插不进去.
class Solution 
{
public:
	int maximumInvitations(vector<int>& favorite)
	{
		//给的favorite数组本身就成一个图: 一个图或多个图的集合
		//favorite[a]=b  :  a -> b
		int length = favorite.size();
		vector<int> indegree(length, 0);
		for (int i = 0; i < length; i++)
		{
			indegree[favorite[i]]++;
		}

		vector<int> deep(length, 0);// deep[i] : 不包括i在内，i之前的最长链的长度
		vector<int> deq(length, 0);
		int l = 0, r = 0;
		for (int i = 0; i < length; i++)
		{
			if (indegree[i] == 0)
			{
				deq[r++] = i;
			}
		}

		while (l < r)
		{
			int cur = deq[l++];
			int nxt = favorite[cur];
			deep[nxt] = max(deep[nxt], deep[cur] + 1);
			if (--indegree[nxt] == 0)
				deq[r++] = nxt;
		}

		// 目前图中的点，不在环上的点，都删除了！ indegree[i] == 0
		// 可能性1 : 所有小环(中心个数 == 2)，算上中心点 + 延伸点，总个数
		int sumOfSmallRings = 0;
		// 可能性2 : 所有大环(中心个数 > 2)，只算中心点，最大环的中心点个数
		int bigRings = 0;
		for (int i = 0; i < length; i++)
		{
			if (indegree[i] > 0)//只看成环部分
			{
				int ringSize = 1;//环的大小
				indegree[i] = 0;
				for (int e = favorite[i]; e != i; e = favorite[e])
				{
					ringSize++;
					indegree[e] = 0;//已经检查过的部分设置为0
				}

				if (ringSize == 2)
				{
					sumOfSmallRings += deep[i] + deep[favorite[i]] + 2;
				}
				else
				{
					bigRings = max(bigRings, ringSize);
				}
			}
		}
		return max(sumOfSmallRings, bigRings);
	}
};



