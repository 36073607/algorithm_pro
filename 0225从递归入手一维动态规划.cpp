#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
using namespace std;



//动态规划：用空间代替重复计算，包含一整套原理和技巧的总和，课程会用非常大的篇幅来全盘介绍

//知道怎么算的算法 vs 知道怎么试的算法

//有些递归在展开计算时，总是重复调用同一个子问题的解，这种重复调用的递归变成动态规划很有收益
//如果每次展开都是不同的解，或者重复调用的现象很少，那么没有改动态规划的必要
//下节课会举例，哪些递归没有改动态规划的必要！

//任何动态规划问题都一定对应着一个有重复调用行为的递归

//所以任何动态规划的题目都一定可以从递归入手，逐渐实现动态规划的方法

//尝试策略 就是 转移方程 ，完全一回事！
//推荐从尝试入手，因为代码好写，并且一旦发现尝试错误，重新想别的递归代价轻！

//动态规划表就是装递归返回值的容器
// 
//递归->dp递归->dp迭代->dp状态压缩迭代

//记忆化搜索是动态规划的一种递归实现形式（自顶向下），而通常所说的动态规划往往指迭代的递推实现（自底向上）
// 
//记忆化搜索是自顶向下的方法，它从大问题开始，递归地解决子问题，直到达到基本情况。
//这种方法更接近于普通的递归搜索，但通过记忆化来避免重复计算。
//
//动态规划是自底向上的方法，它从最基本的子问题开始，逐步构建解决方案，直到解决最终的问题。
//这种方法需要明确的计算顺序，并且通常更容易理解和实现。



//动态规划的大致过程：
//想出设计优良的递归尝试（方法、经验、固定套路很多），有关尝试展开顺序的说明: 除去一些题目只有固定展开顺序，大多数情况下，其实就是要符合我们习惯的递推方向 然后再来定义这个顺序
//  -> 记忆化搜索（从顶到底的动态规划），如果每个状态的计算枚举代价很低，往往到这里就可以了
//  -> 严格位置依赖的动态规划（从底到顶的动态规划），更多是为了下面说的 进一步优化枚举做的准备
//  -> 进一步优化空间（空间压缩），一维、二维、多维动态规划都存在这种优化
//  -> 进一步优化枚举也就是优化时间 （本节课没有涉及，但是后续巨多内容和这有关）

//解决一个问题，可能有很多尝试方法
//众多的尝试方法中，可能若干的尝试方法有重复调用的情况，可以转化成动态规划
//若干个可以转化成动态规划的方法中，又可能有优劣之分
//判定哪个是最优的动态规划方法，依据来自题目具体参数的数据量
//最优的动态规划方法实现后，后续又有一整套的优化技巧



//题目一到题目四，都从递归入手，逐渐改出动态规划的实现

//题目一：
// 斐波那契数
// 斐波那契数 （通常用 F(n) 表示）形成的序列称为 斐波那契数列
// 该数列由 0 和 1 开始，后面的每一项数字都是前面两项数字的和。
// 也就是：F(0) = 0，F(1) = 1
// F(n) = F(n - 1) + F(n - 2)，其中 n > 1
// 给定 n ，请计算 F(n)
// 测试链接 : https://leetcode.cn/problems/fibonacci-number/
// 注意：最优解来自矩阵快速幂，时间复杂度可以做到O(log n)
// 后续课程一定会讲述！本节课不涉及！
class Solution 
{
public:
    //时间复杂度 O(2^n)，来到每个点，都要左右展开去递归
    int f1(int i)
    {
        if (i == 0)
            return 0;
        if (i == 1)
            return 1;

        return f1(i - 1) + f1(i - 2);
    }

    int fib1(int n)
    {
        return f1(n);
    }


    //时间复杂度 O(n)，记忆化搜索
    int f2(int i, vector<int>& dp)//从顶到底
    {
        if (i == 0) return 0;
        if (i == 1) return 1;

        if (dp[i] != -1)//之前展开过，直接返回答案
            return dp[i];

        dp[i] = f2(i - 1, dp) + f2(i - 2, dp);
        return dp[i];
    }

