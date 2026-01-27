#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

//并查集是一种用于管理元素所属集合的数据结构，实现为一个森林，其中每棵树表示一个集合，树中的节点表示对应集合中的元素

//并查集（Disjoint Set Union, DSU）是一种高效维护若干不相交集合的数据结构，
//核心支持 合并（unite） 和 查询（find） 两种操作，常用于连通性判断、最小生成树（Kruskal）等场景。
//
//初始时，每个元素自成一个集合，父节点指向自己。 查询时沿父指针找到根节点；
//合并时将一个集合的根挂到另一个集合的根上。 为了优化性能，常用 路径压缩 与 按秩 / 按大小合并，使得单次操作的均摊复杂度接近常数 O(α(n))。



//并查集--上
//
//并查集的使用是如下的场景：
//1.一开始每个元素都拥有自己的集合，在自己的集合里只有这个元素自己
//2. find(i)：查找i所在集合的代表元素，代表元素来代表i所在的集合
//3. bool isSameSet(a, b): 判断a和b在不在一个集合里
//4. void union(a, b)：a所在集合所有元素 与 b所在集合所有元素 合并成一个集合
//5.各种操作单次调用的均摊时间复杂度为O(1)

//并查集的两个优化，都发生在find方法里
//1.扁平化（一定要做）
//2.小挂大（可以不做，原论文中 “小”和“大” 是秩的概念，可以理解为 粗略高度 或者 大小）

//并查集的小扩展（在 “并查集--下” 中重点展示）
//可以定制信息：并查集目前有多少个集合，以及给每个集合打上标签信息

//并查集时间复杂度的理解：
//作为如此简单、小巧的结构，
//感性理解单次调用的均摊时间复杂度为O(1)即可，其实为O(α(n)) ，阿克曼函数
//当 n = 10^80 次方即可探明宇宙原子量，α(n)的返回值也不超过6，那就可以认为是O(1)
//证明难度极大！建议记住即可

//                   --
//                 |    |
//  a -> b -> c -> d -- |


//题目一：
// 并查集模版(牛客)
// 路径压缩 + 小挂大
// 测试链接 : https://www.nowcoder.com/practice/e7ed657974934a30b2010046536a5372
#include <iostream>
using namespace std;
const int MAXSIZE = 1000005;
int n, m;
int stk[MAXSIZE];//栈，扁平化处理时使用
//下标即为结点，数组存放结点信息
//类似用数组模拟链式结构
int father[MAXSIZE];//存放某个节点所在集合的代表结点
int Size[MAXSIZE];//存放某个结点所在集合的集合大小(集合中有几个元素) ，“小挂大”优化时使用

void build() 
{
    for (int i = 0; i <= n; i++) 
    {
        father[i] = i; //一开始每个元素都拥有自己的集合，在自己的集合里只有这个元素自己
        Size[i] = 1;
    }
}

int find(int i) //查找i所在集合的代表元素
{ 
    int top = 0;//栈顶
    while (i != father[i])
    {
        stk[top++] = i;
        i = father[i];
    }

    while (top > 0)//扁平化处理，将 查找a元素所在集合的代表元素 的查找路径上每一个元素都设置成集合代表元素
    { 
        father[stk[--top]] = i;
    }

    return i;
}

bool isSameSet(int a, int b)//查找a,b是否属于同一集合
{ 
    return find(a) == find(b);
}

void Union(int a, int b) //把a所在集合 和 b所在集合 合并到一起
{ 
    int a_F = find(a);//a元素所在集合的代表元素
    int b_F = find(b);//b元素所在集合的代表元素

    if (a_F != b_F)//a,b不在同一集合
    {
        if (Size[a_F] >= Size[b_F])//b集合比a集合小，把b集合挂到a集合上
        {
            
            Size[a_F] += Size[b_F];
            father[b_F] = a_F;
        }
        else {
            Size[b_F] += Size[a_F];
            father[a_F] = b_F;
        }
    }

}

int main() 
{
    // 提高输入输出效率
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;
    build();
    for (int i = 0; i < m; i++)
    {
        int opt, x, y;
        cin >> opt >> x >> y;
        if (opt == 1)
        {
            cout << (isSameSet(x, y) ? "Yes" : "No") << endl;
        }
        else if (opt == 2) 
        {
            Union(x, y);
        }
    }

    return 0;
}



