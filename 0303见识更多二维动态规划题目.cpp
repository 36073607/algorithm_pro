#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <climits>
using namespace std;




//题目一：
// 不同的子序列
// 给你两个字符串s和t ，统计并返回在s的子序列中t出现的个数
// 测试链接 : https://leetcode.cn/problems/distinct-subsequences/
//
//法一：记忆化搜索
class Solution 
{
public:
    //递归定义：s里取(0,..,i)，t里取(0,...j)，看此时s'中t'出现的个数
    // (这里的 i j 均为下标)
    //1.不要s[i]
    //2.若s[i]==t[j]，要s[i]
    int f(string& s, string& t, int i, int j, vector<vector<int>>& dp)
    {
        if (i < j) return 0;
        if (j < 0) return 1;

        if (dp[i][j] != -1) return dp[i][j];

        dp[i][j] = f(s, t, i - 1, j, dp);
        if (s[i] == t[j])
            dp[i][j] += f(s, t, i - 1, j - 1, dp);
        return dp[i][j];
    }

    int numDistinct(string s, string t) 
    {
        int m = s.size();
        int n = t.size();
        vector<vector<int>> dp(m, vector<int>(n, -1));
        return f(s, t, m - 1, n - 1, dp);
    }
};

//法二：动态规划
class Solution 
{
public:
    int numDistinct(string s, string t)
    {
        int m = s.size();
        int n = t.size();
        //由于记忆化搜索中用下标定义递归，导致改动态规划不好改
        //因此动态规划中用长度定义
        // dp[i][j] :
        // s[前缀长度为i]的所有子序列中，有多少个子序列等于t[前缀长度为j]
        vector<vector<unsigned long long>> dp(m + 1, vector<unsigned long long>(n + 1, 0));
        //初始化：t长度为0，说明匹配完成
        for (int i = 0; i <= m; i++)
            dp[i][0] = 1;
        for (int i = 1; i <= m; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                dp[i][j] = dp[i - 1][j];
                if (s[i - 1] == t[j - 1])
                    dp[i][j] += dp[i - 1][j - 1];
            }
        }
        return dp[m][n];
    }
};

//法三：动态规划+空间压缩
//一个格子依赖 上方和左上角 的格子，所以从右往左填
class Solution
{
public:
    int numDistinct(string s, string t) 
    {
        int m = s.size();
        int n = t.size();
        vector<unsigned long long> dp(n + 1, 0);
        dp[0] = 1;//初始化
        for (int i = 1; i <= m; i++)
        {
            for (int j = n; j >= 1; j--)
            {
                //dp[j]=dp[j] -> dp[i][j]=dp[i][j-1]
                if (s[i - 1] == t[j - 1])
                    dp[j] += dp[j - 1];
            }
        }
        return dp[n];
    }
};



//题目二：
// 编辑距离
// 给你两个单词 word1 和 word2
// 请返回将 word1 转换成 word2 所使用的最少代价
// 你可以对一个单词进行如下三种操作：
// 插入一个字符，代价a
// 删除一个字符，代价b
// 替换一个字符，代价c
// 测试链接 : https://leetcode.cn/problems/edit-distance/

//注：这是一个很重要的算法：可以用来评价两个字符串的相似程度
//力扣这道题只是“编辑距离”问题的一种情况，完整的问题是：插入、删除、替换等操作都有相应的代价，各个操作的代价不一定相等
//这里给出完整的解答：

