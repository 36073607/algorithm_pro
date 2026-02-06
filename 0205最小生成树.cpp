#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;



//最小生成树：在 无向带权图 中选择一些边，在 保证连通性 的情况下，边的总权值最小
//最小生成树可能不只一棵，只要保证边的总权值最小，就都是正确的最小生成树
//如果无向带权图有 n 个点，那么最小生成树一定有 n-1 条边

//扩展：最小生成树一定是最小瓶颈树（题目5）
//注意：最小生成树扩展很多，除了这节课讲的，大部分都是比赛需要的内容，有兴趣可以继续研究


//Kruskal算法（最常用）
//1.把所有的边，根据权值从小到大排序，从权值小的边开始考虑
//2.如果连接当前的边不会形成环，就选择当前的边
//3.如果连接当前的边会形成环，就不要当前的边
//4.考察完所有边之后，最小生成树就也得到了
// -->>连图都不需要建，利用并查集
//检查图是否有环：
//如果无向带权图有 n 个点，那么最小生成树一定有 n-1 条边
// 时间复杂度O(m * log m) + O(n + m)


//Prim算法（不算常用）
//1.解锁的点的集合叫set（普通集合）、解锁的边的集合叫heap（小根堆）。set和heap都为空
//2.可从任意点开始，开始点加入到set，开始点的所有边加入到heap
//3.从heap中弹出权值最小的边e，查看边e所去往的点x
//  A：如果x已经在set中边e舍弃，重复步骤3
//  B：如果x不在set中，边e属于最小生成树，把x加入set，重复步骤3
//4.当heap为空，最小生成树也就得到了
// -->>需要建图
//检查图是否有环：
//统计运行时遍历到的节点数目，若与题目给的一致，则无环：否则，有环
// 时间复杂度O(n + m) + O(m * log m)


//题目一：
// Kruskal算法模版（洛谷）
// 静态空间实现
// 测试链接 : https://www.luogu.com.cn/problem/P3366
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

const int MAXN = 5005;
const int MAXM = 200005;
int n, m;

vector<vector<int>> edges;
vector<int> father(MAXN);

bool cmp(vector<int>& a, vector<int>& b)
{
	return a[2] < b[2];
}

void build(int n)
{
	for (int i = 1; i <= n; i++)
	{
		father[i] = i;
	}
}

int find(int i)
{
	if (i == father[i])
		return i;
	return father[i] = find(father[i]);
}

bool Union(int a, int b)
{
	int fa = find(a);
	int fb = find(b);
	if (fa != fb)//不在同一集合
	{
		father[fa] = fb;
		return true;
	}
	return false;//在同一集合
}

int main()
{
	cin >> n >> m;
	build(n);

	for (int i = 0; i < m; i++)
	{
		int x, y, z;
		cin >> x >> y >> z;
		edges.push_back({ x,y,z });//建出存放边的集合
	}

	int ans = 0;
	int edgecnt = 0;//检测图是否连通
	//Kruskal
	sort(edges.begin(), edges.end(), cmp);
	for (auto& edge : edges)
	{
		if (Union(edge[0], edge[1]))//一条边上的两个点不在同一集合
		{
			ans += edge[2];
			edgecnt++;
		}
	}

    cout << ((edgecnt == n - 1) ? to_string(ans) : string("orz"));//使用这种写法时要注意打印结果的类型应一致
	return 0;
}



//题目二：
// Prim算法模版（洛谷）
// 动态空间实现
// 测试链接 : https://www.luogu.com.cn/problem/P3366
#include <iostream>
#include <vector>
#include <string>
#include <queue>
using namespace std;

const int MAXN = 5005;
const int MAXM = 200005;

int n, m;

struct comp
{
	bool operator()(vector<int>& a, vector<int>& b)
	{
		return a[1] > b[1];//小根堆
	}
};

priority_queue<vector<int>, vector<vector<int>>, comp> pq;//小根堆
vector<vector<vector<int>>> AdjList(MAXN);

int main()
{
	cin >> n >> m;
	vector<int> set(n + 1, 0);
	//注意：动态创建vector一定要在参数完成初始化后在创建
	for (int i = 0; i < m; i++)
	{
		int x, y, z;
		cin >> x >> y >> z;
		//加边
		AdjList[x].push_back({ y,z });
		AdjList[y].push_back({ x,z });
	}

	int ans = 0;
	//Prim
	set[1] = 1;//可以取图中任意一点，这里取编号为1的点
	int nodecnt = 1;
	for (auto& ele : AdjList[1])
	{
		pq.push({ele[0],ele[1]});
	}

	while (!pq.empty())
	{
		int nextnode = pq.top()[0];
		int val = pq.top()[1];
		pq.pop();
		if (!set[nextnode])//该节点尚未检查过
		{
			set[nextnode] = 1;
			ans += val;
			nodecnt++;

			for (auto& ele : AdjList[nextnode])// 将新加入节点的边加入优先队列
			{
				if (!set[ele[0]])// 只加入目标节点未在生成树中的边
				{
					pq.push({ ele[0],ele[1] });
				}
			}
		}
	}

	cout << (nodecnt == n ? to_string(ans) : "orz");
	return 0;
}

