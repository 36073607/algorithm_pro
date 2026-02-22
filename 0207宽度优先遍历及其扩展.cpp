#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <deque>
#include <queue>
#include <algorithm>
#include <climits>
using namespace std;



//总览：
//本节课讲述：
//单源、多源宽度优先遍历基本过程
//01bfs，宽度优先遍历与双端队列结合
//宽度优先遍历与优先级队列（堆）结合
//宽度优先遍历与深度优先遍历结合，去生成路径



//一.宽度优先遍历基本内容：
//bfs的特点是逐层扩散，从源头点到目标点扩散了几层，最短路径就是多少
//bfs可以使用的特征是 ：任意两个节点之间的相互距离相同（无权图），这里注意：BFS完全可以处理有向图，其与 最短路算法 的区别在于BFS不能处理带权图
//bfs开始时，可以是 单个源头 ，也可以是 多个源头(第0层有1个源头-->>单源；第0层有多个源头-->>多源)(多个源头同时进行bfs，标记过的不重复访问即可)
//bfs频繁使用队列，形式可以是 单点弹出(需将每个点的信息也放进队列) 或者 整层弹出(一次处理一层，不用将每个节点信息放进队列，只需在队列外使用变量维护即可)
//bfs进行时，进入队列的节点需要标记状态，防止 同一个节点重复进出队列
//bfs进行时，可能会包含 剪枝策略 的设计

//bfs是一个理解难度很低的算法，难点在于 节点如何找到路、路的展开 和 剪枝设计




//题目一：
// 地图分析
// 你现在手里有一份大小为 n x n 的 网格 grid
// 上面的每个 单元格 都用 0 和 1 标记好了其中 0 代表海洋，1 代表陆地。
// 请你找出一个海洋单元格，这个海洋单元格到离它最近的陆地单元格的距离是最大的
// 并返回该距离。如果网格上只有陆地或者海洋，请返回 -1。
// 我们这里说的距离是「曼哈顿距离」（ Manhattan Distance）：
// (x0, y0) 和 (x1, y1) 这两个单元格之间的距离是 |x0 - x1| + |y0 - y1| 。
// 测试链接 : https://leetcode.cn/problems/as-far-from-land-as-possible/
class Solution 
{
public:
    int maxDistance(vector<vector<int>>& grid)
    {
        const int MAXSIZE = 101;
        vector<vector<int>> deq(MAXSIZE * MAXSIZE, vector<int>(2, -1));
        int l = 0, r = 0;
        vector<vector<bool>> check(MAXSIZE, vector<bool>(MAXSIZE, false));//状态数组
        // 方向数组：上下左右 -->> 方便
        vector<vector<int>> directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

        int seas = 0;//海洋数，用于题目要求的特判使用
        int m = grid.size();
        int n = grid[0].size();
        for (int i = 0; i < m; i++)//将陆地放入队列，为最初层
        {
            for (int j = 0; j < n; j++)
            {
                if (grid[i][j] == 1)
                {
                    deq[r++] = { i,j };
                    check[i][j] = true;
                }
                else
                {
                    seas++;
                }
            }
        }

        if (seas == 0 || seas == m * n)//题目要求的特判
            return -1;

        //按层处理的bfs
        int level = 0;
        while (l < r)
        {
            level++;
            int curLevelSize = r - l;
            for (int i = 0; i < curLevelSize; i++)
            {
                int curx = deq[l][0];
                int cury = deq[l++][1];
                for (auto& move : directions)
                {
                    int dx = curx + move[0];
                    int dy = cury + move[1];
                    if (dx >= 0 && dx <= m - 1 && dy >= 0 && dy <= n - 1 && !check[dx][dy])
                    {
                        check[dx][dy] = true;
                        deq[r++] = { dx,dy };
                    }
                }
            }
        }
        return level - 1;
    }
};