//题目二：
// 并查集模版(洛谷)
// 本实现用递归函数实现路径压缩，而且省掉了小挂大的优化，一般情况下可以省略
// 测试链接 : https://www.luogu.com.cn/problem/P3367
#include <iostream>
using namespace std;
const int MAXSIZE = 200005;
int n, m;
int father[MAXSIZE];

void build()
{
    for (int i = 0; i <= n; i++)
    {
        father[i] = i;
    }
}

int find(int i)
{
    if (i == father[i])
        return i;

    return father[i] = find(father[i]);//注意return
}

bool isSameSet(int a, int b)
{
    return find(a) == find(b);
}

void Union(int a, int b)
{
    //把a所在的集合挂在b所在的集合上
    father[find(a)] = find(b);
}

int main()
{
    // 提高输入输出效率
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;
    build();
    for (int i = 0; i < m; i++)
    {
        int z, x, y;
        cin >> z >> x >> y;
        if (z == 1)
            Union(x, y);
        else if (z == 2)
            cout << (isSameSet(x, y) ? "Y" : "N") << endl;
    }

    return 0;
}



//题目三：
// 情侣牵手
// n对情侣坐在连续排列的 2n 个座位上，想要牵到对方的手
// 人和座位由一个整数数组 row 表示，其中 row[i] 是坐在第 i 个座位上的人的ID
// 情侣们按顺序编号，第一对是 (0, 1)，第二对是 (2, 3)，以此类推，最后一对是 (2n-2, 2n-1)
// 返回 最少交换座位的次数，以便每对情侣可以并肩坐在一起
// 每次交换可选择任意两人，让他们站起来交换座位
// 测试链接 : https://leetcode.cn/problems/couples-holding-hands/
class Solution
{
private:
    static const int MAXSIZE = 35;
    int father[MAXSIZE];//情侣对数,不是单个人
    int set;

    void build(int n)
    {
        for (int i = 0; i <= n; i++)
            father[i] = i;
        set = n;
    }

    int find(int i)
    {
        if (i == father[i])
            return i;
        return father[i] = find(father[i]);
    }

    void Union(int x, int y)
    {
        int fx = find(x);
        int fy = find(y);

        if (fx != fy)
        {
            father[fx] = find(fy);
            set--;
        }
    }

public:
    int minSwapsCouples(vector<int>& row)
    {
        int n = row.size();//总人数
        build(n / 2);
        for (int i = 0; i < n; i += 2)
        {
            //依次检查相邻的两人是否是一对情侣
            //如果是(find方法查出来一样)，则不进行Union方法
            //如果不是，则放入一个集合，进行安排-->>使情侣对正确的操作数 = 集合中的情侣对数 - 1
            Union(row[i] / 2, row[i + 1] / 2);//每个人所在下标除以二即为这个人情侣对的编号
        }

        //操作次数 = 总情侣对数 - 集合数
        return n / 2 - set;
    }
};



//题目四：
// 相似字符串组
// 如果交换字符串 X 中的两个不同位置的字母，使得它和字符串 Y 相等
// 那么称 X 和 Y 两个字符串相似
// 如果这两个字符串本身是相等的，那它们也是相似的
// 例如，"tars" 和 "rats" 是相似的 (交换 0 与 2 的位置)；
// "rats" 和 "arts" 也是相似的，但是 "star" 不与 "tars"，"rats"，或 "arts" 相似
// 总之，它们通过相似性形成了两个关联组：{"tars", "rats", "arts"} 和 {"star"}
// 注意，"tars" 和 "arts" 是在同一组中，即使它们并不相似
// 形式上，对每个组而言，要确定一个单词在组中，只需要这个词和该组中至少一个单词相似。
// 给你一个字符串列表 strs列表中的每个字符串都是 strs 中其它所有字符串的一个字母异位词。
// 返回 strs 中有多少字符串组
// 测试链接 : https://leetcode.cn/problems/similar-string-groups/
//
//本题与题目三相似
//如何判断两个字符串是否相似？依次比对两个字符串中每个字符，如果不相同的字符数为0或2，则两个字符串相似
class Solution 
{
private:
    static const int MAXSIZE = 305;
    int father[MAXSIZE];
    int sets;

    void build(int n)
    {
        for (int i = 0; i < n; i++)
            father[i] = i;
        sets = n;
    }

    int find(int i)
    {
        if (father[i] == i)
            return i;
        return father[i] = find(father[i]);
    }