    int fib2(int n)
    {
        vector<int> dp(n + 1, -1);//缓存表
        return f2(n, dp);
    }


    int fib3(int n)//从底到顶
    {
        if (n == 0) return 0;
        if (n == 1) return 1;

        vector<int>dp(n + 1);
        dp[1] = 1;
        for (int i = 2; i <= n; i++)
        {
            dp[i] = dp[i - 1] + dp[i - 2];
        }
        return dp[n];
    }


    int fib4(int n)//滚动更新
    {
        if (n == 0) return 0;
        if (n == 1) return 1;

        int lastlast = 0, last = 1;
        for (int i = 2; i <= n; i++)
        {
            int cur = lastlast + last;
            lastlast = last;
            last = cur;
        }
        return last;
    }
};



//题目二：
// 最低票价
// 在一个火车旅行很受欢迎的国度，你提前一年计划了一些火车旅行
// 在接下来的一年里，你要旅行的日子将以一个名为 days 的数组给出
// 每一项是一个从 1 到 365 的整数
// 火车票有 三种不同的销售方式
// 一张 为期1天 的通行证售价为 costs[0] 美元
// 一张 为期7天 的通行证售价为 costs[1] 美元
// 一张 为期30天 的通行证售价为 costs[2] 美元
// 通行证允许数天无限制的旅行
// 例如，如果我们在第 2 天获得一张 为期 7 天 的通行证
// 那么我们可以连着旅行 7 天(第2~8天)
// 返回 你想要完成在给定的列表 days 中列出的每一天的旅行所需要的最低消费
// 测试链接 : https://leetcode.cn/problems/minimum-cost-for-tickets/
//
//法一：暴力递归
class Solution 
{
private:
    vector<int> duration = { 1,7,30 };

public:
    int f1(vector<int>& days, vector<int>& costs, int i)
    {
        if (i == days.size())//base case ：下标已经遍历完days数组
            return 0;

        int ans = INT_MAX;
        for (int k = 0; k < 3; k++)//搜索三种买票可能
        {
            int j = i;//下一轮递归开始时，days数组的下标
            while (j < days.size() && days[i] + duration[k] - 1 >= days[j])
                j++;
            ans = min(ans, costs[k] + f1(days, costs, j));
        }
        return ans;
    }

    int mincostTickets(vector<int>& days, vector<int>& costs)
    {
        return f1(days, costs, 0);
    }
};

//法二：可以分析出：暴力递归中有重复调用 -->> 改记忆化搜索
//从顶到底的动态规划
class Solution
{
private:
    vector<int> duration = { 1,7,30 };

public:
    int f2(vector<int>& days, vector<int>& costs, int i, vector<int>& dp)
    {
        if (i == days.size())
            return 0;

        if (dp[i] != -1)
            return dp[i];

        int ans = INT_MAX;
        for (int k = 0; k < 3; k++)
        {
            int j = i;
            while (j < days.size() && days[i] + duration[k] - 1 >= days[j])
                j++;
            ans = min(ans, costs[k] + f2(days, costs, j, dp));
        }
        dp[i] = ans;
        return ans;
    }

    int mincostTickets(vector<int>& days, vector<int>& costs)
    {
        vector<int> dp(days.size(), -1);
        return f2(days, costs, 0, dp);
    }
};

//法三：
//从底到顶，严格路径依赖的动态规划
class Solution 
{
private:
    vector<int> duration = { 1,7,30 };

public:
    int mincostTickets(vector<int>& days, vector<int>& costs) 
    {
        int n = days.size();
        vector<int> dp(n + 1, INT_MAX);
        
        dp[n] = 0;
        for (int i = n - 1; i >= 0; i--)
        {
            for (int k = 0; k < 3; k++)
            {
                int j = i;
                while (j < n && days[i] + duration[k] - 1 >= days[j])
                    j++;
                dp[i] = min(dp[i], costs[k] + dp[j]);
            }
        }
        return dp[0];
    }
};



