#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <cctype>
#include <algorithm>
using namespace std;


//一.Dijkstra算法
//Dijkstra算法：给定一个源点，求解从源点到每个点的最短路径长度。单源最短路径算法。
//适用范围：有向图、边的权值没有负数

//彻底暴力的Dijkstra算法，不讲、时间复杂度太差、无意义

//普通堆实现的Dijkstra算法，最普遍、最常用
//算法核心过程：
//1.节点弹出就忽略（源点到该点的最短距离已经确定）
//2.节点没弹出过，处理所有边，让其他没弹出的节点，距离变小的记录，加入堆
//剪枝：由于每次都操作小根堆堆顶，所以一旦遇到终点，即可结算

//反向索引堆实现的Dijkstra算法，最快速、最极致
//核心在于掌握反向索引堆

//0-1BFS就是DJ算法是简化版，是一种特殊情况：因为只有0，1。所以把堆优化成双端队列了

//相关题目：题目一 -- 题目三


//普通堆实现的Dijkstra算法，时间复杂度O(m * log m) ，m为边数
//
//1.distance[i]表示从源点到i点的最短距离，visited[i]表示i节点是否从小根堆弹出过
//2.准备好小根堆，小根堆存放记录：（x点，源点到x的距离），小根堆根据距离组织
//3.令distance[源点]=0，（源点，0）进入小根堆
//4.从小根堆弹出（u点，源点到u的距离）
//  a.如果 visited[u] == true，不做任何处理，重复步骤4
//  b.如果 visited[u] == false，令visited[u] = true，u就算弹出过了
//    然后考察u的每一条边，假设某边去往v，边权为w
//    1) 如果visited[v] == false 并且 distance[u] + w < distance[v]
//       令distance[v] = distance[u] + w，把(v，distance[u] + w)加入小根堆
//    2) 处理完u的每一条边之后，重复步骤4
//5.小根堆为空过程结束，distance表记录了源点到每个节点的最短距离


//反向索引堆实现Dijkstra算法，时间复杂度O(m * log n)，n为节点数，m为边数
//
//1.准备好反向索引堆，根据源点到当前点的距离组织小根堆，可以做到如下操作
//  a.新增记录(x，源点到x的距离)    b.当源点到x的距离更新时，可以进行堆的调整
//  b.x点一旦弹出，以后忽略x        d.弹出堆顶的记录(u，源点到u的距离)
//2.把(源点，0)加入反向索引堆，过程开始
//3.反向索引堆弹出(u，源点到u的距离)，考察u的每一条边，假设某边去往v，边权为w
//  1) 如果v没有进入过反向索引堆里，新增记录（v，源点到u的距离 + w）
//  2) 如果v曾经从反向索引堆弹出过，忽略
//  3) 如果v在反向索引堆里，看看源点到v的距离能不能变得更小，如果能，调整堆；不能，忽略
//  4) 处理完u的每一条边，处理步骤3
//4.反向索引堆为空，过程结束。反向索引堆里记录了源点到每个节点的最短距离


//题目一：
// Dijkstra算法模版（Leetcode）
// 网络延迟时间
// 有 n 个网络节点，标记为 1 到 n
// 给你一个列表 times，表示信号经过 有向 边的传递时间
// times[i] = (ui, vi, wi)，表示从ui到vi传递信号的时间是wi
// 现在，从某个节点 s 发出一个信号
// 需要多久才能使所有节点都收到信号
// 如果不能使所有节点收到信号，返回 -1
// 测试链接 : https://leetcode.cn/problems/network-delay-time
//
//动态建图+普通堆 实现
class Solution
{
private:
    struct cmp
    {
        bool operator()(vector<int>& a, vector<int>& b)
        {
            return a[1] > b[1];//小根堆
        }
    };

public:
    int networkDelayTime(vector<vector<int>>& times, int n, int k)
    {
        //建图 加边
        vector<vector<vector<int>>> AdjList(n + 1);
        for (auto& time : times)
        {
            AdjList[time[0]].push_back({ time[1],time[2] });
        }

        //Dijkstra
        vector<int> distance(n + 1, INT_MAX);
        vector<int> visited(n + 1, false);
        priority_queue<vector<int>, vector<vector<int>>, cmp> pq;//小根堆
        //处理源点
        distance[k] = 0;
        pq.push({ k,0 });
        while (!pq.empty())
        {
            int curNode = pq.top()[0];
            pq.pop();
            if (visited[curNode])
                continue;
            else
                visited[curNode] = true;
            //当前节点之前从未弹出过
            for (auto& next : AdjList[curNode])
            {
                int nextNode = next[0];
                int weight = next[1];

                if (!visited[nextNode] && distance[curNode] + weight < distance[nextNode])
                {
                    distance[nextNode] = distance[curNode] + weight;
                    pq.push({ nextNode,distance[nextNode] });
                }
            }
        }

        //结算答案
        int ans = 0;
        for (int i = 1; i <= n; i++)
        {
            if (distance[i] == INT_MAX)
                return -1;
            ans = max(ans, distance[i]);
        }
        return ans;
    }
};