//直接上动态规划：
//状态定义：i代表s1此时的长度，j代表s2此时的长度；dp[i][j]表示s1(i长度)转化到s2(j长度)所需要的最小代价
//思路与状态转移方程：
//1.s1[i-1]参与：
//  a.s1[i-1]变成s2[j-1]:
//    1) s1[i-1]==s2[j-1]，dp[i][j] = dp[i-1][j-1]
//    2) s1[i-1]!=s2[j-1]，dp[i][j] = dp[i-1][j-1] + 替换代价
//  b.s1[i-1]不会变成s2[j-1]:
//    dp[i][j] = dp[i][j-1] + 插入代价
//2.s1[i-1]不参与：
//  dp[i][j] = dp[i-1][j] + 删除代价
class Solution
{
public:
    int editDistance(string& s1, string& s2, int a, int b, int c)
    {
        int m = s1.size();
        int n = s2.size();
        // dp[i][j] :
        // s1[前缀长度为i]想变成s2[前缀长度为j]，至少付出多少代价
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
        //初始化：
        for (int i = 0; i <= m; i++) dp[i][0] = i * b;//s2为空，则都是删除代价
        for (int j = 0; j <= n; j++) dp[0][j] = j * a;//s1为空，则都是插入代价

        for (int i = 1; i <= m; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                int p1, p2, p3, p4;
                p1 = p2 = p3 = p4 = INT_MAX;
                //s1[i-1]参与:
                //a.s1[i-1]变成s2[j-1]
                if (s1[i - 1] == s2[j - 1])
                    p1 = dp[i - 1][j - 1];
                else
                    p2 = dp[i - 1][j - 1] + c;
                //b.s1[i-1]不变成s2[j-1]
                p3 = dp[i][j - 1] + a;

                //s1[i-1]不参与
                p4 = dp[i - 1][j] + b;

                //汇总
                dp[i][j] = min(min(p1, p2), min(p3, p4));
            }
        }
        return dp[m][n];
    }

    int minDistance(string word1, string word2)
    {
        return editDistance(word1, word2, 1, 1, 1);//三种操作的代价都是1
    }
};

//法二：动态规划优化(贪心)
//s1[i-1]==s2[j-1]最好的情况，直接讨论下一轮
//s1[i-1]!=s2[j-1]，插入、删除、替换三种操作选代价最小的
class Solution 
{
public:
    int editDistance(string& s1, string& s2, int a, int b, int c)
    {
        int m = s1.size();
        int n = s2.size();
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
        //初始化：
        for (int i = 0; i <= m; i++) dp[i][0] = i * b;
        for (int j = 0; j <= n; j++) dp[0][j] = j * a;

        for (int i = 1; i <= m; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (s1[i - 1] == s2[j - 1])
                    dp[i][j] = dp[i - 1][j - 1];
                else
                    dp[i][j] = min(min(dp[i][j - 1] + a, dp[i - 1][j] + b), dp[i - 1][j - 1] + c);
            }
        }
        return dp[m][n];
    }

    int minDistance(string word1, string word2)
    {
        return editDistance(word1, word2, 1, 1, 1);
    }
};

//法三：动态规划+空间压缩
class Solution
{
public:
    int editDistance(string& s1, string& s2, int a, int b, int c)
    {
        int m = s1.size();
        int n = s2.size();
        vector<int> dp(n + 1, 0);
        //初始化
        //dp[0]=0;
        for (int j = 0; j <= n; j++) dp[j] = a * j;

        for (int i = 1; i <= m; i++)
        {
            int leftup, backup;
            leftup = dp[0];
            dp[0] = i * b;
            for (int j = 1; j <= n; j++)
            {
                backup = dp[j];

                if (s1[i - 1] == s2[j - 1])
                    dp[j] = leftup;
                else
                    dp[j] = min(min(dp[j - 1] + a, dp[j] + b), leftup + c);

                leftup = backup;
            }
        }
        return dp[n];
    }

    int minDistance(string word1, string word2) 
    {
        return editDistance(word1, word2, 1, 1, 1);
    }
};



//题目三：
// 交错字符串
// 给定三个字符串 s1、s2、s3
// 请帮忙验证s3是否由s1和s2交错组成
// 测试链接 : https://leetcode.cn/problems/interleaving-string/
//
//法一：动态规划
//状态定义: dp[i][j]表示：s1[前i个]，s2[前j个]，能否拼接成s3[前i+j个]
//思路与转移方程：
//1.s3[i+j-1]==s1[i-1] : dp[i][j] -> dp[i-1][j]
//2.s3[i+j-1]==s2[j-1] : dp[i][j] -> dp[i][j-1]
class Solution 
{
public:
    bool isInterleave(string s1, string s2, string s3) 
    {
        int len1 = s1.size();
        int len2 = s2.size();
        int len3 = s3.size();
        if (len1 + len2 != len3) return false;
        // dp[i][j]:
        // s1[前缀长度为i]和s2[前缀长度为j]，能否交错组成出s3[前缀长度为i+j]
        vector<vector<bool>> dp(len1 + 1, vector<bool>(len2 + 1, false));
        //初始化
        dp[0][0] = true;
        for (int i = 1; i <= len1; i++)
        {
            if (s1[i - 1] != s3[i - 1])
                break;
            dp[i][0] = true;
        }
        for (int j = 1; j <= len2; j++)
        {
            if (s2[j - 1] != s3[j - 1])
                break;
            dp[0][j] = true;
        }

        for (int i = 1; i <= len1; i++)
        {
            for (int j = 1; j <= len2; j++)
            {
                dp[i][j] = (s1[i - 1] == s3[i + j - 1] && dp[i - 1][j])
                        || (s2[j - 1] == s3[i + j - 1] && dp[i][j - 1]);
            }
        }
        return dp[len1][len2];
    }
};