//题目三：
// 解码方法
// 一条包含字母 A-Z 的消息通过以下映射进行了 编码 ：
// 'A' -> "1"
// 'B' -> "2"
// ...
// 'Z' -> "26"
// 要 解码 已编码的消息，所有数字必须基于上述映射的方法，反向映射回字母（可能有多种方法）
// 例如，"11106" 可以映射为："AAJF"、"KJF"
// 注意，消息不能分组为(1 11 06)，因为 "06" 不能映射为 "F"
// 这是由于 "6" 和 "06" 在映射中并不等价
// 给你一个只含数字的 非空 字符串 s ，请计算并返回 解码 方法的 总数
// 题目数据保证答案肯定是一个 32位 的整数
// 测试链接 : https://leetcode.cn/problems/decode-ways/
//
//法一：暴力递归
class Solution 
{
public:
    int f1(string& s, int i)
    {
        if (i == s.size())//顺利遍历完整个字符串
            return 1;

        if (s[i] == '0')//当前位遇到0，说明之前存在决策错误，不能成为一种解码方法
            return 0;
        int ans = 0;
        ans = f1(s, i + 1);//一位数字的编码必然对应一个字母，i自己单独转
        if (i + 1 < s.size() && ((s[i] - '0') * 10 + s[i + 1] - '0') <= 26)//两位数字能构成一段正确编码，(i i+1)一起转
            ans += f1(s, i + 2);

        return ans;
    }

    int numDecodings(string s) 
    {
        return f1(s, 0);
    }
};

//法二：暴力递归改记忆化搜索
class Solution 
{
public:
    int f2(string& s, int i, vector<int>& dp)
    {
        if (i == s.size())//base case
            return 1;

        if (dp[i] != -1)//之前得到过答案，直接返回
            return dp[i];
        if (s[i] == '0')//之前决策错误，不可能得到一种完整编码
            return 0;
        int ans = 0;
        ans = f2(s, i + 1, dp);
        if (i + 1 < s.size() && ((s[i] - '0') * 10 + (s[i + 1] - '0')) <= 26)
            ans += f2(s, i + 2, dp);

        dp[i] = ans;
        return ans;
    }

    int numDecodings(string s) 
    {
        vector<int> dp(s.size(), -1);
        return f2(s, 0, dp);
    }
};

//法三：严格位置依赖的动态规划
class Solution 
{
public:
    int numDecodings(string s) 
    {
        int n = s.size();
        vector<int> dp(n + 1, -1);
        dp[n] = 1;
        for (int i = n - 1; i >= 0; i--)
        {
            if (s[i] == '0')
                dp[i] = 0;
            else
            {
                int tmp = dp[i + 1];
                if (i + 1 < n && ((s[i] - '0') * 10 + (s[i + 1] - '0')) <= 26)
                    tmp += dp[i + 2];
                dp[i] = tmp;
            } 
        }
        return dp[0];
    }
};

//法四：严格位置依赖的动态规划 + 空间压缩
//类似斐波那契的滚动数组优化
class Solution
{
public:
    int numDecodings(string s)
    {
        int n = s.size();
        
        int last = 1;//最初指向dp[n]
        int lastlast = 0;//最初指向dp[n+1]，不存在
        for (int i = n - 1; i >= 0; i--)
        {
            int cur = 0;
            if (s[i] == '0')
                cur = 0;
            else
            {
                cur = last;
                if (i + 1 < n && ((s[i] - '0') * 10 + (s[i + 1] - '0')) <= 26)
                    cur += lastlast;
            }
            lastlast = last;
            last = cur;
        }
        return last;
    }
};