//注意：在main函数开始前，n的值为0，所以vector<int> set(n + 1, 0)实际上创建了一个大小为1的向量。



//Prim算法的优化--当节点数目较少时使用
// 时间复杂度O(n + m) + O((m+n) * log n)
//详情见：https://www.bilibili.com/video/BV1sK4y1F7LH/?spm_id_from=333.1387.collection.video_card.click&vd_source=fdf4f9cadef70de43e5071838cb1aecc



//题目三：
// 水资源分配优化
// 村里面一共有 n 栋房子。我们希望通过建造水井和铺设管道来为所有房子供水。
// 对于每个房子 i，我们有两种可选的供水方案：一种是直接在房子内建造水井
// 成本为 wells[i - 1] （注意 -1 ，因为 索引从0开始 ）
// 另一种是从另一口井铺设管道引水，数组 pipes 给出了在房子间铺设管道的成本，
// 其中每个 pipes[j] = [house1j, house2j, costj] 
// 代表用管道将 house1j 和 house2j连接在一起的成本。连接是双向的。
// 请返回 为所有房子都供水的最低总成本
// 测试链接 : https://leetcode.cn/problems/optimize-water-distribution-in-a-village/
//洛谷：https://www.luogu.com.cn/problem/P1194
//
// 核心：建立虚拟节点，使其转换成只包含“最小生成树”考点的一道题
//思路：
//想象图中有一个水源地，从该水源地到每个房子铺设管道的费用即为每个房子自己建造水井的费用；
//同时房子与房子之间也可以铺设管道
// -->>直接使用最小生成树
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const int MAXN = 1010;
int a, b;
vector<vector<int>> edges;
vector<int> father(MAXN);

bool cmp(vector<int>& a, vector<int>& b)
{
	return a[2] < b[2];
}

void build(int b)
{
	for (int i = 0; i <= b; i++)
	{
		father[i] = i;
	}
}

int find(int i)
{
	if (father[i] == i)
		return i;
	return father[i] = find(father[i]);
}

bool Union(int a, int b)
{
	int fa = find(a);
	int fb = find(b);
	if (fa != fb)
	{
		father[fa] = fb;
		return true;
	}
	return false;
}

int main()
{
	cin >> a >> b;
	build(b);
	for (int i = 0; i < b; i++)//设置一个虚拟点
	{
		edges.push_back({ b,i,a });
	}

	for (int i = 0; i < b; i++)
	{
		for (int j = 0; j < b; j++)
		{
			int val;
			cin >> val;
			// 只添加有效的优惠边
			if (val != 0 && i < j)  // 只添加一次，避免重复
			{
				edges.push_back({ i, j, val });
			}
		}
	}

	int ans = 0;
	int edgecnt = 0;
	//Kruskal
	sort(edges.begin(), edges.end(), cmp);
	for (auto& edge : edges)
	{
		if (Union(edge[0], edge[1]))
		{
			ans += edge[2];
			edgecnt++;
			if (edgecnt == b)//算上虚拟节点，一共b+1个节点，则最小生成树有b条边
				break;
		}
	}
	cout << ans;
	return 0;
}



//题目四：
// 检查边长度限制的路径是否存在
// 给你一个 n 个点组成的无向图边集 edgeList
// 其中 edgeList[i] = [ui, vi, disi] 表示点 ui 和点 vi 之间有一条长度为 disi 的边
// 请注意，两个点之间可能有 超过一条边 。
// 给你一个查询数组queries ，其中 queries[j] = [pj, qj, limitj]
// 你的任务是对于每个查询 queries[j] ，判断是否存在从 pj 到 qj 的路径
// 且这条路径上的每一条边都 严格小于 limitj 。
// 请你返回一个 布尔数组 answer ，其中 answer.length == queries.length
// 当 queries[j] 的查询结果为 true 时， answer 第 j 个值为 true ，否则为 false
// 测试链接 : https://leetcode.cn/problems/checking-existence-of-edge-length-limited-paths/
//
//分析：将权值<limit的边合并，最终检查两个节点是否属于同一个集合
//这道题更多的是对并查集的考察
class Solution 
{
private:
	static const int MAXN = 100005;
	//将参数列表中queries数组按照limit从小到大排序，效率更高
	vector<vector<int>> questions;
	vector<int> Father;

