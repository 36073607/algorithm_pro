#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <climits>
using namespace std;



//尝试函数有1个可变参数可以完全决定返回值，进而可以改出1维动态规划表的实现
//同理：
//尝试函数有2个可变参数可以完全决定返回值，那么就可以改出2维动态规划表的实现

//一维、二维、三维甚至多维动态规划问题，大体过程都是：
//写出尝试递归
//记忆化搜索（从顶到底的动态规划）
//严格位置依赖的动态规划（从底到顶的动态规划）
//空间、时间的更多优化


//动态规划表的大小：每个可变参数的可能性数量相乘
//动态规划方法的时间复杂度：动态规划表的大小 * 每个格子的枚举代价

//二维动态规划依然需要去整理 动态规划表的格子之间的依赖关系
//找寻依赖关系，往往 通过画图来建立空间感，使其更显而易见
//然后依然是 从简单格子填写到复杂格子 的过程，即严格位置依赖的动态规划（从底到顶）

//二维动态规划的压缩空间，原理不难，会了之后千篇一律
//但是不同题目依赖关系不一样，需要 很细心的画图来整理具体题目的依赖关系
//最后进行空间压缩的实现


//能改成动态规划的递归，统一特征：
//决定返回值的可变参数类型(可变函数入参类型)往往都比较简单，一般不会比int类型更复杂
//
//从这个角度，可以解释 带路径的递归（可变参数类型复杂），不适合或者说没有必要改成动态规划
//题目二就是说明这一点的：
//由于递归过程中为了避免重复讨论某一字符，因此做了 先设置为空字符再回溯 的操作，所以递归函数传递的参数还包括二维字符串数组，属于复杂类型


//不管几维动态规划
//经常从递归的定义出发，避免后续进行很多边界讨论
//这需要一定的经验来预知



//题目一：
// 最小路径和
// 给定一个包含非负整数的 m x n 网格 grid
// 请找出一条从左上角到右下角的路径，使得路径上的数字总和为最小。
// 说明：每次只能向下或者向右移动一步。
// 测试链接 : https://leetcode.cn/problems/minimum-path-sum/
//
//法一：Dijkstra
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
    int minPathSum(vector<vector<int>>& grid)
    {
        int m = grid.size();
        int n = grid[0].size();
        vector<vector<int>> move = { {1,0},{0,1} };

        vector<vector<int>> distance(m, vector<int>(n, INT_MAX));
        priority_queue<vector<int>, vector<vector<int>>, cmp> heap;
        vector<vector<int>> visited(m, vector<int>(n, false));

        //处理源点(0,0)
        distance[0][0] = grid[0][0];
        heap.push({ 0, 0, distance[0][0] });
        while (!heap.empty())
        {
            int curx = heap.top()[0];
            int cury = heap.top()[1];
            int curval = heap.top()[2];
            heap.pop();

            if (visited[curx][cury])
                continue;
            visited[curx][cury] = true;

            if (curx == m - 1 && cury == n - 1)
                return distance[curx][cury];

            for (auto& ele : move)
            {
                int dx = curx + ele[0];
                int dy = cury + ele[1];
                if (dx >= 0 && dx <= m - 1 && dy >= 0 && dy <= n - 1)
                {
                    if (!visited[dx][dy] && distance[curx][cury] + grid[dx][dy] < distance[dx][dy])
                    {
                        distance[dx][dy] = distance[curx][cury] + grid[dx][dy];
                        heap.push({ dx,dy,distance[dx][dy] });
                    }
                }
            }
        }
        return -1;
    }
};

//法二：暴力递归，超时
class Solution
{
public:
    //递归函数这样定义：
    //从(0,0)到(i,j)的最小路径和
    //每次只能向右或向下
    //则我们从终点依次倒推即可
    int f(vector<vector<int>>& grid, int i, int j)
    {
        if (i == 0 && j == 0)
            return grid[0][0];

        int up = INT_MAX;
        int left = INT_MAX;
        if (i - 1 >= 0)
            up = f(grid, i - 1, j);
        if (j - 1 >= 0)
            left = f(grid, i, j - 1);
        return min(up, left) + grid[i][j];
    }

    int minPathSum(vector<vector<int>>& grid) 
    {
        return f(grid, grid.size() - 1, grid[0].size() - 1);
    }
};