//题目二：
// 贴纸拼词
// 我们有 n 种不同的贴纸。每个贴纸上都有一个小写的英文单词。
// 您想要拼写出给定的字符串 target ，方法是从收集的贴纸中切割单个字母并重新排列它们
// 如果你愿意，你可以多次使用每个贴纸，每个贴纸的数量是无限的。
// 返回你需要拼出 target 的最小贴纸数量。如果任务不可能，则返回 -1
// 注意：在所有的测试用例中，所有的单词都是从 1000 个最常见的美国英语单词中随机选择的
// 并且 target 被选择为两个随机单词的连接。
// 测试链接 : https://leetcode.cn/problems/stickers-to-spell-word/
// 
//分析如何找到路、路的展开：
//大的字符串target结合每个贴纸就是一条路，得到的剩余字符串就是下一层
//-->>彻底被搞定变成空字符串在第几层
class Solution
{
private:
    static const int MAXN = 1000;

    string eliminate(string cur, string s)
    {
        string ret;
        for (int i = 0, j = 0; i < cur.size(); )
        {
            if (j == s.size())//贴纸已经检测完最后一个字符，但cur字符串仍未检测完
                ret += cur[i++];
            else
            {
                if (cur[i] < s[j])
                    ret += cur[i++];
                else if (cur[i] > s[j])
                    j++;
                else
                {
                    i++;
                    j++;
                }
            } 
        }
        return ret;
    }

public:
    int minStickers(vector<string>& stickers, string target)
    {
        //按照字典序把target排序，优先把同一字符全部消掉后再进行后续操作
        sort(target.begin(), target.end());
        //为了方便检查消掉某个字符需要哪张贴纸，建图
        vector<vector<string>> AdjList(26);
        //建立哈希表，避免重复检查
        unordered_set<string> visited;
        //双端队列
        vector<string> deq(MAXN);
        int l = 0, r = 0;

        for (auto& str : stickers)//建图加边
        {
            sort(str.begin(), str.end());
            for (int i = 0; i < str.size(); i++)
            {
                if (i == 0 || str[i] != str[i - 1])
                {
                    AdjList[str[i] - 'a'].push_back(str);
                }
            }
        }
   
        //bfs(按层)
        int level = 0;//处理的次数 / 贴纸的张数
        deq[r++] = target;
        visited.insert(target);
        while (l < r)
        {
            int curLevelSize = r - l;
            for (int i = 0; i < curLevelSize; i++)
            {
                string curStr = deq[l++];
                for (auto& ele : AdjList[curStr[0] - 'a'])
                {
                    string nextStr = eliminate(curStr, ele);
                    if (nextStr == "")
                        return level + 1;
                    else if (visited.find(nextStr) == visited.end())//之前未加入
                    {
                        visited.insert(nextStr);
                        deq[r++] = nextStr;
                    }
                } 
            }
            level++; // 当前层处理完毕，层数增加
        }
        return -1;
    }
};





//二.01bfs过程详解
//01bfs，适用于 图中所有边的权重只有0和1两种值，求源点到目标点的最短距离
//时间复杂度为 O(节点数量+边的数量)
//Q: 为什么不能用传统bfs?
//A: 由于有的边权值为0，不好直接将节点分层；如果想，需先把边按照权值排序再分层，但这样时间复杂度太高

//过程：
//1.distance[i]表示从源点到i点的最短距离，初始时所有点的distance设置为无穷大
//2.源点进入双端队列，distance[源点]=0
//3.双端队列 头部弹出 x,
//  A.如果x是目标点，返回distance[x]表示源点到目标点的最短距离
//  B.考察从x出发的每一条边，假设某边去y点，边权为w
//    1) 如果 distance[y] > distance[x] + w , 处理该边；否则忽略该边
//    2) 处理时，更新distance[y] = distance[x] + w
//           如果w==0，y从头部进入双端队列。继续重复步骤3
//           如果w==1，y从尾部进入双端队列。继续重复步骤3
//4.双端队列为空停止

//正确性证明：
//双端队列只能从头部弹出；当某节点a从头部进入时，可以对双端队列中在尾部的节点a进行答案的修改
//详情见 https://www.bilibili.com/video/BV1Dw411w7P5/?spm_id_from=333.1387.collection.video_card.click&vd_source=fdf4f9cadef70de43e5071838cb1aecc

