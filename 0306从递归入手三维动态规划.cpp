#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
using namespace std;




//尝试函数有1个可变参数可以完全决定返回值，进而可以改出1维动态规划表的实现
//同理
//尝试函数有2个可变参数可以完全决定返回值，进而可以改出2维动态规划表的实现
//同理
//尝试函数有3个可变参数可以完全决定返回值，进而可以改出3维动态规划表的实现


//大体过程都是：
//写出尝试递归
//记忆化搜索（从顶到底的动态规划）
//严格位置依赖的动态规划（从底到顶的动态规划）
//空间、时间的更多优化



//题目一：
// 一和零(多维费用背包)
// 给你一个二进制字符串数组 strs 和两个整数 m 和 n
// 请你找出并返回 strs 的最大子集的长度
// 该子集中 最多 有 m 个 0 和 n 个 1
// 如果 x 的所有元素也是 y 的元素，集合 x 是集合 y 的 子集
// 测试链接 : https://leetcode.cn/problems/ones-and-zeroes/
//
//这题其实本质上就是一个零一背包的问题，只不过背包容量是由两个参数界定的

//法一：暴力递归
class Solution 
{
private:
    int zeros, ones;

    void computeZerosAndOnes(string& s)
    {
        zeros = ones = 0;
        for (auto& ch : s)
        {
            if (ch == '0') zeros++;
            if (ch == '1') ones++;
        }
    }

public:
    //递归1定义：来到第i个字符串，1.要当前字符串，记录当前字符串0/1数量，调用下一轮(要求的0/1数量减少)  2.不要当前字符串
    // strs[i...strs.size()-1]自由选择，希望0的数量不超过z、1的数量不超过o
    // 最多能选多少个字符串
    int f1(vector<string>& strs, int i, int m, int n)
    {
        if (i == strs.size())
            return 0;

        int ans = 0;
        //1.不要当前字符串
        ans = f1(strs, i + 1, m, n);
        //2.要当前字符串
        computeZerosAndOnes(strs[i]);
        if (zeros <= m && ones <= n)
            ans = max(ans, 1 + f1(strs, i + 1, m - zeros, n - ones));
        return ans;
    }

    //递归2定义：在strs[0,..i]选字符串，1.选末位字符串strs[i]  2.不选末位字符串strs[i]
    // strs[0,...,i]自由选择，希望0的数量不超过z，1的数量不超过o
    // 最多能选多少个字符串
    int f2(vector<string>& strs, int i, int m, int n)
    {
        if(i < 0) return 0;

        int ans = 0;
        ans = f2(strs, i - 1, m, n);
        computeZerosAndOnes(strs[i]);
        if (zeros <= m && ones <= n)
            ans = max(ans, 1 + f2(strs, i - 1, m - zeros, n - ones));
        return ans;
    }

    int findMaxForm(vector<string>& strs, int m, int n) 
    {
        return f1(strs, 0, m, n);
        //return f2(strs,strs.size()-1,m,n);
    }
};

//法二：记忆化搜索
class Solution 
{
private:
    int zeros, ones;

    void computeZerosAndOnes(string& s)
    {
        zeros = ones = 0;
        for (auto& ch : s)
        {
            if (ch == '0') zeros++;
            if (ch == '1') ones++;
        }
    }

public:
    int f(vector<string>& strs, int i, int m, int n, vector<vector<vector<int>>>& dp)
    {
        if (i == strs.size()) return 0;

        if (dp[i][m][n] != -1) return dp[i][m][n];

        int ans = 0;
        ans = f(strs, i + 1, m, n, dp);
        computeZerosAndOnes(strs[i]);
        if (zeros <= m && ones <= n)
            ans = max(ans, 1 + f(strs, i + 1, m - zeros, n - ones, dp));
        dp[i][m][n] = ans;
        return ans;
    }

    int findMaxForm(vector<string>& strs, int m, int n) 
    {
        int len = strs.size();
        vector<vector<vector<int>>> dp(len, vector<vector<int>>(m + 1, vector<int>(n + 1, -1)));
        return f(strs, 0, m, n, dp);
    }
};

//法三：动态规划
class Solution 
{
private:
    int zeros, ones;