	static bool cmp(vector<int>& a, vector<int>& b)
	{
		return a[2] < b[2];
	}

	void build(int n)
	{
		Father.resize(n);
		for (int i = 0; i < n; i++)
		{
			Father[i] = i;
		}
	}

	int find(int i)
	{
		if (i == Father[i])
			return i;
		return Father[i] = find(Father[i]);
	}

	bool isSameSet(int a, int b)
	{
		return find(a) == find(b);
	}

	void Union(int a, int b)
	{
		int fa = find(a);
		int fb = find(b);
		if (fa != fb)
		{
			Father[fa] = fb;
		}
	}

public:
	vector<bool> distanceLimitedPathsExist(int n, vector<vector<int>>& edgeList, vector<vector<int>>& queries)
	{
		vector<bool> ans(queries.size(), false);
		build(n);
		for (int i = 0; i < queries.size(); i++)
		{
			questions.push_back({ queries[i][0],queries[i][1],queries[i][2],i });
		}
		sort(questions.begin(), questions.end(), cmp);

		//并查
		sort(edgeList.begin(), edgeList.end(), cmp);
		//双指针，edgeList数组和question数组都是已经排好序的，有单调性
		for (int queIdx = 0, edgeIdx = 0; queIdx < questions.size(); queIdx++)
		{
			for (; edgeIdx < edgeList.size() && edgeList[edgeIdx][2] < questions[queIdx][2]; edgeIdx++)
			{
				Union(edgeList[edgeIdx][0], edgeList[edgeIdx][1]);
			}

			if (isSameSet(questions[queIdx][0], questions[queIdx][1]))
			{
				ans[questions[queIdx][3]] = true;
			}
		}

		return ans;
	}
};
//注：
//初始化问题：原代码中 vector<int> Father(MAXN); 在类定义中不允许这样初始化，改为在 build 函数中通过 resize 初始化



//题目五：
// 繁忙的都市
// 一个非常繁忙的大都市，城市中的道路十分的拥挤，于是市长决定对其中的道路进行改造
// 城市的道路是这样分布的：城市中有n个交叉路口，有些交叉路口之间有道路相连
// 两个交叉路口之间最多有一条道路相连接，这些道路是双向的
// 且把所有的交叉路口直接或间接的连接起来了
// 每条道路都有一个分值，分值越小表示这个道路越繁忙，越需要进行改造
// 但是市政府的资金有限，市长希望进行改造的道路越少越好，于是他提出下面的要求：
// 1. 改造的那些道路能够把所有的交叉路口直接或间接的连通起来
// 2. 在满足要求1的情况下，改造的道路尽量少
// 3. 在满足要求1、2的情况下，改造的那些道路中分值最大的道路分值尽量小
// 作为市规划局的你，应当作出最佳的决策，选择哪些道路应当被修建
// 返回选出了几条道路 以及 分值最大的那条道路的分值是多少
// 测试链接 : https://www.luogu.com.cn/problem/P2330
//
//最小生成树一定是最小瓶颈树
//最小瓶颈生成树（Minimum Bottleneck Spanning Tree, MBST）是指在一个无向图中，所有生成树中具有最小的最大边权值的生成树。
//这意味着，在最小瓶颈生成树中，任何两个顶点之间的路径上的最大边权值是所有可能的生成树中最小的。
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const int MAXN = 8005;
vector<int> father(MAXN);
vector<vector<int>> edges;

bool cmp(vector<int>& a, vector<int>& b)
{
	return a[2] < b[2];
}

void build(int m)
{
	for (int i = 1; i <= m; i++)
		father[i] = i;
}

int find(int i)
{
	if (i == father[i])
		return i;
	return father[i] = find(father[i]);
}

bool Union(int a, int b)
{
	int fa = find(a);
	int fb = find(b);
	if (fa != fb)
	{
		father[fa] = fb;
		return true;
	}
	return false;
}

int main()
{
	int m, n;
	cin >> n >> m;
	build(m);
	for (int i = 0; i < m; i++)
	{
		int u, v, c;
		cin >> u >> v >> c;
		edges.push_back({ u,v,c });
	}

	//Kruskal
	int edgecnt = 0;
	int maxs = 0;
	sort(edges.begin(), edges.end(), cmp);
	for (auto& edge : edges)
	{
		if (Union(edge[0], edge[1]))
		{
			edgecnt++;
			maxs = max(maxs, edge[2]);
		}
	}
	cout << edgecnt << " " << maxs;

	return 0;
}