//补充：反向索引堆
//堆的本质是用数组模拟的完全二叉树
//普通堆想要改变堆中某个节点的数据，需知道该节点的位置(即数组中下标的位置) -->> 需要遍历，代价太高
//反向索引堆则创建了一张反向索引表，能看出当前节点所在的数组下标


//链式前向星+反向索引堆 实现
// Dijkstra算法模版（洛谷）
// 静态空间实现 : 链式前向星 + 反向索引堆
// 测试链接 : https://www.luogu.com.cn/problem/P4779
#include <iostream>
#include <vector>
#include <climits>
using namespace std;

const int MAXN = 100005;//点
const int MAXM = 200005;//边

//链式前向星建图
vector<int> head(MAXN, 0);
vector<int> nxt(MAXM, 0);
vector<int> to(MAXM, 0);
vector<int> weight(MAXM, 0);
int edgeNum = 1;

//反向索引堆 -- 小根堆
vector<int> heap(MAXN, 0);//堆中存放节点编号
// where[v] = -1，表示v这个节点，从来没有进入过堆
// where[v] = -2，表示v这个节点，已经弹出过了
// where[v] = i(>=0)，表示v这个节点，在堆上的i位置
vector<int> where(MAXN, -1);//反向索引表
vector<int> distance(MAXN, INT_MAX);//每个点到源点的距离
int heapsize = 0;

int n, m, s;

void addEdge(int u, int v, int w)
{
    to[edgeNum] = v;
    nxt[edgeNum] = head[u];
    head[u] = edgeNum;
    weight[edgeNum++] = w;
}

bool isHeapEmpty()
{
    return heapsize == 0;
}

void heapInsert(int i)
{
    while (::distance[heap[i]] < ::distance[heap[(i - 1) / 2]])
    {
        swap(heap[i], heap[(i - 1) / 2]);
        swap(where[heap[i]], where[heap[(i - 1) / 2]]);
        i = (i - 1) / 2;
    }
}

void heapify(int i)
{
    int l = 2 * i + 1;
    while (l < heapsize)//该节点还有下一层
    {
        int r = l + 1 < heapsize ? l + 1 : -1;
        int minIdx = (r > 0 && ::distance[heap[r]] < ::distance[heap[l]]) ? r : l;
        minIdx = ::distance[heap[i]] < ::distance[heap[minIdx]] ? i : minIdx;
        if (i == minIdx)
            return;
        swap(heap[i], heap[minIdx]);
        swap(where[heap[i]], where[heap[minIdx]]);
        i = minIdx;
        l = i * 2 + 1;
    }
}

int heapPop()
{
    int ret = heap[0];
    swap(heap[0], heap[--heapsize]);
    swap(where[heap[0]], where[heap[heapsize]]);
    where[ret] = -2;
    heapify(0);
    return ret;
}

void work(int v, int c)
{
    if (where[v] == -1)//之前从未加入过堆
    {
        heap[heapsize++] = v;
        where[v] = heapsize - 1;
        ::distance[v] = c;
        heapInsert(heapsize - 1);
    }
    else if (where[v] >= 0)//之前加入过堆
    {
        ::distance[v] = min(::distance[v], c);
        heapInsert(where[v]);
    }
    //else if (where[v] == -2)   说明之前加入过并弹出，不用管
}