//题目四：
// 解码方法 II
// 一条包含字母 A-Z 的消息通过以下的方式进行了 编码 ：
// 'A' -> "1"
// 'B' -> "2"
// ...
// 'Z' -> "26"
// 要 解码 一条已编码的消息，所有的数字都必须分组
// 然后按原来的编码方案反向映射回字母（可能存在多种方式）
// 例如，"11106" 可以映射为："AAJF"、"KJF"
// 注意，像 (1 11 06) 这样的分组是无效的，"06"不可以映射为'F'
// 除了上面描述的数字字母映射方案，编码消息中可能包含 '*' 字符
// 可以表示从 '1' 到 '9' 的任一数字（不包括 '0'）
// 例如，"1*" 可以表示 "11"、"12"、"13"、"14"、"15"、"16"、"17"、"18" 或 "19"
// 对 "1*" 进行解码，相当于解码该字符串可以表示的任何编码消息
// 给你一个字符串 s ，由数字和 '*' 字符组成，返回 解码 该字符串的方法 数目
// 由于答案数目可能非常大，答案对 1000000007 取模
// 测试链接 : https://leetcode.cn/problems/decode-ways-ii/
//
//法一：暴力递归, 取模也究极简单暴力
class Solution
{
public:
    int f1(string& s, int i)
    {
        if (i == s.size())
            return 1;

        if (s[i] == '0')//之前决策失误，不能形成答案
            return 0;

        //1. i自己单独转
        int ans = s[i] == '*' ? 9 * f1(s, i + 1) : f1(s, i + 1);
        //2. (i i+1) 一起转
        if (s[i] != '*')//s[i]是数字
        {
            if (i + 1 < s.size())//不越界
            {
                if (s[i + 1] != '*' && ((s[i] - '0') * 10 + (s[i + 1] - '0')) <= 26)//s[i+1]也是数字，且能构成两位编码
                    ans += f1(s, i + 2);
                else if (s[i + 1] == '*')//s[i+1]是'*'
                {
                    if (s[i] == '1')
                        ans += 9 * f1(s, i + 2);
                    else if (s[i] == '2')
                        ans += 6 * f1(s, i + 2);
                }
            }
        }
        else if (s[i] == '*')//s[i]是'*'
        {
            if (i + 1 < s.size())//不越界
            {
                if (s[i + 1] != '*')//s[i+1]是数字
                {
                    if ((s[i + 1] - '0') <= 6)
                        ans += 2 * f1(s, i + 2);
                    else
                        ans += f1(s, i + 2);
                }
                else if (s[i + 1] == '*')
                {
                    ans += 15 * f1(s, i + 2);
                }
            }
        }
        return ans;
    }

    int numDecodings(string s) 
    {
        return f1(s, 0) % 1000000007;
    }
};

//法二：记忆化搜索
class Solution
{
private:
    static const long MOD = 1000000007;

public:
    long f2(string& s, int i, vector<long>& dp)
    {
        if (i == s.size())//base case
            return 1;

        if (s[i] == '0')//之前决策失误，不能形成答案
            return 0;

        if (dp[i] != -1)//之前得到过答案
            return dp[i];

        //1. i自己单独转
        long ans = s[i] == '*' ? 9 * f2(s, i + 1, dp) : f2(s, i + 1, dp);
        //2. (i i+1) 一起转
        if (s[i] != '*')//s[i]是数字
        {
            if (i + 1 < s.size())//不越界
            {
                if (s[i + 1] != '*' && ((s[i] - '0') * 10 + (s[i + 1] - '0')) <= 26)//s[i+1]也是数字，且能构成两位编码
                    ans += f2(s, i + 2, dp);
                else if (s[i + 1] == '*')//s[i+1]是'*'
                {
                    if (s[i] == '1')
                        ans += 9 * f2(s, i + 2, dp);
                    else if (s[i] == '2')
                        ans += 6 * f2(s, i + 2, dp);
                }
            }
        }
        else if (s[i] == '*')//s[i]是'*'
        {
            if (i + 1 < s.size())//不越界
            {
                if (s[i + 1] != '*')//s[i+1]是数字
                {
                    if ((s[i + 1] - '0') <= 6)
                        ans += 2 * f2(s, i + 2, dp);
                    else
                        ans += f2(s, i + 2, dp);
                }
                else if (s[i + 1] == '*')
                {
                    ans += 15 * f2(s, i + 2, dp);
                }
            }
        }
        ans %= MOD;
        dp[i] = ans;
        return ans;
    }

    int numDecodings(string s) 
    {
        vector<long> dp(s.size(), -1);
        return static_cast<int>(f2(s, 0, dp));
    }
};

