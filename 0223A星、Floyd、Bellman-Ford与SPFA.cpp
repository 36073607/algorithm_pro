#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;



//一：
//A*算法，指定源点，指定目标点，求源点到达目标点的最短距离
//增加了当前点到终点的预估函数
//在堆中根据 从源点出发到达当前点的距离+当前点到终点的预估距离 来进行排序
//剩下的所有细节和Dijkstra算法完全一致
//
//预估函数要求(核心)：当前点到终点的预估距离 <= 当前点到终点的真实最短距离
// 平凡的有向图是估计不出来的，但是像二维网格等可以估计的问题就可以采用AStar算法
//预估函数只参与：哪些节点先从堆中弹出
//预估函数是一种吸引力
//1.合适的吸引力可以提升算法的速度，吸引力过强会出现错误
//2.保证 预估距离 <= 真实最短距离 的情况下，尽量接近真实最短距离，可以做到功能正确 且 最快
//  若 预估距离 > 真实最短距离，则预估函数展现了过强的吸引力，可能会忽略掉源点到x点的距离，更在乎x到目标点更近这件事
//  eg:二维网格只能上下左右走，则预估函数选择曼哈顿距离(这种情况下最优，吸引力更强)、对角线距离都正确；若二维网格还能走斜线，则预估函数更应采用对角线距离
//
//预估终点距离经常选择：
//曼哈顿距离：abs(x1-x2)+abs(y1-y2)
//欧氏距离：勾股定理
//对角线距离：max(abs(x1-x2),abs(y1-y2))


//题目一：A*算法模板（自测，没有测试链接）
// grid[i][j] == 0 代表障碍
// grid[i][j] == 1 代表道路
// 只能走上、下、左、右，不包括斜线方向
// 返回从(startX, startY)到(targetX, targetY)的最短距离
class Solution
{
	struct cmp
	{
		bool operator()(vector<int>& a, vector<int>& b)
		{
			return a[2] > b[2];//小根堆
		}
	};

	int f(int x1, int y1, int x2, int y2)
	{
		return abs(x1 - x2) + abs(y1 - y2);
	}

	int minDistance(vector<vector<int>>& grid, int startX, int startY, int targetX, int targetY)
	{
		if (grid[startX][startY] == 0 || grid[targetX][targetY] == 0)
			return -1;

		int m = grid.size();
		int n = grid[0].size();
		vector<vector<int>> move = { {-1,0},{1,0},{0,1},{0,-1} };
		vector<vector<int>> distance(m, vector<int>(n, INT_MAX));
		priority_queue<vector<int>, vector<vector<int>>, cmp> pq;
		vector<vector<int>> visited(m, vector<int>(n, false));

		//AStar
		//处理源点
		distance[startX][startY] = 1;//求的是路线长度。所以要加一
		pq.push({ startX,startY,1 + f(startX,startY,targetX,targetY) });
		while (!pq.empty())
		{
			int curx = pq.top()[0];
			int cury = pq.top()[1];
			pq.pop();

			if (visited[curx][cury])
				continue;
			visited[curx][cury] = true;

			if (curx == targetX && cury == targetY)
				return distance[curx][cury];

			for (auto& ele : move)
			{
				int dx = curx + ele[0];
				int dy = cury + ele[1];
				if (dx >= 0 && dx <= m - 1 && dy >= 0 && dy <= n - 1 && !visited[dx][dy]
					&& grid[dx][dy] == 1
					&& distance[curx][cury] + 1 < distance[dx][dy])//根据真实情况判断是否刷新、入堆，只是在堆中的排序是根据distance数组与f函数共同完成
				{
					distance[dx][dy] = distance[curx][cury] + 1;
					pq.push({ dx,dy,distance[dx][dy] + f(dx,dy,targetX,targetY) });
				}
			}
		}
		return -1;
	}
};



//二：
//Floyd算法，得到图中任意两点之间的最短距离（多源最短路）
//时间复杂度O(n^3)，空间复杂度O(n^2)，常数时间小，容易实现
//适用于任何图，不管有向无向、不管边权正负，但是不能有负环（保证最短路存在，这也是实现最短路算法的必要条件：因为一旦有负环，只要一直绕着负环走，最后最短距离为负无穷）
//
//过程简述：
//distance[i][j]表示i和j之间的最短距离
//distance[i][j] = min( distance[i][j], distance[i][k] + distance[k][j] )
//枚举所有的k即可


//题目二：
// Floyd算法模版（洛谷）
// 测试链接 : https://www.luogu.com.cn/problem/P2910
#include <iostream>
#include <vector>
#include <climits>
using namespace std;

const int MAXN = 105;
const int MAXM = 10005;

int n, m;
vector<vector<int>> distance(MAXN, vector<int>(MAXN, INT_MAX));
vector<int> path(MAXM);