int main()
{
    cin >> n >> m >> s;
    for (int i = 0; i < m; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        addEdge(u, v, w);
    }

    //Dijkstra
    work(s, 0);//处理源点
    while (!isHeapEmpty())
    {
        int curNode = heapPop();
        for (int ei = head[curNode]; ei > 0; ei = nxt[ei])
        {
            work(to[ei], ::distance[curNode] + weight[ei]);
        }
    }

    for (int i = 1; i <= n; i++)
        cout << ::distance[i] << " ";

    return 0;
}



//题目二：
// 最小体力消耗路径
// 你准备参加一场远足活动。给你一个二维 rows x columns 的地图 heights
// 其中 heights[row][col] 表示格子 (row, col) 的高度
// 一开始你在最左上角的格子 (0, 0) ，且你希望去最右下角的格子 (rows-1, columns-1) 
// （注意下标从 0 开始编号）。你每次可以往 上，下，左，右 四个方向之一移动
// 你想要找到耗费 体力 最小的一条路径
// 一条路径耗费的体力值是路径上，相邻格子之间高度差绝对值的最大值
// 请你返回从左上角走到右下角的最小 体力消耗值
// 测试链接 ：https://leetcode.cn/problems/path-with-minimum-effort/
class Solution
{
private:
    struct cmp
    {
        bool operator()(vector<int>& a, vector<int>& b)
        {
            return a[2] > b[2];//小根堆
        }
    };

public:
    int minimumEffortPath(vector<vector<int>>& heights) 
    {
        int m = heights.size();
        int n = heights[0].size();
        vector<vector<int>> move = { {-1,0},{1,0},{0,1},{0,-1} };
        vector<vector<int>> distance(m, vector<int>(n, INT_MAX));
        vector<vector<int>> visited(m, vector<int>(n, false));//弹出小根堆的记录
        priority_queue<vector<int>, vector<vector<int>>, cmp> pq;//小根堆

        //Dijkstra
        pq.push({ 0,0,0 });
        while (!pq.empty())
        {
            int curx = pq.top()[0];
            int cury = pq.top()[1];
            int curval = pq.top()[2];
            pq.pop();
            if (visited[curx][cury])
                continue;
            visited[curx][cury] = true;

            if (curx == m - 1 && cury == n - 1)//剪枝：到达终点，直接结束
                return curval;

            for (auto& ele : move)
            {
                int dx = curx + ele[0];
                int dy = cury + ele[1];

                if (dx >= 0 && dx <= m - 1 && dy >= 0 && dy <= n - 1 && !visited[dx][dy])
                {
                    int w = max(curval, abs(heights[curx][cury] - heights[dx][dy]));
                    if (w < distance[dx][dy])
                    {
                        distance[dx][dy] = w;
                        pq.push({ dx,dy,w });
                    }
                }
            }
        }
        return -1;
    }
};