//法三：记忆化搜索
class Solution 
{
public:
    int f(vector<vector<int>>& grid, int i, int j, vector<vector<int>>& dp)
    {
        if (i == 0 && j == 0)
            return grid[0][0];

        if (dp[i][j] != -1)
            return dp[i][j];

        int up = INT_MAX;
        int left = INT_MAX;
        if (i - 1 >= 0) up = f(grid, i - 1, j, dp);
        if (j - 1 >= 0) left = f(grid, i, j - 1, dp);
        dp[i][j] = min(up, left) + grid[i][j];
        return dp[i][j];
    }

    int minPathSum(vector<vector<int>>& grid)
    {
        int m = grid.size();
        int n = grid[0].size();
        vector<vector<int>> dp(m, vector<int>(n, -1));
        return f(grid, m - 1, n - 1, dp);
    }
};

//法四：动态规划
class Solution
{
public:
    int minPathSum(vector<vector<int>>& grid)
    {
        int m = grid.size();
        int n = grid[0].size();
        vector<vector<int>> dp(m, vector<int>(n, -1));

        //dp数组初始化
        dp[0][0] = grid[0][0];
        for (int i = 1; i < m; i++)
            dp[i][0] = dp[i - 1][0] + grid[i][0];
        for (int j = 1; j < n; j++)
            dp[0][j] = dp[0][j - 1] + grid[0][j];
        //正式开始动态规划
        for (int i = 1; i < m; i++)
        {
            for (int j = 1; j < n; j++)
            {
                dp[i][j] = min(dp[i - 1][j], dp[i][j - 1]) + grid[i][j];
            }
        }
        return dp[m - 1][n - 1];
    }
};

//法五：动态规划+空间压缩
class Solution 
{
public:
    int minPathSum(vector<vector<int>>& grid)
    {
        int m = grid.size();
        int n = grid[0].size();
        vector<int> dp(n);
        dp[0] = grid[0][0];
        for (int j = 1; j < n; j++)
        {
            dp[j] = dp[j - 1] + grid[0][j];
        }

        for (int i = 1; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (j == 0)
                    dp[0] = dp[0] + grid[i][j];
                else
                {
                    dp[j] = min(dp[j - 1], dp[j]) + grid[i][j];
                }
            }
        }
        return dp[n - 1];
    }
};



//题目二：
// 单词搜索（无法改成动态规划）
// 给定一个 m x n 二维字符网格 board 和一个字符串单词 word
// 如果 word 存在于网格中，返回 true ；否则，返回 false 。
// 单词必须按照字母顺序，通过相邻的单元格内的字母构成
// 其中"相邻"单元格是那些水平相邻或垂直相邻的单元格
// 同一个单元格内的字母不允许被重复使用
// 测试链接 : https://leetcode.cn/problems/word-search/
class Solution
{
public:
    //递归定义：
    //从(i,j)出发，此时遍历到目标字符串第k个字符，继续搜索看是否能走完目标字符
    bool f(vector<vector<char>>& board, int i, int j, string& word, int k)
    {
        if (k == word.size())//base case
            return true;

        if (i < 0 || i >= board.size() || j < 0 || j >= board[0].size() || word[k] != board[i][j])
            return false;

        //board[i][j]与word[k]匹配上了
        board[i][j] = '0';//先设置为空，避免重复访问
        bool ans = f(board, i - 1, j, word, k + 1) 
                || f(board, i + 1, j, word, k + 1)
                || f(board, i, j - 1, word, k + 1)
                || f(board, i, j + 1, word, k + 1);
        board[i][j] = word[k];
        return ans;
    }

    bool exist(vector<vector<char>>& board, string word)
    {
        int m = board.size();
        int n = board[0].size();
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (f(board, i, j, word, 0))
                    return true;
            }
        }
        return false;
    }
};



//题目三：
// 最长公共子序列
// 给定两个字符串text1和text2
// 返回这两个字符串的最长 公共子序列 的长度
// 如果不存在公共子序列，返回0
// 两个字符串的 公共子序列 是这两个字符串所共同拥有的子序列
// 测试链接 : https://leetcode.cn/problems/longest-common-subsequence/
//
//核心思想：以结尾讨论可能性