//为什么不需要visited来标记节点？
//distance数组已经起到了该作用



//题目三：
// 到达角落需要移除障碍物的最小数目
// 给你一个下标从 0 开始的二维整数数组 grid ，数组大小为 m x n
// 每个单元格都是两个值之一：
// 0 表示一个 空 单元格，
// 1 表示一个可以移除的 障碍物
// 你可以向上、下、左、右移动，从一个空单元格移动到另一个空单元格。
// 现在你需要从左上角 (0, 0) 移动到右下角 (m - 1, n - 1) 
// 返回需要移除的障碍物的最小数目
// 测试链接 : https://leetcode.cn/problems/minimum-obstacle-removal-to-reach-corner/
//
//这里不采用手写deque了，练练deque容器(数据太大，用数组模拟双端队列似乎不太好)
//注：正如01bfs详解中提到的，没有明确的“层数”概念，故使用单点弹出bfs
class Solution 
{
public:
    int minimumObstacles(vector<vector<int>>& grid)
    {
        int m = grid.size();
        int n = grid[0].size();
        vector<vector<int>> distance(m, vector<int>(n, INT_MAX));//源点到其他点的距离，初始化全为无穷大
        vector<vector<int>> move = { {-1,0},{1,0},{0,1},{0,-1} };//上、下、左、右四个方向
        deque<vector<int>> deq;//双端队列

        deq.push_front({ 0,0 });
        distance[0][0] = 0;

        while (!deq.empty())
        {
            int curx = deq.front()[0];
            int cury = deq.front()[1];
            deq.pop_front();
            if (curx == m - 1 && cury == n - 1)
                return distance[curx][cury];

            for (auto& ele : move)
            {
                int dx = curx + ele[0];
                int dy = cury + ele[1];
                if (dx >= 0 && dx <= m - 1 && dy >= 0 && dy <= n - 1)
                {
                    if (distance[curx][cury] + grid[dx][dy] < distance[dx][dy])
                    {
                        distance[dx][dy] = distance[curx][cury] + grid[dx][dy];
                        if (grid[dx][dy] == 0)
                            deq.push_front({ dx,dy });
                        else if (grid[dx][dy] == 1)
                            deq.push_back({ dx,dy });
                    }
                }
            }
        }
        return -1;
    }
};



//题目四：
// 使网格图至少有一条有效路径的最小代价
// 给你一个 m * n 的网格图 grid 。 grid 中每个格子都有一个数字
// 对应着从该格子出发下一步走的方向。 grid[i][j] 中的数字可能为以下几种情况：
// 1 ，下一步往右走，也就是你会从 grid[i][j] 走到 grid[i][j + 1]
// 2 ，下一步往左走，也就是你会从 grid[i][j] 走到 grid[i][j - 1]
// 3 ，下一步往下走，也就是你会从 grid[i][j] 走到 grid[i + 1][j]
// 4 ，下一步往上走，也就是你会从 grid[i][j] 走到 grid[i - 1][j]
// 注意网格图中可能会有 无效数字 ，因为它们可能指向 grid 以外的区域
// 一开始，你会从最左上角的格子 (0,0) 出发
// 我们定义一条 有效路径 为从格子 (0,0) 出发，每一步都顺着数字对应方向走
// 最终在最右下角的格子 (m - 1, n - 1) 结束的路径
// 有效路径 不需要是最短路径
// 你可以花费1的代价修改一个格子中的数字，但每个格子中的数字 只能修改一次
// 请你返回让网格图至少有一条有效路径的最小代价
// 测试链接 : https://leetcode.cn/problems/minimum-cost-to-make-at-least-one-valid-path-in-a-grid/
//
//思路：以当前格子为右箭头为例：
//则当前格子到上、左、下方格子的代价是1；到右方格子的代价为0  -->> 边的权值为0/1
// -->> 01bfs
class Solution
{
public:
    int minCost(vector<vector<int>>& grid)
    {
        int m = grid.size();
        int n = grid[0].size();
        vector<vector<int>> move = { {},{0,1},{0,-1},{1,0},{-1,0} };//模拟题目信息：下标为1往右走、下标为2往左走……
        //                            0   1      2     3      4
        vector<vector<int>> distance(m, vector<int>(n, INT_MAX));
        deque<vector<int>> deq;
        
        //01bfs
        //处理队头
        deq.push_front({ 0,0 });
        distance[0][0] = 0;
        while (!deq.empty())
        {
            int curx = deq.front()[0];
            int cury = deq.front()[1];
            deq.pop_front();
            
            if (curx == m - 1 && cury == n - 1)
                return distance[curx][cury];

            for (int i = 1; i <= 4; i++)
            {
                int dx = curx + move[i][0];
                int dy = cury + move[i][1];
                if (dx >= 0 && dx <= m - 1 && dy >= 0 && dy <= n - 1)
                {
                    int w = grid[curx][cury] == i ? 0 : 1;
                    if (distance[curx][cury] + w < distance[dx][dy])
                    {
                        distance[dx][dy] = distance[curx][cury] + w;
                        if (w == 0)
                            deq.push_front({ dx,dy });
                        else if (w == 1)
                            deq.push_back({ dx,dy });
                    }
                }       
            }
        }
        return -1;
    }
};