void floyd()
{
	// O(N^3)的过程
	// 枚举每个跳板
	// 注意，跳板要最先枚举！跳板要最先枚举！跳板要最先枚举！
	for (int bridge = 1; bridge <= n; bridge++)
	{
		for (int i = 1; i <= n; i++)
		{
			for (int j = 1; j <= n; j++)
			{
				if (::distance[i][bridge] != INT_MAX
					&& ::distance[bridge][j] != INT_MAX
					&& ::distance[i][bridge] + ::distance[bridge][j] < ::distance[i][j])
				{
					// i -> .....bridge .... -> j
					// distance[i][j]能不能缩短
					// distance[i][j] = min ( distance[i][j] , distance[i][bridge] + distance[bridge][j])
					::distance[i][j] = ::distance[i][bridge] + ::distance[bridge][j];
				}
			}
		}
	}
}

int main()
{
	// 提高输入输出效率
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> n >> m;
	for (int i = 0; i < m; i++)
	{
		cin >> path[i];
	}
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= n; j++)
		{
			cin >> ::distance[i][j];
		}
	}

	floyd();
	int ans = 0;
	for (int i = 0; i <= m - 2; i++)
	{
		int j = i + 1;
		ans += ::distance[path[i]][path[j]];
	}
	cout << ans;

	return 0;
}



//三：
//Bellman-Ford算法，解决可以有负权边但是不能有负环（保证最短路存在）的图，单源最短路径算法
// 辨析：Dijkstra算法同样解决单源最短路径问题，但要求：1.不能有负权边 2.最短路存在
//
//松弛操作：这就是对DJ、A*算法中调整distance数组起了个专有名词
//假设源点为A，从A到任意点F的最短距离为distance[F]
//假设从点P出发某条边。去往点S，边权为W
//如果发现，distance[P] + W < distance[S]，也就是通过该边可以让distance[S]变小
//那么就说，P出发的这条边对点S进行了松弛操作
//
//Bellman-Ford过程：
//1.每一轮考察每条边，每条边都尝试进行松弛操作，那么若干点的distance会变小
//2.当某一轮发现不再有松弛操作出现时，算法停止
//
//Bellman-Ford算法时间复杂度：
//假设点的数量为N，边的数量为M，每一轮时间复杂度O(M)
//最短路存在的情况下，因为1次松弛操作会使1个点的最短路的边数+1 （注：在存在最短路的情况下，除了最后一轮确认以外，每一轮都至少进行一次松弛操作）
//而从源点出发到任何点的最短路最多走过全部的n个点(即:最长的最短路边数为n-1)，所以松弛的轮数必然 <= n - 1
//所以Bellman-Ford算法的时间复杂度为O(M*N)
//
//重要推广：判断从某个点出发能不能到达负环
//上面已经说了，如果从A出发存在最短路（没有负环），那么松弛的轮数必然 <= n - 1
//而如果从A点出发到达一个负环，那么松弛操作显然会无休止地进行下去
//所以，如果发现从A点出发，在第n轮时松弛操作依然存在，说明从A点出发能够到达一个负环



//题目三：
// Bellman-Ford算法应用（不是模版）
// k站中转内最便宜的航班
// 有 n 个城市通过一些航班连接。给你一个数组 flights
// 其中 flights[i] = [fromi, toi, pricei]
// 表示该航班都从城市 fromi 开始，以价格 pricei 抵达 toi。
// 现在给定所有的城市和航班，以及出发城市 src 和目的地 dst，你的任务是找到出一条最多经过 k 站中转的路线
// 使得从 src 到 dst 的 价格最便宜 ，并返回该价格。 如果不存在这样的路线，则输出 -1。
// 测试链接 : https://leetcode.cn/problems/cheapest-flights-within-k-stops/
//
//使用Bellman-Ford算法，因为它能处理带限制条件（k次中转）的最短路径问题
//注意：由于标准的Bellman-Ford算法的松弛次数和此题目中转次数不统一，因此需要转换一下：使用cur数组和nxt数组
class Solution 
{
public:
	//专门解决这道题的改变阉割版Bellman-Ford算法
	int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) 
	{
		vector<int> curDistance(n, INT_MAX);
		curDistance[src] = 0;
		for (int i = 0; i <= k; i++)
		{
			vector<int> nxtDistance = curDistance;
			for (auto& ele : flights)
			{
				if (curDistance[ele[0]] != INT_MAX && curDistance[ele[0]] + ele[2] < nxtDistance[ele[1]])
				{
					nxtDistance[ele[1]] = curDistance[ele[0]] + ele[2];
				}
				//等价写法
				/*if (curDistance[ele[0]] != INT_MAX)
				{
					nxtDistance[ele[1]] = min(nxtDistance[ele[1]], curDistance[ele[0]] + ele[2]);
				}*/
			}
			curDistance = nxtDistance;
		}
		return curDistance[dst] == INT_MAX ? -1 : curDistance[dst];
	}
};