    void computeZerosAndOnes(string& s)
    {
        zeros = ones = 0;
        for (auto& ch : s)
        {
            if (ch == '0') zeros++;
            if (ch == '1') ones++;
        }
    }

public:
    int findMaxForm(vector<string>& strs, int m, int n)
    {
        //依照递归1的思路，每个格子依赖“上一层”的内容（三维），所以从上层往下层填
        int len = strs.size();
        vector<vector<vector<int>>> dp(len + 1, vector<vector<int>>(m + 1, vector<int>(n + 1, 0)));
        //根据递归1中base case，最上层全为0：dp[len][][] = 0
        for (int i = len - 1; i >= 0; i--)
        {
            computeZerosAndOnes(strs[i]);
            for (int z = 0; z <= m; z++)
            {
                for (int o = 0; o <= n; o++)
                {
                    int p1 = dp[i + 1][z][o];
                    int p2 = 0;
                    if (zeros <= z && ones <= o)
                        p2 = 1 + dp[i + 1][z - zeros][o - ones];
                    dp[i][z][o] = max(p1, p2);
                }
            }
        }
        return dp[0][m][n];
    }
};

//法四：动态规划+空间压缩
class Solution 
{
private:
    int zeros, ones;

    void computeZerosAndOnes(string& s)
    {
        zeros = ones = 0;
        for (auto& ch : s)
        {
            if (ch == '0') zeros++;
            if (ch == '1') ones++;
        }
    }

public:
    int findMaxForm(vector<string>& strs, int m, int n) 
    {
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
        //根据法三动态规划的状态转移方程：
        //每个格子需要上一层的当前位置格子及上一层的当前位置左上方的格子
        //因此先填最高层，每一层中从右往左（列）、从下往上（行）填即可
        for (auto& s : strs)
        {
            computeZerosAndOnes(s);
            //优化：在循环时就已经有了 -> if (zeros <= m && ones <= n)
            for (int z = m; z >= zeros; z--)
            {
                for (int o = n; o >= ones; o--)
                {
                    dp[z][o] = max(dp[z][o], 1 + dp[z - zeros][o - ones]);
                }
            }
        }
        return dp[m][n];
    }
};



//题目二：
// 盈利计划(多维费用背包)
// 集团里有 n 名员工，他们可以完成各种各样的工作创造利润
// 第 i 种工作会产生 profit[i] 的利润，它要求 group[i] 名成员共同参与
// 如果成员参与了其中一项工作，就不能参与另一项工作
// 工作的任何至少产生 minProfit 利润的子集称为 盈利计划
// 并且工作的成员总数最多为 n
// 有多少种计划可以选择？因为答案很大，答案对 1000000007 取模
// 测试链接 : https://leetcode.cn/problems/profitable-schemes/

//法一：记忆化搜索(我自己写的)
//递归定义：[0,...,i]个任务中，现有金额为sum，现有人员为n，求这种状态下能有几种方案
class Solution
{
private:
    static const int MOD = 1e9 + 7;
public:
    int f(vector<int>& group, vector<int>& profit, int minProfit, int i, int n, int sum, vector<vector<vector<long>>>& dp)
    {
        if (i < 0) return sum >= minProfit ? 1 : 0;
        if (n <= 0) return sum >= minProfit ? 1 : 0;

        if (sum > minProfit) sum = minProfit;//优化内存

        if (dp[i][n][sum] != -1) return dp[i][n][sum];

        long ans1 = f(group, profit, minProfit, i - 1, n, sum, dp);
        long ans2 = 0;
        if (n >= group[i])
            ans2 = f(group, profit, minProfit, i - 1, n - group[i], sum + profit[i], dp);

        dp[i][n][sum] = (ans1 + ans2) % MOD;
        return dp[i][n][sum];
    }

    int profitableSchemes(int n, int minProfit, vector<int>& group, vector<int>& profit)
    {
        int m = profit.size();
        vector<vector<vector<long>>> dp(m, vector<vector<long>>(n + 1, vector<long>(minProfit + 1, -1)));
        return f(group, profit, minProfit, m - 1, n, 0, dp);
    }
};




//题目三：
// 骑士在棋盘上的概率
// n * n的国际象棋棋盘上，一个骑士从单元格(row, col)开始，并尝试进行 k 次移动
// 行和列从0开始，所以左上单元格是 (0,0)，右下单元格是 (n-1, n-1)
// 象棋骑士有8种可能的走法。每次移动在基本方向上是两个单元格，然后在正交方向上是一个单元格
// 每次骑士要移动时，它都会随机从8种可能的移动中选择一种，然后移动到那里
// 骑士继续移动，直到它走了 k 步或离开了棋盘
// 返回 骑士在棋盘停止移动后仍留在棋盘上的概率 
// 测试链接 : https://leetcode.cn/problems/knight-probability-in-chessboard/