//法一：暴力递归
class Solution 
{
public:
    //递归定义：
    //求s1[0,...,i1]与s2[0,...,i2]最长公共子序列长度
    //以结尾讨论可能性，这样，就有四种可能：
    //1.s1[i1]与s2[i2]都不要，再对s1[0,...,i1-1]与s2[0,...,i2-1]调用递归
    //2.不要s1[i1]，讨论s1[0,...,i1-1]与s2[0,...,i2]
    //3.不要s2[i2]，讨论s1[0,...,i1]与s2[0,...,i2-1]
    //4.s1[i1]==s2[i2]，讨论s1[0,...,i1-1]与s2[0,...,i2-1]，最后答案再+1
    int f(string& s1, string& s2, int i1, int i2)
    {
        if (i1 < 0 || i2 < 0)//base case
            return 0;

        int p1 = f(s1, s2, i1 - 1, i2 - 1);
        int p2 = f(s1, s2, i1, i2 - 1);
        int p3 = f(s1, s2, i1 - 1, i2);
        int p4 = s1[i1] == s2[i2] ? (1 + p1) : 0;
        return max(max(p1, p2), max(p3, p4));
    }

    int longestCommonSubsequence(string text1, string text2)
    {
        int m = text1.size();
        int n = text2.size();
        return f(text1, text2, m - 1, n - 1);
    }
};

// 为了避免很多边界讨论
// 很多时候往往不用下标来定义尝试，而是用长度来定义尝试
// 因为长度最短是0，而下标越界的话讨论起来就比较麻烦
class Solution 
{
public:
    //递归定义：s1 (len1)长度内与s2 (len2)长度内的最长公共子序列长度
    //以结尾讨论可能性（优化）
    //1.s1[len1-1]==s2[len2-1] ，讨论s1 (len1-1)长度与s2 (len2-1)长度 ，最后加上1
    //2.s1[len1-1]!=s2[len2-1] ，讨论s1 (len1)长度与s2 (len2-1)长度 
    //                             和s1 (len1-1)长度与s2 (len)长度
    int f(string& s1, string& s2, int len1, int len2)
    {
        if (len1 == 0 || len2 == 0)
            return 0;

        if (s1[len1 - 1] == s2[len2 - 1])
            return 1 + f(s1, s2, len1 - 1, len2 - 1);
        else
            return max(f(s1, s2, len1, len2 - 1), f(s1, s2, len1 - 1, len2));
    }

    int longestCommonSubsequence(string text1, string text2)
    {
        int m = text1.size();
        int n = text2.size();
        return f(text1, text2, m, n);
    }
};

//法二：记忆化搜索
class Solution 
{
public:
    int f(string& s1, string& s2, int len1, int len2, vector<vector<int>>& dp)
    {
        if (len1 == 0 || len2 == 0)
            return 0;

        if (dp[len1][len2] != -1)
            return dp[len1][len2];

        if (s1[len1 - 1] == s2[len2 - 1])
        {
            dp[len1][len2] = 1 + f(s1, s2, len1 - 1, len2 - 1, dp);
        }
        else
        {
            dp[len1][len2] = max(f(s1, s2, len1 - 1, len2, dp), f(s1, s2, len1, len2 - 1, dp));
        }
        return dp[len1][len2];
    }

    int longestCommonSubsequence(string text1, string text2) 
    {
        int m = text1.size();
        int n = text2.size();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, -1));
        return f(text1, text2, m, n, dp);
    }
};

//法三：动态规划
class Solution 
{
public:
    int longestCommonSubsequence(string text1, string text2)
    {
        int m = text1.size();
        int n = text2.size();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
        //dp[0][0] = 0, dp[0][1] = 0, dp[1][0] = 0;
        for (int len1 = 1; len1 <= m; len1++)
        {
            for (int len2 = 1; len2 <= n; len2++)
            {
                if (text1[len1 - 1] == text2[len2 - 1])
                    dp[len1][len2] = 1 + dp[len1 - 1][len2 - 1];
                else
                    dp[len1][len2] = max(dp[len1 - 1][len2], dp[len1][len2 - 1]);
            }
        }
        return dp[m][n];
    }
};

//法四：空间压缩动态规划
class Solution
{
public:
    int longestCommonSubsequence(string text1, string text2)
    {
        int m, n;
        // 为了保证 text1 是短字符串，text2 是长字符串，方便统一处理
        // 如果 text1 比 text2 长，则交换它们，确保 m 是短串长度，n 是长串长度
        if (text1.size() > text2.size())
        {
            swap(text1, text2);
        }

        m = text1.size(); // 短
        n = text2.size(); // 长
        vector<int> dp(m + 1, 0);

        for (int len2 = 1; len2 <= n; len2++) // 外层遍历长串 text2
        {
            int leftup = 0; // 记录 dp[i-1][j-1]
            for (int len1 = 1; len1 <= m; len1++) // 内层遍历短串 text1
            {
                int backup = dp[len1]; // 备份当前 dp[len1]，它将在下一次迭代中成为 leftup
                if (text1[len1 - 1] == text2[len2 - 1])
                {
                    dp[len1] = 1 + leftup;
                }
                else
                {
                    dp[len1] = max(dp[len1], dp[len1 - 1]);
                }
                leftup = backup;
            }
        }
        return dp[m];
    }
};