//题目三：
// 水位上升的泳池中游泳
// 在一个 n x n 的整数矩阵 grid 中
// 每一个方格的值 grid[i][j] 表示位置 (i, j) 的平台高度
// 当开始下雨时，在时间为 t 时，水池中的水位为 t
// 你可以从一个平台游向四周相邻的任意一个平台，但是前提是此时水位必须同时淹没这两个平台
// 假定你可以瞬间移动无限距离，也就是默认在方格内部游动是不耗时的
// 当然，在你游泳的时候你必须待在坐标方格里面。
// 你从坐标方格的左上平台 (0，0) 出发
// 返回 你到达坐标方格的右下平台 (n-1, n-1) 所需的最少时间
// 测试链接 : https://leetcode.cn/problems/swim-in-rising-water/
//
//翻译：如果当前格子跟邻居格子高度一致，则移动无成本；否则，成本是两者的最大值。从左上到右下，找一条最短路径
//实际上跟题目二一样
class Solution
{
private:
    struct cmp
    {
        bool operator()(vector<int>& a, vector<int>& b)
        {
            return a[2] > b[2];//小根堆
        }
    };

public:
    int swimInWater(vector<vector<int>>& grid) 
    {
        int m = grid.size();
        int n = grid[0].size();
        vector<vector<int>> move = { {-1,0},{1,0},{0,1},{0,-1} };
        vector<vector<int>> distance(m, vector<int>(n, INT_MAX));
        vector<vector<int>> visited(m, vector<int>(n, false));
        priority_queue<vector<int>, vector<vector<int>>, cmp> pq;

        //Dijkstra
        distance[0][0] = grid[0][0];//处理源点
        pq.push({ 0,0,distance[0][0] });
        while (!pq.empty())
        {
            int curx = pq.top()[0];
            int cury = pq.top()[1];
            int curval = pq.top()[2];
            pq.pop();
            if (visited[curx][cury])
                continue;
            visited[curx][cury] = true;

            if (curx == m - 1 && cury == n - 1)
                return curval;

            for (auto& ele : move)
            {
                int dx = curx + ele[0];
                int dy = cury + ele[1];
                if (dx >= 0 && dx <= m - 1 && dy >= 0 && dy <= n - 1 && !visited[dx][dy])
                {
                    int w = max(curval, grid[dx][dy]);
                    if (w < distance[dx][dy])
                    {
                        distance[dx][dy] = w;
                        pq.push({ dx,dy,w });
                    }
                }
            }
        }
        return -1;
    }
};

//小结题目二、题目三：
//1.这两道题对DJ算法的应用体现在distance数组的信息处理当中：distance数组中存放的数据不一定是某个点到源点到距离，还可能是其他值
//2.这两道题都可以用并查集
//3.这两道题都可以用二分答案+搜索(bfs,dfs)





//二.分层图最短路（带信息的bfs和Dijkstra）
//分层图最短路，又叫扩点最短路

//不把实际位置看做图上的点，而是把 实际位置及其状态的组合 看作是图上的点，
//然后搜索bfs或者Dijkstra的过程不变，只是扩了点（分层）而已
//原理简单，核心在于如何扩点、如何到达、如何算距离，每个题可能都不一样

//对应本节题目4、题目5、题目6


//题目四：
// 获取所有钥匙的最短路径
// 给定一个二维网格 grid ，其中：
// '.' 代表一个空房间、'#' 代表一堵、'@' 是起点
// 小写字母代表钥匙、大写字母代表锁
// 从起点开始出发，一次移动是指向四个基本方向之一行走一个单位空间
// 不能在网格外面行走，也无法穿过一堵墙
// 如果途经一个钥匙，我们就把它捡起来。除非我们手里有对应的钥匙，否则无法通过锁。
// 假设 k 为 钥匙/锁 的个数，且满足 1 <= k <= 6，
// 字母表中的前 k 个字母在网格中都有自己对应的一个小写和一个大写字母
// 换言之，每个锁有唯一对应的钥匙，每个钥匙也有唯一对应的锁
// 另外，代表钥匙和锁的字母互为大小写并按字母顺序排列
// 返回获取所有钥匙所需要的移动的最少次数。如果无法获取所有钥匙，返回 -1 。
// 测试链接：https://leetcode.cn/problems/shortest-path-to-get-all-keys
class Solution 
{
private:
    static const int MAXN = 31;
    static const int MAXM = 31;
    static const int MAXK = 6;

public:
    int shortestPathAllKeys(vector<string>& grid) 
    {
        int m = grid.size();
        int n = grid[0].size();
        vector<vector<int>> move = { {0,1},{0,-1},{1,0},{-1,0} };

        queue<vector<int>> que;//动态创建队列
        //[i][0] -> x:横坐标
        //[i][1] -> y:纵坐标
        //[i][2] -> s:当前钥匙的状态，用位信息储存

        vector<vector<vector<int>>> visited(MAXN, vector<vector<int>>(MAXM, vector<int>(1 << MAXK, false)));
        //[x][y][s]

        int key = 0;//拥有所有钥匙的最终状态，用于判断
        int level = 0;
        //初始化
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (grid[i][j] == '@')
                {
                    que.push({ i,j,0 });
                    visited[i][j][0] = true;
                }
                    
                if (islower(grid[i][j]))
                    key |= 1 << (grid[i][j] - 'a');
            }
        }

        //按层bfs
        while (!que.empty())
        {
            int curLevelSize = que.size();
            for (int i = 0; i < curLevelSize; i++)
            {
                int curx = que.front()[0];
                int cury = que.front()[1];
                int curs = que.front()[2];
                que.pop();

                if (curs == key)//剪枝：收集好了所有钥匙，返回答案
                    return level + 1;

                for (auto& ele : move)
                {
                    int dx = curx + ele[0];
                    int dy = cury + ele[1];
                    int ds = curs;

                    //越界，跳过
                    //注意：这个判断要放在最前面，否则下面的判断过程中可能越界
                    if (dx < 0 || dx >= m || dy < 0 || dy >= n)
                        continue;

                    if (grid[dx][dy] == '#' || (isupper(grid[dx][dy]) && ((ds & (1 << (grid[dx][dy] - 'A'))) == 0)))//遇到墙 或 遇到锁但没有对应钥匙
                        continue;

                    if (islower(grid[dx][dy]))//如果遇到新钥匙，调整当前钥匙状态
                        ds |= 1 << (grid[dx][dy] - 'a');

                    if (!visited[dx][dy][ds])
                    {
                        que.push({ dx,dy,ds });
                        visited[dx][dy][ds] = true;
                    }
                }
            }
            level++;
        }

        return -1;
    }
};