//法三：动态规划
class Solution 
{
private:
    static const long MOD = 1000000007;

public:
    int numDecodings(string s) 
    {
        int n = s.size();
        vector<long> dp(n + 1);
        dp[n] = 1;//相当于ms方法中的base case

        for (int i = n - 1; i >= 0; i--)
        {
            if (s[i] != '0')//能形成答案
            {
                //1. i 单独转
                dp[i] = dp[i + 1] * (s[i] == '*' ? 9 : 1);
                //2. (i i+1)一起转
                if (i + 1 < n)
                {
                    if (s[i] != '*')//s[i]是数字
                    {
                        if (s[i + 1] != '*' && ((s[i] - '0') * 10 + (s[i + 1] - '0')) <= 26)//s[i+1]也是数字，且能构成合法编码
                        {
                            dp[i] += dp[i + 2];
                        }
                        else if (s[i + 1] == '*')//s[i+1]是'*'
                        {
                            if (s[i] == '1')
                                dp[i] += 9 * dp[i + 2];
                            else if (s[i] == '2')
                                dp[i] += 6 * dp[i + 2];
                        }
                    }
                    else if (s[i] == '*')//s[i]是'*'
                    {
                        if (s[i + 1] != '*')//s[i+1]是数字
                        {
                            if (s[i + 1] - '0' <= 6)
                                dp[i] += 2 * dp[i + 2];
                            else
                                dp[i] += dp[i + 2];
                        }
                        else if (s[i + 1] == '*')//s[i+1]也是'*'
                            dp[i] += 15 * dp[i + 2];
                    }
                }
                dp[i] %= MOD;
            }
        }
        return (int)dp[0];
    }
};

//法四：动态规划+空间压缩
class Solution 
{
private:
    static const long MOD = 1000000007;

public:
    int numDecodings(string s)
    {
        int n = s.size();
        long last = 1;//dp[i+1]，此时指向dp[n+1]
        long lastlast = 0;//dp[i+2]

        for (int i = n - 1; i >= 0; i--)
        {
            long cur = 0;//dp[i]
            if (s[i] != '0')
            {
                cur = last * (s[i] == '*' ? 9 : 1);
                if (i + 1 < n)
                {
                    if (s[i] != '*')
                    {
                        if (s[i + 1] != '*' && ((s[i] - '0') * 10 + (s[i + 1] - '0')) <= 26)
                        {
                            cur += lastlast;
                        }
                        else if (s[i + 1] == '*')
                        {
                            if (s[i] == '1')
                                cur += 9 * lastlast;
                            else if (s[i] == '2')
                                cur += 6 * lastlast;
                        }
                    }
                    else if (s[i] == '*')
                    {
                        if (s[i + 1] != '*')
                        {
                            if (s[i + 1] - '0' <= 6)
                                cur += 2 * lastlast;
                            else
                                cur += lastlast;
                        }
                        else if (s[i + 1] == '*')
                        {
                            cur += 15 * lastlast;
                        }
                    }
                }
                cur %= MOD;
            }
            //无论 s[i] 是否为 '0'，都需要更新状态变量，让指针前移
            lastlast = last;
            last = cur;
        }
        return (long)last;
    }
};



//当熟悉了从递归到动态规划的转化过程
//那么就可以纯粹用动态规划的视角来分析问题了
//题目五到题目八，都是纯粹用动态规划的视角来分析、优化的
//
//如果不熟悉这个过程，直接一上来就硬去理解状态转移方程
//那么往往会步履维艰、邯郸学步、东施效颦
//这是多年教学看到的真实情况
//
//很多极为优异的想法、设计和优化，来自 努力 or 天赋

//建议脚踏实地，真正做好从递归到动态规划的练习
//接下来的几节课也都会从最基本递归入手，逐渐写出动态规划的版本


//题目五：
// 丑数 II
// 给你一个整数 n ，请你找出并返回第 n 个 丑数
// 丑数 就是只包含质因数 2、3 或 5 的正整数
// 测试链接 : https://leetcode.cn/problems/ugly-number-ii/
//
//法一：哈希集合去重+堆 暴力
class Solution
{
public:
    int nthUglyNumber(int n)
    {
        vector<int> factor = { 2,3,5 };
        unordered_set<long> got;
        priority_queue<long, vector<long>, greater<long>> heap;

        //处理第一个丑数 1
        heap.push(1L);
        got.insert(1L);
        long ans = 0;
        for (int i = 0; i < n; i++)
        {
            long cur = heap.top();
            heap.pop();
            for (auto& ele : factor)
            {
                if (got.find(cur * ele) == got.end())
                {
                    heap.push(cur * ele);
                    got.insert(cur * ele);
                }
            }
            ans = cur;
        }
        return ans;
    }
};