//题目四：
// 最长回文子序列
// 给你一个字符串 s ，找出其中最长的回文子序列，并返回该序列的长度
// 测试链接 : https://leetcode.cn/problems/longest-palindromic-subsequence/

//注：这题也可用两个字符串的最长公共子序列长度的思路来解，str, str.reverse()

//法一：暴力递归
class Solution
{
public:
    //递归定义：求s[l,...,r]范围内的最长回文子序列长度
    //则有：
    //1.s[l]==s[r]，进一步讨论s[l+1,...,r-1]
    //2.s[l]!=s[r]，则讨论三种情况：
    //  1) s[l,...,r-1]
    //  2) s[l+1,...,r]
    //  3) s[l+1,...,r-1] -->> 不过，这个分支求出来的答案是1)、2)分支的子集，可以不用写这个分支
    int f(string& s, int l, int r)
    {
        //可以把这个判断换成base case2: 
        //if(l==r+1) return s[l]==s[r]?2:1;
        if (l > r)
            return 0;

        if (l == r)//base case
            return 1;

        if (s[l] == s[r])
            return 2 + f(s, l + 1, r - 1);
        else
            return max(f(s, l, r - 1), f(s, l + 1, r));
    }

    int longestPalindromeSubseq(string s) 
    {
        int n = s.size();
        return f(s, 0, n - 1);
    }
};

//法二：记忆化搜索
class Solution 
{
public:
    int f(string& s, int l, int r, vector<vector<int>>& dp)
    {
        if (l > r) return 0;
        if (l == r) return 1;

        if (dp[l][r] != -1)
            return dp[l][r];

        if (s[l] == s[r])
            dp[l][r] = 2 + f(s, l + 1, r - 1, dp);
        else
            dp[l][r] = max(f(s, l + 1, r, dp), f(s, l, r - 1, dp));
        return dp[l][r];
    }

    int longestPalindromeSubseq(string s) 
    {
        int n = s.size();
        vector<vector<int>> dp(n, vector<int>(n, -1));
        return f(s, 0, n - 1, dp);
    }
};

//法三：动态规划
class Solution 
{
public:
    int longestPalindromeSubseq(string s)
    {
        int n = s.size();
        vector<vector<int>> dp(n, vector<int>(n, 0));
        for (int l = n - 1; l >= 0; l--)
        {
            for (int r = l; r <= n - 1; r++)//l <= r
            {
                if (l == r)
                    dp[l][r] = 1;
                else
                {
                    if (s[l] == s[r])
                        dp[l][r] = 2 + dp[l + 1][r - 1];
                    else
                        dp[l][r] = max(dp[l + 1][r], dp[l][r - 1]);
                }
            }
        }
        return dp[0][n - 1];
    }
};

//法四：动态规划+空间压缩
class Solution
{
public:
    int longestPalindromeSubseq(string s)
    {
        int n = s.size();
        vector<int> dp(n, 0);
        for (int l = n - 1; l >= 0; l--)
        {
            int leftdown = 0;//记录左下角点的值
            int backup = 0;//临时储存
            for (int r = l; r <= n - 1; r++)
            {
                backup = dp[r];
                if (l == r)
                    dp[r] = 1;
                else
                {
                    if (s[l] == s[r])
                        dp[r] = 2 + leftdown;
                    else
                        dp[r] = max(dp[r - 1], dp[r]);
                }
                leftdown = backup;
            }
        }
        return dp[n - 1];
    }
};



//题目五：
// 节点数为n高度不大于m的二叉树个数
// 现在有n个节点，计算出有多少个不同结构的二叉树
// 满足节点个数为n且树的高度不超过m的方案
// 因为答案很大，所以答案需要模上1000000007后输出
// 测试链接 : https://www.nowcoder.com/practice/aaefe5896cce4204b276e213e725f3ea

//核心：二叉树根据头结点的不同，将左右孩子分局头结点两侧讨论

//法一：记忆化搜索
//递归定义：给出节点数和不能超过的最高高度，问有多少种结构的数量
#include <iostream>
#include <vector>
using namespace std;