    void Union(int a, int b)
    {
        int fa = find(a);
        int fb = find(b);

        if (fa != fb)
        {
            father[fa] = find(fb);
            sets--;
        }
    }

    bool isSimiliar(string a, string b, int m)
    {
        int diff = 0;
        for (int i = 0; i < m; i++)
        {
            if (diff > 3)
                return false;

            if (a[i] != b[i])
                diff++;
        }
        return diff == 0 || diff == 2;
    }

public:
    int numSimilarGroups(vector<string>& strs) 
    {
        int n = strs.size();//字符串数目
        int m = strs[0].size();//由于所有字符串都是异构词，因此每个字符串的长度相等

        build(n);
        for (int i = 0; i < n; i++)
        {
            for (int j = i + 1; j < n; j++)
            {
                if (find(i) != find(j))//如果两个字符串不在同一集合
                {
                    string a = strs[i];
                    string b = strs[j];
                    if (isSimiliar(a, b, m))
                    {
                        Union(i, j);
                    }
                }
            }
        }
        return sets;
    }
};
//注：
//在C / C++中，逻辑运算符返回的是布尔值（true 或 false，即 1 或 0）
//0 || 2 的运算过程：
//先计算 0（为假）
//再计算 2（为非零，为真）
//逻辑或的结果为 true，在整数上下文中就是 1
//所以 diff == (0 || 2) 等价于 diff == 1



//题目五：
// 岛屿数量
// 给你一个由 '1'（陆地）和 '0'（水）组成的的二维网格，请你计算网格中岛屿的数量
// 岛屿总是被水包围，并且每座岛屿只能由水平方向和/或竖直方向上相邻的陆地连接形成
// 此外，你可以假设该网格的四条边均被水包围
// 测试链接 : https://leetcode.cn/problems/number-of-islands/
// 
//注：本题还可以用洪水填充算法求解
//技巧：二维数组是一维数组的数组 -->> 二维数组中每个元素都有自己的一维编号 ,就可以使用并查集
class Solution 
{
private:
    static const int MAXSIZE = 100000;
    int father[MAXSIZE];
    int sets;

    int index(int i, int j, int n)
    {
        return i * n + j;
    }

    void build(vector<vector<char>>& grid)
    {
        int m = grid.size();
        int n = grid[0].size();

        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (grid[i][j] == '1')
                {
                    int idx = index(i, j, n);
                    father[idx] = idx;
                    sets++;
                }
            }
        }
    }

    int find(int i)
    {
        if (i == father[i])
            return i;
        return father[i] = find(father[i]);
    }

    void Union(int a, int b)
    {
        int fa = find(a);
        int fb = find(b);
        if (fa != fb)
        {
            father[fa] = fb;
            sets--;
        }
    }
    

public:
    int numIslands(vector<vector<char>>& grid) 
    {
        int m = grid.size();
        int n = grid[0].size();

        build(grid);
        //因为从上往下、从左往右遍历，所以到达每个点时，只需检验上方、左方的元素是否在同一个集合内
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (grid[i][j] == '1')
                {
                    if (i > 0 && grid[i - 1][j] == '1')//检测上方
                        Union(index(i - 1, j, n), index(i, j, n));
                    if (j > 0 && grid[i][j - 1] == '1')//检测左方
                        Union(index(i, j - 1, n), index(i, j, n));
                }
            }
        }
        return sets;
    }
};







//并查集--下
//并查集的小扩展
//可以定制信息：并查集目前有多少个集合，以及给每个集合打上标签信息
//类似于size数组

//注意：
//带权并查集、可持久化并查集、可撤销并查集，都是备战算法竞赛的同学必学的内容


//题目一：
// 移除最多的同行或同列石头
// n 块石头放置在二维平面中的一些整数坐标点上。每个坐标点上最多只能有一块石头
// 如果一块石头的 同行或者同列 上有其他石头存在，那么就可以移除这块石头
// 给你一个长度为 n 的数组 stones ，其中 stones[i] = [xi, yi] 表示第 i 块石头的位置
// 返回 可以移除的石子 的最大数量。
// 测试链接 : https://leetcode.cn/problems/most-stones-removed-with-same-row-or-column/
//
//思路：处在同一行或同一列的石头为一个集合，经过处理后每个集合只有一块石头，完美符合要求
//1：【x轴相同】 or 【y轴相同】 的石头是关联的。
//2：如果石头A与B关联，B与C关联，那么A与C也是关联的，同属于一个关联分组。
//3：同一个关联分组的石头，只会保留一个，剩下的可以全部删除。
//目标结果：石头总数 - 所有分组的数量。
//方法：并查集。
class Solution 
{
private:
    static const int MAXN = 1005;
    static const int MAXSIZE = 10005;
    int father[MAXN];
    //离散化处理每个坐标，数组模拟哈希表
    int Row[MAXSIZE];//第i个石头的行信息: 第x行已经有编号为i个石头了
    int Col[MAXSIZE];//第i个石头的列信息: 第y列已经有编号为i个石头了
    int sets;