//四：
//Bellman-Ford + SPFA优化 （Shortest Path Faster Algorithm）
//很轻易就能发现，每一轮考察所有的边看看能否做松弛操作是不必要的
//因为只有上一次被某条边松弛过的节点，所连接的边，才有可能引起下一次的松弛操作
//所以用队列来维护“这一轮哪些节点的distance变小了”
//下一轮只需要对这些点的所有边，考察有没有松弛操作即可
//
//SPFA只优化了常数时间，在大多数情况下跑得很快，但时间复杂度为O(n*m)
//看复杂度就知道只适用于小图，根据数据量谨慎使用，在没有负权边时要使用Dijksrta算法
//
//
//Bellman-Ford + SPFA优化的用途
//1.适用于小图
//2.解决有负边（没有负环）的图的单源最短路径问题
//3.可以判断从某个点出发是否能遇到负环，如果想判断整张有向图有没有负环，需要设置虚拟源点
//4.(*)并行计算时会有很大优势，因为每一轮多点判断松弛操作是相互独立的，可以交给多线程处理
//
//注意：
//SPFA的另一个重要的用途是解决“费用流”问题，当然也可以被Primal-Dual原始对偶算法代替


//题目四：
// Bellman-Ford + SPFA优化模版（洛谷）
// 给定n个点的有向图，请求出图中是否存在从顶点1出发能到达的负环
// 负环的定义是：一条边权之和为负数的回路
// 测试链接 : https://www.luogu.com.cn/problem/P3385
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;

const int MAXN = 2005;
const int MAXM = 6005;

//链式前向星
vector<int> head(MAXN, 0);
vector<int> nxt(MAXM, 0);
vector<int> to(MAXM, 0);
vector<int> weight(MAXM, 0);
int edgecnt = 1;

//SPFA
const int MAXQ = 4000001;
vector<int> dis(MAXN, INT_MAX);// 源点出发到每个节点的距离表
vector<int> que(MAXQ, 0);// 哪些节点被松弛了放入队列
int l = 0, r = 0;
vector<int> enter(MAXN, false);// 节点是否已经在队列中
// 注：que数组与enter数组一定同步更新
//判断负环需要：
vector<int> updateCnt(MAXN, 0);// 节点被松弛的次数

void build()
{
	edgecnt = 1;
	l = r = 0;
	fill(head.begin(), head.end(), 0);
	fill(enter.begin(), enter.end(), false);
	fill(dis.begin(), dis.end(), INT_MAX);
	fill(updateCnt.begin(), updateCnt.end(), 0);
}

void addEdge(int u, int v, int w)
{
	//根据题目的奇异搞笑要求加边
	if (w >= 0)
	{
		nxt[edgecnt] = head[u];
		to[edgecnt] = v;
		weight[edgecnt] = w;
		head[u] = edgecnt++;

		nxt[edgecnt] = head[v];
		to[edgecnt] = u;
		weight[edgecnt] = w;
		head[v] = edgecnt++;
	}
	else
	{
		nxt[edgecnt] = head[u];
		to[edgecnt] = v;
		weight[edgecnt] = w;
		head[u] = edgecnt++;
	}
}

// Bellman-Ford + SPFA优化的模版
bool SPFA(int n)
{
	//源点为1号点，进行处理
	dis[1] = 0;
	que[r++] = 1;
	enter[1] = true;
	updateCnt[1]++;

	while (l < r)
	{
		int cur = que[l++];
		enter[cur] = false;
		for (int ei = head[cur]; ei > 0; ei = nxt[ei])
		{
			int dpoint = to[ei];
			if (dis[cur] + weight[ei] < dis[dpoint])
			{
				dis[dpoint] = dis[cur] + weight[ei];//不论节点之前在不在队列中，只要距离更短，就更新
				if (!enter[dpoint])//节点之前不在队列中/首次进入队列
				{
					if (++updateCnt[dpoint] > n - 1)// 松弛次数超过n-1就有负环
						return true;

					que[r++] = dpoint;
					enter[dpoint] = true;
				}
			}
		}
	}
	return false;
}

int main()
{
	int t;
	cin >> t;
	for (int a = 0; a < t; a++)
	{
		int n, m;
		cin >> n >> m;
		build();
		for (int i = 0; i < m; i++)
		{
			int u, v, w;
			cin >> u >> v >> w;
			addEdge(u, v, w);
		}

		cout << (SPFA(n) ? "YES" : "NO") << endl;
	}
	
	return 0;
}