//法二：动态规划+空间压缩
class Solution
{
public:
    bool isInterleave(string s1, string s2, string s3)
    {
        int len1 = s1.size();
        int len2 = s2.size();
        int len3 = s3.size();
        if (len1 + len2 != len3) return false;

        vector<bool> dp(len2 + 1, false);
        //初始化
        dp[0] = true;
        for (int j = 1; j <= len2; j++)
        {
            if (s2[j - 1] != s3[j - 1])
                break;
            dp[j] = true;
        }

        for (int i = 1; i <= len1; i++)
        {
            for (int k = 1; k <= i; k++)
            {
                if (s1[k - 1] != s3[k - 1])
                {
                    dp[0] = false;
                    break;
                }
                dp[0] = true;
            }
            for (int j = 1; j <= len2; j++)
            {
                dp[j] = (s1[i - 1] == s3[i + j - 1] && dp[j])
                    || (s2[j - 1] == s3[i + j - 1] && dp[j - 1]);
            }
        }
        return dp[len2];
    }
};



//题目四：
// 有效涂色问题
// 给定n、m两个参数
// 一共有n个格子，每个格子可以涂上一种颜色，颜色在m种里选
// 当涂满n个格子，并且m种颜色都使用了，叫一种有效方法
// 求一共有多少种有效的涂色方法
// 1 <= n, m <= 5000
// 结果比较大请 % 1000000007 之后返回
// 对数器验证
//

//状态dp[i][j]定义：当前i个格子涂了j种颜色
//思路及状态转移方程：
//1.开辟新颜色 -> 前一个格子已经涂了j种颜色：dp[i][j]=dp[i-1][j]*j
//2.不开辟新颜色 -> 前一个格子少涂了一个格子：dp[i][j]=dp[i-1][j-1]*(m-(j-1))
class Solution
{
public:
    int way(int n, int m)
    {
        const int MOD = 1e9 + 7;
        // dp[i][j]:
        // 一共有m种颜色
        // 前i个格子涂满j种颜色的方法数
        vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
        //初始化：
        for (int i = 1; i <= n; i++)
            dp[i][1] = m;

        for (int i = 2; i <= n; i++)
        {
            for (int j = 2; j <= m; j++)
            {
                dp[i][j] = ((long)dp[i - 1][j] * j) & MOD;
                dp[i][j] = (dp[i][j] + (long)dp[i - 1][j - 1] * (m - (j - 1))) % MOD;
            }
        }
        return dp[n][m];
    }
};



//题目五：
// 删除至少几个字符可以变成另一个字符串的子串
// 给定两个字符串s1和s2
// 返回s1至少删除多少字符可以成为s2的子串
// 对数器验证
//
//状态dp[i][j]定义：s1[前i个字符] 删掉多少字符能变成 s2[前j个字符]的任意后缀串 
//由于原问题是子串，而定义中是任意字符串
//所以返回dp[m][n]中最后一行的最小值
//思路及状态转移方程：
//1.s1[i-1]!=s2[j-1]，dp[i][j]=1+dp[i-1][j]
//2.s1[i-1]==s2[j-1]，dp[i][j]=dp[i-1][j-1]
class Solution
{
public:
    int minDelete(string s1, string s2)
    {
        int m = s1.size();
        int n = s2.size();
        // dp[len1][len2] :
        // s1[前缀长度为i]至少删除多少字符，可以变成s2[前缀长度为j]的任意后缀串
        vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
        //初始化：
        for (int i = 0; i <= m; i++) dp[i][0] = i;

        for (int i = 1; i <= m; i++)
        {
            for (int j = 1; j <= n; j++)
            {
                if (s1[i - 1] != s2[j - 1])
                    dp[i][j] = 1 + dp[i - 1][j];
                else
                    dp[i][j] = dp[i - 1][j - 1];
            }
        }
        int ans = INT_MAX;
        for (int j = 0; j <= n; j++)
            ans = min(ans, dp[m][j]);
        return ans;
    }
};