    void build(int m)
    {
        for (int i = 0; i < m; i++)
        {
            father[i] = i;
        }

        //我们只需要初始化那些会被用到的部分。
        //但问题在于，我们不知道哪些坐标值会被使用，而且这些坐标值可能在0到10^4之间的任意值。
        //因此干脆直接全部初始化
        for (int i = 0; i < MAXSIZE; i++)
        {
            Row[i] = -1;
            Col[i] = -1;
        }

        //等价写法
        //memset(Row, -1, sizeof(Row));
        //memset(Col, -1, sizeof(Col));

        sets = m;
    }

    int find(int i)
    {
        if (father[i] == i)
            return i;
        return father[i] = find(father[i]);
    }

    void Union(int a, int b)
    {
        int fa = find(a);
        int fb = find(b);
        if (fa != fb)
        {
            father[fa] = fb;
            sets--;
        }
    }

public:
    int removeStones(vector<vector<int>>& stones) 
    {
        int m = stones.size();//一共m个石头
        build(m);
        for (int i = 0; i < m; i++)
        {
            int curRow = stones[i][0];
            int curCol = stones[i][1];
            if (Row[curRow] == -1)
                Row[curRow] = i;
            else
                Union(Row[curRow], i);

            if (Col[curCol] == -1)
                Col[curCol] = i;
            else
                Union(Col[curCol], i);
        }

        return m - sets;//一共移除的石头数量
    }
};



//题目二：
// 找出知晓秘密的所有专家
// 给你一个整数 n ，表示有 n 个专家从 0 到 n - 1 编号
// 另外给你一个下标从 0 开始的二维整数数组 meetings
// 其中 meetings[i] = [xi, yi, timei] 表示专家 xi 和专家 yi 在时间 timei 要开一场会
// 一个专家可以同时参加 多场会议 。最后，给你一个整数 firstPerson
// 专家 0 有一个 秘密 ，最初，他在时间 0 将这个秘密分享给了专家 firstPerson
// 接着，这个秘密会在每次有知晓这个秘密的专家参加会议时进行传播
// 更正式的表达是，每次会议，如果专家 xi 在时间 timei 时知晓这个秘密
// 那么他将会与专家 yi 分享这个秘密，反之亦然。秘密共享是 瞬时发生 的
// 也就是说，在同一时间，一个专家不光可以接收到秘密，还能在其他会议上与其他专家分享
// 在所有会议都结束之后，返回所有知晓这个秘密的专家列表
// 你可以按 任何顺序 返回答案
// 链接测试 : https://leetcode.cn/problems/find-all-people-with-secret/
//
//时间复杂度：O(mlogm+n)
//会议排序：O(mlogm)
//处理答案：O(m)
//收集答案：O(n)
class Solution
{
private:
    static const int MAXSIZE = 100005;
    int father[MAXSIZE];                       
    // 集合的标签信息 : 设置集合的一些属性
    // 属性在哪？secret[代表元素] 代表集合的属性: 是否知道秘密
    bool secret[MAXSIZE];

    static bool cmp(vector<int>& a, vector<int>& b)//作为类成员的非静态函数不能直接用作 sort 的比较函数，需要改为静态函数或lambda表达式。
    {
        return a[2] < b[2];
    }

    void build(int n, int firstPerson)
    {
        for (int i = 0; i < n; i++)
        {
            father[i] = i;
            secret[i] = false;
        }
        secret[0] = true;
        father[firstPerson] = 0;
    }

    int find(int i)
    {
        if (i == father[i])
            return i;
        return father[i] = find(father[i]);
    }