//三.宽度优先遍历与优先队列结合，更进一步的内容在讲Dijkstra算法时说明


//题目五：
// 二维接雨水
// 给你一个 m * n 的矩阵，其中的值均为非负整数，代表二维高度图每个单元的高度
// 请计算图中形状最多能接多少体积的雨水。
// 测试链接 : https://leetcode.cn/problems/trapping-rain-water-ii/
//
//从最弱的点开始，保证水线一定能涨到这个程度
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
    int trapRainWater(vector<vector<int>>& heightMap) 
    {
        int m = heightMap.size();
        int n = heightMap[0].size();
        int ans = 0;
        vector<vector<int>> move = { {0,-1},{0,1},{1,0},{-1,0} };
        vector<vector<int>> visited(m, vector<int>(n, 0));
        priority_queue<vector<int>, vector<vector<int>>, cmp> pq;
        //[0] -> 行 ; [1] -> 列 ; [2] -> 记录的水线最高高度

        //边界信息先入堆
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (i == 0 || i == m - 1 || j == 0 || j == n - 1)
                {
                    pq.push({ i,j,heightMap[i][j] });
                    visited[i][j] = 1;
                }
            }
        }

        while (!pq.empty())
        {
            int x = pq.top()[0];
            int y = pq.top()[1];
            int val = pq.top()[2];
            ans += val - heightMap[x][y];
            pq.pop();

            for (auto& ele : move)
            {
                int dx = x + ele[0];
                int dy = y + ele[1];

                if (dx >= 0 && dx <= m - 1 && dy >= 0 && dy <= n - 1 && !visited[dx][dy])
                {
                    visited[dx][dy] = 1;
                    int dval = max(val, heightMap[dx][dy]);
                    pq.push({ dx,dy,dval });
                }
            }
        }
        return ans;
    }
};



//四.宽度优先遍历与深度优先遍历结合，去生成路径
//1.bfs建图
//2.dfs利用图生成路径