//法二：动态规划优化
//当前位置的丑数依赖于之前最小的那个丑数，所以用了依赖位置的转移方程
class Solution 
{
public:
    int nthUglyNumber(int n) 
    {
        vector<int> dp(n + 1);
        dp[1] = 1;//第1个丑数就是1，0位置弃而不用
        for (int i = 2, i2 = 1, i3 = 1, i5 = 1; i <= n; i++)
        {
            //i2,i3,i5 分别表示乘2倍、3倍、5倍的指针
            //指针指向第某个丑数
            int a = dp[i2] * 2;
            int b = dp[i3] * 3;
            int c = dp[i5] * 5;
            int cur = min(min(a, b), c);

            if (cur == a)
                i2++;//指向下一个丑数
            if (cur == b)
                i3++;
            if (cur == c)
                i5++;

            dp[i] = cur;
        }
        return dp[n];
    }
};



//题目六：
// 最长有效括号
// 给你一个只包含 '(' 和 ')' 的字符串
// 找出最长有效（格式正确且连续）括号子串的长度。
// 测试链接 : https://leetcode.cn/problems/longest-valid-parentheses/
//
//核心思路：
//求dp[i]:
//1.s[i]=='(' , dp[i]=0
//2.s[i]==')' , 根据dp[i-1]的长度往前跳到匹配的p位置
//  a) s[p]==')' , dp[i]=0
//  b) s[p]=='(' , dp[i]=dp[i-1]+2+dp[p-1]

//法一：记忆化搜索
//我们可以这样定义：
//由于括号成对出现，来到某个下标处，得到以该下标结算时的长度
//例如，来到s[i]，求出以这个位置括号结算所得到的括号字串长度
//因此，从字符串末位向首位递归
class Solution 
{
public:
    int ms(string& s, int i, vector<int>& dp)
    {
        if (i == 0)//base case ：0下标处
        {
            dp[0] = 0;
            return 0;
        }
            
        if (s[i] == '(')
        {
            //虽然当前位置无法作为有效结尾，但必须继续向前递归，
            // 否则 dp 数组前面的值不会被计算，导致最终结果错误。
            ms(s, i - 1, dp);
            dp[i] = 0;
            return 0;
        }
            
        if (dp[i] != -1)
            return dp[i];

        int ans = 0;
        int prevLen = ms(s, i - 1, dp); // 获取以前一个字符结尾的有效长度
        int matchIdx = i - prevLen - 1;
        if (matchIdx >= 0 && s[matchIdx] == '(')
        {
            ans = prevLen + 2 + (matchIdx - 1 >= 0 ? ms(s, matchIdx - 1, dp) : 0);
        }
        dp[i] = ans;
        return ans;
    }

    int longestValidParentheses(string s) 
    {
        int n = s.size();
        if (n == 0) return 0;

        vector<int> dp(n, -1);
        ms(s, n - 1, dp);
        
        int maxlength = 0;
        for (auto& ele : dp)
            maxlength = max(maxlength, ele);
        return maxlength;
    }
};

//法二：动态规划
class Solution 
{
public:
    int longestValidParentheses(string s) 
    {
        int n = s.size();
        if (n == 0) return 0;

        vector<int> dp(n, 0);
        //dp[0] = 0;
        for (int i = 1; i < n; i++)
        {
            if(s[i]==')')
            {
                int prevLen = dp[i - 1];
                int matchIdx = i - prevLen - 1;
                if (matchIdx >= 0 && s[matchIdx] == '(')
                {
                    dp[i] = prevLen + 2 + (matchIdx - 1 >= 0 ? dp[matchIdx - 1] : 0);
                }
            }
        }

        int ans = 0;
        for (auto& ele : dp)
            ans = max(ans, ele);
        return ans;
    }
};