const int MOD = 1e9 + 7;

int f1(int n, int m, vector<vector<int>>& dp)
{
    if (n == 0)//空树，算一种结构
        return 1;

    if (m == 0)
        return 0;

    if (dp[n][m] != -1)
        return dp[n][m];

    long ans = 0;
    for (int lchildNum = 0; lchildNum < n; lchildNum++)
    {
        int rchildNum = n - 1 - lchildNum;
        ans = (ans + ((long)f1(lchildNum, m - 1, dp) * f1(rchildNum, m - 1, dp)) % MOD) % MOD;
    }
    dp[n][m] = ans;
    return (int)ans;
}

int main()
{
    int n, m;
    cin >> n >> m;
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, -1));
    cout << f1(n, m, dp);

    return 0;
}

//法二：动态规划
#include <iostream>
#include <vector>
using namespace std;
const int MOD = 1e9 + 7;
int main()
{
    int n, m;
    cin >> n >> m;
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    for (int j = 0; j <= m; j++)//初始化
        dp[0][j] = 1;

    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= m; j++)
        {
            for (int k = 0; k < i; k++)
                dp[i][j] = (dp[i][j] + ((long)dp[k][j - 1] * dp[i - 1 - k][j - 1]) % MOD) % MOD;
        }
    }
    cout << dp[n][m];

    return 0;
}

//法三：动态规划+空间压缩
//法二当中dp表是(n+1)行*(m+1)列，且填格子时任意一个格子需要其左上列的格子
//因此压缩后的dp表大小为(n+1)，模拟“一列”，且由最下方到最上方填格子
#include <iostream>
#include <vector>
using namespace std;
const int MOD = 1e9 + 7;
int main()
{
    int n, m;
    cin >> n >> m;
    vector<int> dp(n + 1, 0);//竖着摆放，模拟“一列”
    //初始化，之后的填格子不必再填dp[0]，因为0节点都为空树
    dp[0] = 1;//0节点为空树，算一种结构
    for (int j = 1; j <= m; j++)//dp表模拟“一列”，从左至右移动，故先按列遍历
    {
        for (int i = n; i >= 1; i--)//依照法二，从下到上装填每个格子
        {
            dp[i] = 0; //每一轮高度循环，需要重新计算方案数
            for (int k = 0; k < i; k++)
            {
                dp[i] = (dp[i] + ((long)dp[k] * dp[i - 1 - k]) % MOD) % MOD;
            }
        }
    }
    cout << dp[n];
    return 0;
}



//题目六：
// 矩阵中的最长递增路径
// 给定一个 m x n 整数矩阵 matrix ，找出其中 最长递增路径 的长度
// 对于每个单元格，你可以往上，下，左，右四个方向移动
// 你 不能 在 对角线 方向上移动或移动到 边界外（即不允许环绕）
// 测试链接 : https://leetcode.cn/problems/longest-increasing-path-in-a-matrix/
//
//记忆化搜索，本题不好用动态规划：依赖关系整理起来很棘手
class Solution {
public:
    // 递归定义：来到(i,j)，求当前位置的最长递增路径
    int f(const vector<vector<int>>& matrix, int i, int j, vector<vector<int>>& dp) 
    {
        if (dp[i][j] != -1)
            return dp[i][j];

        int next = 0;
        if (i + 1 <= matrix.size() - 1 && matrix[i + 1][j] > matrix[i][j])
            next = max(next, f(matrix, i + 1, j, dp));
        if (i - 1 >= 0 && matrix[i - 1][j] > matrix[i][j])
            next = max(next, f(matrix, i - 1, j, dp));
        if (j + 1 <= matrix[0].size() - 1 && matrix[i][j + 1] > matrix[i][j])
            next = max(next, f(matrix, i, j + 1, dp));
        if (j - 1 >= 0 && matrix[i][j - 1] > matrix[i][j])
            next = max(next, f(matrix, i, j - 1, dp));

        dp[i][j] = 1 + next;
        return dp[i][j];
    }

    int longestIncreasingPath(vector<vector<int>>& matrix)
    {
        int m = matrix.size();
        int n = matrix[0].size();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, -1));//这里dp表设置大了一点，因为f函数中对dp表if判断时没有检测是否越界
        int ans = 0;
        for (int i = 0; i < m; i++) 
        {
            for (int j = 0; j < n; j++) 
            {
                ans = max(ans, f(matrix, i, j, dp));
            }
        }
        return ans;
    }
};