//法一：记忆化搜索
//dp[i][j][k]: 当前来到(i, j)，还能走k步，问能留在棋盘上的概率
class Solution
{
public:
    double f(int n, int i, int j, int k, vector<vector<vector<double>>>& dp)
    {
        if (i < 0 || i >= n || j < 0 || j >= n) return 0;
        if (k == 0) return 1;

        if (dp[i][j][k] != -1) return dp[i][j][k];
        double ans = 0.0;
        ans += f(n, i - 1, j - 2, k - 1, dp) / 8;
        ans += f(n, i - 1, j + 2, k - 1, dp) / 8;
        ans += f(n, i - 2, j - 1, k - 1, dp) / 8;
        ans += f(n, i - 2, j + 1, k - 1, dp) / 8;
        ans += f(n, i + 1, j - 2, k - 1, dp) / 8;
        ans += f(n, i + 1, j + 2, k - 1, dp) / 8;
        ans += f(n, i + 2, j - 1, k - 1, dp) / 8;
        ans += f(n, i + 2, j + 1, k - 1, dp) / 8;
        dp[i][j][k] = ans;
        return ans;
    }

    double knightProbability(int n, int k, int row, int column)
    {
        vector<vector<vector<double>>> dp(n, vector<vector<double>>(n, vector<double>(k + 1, -1)));
        return f(n, row, column, k, dp);
    }
};

//法二：动态规划
class Solution 
{
public:
    double knightProbability(int n, int k, int row, int column) 
    {
        vector<vector<int>> move = { {-1,-2},{-1,2},{-2,-1},{-2,1},{1,-2},{1,2},{2,-1},{2,1} };
        //注意为了方便初始化，dp表参数顺序变了一下
        //为了防止越界,边界由 0~n-1 调整为 2~n+1
        //  ->注意：为了防止在边界处move导致越界，数组应开n+4大小
        vector<vector<vector<double>>> dp(k + 1, vector<vector<double>>(n + 4, vector<double>(n + 4, 0)));
        //初始化：
        for (int i = 2; i <= n + 1; i++)
        {
            for (int j = 2; j <= n + 1; j++)
            {
                dp[0][i][j] = 1;
            }
        }

        for (int step = 1; step <= k; step++)
        {
            for (int i = 2; i <= n + 1; i++)
            {
                for (int j = 2; j <= n + 1; j++)
                {
                    double tmp = 0;
                    for (auto& ele : move)
                    {
                        int dx = i + ele[0];
                        int dy = j + ele[1];
                        tmp += dp[step - 1][dx][dy] / 8;
                    }
                    dp[step][i][j] = tmp;
                }
            }
        }
        return dp[k][row + 2][column + 2];
    }
};



//题目四：
// 矩阵中和能被 K 整除的路径
// 给一个下标从0开始的 n * m 整数矩阵 grid 和一个整数 k
// 从起点(0,0)出发，每步只能往下或者往右，你想要到达终点(m-1, n-1)
// 请你返回路径和能被 k 整除的路径数目
// 答案对 1000000007 取模
// 测试链接 : https://leetcode.cn/problems/paths-in-matrix-whose-sum-is-divisible-by-k/
//
// 核心技巧其实是同余原理：如果把第三个参数设置为当前路径和，则会爆内存；而利用同余原理。第三个参数设置为 当前位置的数&k=r，r是否是我希望得到的余数
// 其他定义：试了下这里直接拿 (累加和 % k) 当第三个参数也过了
// 
//法一：记忆化搜索
class Solution 
{
private:
    static const int MOD = 1e9 + 7;

public:
    //递归定义：当前位置是(i, j)，当前位置我希望%k得到的余数r，问到终点有多少条路径
    int f(vector<vector<int>>& grid, int k, int i, int j, int r, vector<vector<vector<int>>>& dp)
    {
        if (i == grid.size() - 1 && j == grid[0].size() - 1) return grid[i][j] % k == r ? 1 : 0;

        if (dp[i][j][r] != -1) return dp[i][j][r];

        // 我们需要 (当前值 + 后续路径和) % k == r
        // 即 (后续路径和) % k == (r - 当前值 % k + k) % k
        //
        //当然，不写这个公式，用if判断也可以：
        //1.cur<=r: r-cur
        //2.cur>r:  r-cur+k
        int need = (r - grid[i][j] % k + k) % k;
        int ans = 0;
        if (i + 1 <= grid.size() - 1)
            ans = f(grid, k, i + 1, j, need, dp);
        if (j + 1 <= grid[0].size() - 1)
            ans = (ans + f(grid, k, i, j + 1, need, dp)) % MOD;
        dp[i][j][r] = ans;
        return ans;
    }