//题目五：
// 电动车游城市
// 小明的电动车电量充满时可行驶距离为 cnt，每行驶 1 单位距离消耗 1 单位电量，且花费 1 单位时间
// 小明想选择电动车作为代步工具。地图上共有 N 个景点，景点编号为 0 ~ N-1
// 他将地图信息以 [城市 A 编号,城市 B 编号,两城市间距离] 格式整理在在二维数组 paths，
// 表示城市 A、B 间存在双向通路。
// 初始状态，电动车电量为 0。每个城市都设有充电桩，
// charge[i] 表示第 i 个城市每充 1 单位电量需要花费的单位时间。
// 请返回小明最少需要花费多少单位时间从起点城市 start 抵达终点城市 end
// 测试链接 : https://leetcode.cn/problems/DFPeFJ/
//
//核心：每个点的信息扩充 -->> 在某个城市且当前电量为n 即为一个"新点"
// 每当到达一个"新点"，都有两种选择：
//   1.充一格电(不超过电量上限)
//   2.不充电，直接去别的景点
// 这两种选择都对应着一个"新点"
//利用最短路算法(带权图)，当首次到达终点时，得到答案
class Solution 
{
    struct cmp
    {
        bool operator()(vector<int>& a, vector<int>& b)
        {
            return a[2] > b[2];//小根堆
        }
    };

public:
    int electricCarPlan(vector<vector<int>>& paths, int cnt, int start, int end, vector<int>& charge)
    {
        int n = charge.size();
        //建图(题目的原始图)
        vector<vector<vector<int>>> AdjList(n);
        for (auto& edge : paths)
        {
            //无向图
            AdjList[edge[0]].push_back({ edge[1],edge[2] });
            AdjList[edge[1]].push_back({ edge[0],edge[2] });
        }

        //起始点到每个"新点"的代价
        vector<vector<int>> distance(n, vector<int>(cnt + 1, INT_MAX));
        //[i][0] -> 每个"新点"的景点信息
        //[i][1] -> 每个"新点"的电量信息
        //[i][2] -> 到达每个"新点"的代价
        priority_queue<vector<int>, vector<vector<int>>, cmp> pq;
        vector<vector<int>> visited(n, vector<int>(cnt + 1, false));

        //Dijkstra
        //处理起点信息
        pq.push({ start,0,0 });
        distance[start][0] = 0;
        while (!pq.empty())
        {
            int curPoint = pq.top()[0];//每个"新点"的景点信息
            int curCharge = pq.top()[1];//每个"新点"的电量信息
            int curCost = pq.top()[2];//到达每个"新点"的代价
            pq.pop();
            if (visited[curPoint][curCharge])//这个"新点"此前已经结算，直接撇掉
                continue;
            visited[curPoint][curCharge] = true;

            if (curPoint == end)//剪枝
                return curCost;

            //每个新点有两条路径：
            //1.充一格电
            if (curCharge < cnt)
            {
                int dPoint = curPoint;
                int dCharge = curCharge + 1;
                if (!visited[dPoint][dCharge] && distance[curPoint][curCharge] + charge[curPoint] < distance[dPoint][dCharge])
                {
                    distance[dPoint][dCharge] = distance[curPoint][curCharge] + charge[curPoint];
                    pq.push({ dPoint,dCharge,distance[dPoint][dCharge] });
                }
            }
            //2.不充电，直接去别的景点
            for (auto& ele : AdjList[curPoint])
            {
                int dPoint = ele[0];
                int dCharge = curCharge - ele[1];
                //细节：要先检查dCharge的正负，否则可能导致数组越界！！！
                if (dCharge >= 0 && !visited[dPoint][dCharge] && distance[curPoint][curCharge] + ele[1] < distance[dPoint][dCharge])
                {
                    distance[dPoint][dCharge] = distance[curPoint][curCharge] + ele[1];
                    pq.push({ dPoint, dCharge, distance[dPoint][dCharge] });
                }
            }
        }
        return -1;
    }
};