//题目六：
// 单词接龙 II
// 按字典 wordList 完成从单词 beginWord 到单词 endWord 转化
// 一个表示此过程的 转换序列 是形式上像 
// beginWord -> s1 -> s2 -> ... -> sk 这样的单词序列，并满足：
// 每对相邻的单词之间仅有单个字母不同
// 转换过程中的每个单词 si（1 <= i <= k）必须是字典 wordList 中的单词
// 注意，beginWord 不必是字典 wordList 中的单词
// sk == endWord
// 给你两个单词 beginWord 和 endWord ，以及一个字典 wordList
// 请你找出并返回所有从 beginWord 到 endWord 的 最短转换序列
// 如果不存在这样的转换序列，返回一个空列表
// 每个序列都应该以单词列表 [beginWord, s1, s2, ..., sk] 的形式返回
// 测试链接 : https://leetcode.cn/problems/word-ladder-ii/
//
//思路：利用BFS反向建图：endWord可以由序列a中的单词转化来，序列a中的单词可以由其他序列中的单词转化而来……
//小的优化：若想要知道单词"abc"可以转化成哪些单词，若依次遍历字典wordList中的所有单词，效率较低；
//          根据题目给出的数据量，可以将每个字母替换成26个字母中的其他字母，检查替换后的单词在worldList中是否出现过 -->> 哈希表
//          eg: 检查worldList中是否有"bbc","cbc"…… "aac","acc"…… "aba","abb"……
class Solution 
{
private:
    unordered_set<string> dict;//词典，将wordLsit转化为哈希表方便查找
    unordered_set<string> curLevel;//BFS当前层
    unordered_set<string> nextLevel;//BFS从当前层查找出来的下一层
    unordered_map<string, vector<string>> AdjList;//利用BFS反向建图
    vector<vector<string>> ans;
    vector<string> path;//记录答案中的每一个字符串序列

    void build(vector<string>& wordList)
    {
        dict.insert(wordList.begin(), wordList.end());
    }

    bool BFS(string beginWord, string endWord)
    {
        bool whetherFoundGraph = false;
        curLevel.insert(beginWord);
        while (!curLevel.empty())
        {
            //避免重复，把操作过的字符串删除
            for (auto& ele : curLevel)
            {
                dict.erase(ele);
            }

            for (auto& ele : curLevel)//对当前层中的所有字符串搜索
            {   
                //对每个字符串的每个字母进行替换，并检查词典中是否有该替换成的字符串
                for (int i = 0; i < ele.size(); i++)
                {
                    string tmp = ele;
                    for (char ch = 'a'; ch <= 'z'; ch++)
                    {
                        tmp[i] = ch;
                        if (tmp != ele && dict.find(tmp) != dict.end())//替换ele中某个字母的新字符串可以在词典中找到
                        {
                            if (tmp == endWord)
                                whetherFoundGraph = true;

                            nextLevel.insert(tmp);
                            if (AdjList.find(tmp) == AdjList.end())//这段可有可无，为了逻辑连贯，还是写上
                            {
                                AdjList.insert(make_pair(tmp, vector<string>()));
                            }
                            AdjList[tmp].push_back(ele);
                        }
                    }
                }
            }

            if (whetherFoundGraph)
                return true;
            else
            {
                //将nextLevel中的数据传给curLevel，nextLevel清空
                curLevel = nextLevel;
                nextLevel.clear();
            }
        }
        return false;
    }

    void DFS(string str, string beginWord)
    {
        path.push_back(str);
        if (str == beginWord)
        {
            vector<string> tmp = path;
            reverse(tmp.begin(), tmp.end());
            ans.push_back(tmp);
        }

        if (AdjList.find(str) != AdjList.end())//理论上str一定会在图中，这里保险写法
        {
            for (auto& ele : AdjList[str])
            {
                DFS(ele, beginWord);
            }
        }
        path.pop_back();//回溯
    }

public:
    vector<vector<string>> findLadders(string beginWord, string endWord, vector<string>& wordList)
    {
        build(wordList);
        if (dict.find(endWord) == dict.end())//特判：词典中连endWord都没有，不可能找到
            return ans;
        if (BFS(beginWord, endWord))//能成功建图，说明存在可行的路径
        {
            DFS(endWord, beginWord);
        }
        return ans;
    }
};
//
// bfs正向是为了尽快从start找到end，且把路都找全，因为题目说了要所有的路。
// dfs反向是因为从start到end所有的已经建好，你只需要尽快的把所有路径生成，此时从end出发每走的一步都是某条路径必然包含的一部分，都是最终一定能通往start的必经路，没有冤枉路、无效的路。