//题目七：
// 环绕字符串中唯一的子字符串
// 定义字符串 base 为一个 "abcdefghijklmnopqrstuvwxyz" 无限环绕的字符串
// 所以 base 看起来是这样的：
// "..zabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcd.."
// 给你一个字符串 s ，请你统计并返回 s 中有多少 不同非空子串 也在 base 中出现
// 测试链接 : https://leetcode.cn/problems/unique-substrings-in-wraparound-string/
//
//核心思路：
//  子串的数量，刚好等于以该字母结尾的最长连续子串长度，所以这个题就可以转化为，寻找每一个字母结尾的最长连续子串数量，最后求和
//  既然以字母为结尾求字串长度，很明显想到往该字母左边操作

//法一：记忆化搜索
class Solution 
{
public:
    int f(string& s, int i, vector<int>& dp)
    {
        if (i == 0)//base case
        {
            dp[s[i] - 'a'] = max(dp[s[i] - 'a'], 1);
            return 1;
        }

        int prev = f(s, i - 1, dp);//以s[i-1]结尾的最长字串长度
        int cur = 1;
        if (i - 1 >= 0 && ((s[i] == 'a' && s[i - 1] == 'z') || s[i] - s[i - 1] == 1))
            cur += prev;
        //更新以当前字符 s[i] 结尾的最大长度
        dp[s[i] - 'a'] = max(dp[s[i] - 'a'], cur);
        return cur;
    }

    int findSubstringInWraproundString(string s) 
    {
        int n = s.size();
        if (n == 0) return 0;

        vector<int> dp(26, 0);
        f(s, n - 1, dp);
        int ans = 0;
        for (auto& ele : dp)
            ans += ele;
        return ans;
    }
};

//法二：动态规划
class Solution 
{
public:
    int findSubstringInWraproundString(string s) 
    {
        int n = s.size();
        if (n == 0) return 0;

        vector<int> dp(26, 0);
        dp[s[0] - 'a'] = 1;
        for (int i = 1, len = 1; i < n; i++)
        {
            if ((s[i] == 'a' && s[i - 1] == 'z') || (s[i] - s[i - 1] == 1))
                len++;
            else
                len = 1;
            dp[s[i] - 'a'] = max(dp[s[i] - 'a'], len);
        }

        int ans = 0;
        for (auto& ele : dp)
            ans += ele;
        return ans;
    }
};



//题目八：
// 不同的子序列 II
// 给定一个字符串 s，计算 s 的 不同非空子序列 的个数
// 因为结果可能很大，答案对 1000000007 取模
// 字符串的 子序列 是经由原字符串删除一些（也可能不删除）
// 字符但不改变剩余字符相对位置的一个新字符串
// 例如，"ace" 是 "abcde" 的一个子序列，但 "aec" 不是
// 测试链接 : https://leetcode.cn/problems/distinct-subsequences-ii/
//
//第一反应是采取 回溯DFS+哈希集合去重 ，但肯定超时
//依旧动态规划
//
//思路：把原问题细分一下，改为分别统计以 ‘a’,‘b’,⋯,‘z’ 结尾的不同子序列的个数，问题就迎刃而解了
//观察得来的公式：
//纯新：all - 当前字符上次记录
//当前字符记录 += 纯新
//all += 纯新
class Solution
{
public:
    int distinctSubseqII(string s)
    {
        const int MOD = 1000000007;
        vector<int> dp(26, 0);
        int all = 1;
        int newAdd = 0;
        for (auto& ch : s)
        {
            newAdd = (all - dp[ch - 'a'] + MOD) % MOD;
            dp[ch - 'a'] = (dp[ch - 'a'] + newAdd) % MOD;
            all = (all + newAdd) % MOD;
        }
        return (all - 1 + MOD) % MOD;
    }
};

//题目七和题目八并不是 “以某个位置结尾的算法”，而是“以某个字符结尾的算法”！！！
//当然，以某个 位置/字符 结尾是自己根据具体题目而给出的其中一种定义
//我们完全可以定义 “以某个位置/字符开头的算法”，这并不是固定的！！！！！
//  -> 其实就是要符合我们习惯的递推方向 然后再来定义这个顺序