    int numberOfPaths(vector<vector<int>>& grid, int k) 
    {
        int m = grid.size();
        int n = grid[0].size();
        vector<vector<vector<int>>> dp(m, vector<vector<int>>(n, vector<int>(k, -1)));
        return f(grid, k, 0, 0, 0, dp);
    }
};

//法二：动态规划
class Solution 
{
public:
    int numberOfPaths(vector<vector<int>>& grid, int k) 
    {
        const int MOD = 1e9 + 7;
        int m = grid.size();
        int n = grid[0].size();
        vector<vector<vector<int>>> dp(m, vector<vector<int>>(n, vector<int>(k, 0)));
        //初始化：
        dp[m - 1][n - 1][grid[m - 1][n - 1] % k] = 1;//相当于递归中的 base case
        for (int i = m - 2; i >= 0; i--)//最右边的那一列
        {
            for (int r = 0; r < k; r++)
                dp[i][n - 1][r] = dp[i + 1][n - 1][(r - grid[i][n - 1] % k + k) % k];
        }
        for (int j = n - 2; j >= 0; j--)//最下边的那一行
        {
            for (int r = 0; r < k; r++)
                dp[m - 1][j][r] = dp[m - 1][j + 1][(r - grid[m - 1][j] % k + k) % k];
        }

        for (int i = m - 2; i >= 0; i--)
        {
            for (int j = n - 2; j >= 0; j--)
            {
                for (int r = 0; r < k; r++)
                {
                    int need = (r - grid[i][j] % k + k) % k;
                    int ans = 0;
                    ans = dp[i + 1][j][need];
                    ans = (ans + dp[i][j + 1][need]) % MOD;
                    dp[i][j][r] = ans;
                }
            }
        }
        return dp[0][0][0];
    }
};



//题目五：
// 扰乱字符串
// 使用下面描述的算法可以扰乱字符串 s 得到字符串 t ：
// 步骤1 : 如果字符串的长度为 1 ，算法停止
// 步骤2 : 如果字符串的长度 > 1 ，执行下述步骤：
//        在一个随机下标处将字符串分割成两个非空的子字符串
//        已知字符串s，则可以将其分成两个子字符串x和y且满足s=x+y
//        可以决定是要 交换两个子字符串 还是要 保持这两个子字符串的顺序不变
//        即s可能是 s = x + y 或者 s = y + x
//        在x和y这两个子字符串上继续从步骤1开始递归执行此算法
// 给你两个 长度相等 的字符串 s1 和 s2，判断 s2 是否是 s1 的扰乱字符串
// 如果是，返回true ；否则，返回false
// 测试链接 : https://leetcode.cn/problems/scramble-string/
//
// 感觉其实就是交换和不交换的01背包
//
//法一：暴力递归
class Solution 
{
public:
    //递归定义：s1[l1,...r1] 和 s2[l2,...,r2]是否为“扰乱字符串”
    //由于“扰乱字符串”的定义有 换 和 不换 两种操作
    //因此对两种情况进一步讨论
    //（区间dp）
    bool f(string& s1, int l1, int r1, string& s2, int l2, int r2)
    {
        if (l1 == r1) return s1[l1] == s2[l2];//base case

        // s1[l1..i][i+1....r1]
        // s2[l2..j][j+1....r2]
        // 不交错去讨论扰乱关系
        for (int i = l1, j = l2; i < r1 && j < r2; i++, j++)
        {
            if (f(s1, l1, i, s2, l2, j) && f(s1, i + 1, r1, s2, j + 1, r2))
                return true;
        }
        // 交错去讨论扰乱关系
        // s1[l1..........i][i+1...r1]
        // s2[l2...j-1][j..........r2]
        for (int i = l1, j = r2; i < r1 && j > l2; i++, j--)
        {
            if (f(s1, l1, i, s2, j, r2) && f(s1, i + 1, r1, s2, l2, j - 1))
                return true;
        }
        return false;
    }
    