    void Union(int a, int b)
    {
        int fa = find(a);
        int fb = find(b);
        if (fa != fb)
        {
            //把a所在集合挂在b所在集合上
            father[fa] = fb;
            secret[fb] |= secret[fa];//只要有一个集合为true，则都为true
        }
    }


public:
    vector<int> findAllPeople(int n, vector<vector<int>>& meetings, int firstPerson) 
    {
        sort(meetings.begin(), meetings.end(), cmp);
        vector<int> ans;
        build(n, firstPerson);
        int m = meetings.size();

        for (int l = 0, r = 0; l < m; l = r)//bfs，每次处理同一时间开会的专家
        {
            r = l;
            while (r < m && meetings[r][2] == meetings[l][2])//滑动窗口，找到处于同一时间的会议
                r++;

            for (int i = l; i < r; i++)//遍历处于同一时间的“一层”数据   [l,r)
            {
                Union(meetings[i][0], meetings[i][1]);
            }

            //revoke()函数
            //撤销：将除 “知晓秘密”集合 外的所有集合解散
            for (int i = l; i < r; i++)
            {
                int a = meetings[i][0];
                int b = meetings[i][1];

                if (!secret[find(a)])
                    father[a] = a;
                if (!secret[find(b)])
                    father[b] = b;
            }

        }

        for (int i = 0; i < n; i++)
        {
            if (secret[find(i)])
                ans.push_back(i);
        }

        return ans;
    }
};



//题目三：
// 好路径的数目
// 给你一棵 n 个节点的树（连通无向无环的图）
// 节点编号从0到n-1，且恰好有n-1条边
// 给你一个长度为 n 下标从 0 开始的整数数组 vals
// 分别表示每个节点的值。同时给你一个二维整数数组 edges
// 其中 edges[i] = [ai, bi] 表示节点 ai 和 bi 之间有一条 无向 边
// 好路径需要满足以下条件：开始和结束节点的值相同、 路径中所有值都小于等于开始的值
// 请你返回不同好路径的数目
// 注意，一条路径和它反向的路径算作 同一 路径
// 比方说， 0 -> 1 与 1 -> 0 视为同一条路径。单个节点也视为一条合法路径
// 测试链接 : https://leetcode.cn/problems/number-of-good-paths/
//
//思路：
//将相连的两个结点放入同一集合，并记录最大节点及集合中最大节点的数目（最大节点充当集合代表节点）
//若两个集合最大值一样，则收集答案
//为了防止类似“1 -- 2 -- 1”的情况被收集，提前将二维数组按照“相连的两个结点中的最大值”从小到大排序即可

vector<int> help;//将vals数组设置为“全局”变量，便于在不使用lambda 的情况下排序
class Solution
{
private:
    static const int MAXSIZE = 30005;
    int father[MAXSIZE];//规定集合中最大值充当代表节点
    int maxcnt[MAXSIZE];//集合中最大值的个数

    static bool cmp(vector<int>& a, vector<int>& b)
    {
        int maxa = max(help[a[0]], help[a[1]]);
        int maxb = max(help[b[0]], help[b[1]]);
        return maxa < maxb;
    }

    void build(int n)
    {
        for (int i = 0; i < n; i++)
        {
            father[i] = i;
            maxcnt[i] = 1;
        }
    }
    
    int find(int i)
    {
        if (i == father[i])
            return i;
        return father[i] = find(father[i]);
    }

    int Union(int a, int b, vector<int>& vals)
    {
        int fa = find(a);
        int fb = find(b);
        int ans = 0;

        if (vals[fa] > vals[fb])
            father[fb] = fa;
        else if (vals[fa] < vals[fb])
            father[fa] = fb;
        else//收集答案
        {
            ans = maxcnt[fa] * maxcnt[fb];
            father[fa] = fb;//这里把a所在集合挂在b所在集合上，其实效果都一样
            maxcnt[fb] += maxcnt[fa];
        }
        return ans;
    }


public:
    int numberOfGoodPaths(vector<int>& vals, vector<vector<int>>& edges) 
    {
        int n = vals.size();
        build(n);
        help = vals;
        int ans = n;//由于题目的定义，单个节点本身就是一条好路径

        sort(edges.begin(), edges.end(), cmp);
        for (auto& ele : edges)
        {
            ans += Union(ele[0], ele[1], vals);
        }
        return ans;
    }
};