//题目六：
// Alice和Bob现在要乘飞机旅行，他们选择了一家相对便宜的航空公司
// 该航空公司一共在n个城市设有业务，设这些城市分别标记为0 ~ n−1
// 一共有m种航线，每种航线连接两个城市，并且航线有一定的价格
// Alice 和 Bob 现在要从一个城市沿着航线到达另一个城市，途中可以进行转机
// 航空公司对他们这次旅行也推出优惠，他们可以免费在最多k种航线上搭乘飞机
// 那么 Alice 和 Bob 这次出行最少花费多少
// 测试链接 : https://www.luogu.com.cn/problem/P4568
//
//题目六跟题目五差不多
//"新点"所带信息为：当前的城市 及 当前免单次数

//写法一：语言提供的堆结构+邻接矩阵
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
using namespace std;

int n, m, k;
int s, t;

struct my_cmp
{
    bool operator()(vector<int>& a, vector<int>& b)
    {
        return a[2] > b[2];//小根堆
    }
};

int main()
{
    cin >> n >> m >> k >> s >> t;
    vector<vector<vector<int>>> AdjList(n);
    for (int i = 0; i < m; i++)
    {
        //无向图
        int a, b, c;
        cin >> a >> b >> c;
        AdjList[a].push_back({ b,c });
        AdjList[b].push_back({ a,c });
    }

    vector<vector<int>> distance(n, vector<int>(k + 1, INT_MAX));
    //[i][0] -> "新点"的当前城市
    //[i][1] -> "新点"的当前免单次数
    //[i][2] ->  到达"新点"的代价/花费
    priority_queue<vector<int>, vector<vector<int>>, my_cmp> pq;
    vector<vector<int>> visited(n, vector<int>(k + 1, false));

    //Dijkstra
    //处理源点信息
    distance[s][0] = 0;
    pq.push({ s,0,0 });
    while (!pq.empty())
    {
        int curCity = pq.top()[0];
        int curCnt = pq.top()[1];
        int curCost = pq.top()[2];
        pq.pop();

        if (visited[curCity][curCnt])
            continue;
        visited[curCity][curCnt] = true;

        if (curCity == t)
        {
            cout << curCost;
            return 0;
        }
            
        //跟题目五一样，分两种情况：
        for (auto& ele : AdjList[curCity])
        {
            int dCity = ele[0];
            int w = ele[1];

            //1.免单去下一个城市
            if (curCnt + 1 <= k && !visited[dCity][curCnt + 1] && distance[curCity][curCnt] + 0 < distance[dCity][curCnt + 1])
            {
                distance[dCity][curCnt + 1] = distance[curCity][curCnt];
                pq.push({ dCity,curCnt + 1,distance[dCity][curCnt + 1] });
            }
            //2.直接去下一个城市
            if (!visited[dCity][curCnt] && distance[curCity][curCnt] + w < distance[dCity][curCnt])
            {
                distance[dCity][curCnt] = distance[curCity][curCnt] + w;
                pq.push({ dCity,curCnt,distance[dCity][curCnt] });
            }
        }
    }
    
    return -1;
}