    bool isScramble(string s1, string s2)
    {
        int m = s1.size();
        int n = s2.size();
        return f(s1, 0, m - 1, s2, 0, n - 1);
    }
};

//法二：记忆化搜索(其一)
class Solution 
{
public:
    //由于要验证的两个子串长度一定相等：r1-l1+1 == r2-l2+1
    //因此可以用len(表示长度)，将四维数组降至三维
    //  -> s1[l1,...,l1+len-1] 和 s2[l2,...,l2+len-1]
    int f(string& s1, string& s2, int l1, int l2, int len, vector<vector<vector<int>>>& dp)
    {
        if (len == 1) return s1[l1] == s2[l2];

        if (dp[l1][l2][len] != -1) return dp[l1][l2][len];
        int ans = 0;
        //不交错
        for (int i = l1, j = l2; i < l1 + len - 1 && j < l2 + len - 1; i++, j++)
        {
            if (f(s1, s2, l1, l2, i - l1 + 1, dp) && f(s1, s2, i + 1, j + 1, len - i + l1 - 1, dp))
            {
                ans = 1;
                break;
            }
        }
        //交错
        for (int i = l1, j = l2 + len - 1; i < l1 + len - 1 && j > l2; i++, j--)
        {
            if (f(s1, s2, l1, j, i - l1 + 1, dp) && f(s1, s2, i + 1, l2, len - i + l1 - 1, dp))
            {
                ans = 1;
                break;
            }
        }

        dp[l1][l2][len] = ans;
        return ans;
    }

    bool isScramble(string s1, string s2)
    {
        int m = s1.size();
        int n = s2.size();
        vector<vector<vector<int>>> dp(m, vector<vector<int>>(n, vector<int>(m + 1, -1)));
        return f(s1, s2, 0, 0, m, dp);
    }
};

//记忆化搜索(其二)
class Solution 
{
public:
    int f(string& s1, string& s2, int l1, int l2, int len, vector<vector<vector<int>>>& dp)
    {
        if (len == 1) return s1[l1] == s2[l2];

        if (dp[l1][l2][len] != -1) return dp[l1][l2][len];

        //不交错
        for (int k = 1; k < len; k++)
        {
            if (f(s1, s2, l1, l2, k, dp) && f(s1, s2, l1 + k, l2 + k, len - k, dp))
            {
                dp[l1][l2][len] = 1;
                return 1;
            }
        }
        //交错
        for (int k = 1; k < len; k++)
        {
            if (f(s1, s2, l1, l2 + len - k, k, dp) && f(s1, s2, l1 + k, l2, len - k, dp))
            {
                dp[l1][l2][len] = 1;
                return 1;
            }
        }

        dp[l1][l2][len] = 0;
        return 0;
    }

    bool isScramble(string s1, string s2) 
    {
        int m = s1.size();
        int n = s2.size();
        vector<vector<vector<int>>> dp(m, vector<vector<int>>(n, vector<int>(m + 1, -1)));
        return f(s1, s2, 0, 0, m, dp);
    }
};

//法三：动态规划
class Solution 
{
public:
    bool isScramble(string s1, string s2)
    {
        //根据记忆化搜索，让len作“层”，则每个格子依赖低层的格子，则由最底层(len==1)往最高层填即可
        int m = s1.size();
        int n = s2.size();
        vector<vector<vector<int>>> dp(m, vector<vector<int>>(n, vector<int>(m + 1, 0)));
        //初始化
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (s1[i] == s2[j])
                    dp[i][j][1] = 1;
            }
        }

        for (int len = 2; len <= m; len++)
        {
            //注意下方边界条件，递归中没检测的值在动态规划里也没必要检测
            for (int l1 = 0; l1 + len <= m; l1++)
            {
                for (int l2 = 0; l2 + len <= n; l2++)
                {
                    int ans = 0;
                    for (int k = 1; k < len; k++)
                    {
                        if (dp[l1][l2][k] && dp[l1 + k][l2 + k][len - k])
                        {
                            ans = 1;
                            break;
                        }
                    }
                    if (!ans)
                    {
                        for (int k = 1; k < len; k++)
                        {
                            if (dp[l1][l2 + len - k][k] && dp[l1 + k][l2][len - k])
                            {
                                ans = 1;
                                break;
                            }
                        }
                    }
                    dp[l1][l2][len] = ans;
                }
            }
        }
        return dp[0][0][m];
    }
};