//题目四：
// 尽量减少恶意软件的传播 II
// 给定一个由 n 个节点组成的网络，用 n x n 个邻接矩阵 graph 表示
// 在节点网络中，只有当 graph[i][j] = 1 时，节点 i 能够直接连接到另一个节点 j。
// 一些节点 initial 最初被恶意软件感染。只要两个节点直接连接，
// 且其中至少一个节点受到恶意软件的感染，那么两个节点都将被恶意软件感染。
// 这种恶意软件的传播将继续，直到没有更多的节点可以被这种方式感染。
// 假设 M(initial) 是在恶意软件停止传播之后，整个网络中感染恶意软件的最终节点数。
// 我们可以从 initial 中删除一个节点，
// 并完全移除该节点以及从该节点到任何其他节点的任何连接。
// 请返回移除后能够使 M(initial) 最小化的节点。
// 如果有多个节点满足条件，返回索引 最小的节点 。
// initial 中每个整数都不同
// 测试链接 : https://leetcode.cn/problems/minimize-malware-spread-ii/
//思路：
//将连通且未感染的点放在一个集合，记录与污染结点相连的结点所在的集合（当一个集合有两个及以上的“与污染节点相连的结点”，则该集合直接作废，不参与后续讨论，因为咋救也救不回来）
//讨论：统计 废除一个污染节点时能拯救的结点数 ，拯救结点数最多且索引最小的即为答案
class Solution
{
private:
    static const int MAXSIZE = 305;
    int father[MAXSIZE];//某个元素所在集合的代表结点
    int size[MAXSIZE];//每个集合的大小
    int infect[MAXSIZE];//与某个集合相连的感染节点的下标（用集合中的代表节点记录该信息）
    //infect[i]>=0 -->> 与该集合直接相连的结点为x
    //infect[i]==-1 -->> 该集合没有与其直接相连的感染节点
    //infect[i]==-2 -->> 与该集合直接相连的感染节点数>=2
    int saveCnt[MAXSIZE];//当删除某个感染节点时能够拯救的结点数目
    bool virus[MAXSIZE];//将感染节点信息转化一下，便于查询

    void build(int n, vector<int>& initial)
    {
        for (int i = 0; i < n; i++)
        {
            virus[i] = false;
            father[i] = i;
            size[i] = 1;
            infect[i] = -1;
            saveCnt[i] = 0;
        }
        for (auto& ele : initial)
        {
            virus[ele] = true;
        }
    }

    int find(int i)
    {
        if (i == father[i])
            return i;
        return father[i] = find(father[i]);
    }

    void Union(int a, int b)
    {
        int fa = find(a);
        int fb = find(b);
        if (fa != fb)
        {
            father[fa] = fb;//这里把a所在集合挂在b所在集合，效果都一样
            size[fb] += size[fa];
        }
    }

public:
    int minMalwareSpread(vector<vector<int>>& graph, vector<int>& initial)
    {
        int n = graph.size();
        build(n, initial);

        for (int i = 0; i < n; i++)//将正常结点合并到一个集合里
        {
            for (int j = i + 1; j < n; j++)
            {
                if (graph[i][j] == 1 && !virus[i] && !virus[j])
                {
                    Union(i, j);
                }
            }
        }

        for (auto& sick : initial)
        {
            for (int adjnode = 0; adjnode < n; adjnode++)
            {
                if (adjnode != sick && graph[sick][adjnode] == 1 && !virus[adjnode])//相邻节点必须不为感染节点中的一员
                {
                    int fn = find(adjnode);
                    if ((infect[fn] >= 0 && infect[fn] != sick) || infect[fn] == -2)
                        infect[fn] = -2;
                    else if (infect[fn] == -1)
                    {
                        infect[fn] = sick;
                    }
                }
            }
        }

        // 统计拯救数据
        for (int i = 0; i < n; i++)
        {
            // 不是代表点，不看
            if (i == find(i) && infect[i] >= 0)
            {
                saveCnt[infect[i]] += size[i];
            }
        }

        sort(initial.begin(), initial.end());//当答案相同时收集下标较小的那个
        int ans = initial[0];
        int maxs = saveCnt[initial[0]];
        for (auto& ele : initial)
        {
            if (saveCnt[ele] > maxs)
            {
                ans = ele;
                maxs = saveCnt[ele];
            }

        }
        return ans;
    }
};
//值得注意的细节：
//1.答案的初始值应为initial数组中的一个值，而不是0
//2.maxs值的设置应该放在循环外，否则每次循环都是一个新值