//写法二：手搓堆+链式前向星
#include <iostream>
#include <climits>
#include <algorithm>

using namespace std;

// 注意：节点数若为 1~n，MAXN 需要至少开到 n+1
const int MAXN = 10005;     // 稍微开大一点，防止越界
const int MAXM = 100005;
const int MAXK = 15;

// 链式前向星建图
int head[MAXN];
int to[MAXM * 2];           
int nxt[MAXM * 2];          
int weight[MAXM * 2];       
int cnt;

// Dijkstra 需要
int dist[MAXN][MAXK];
bool vis[MAXN][MAXK];

// 手写堆
int heap[MAXM * MAXK][3];
int heapSize;

int n, m, k, s, t;

void swapHeap(int i, int j) {
    int tmp0 = heap[i][0], tmp1 = heap[i][1], tmp2 = heap[i][2];
    heap[i][0] = heap[j][0];
    heap[i][1] = heap[j][1];
    heap[i][2] = heap[j][2];
    heap[j][0] = tmp0;
    heap[j][1] = tmp1;
    heap[j][2] = tmp2;
}

void build() {
    cnt = 1;
    heapSize = 0;
    for (int i = 0; i <= n; ++i) {
        head[i] = 0;
        for (int j = 0; j <= k; ++j) {
            dist[i][j] = INT_MAX;
            vis[i][j] = false;
        }
    }
}

void addEdge(int u, int v, int w) {
    nxt[cnt] = head[u];
    to[cnt] = v;
    weight[cnt] = w;
    head[u] = cnt++;
}

void push(int u, int use, int cost) {
    heap[heapSize][0] = u;
    heap[heapSize][1] = use;
    heap[heapSize][2] = cost;
    int i = heapSize++;
    while (i > 0 && heap[i][2] < heap[(i - 1) / 2][2]) {
        swapHeap(i, (i - 1) / 2);
        i = (i - 1) / 2;
    }
}

int curU, curUse, curCost;

void pop() {
    curU = heap[0][0];
    curUse = heap[0][1];
    curCost = heap[0][2];
    swapHeap(0, --heapSize);
    int i = 0;
    int l = 1;
    while (l < heapSize) {
        int best = l;
        if (l + 1 < heapSize && heap[l + 1][2] < heap[l][2])
            best = l + 1;
        if (heap[best][2] < heap[i][2]) {
            swapHeap(best, i);
            i = best;
            l = i * 2 + 1;
        }
        else {
            break;
        }
    }
}

int dijkstra() {
    dist[s][0] = 0;
    push(s, 0, 0);
    while (heapSize > 0) {
        pop();
        if (vis[curU][curUse])
            continue;
        vis[curU][curUse] = true;
        // 剪枝优化：如果当前路径长度已经超过已知终点的最小距离，可以跳过（可选）
        if (curU == t)
            return curCost;

        for (int e = head[curU]; e; e = nxt[e]) {
            int v = to[e];
            int w = weight[e];
            // 使用一次免费
            if (curUse < k && dist[v][curUse + 1] > dist[curU][curUse]) {
                dist[v][curUse + 1] = dist[curU][curUse];
                push(v, curUse + 1, dist[v][curUse + 1]);
            }
            // 不使用免费
            if (dist[v][curUse] > dist[curU][curUse] + w) {
                dist[v][curUse] = dist[curU][curUse] + w;
                push(v, curUse, dist[v][curUse]);
            }
        }
    }
    return -1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    while (cin >> n >> m >> k >> s >> t) {
        build();
        for (int i = 0; i < m; ++i) {
            int a, b, c;
            cin >> a >> b >> c;
            addEdge(a, b, c);
            addEdge(b, a, c);
        }
        cout << dijkstra() << endl;
    }
    return 0;
}


