#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
using namespace std;

//使用二分答案法思路：  （核心）
//正着求解不好解-->> 选择 当固定答案情况下，我去推条件够不够用 这个思路(二分答案) -->> 进而去推单调性，想f函数之类的

//二分答案法的初衷：
//正着求解不好求，但我一旦给他一个预期（算出m，设计f函数），有可能我这种选择就变得不再纠结了

//分析单调性：
//套路：如果发现答案越小，越不能（能）满足要求；答案越大，越能（不能）满足要求，就可以尝试二分答案。
//当然还有不易分析的隐单调性，需个人积累

//二分答案法：
//1.估计 最终答案可能的范围 是什么 ，可以定的粗略，反正二分不了几次
//2.分析 问题的答案 和 给定条件 之间的 单调性，大部分时候只需要用到 自然智慧
//3.建立一个f函数，当答案固定的情况下，判断 给定的条件是否达标
//4.在 最终答案可能的范围上不断二分搜索，每次用f函数判断，直到二分结束，找到最合适的答案

//核心点：分析单调性、建立f函数

//注意：
//这个技巧常用且重要，一定要引起重视，非常的美、精妙！


//题目一：
// 爱吃香蕉的珂珂
// 珂珂喜欢吃香蕉。这里有 n 堆香蕉，第 i 堆中有 piles[i] 根香蕉
// 警卫已经离开了，将在 h 小时后回来。
// 珂珂可以决定她吃香蕉的速度 k （单位：根/小时)
// 每个小时，她将会选择一堆香蕉，从中吃掉 k 根
// 如果这堆香蕉少于 k 根，她将吃掉这堆的所有香蕉，然后这一小时内不会再吃更多的香蕉
// 珂珂喜欢慢慢吃，但仍然想在警卫回来前吃掉所有的香蕉。
// 返回她可以在 h 小时内吃掉所有香蕉的最小速度 k（k 为整数）
// 测试链接 : https://leetcode.cn/problems/koko-eating-bananas/
//分析单调性：
//当速度变大时，所耗费的时间变小或者不变
//当速度变小时，所耗费的时间变大或者不变
class Solution 
{
public:
    bool f(int m, int h, const vector<int>& vc)
    {
        long long time = 0;
        for (auto& ele : vc)
            time += ((long long)ele + m - 1) / m;//记得向上取整
        if (time <= h)
            return true;
        return false;
    }

    int minEatingSpeed(vector<int>& piles, int h) 
    {
        // 最小且达标的速度，范围[l,r]
        int l = 1;//答案区间的左端
        int r = 0;
        for (auto& ele : piles)
            r = max(r, ele);
            
        int ans = 0;
        while (l <= r)// [l,r]不停二分
        {
            int m = l + ((unsigned)(r - l) >> 1);
            if (f(m, h, piles))
            {
                ans = m;
                r = m - 1;
            }
            else
                l = m + 1;
        }
        return ans;
    }
};




//题目二：
// 分割数组的最大值(画匠问题)
// 给定一个非负整数数组 nums 和一个整数 m
// 你需要将这个数组分成 m 个非空的连续子数组。
// 设计一个算法使得这 m 个子数组各自和的最大值最小。
// 测试链接 : https://leetcode.cn/problems/split-array-largest-sum/
//单调性：「元素和的最大值」越小，需要划分出的段数就越多；「元素和的最大值」越大，需要划分出的段数就越少
//时间复杂度：O(n*log2(sum)) -->>每次二分都调用f函数遍历一遍数组，因此是nlogn
class Solution
{
public:
    int f(const vector<int>& nums, int m)
    {
        int group = 1;//至少有一组
        int sum = 0;//临时子数组的和
        for (auto& num : nums)
        {
            if (num > m)//数组元素大于当前划分子数组的标准
                return INT_MAX;

            if (sum + num > m)//之前某个子数组的和 + 当前元素 超出 划分范围
            {
                group++;
                sum = num;//将临时子数组和设置为当前元素
            }
            else
                sum += num;
        }
        return group;
    }

    int splitArray(vector<int>& nums, int k)
    {
        //答案所在的范围必定在 0~数组累加和 之间
        int maxs = 0;
        int sum = 0;
        for (auto& ele : nums)
        {
            maxs = max(maxs, ele);
            sum += ele;
        }
            
        int ans = 0;
        for (int l = maxs, r = sum; l <= r;)
        {
            int m = l + ((unsigned)(r - l) >> 1);
            int cur = f(nums, m);//子数组按每组和为m分一组，能分几组
            if (cur <= k)
            {
                //当 cur <= k 时，说明当前 m 可以满足分成最多 k 组，可以尝试更小的 m
                r = m - 1;
                ans = m;
            }
            else
                l = m + 1;
        }
        return ans;
    }
};
//答疑
//问：如何保证二分结果一定对应着一个划分成 k 段的方案？
//
//答：把二分结果代入 check 函数，可以得到一个划分成 ≤k 段的方案。在此基础上，可以得到划分成 k 段的方案。比如划分成 k−1 段，那么把其中的一个长度至少为 2 的段分成两段，这两段的元素和都比原来的一段小，也满足题目要求，这样就得到了一个划分成 k 段的方案。换句话说，题目其实相当于：把数组划分成至多 k 段。
//
//问：设二分算出来的答案为 ans，如何保证至少有一个子数组的元素和恰好等于 ans？
//
//答：用反证法证明。假设所有子数组的元素和都小于 ans，也就是小于等于 ans−1。这意味着 check(ans−1) = true，但是二分结束后必定有 check(ans−1) = false，矛盾，所以至少有一个子数组的元素和恰好等于 ans。
//





//题目三：
// 机器人跳跃问题
// 机器人正在玩一个古老的基于DOS的游戏
// 游戏中有N+1座建筑，从0到N编号，从左到右排列
// 编号为0的建筑高度为0个单位，编号为i的建筑的高度为H(i)个单位
// 起初机器人在编号为0的建筑处
// 每一步，它跳到下一个（右边）建筑。假设机器人在第k个建筑，且它现在的能量值是E
// 下一步它将跳到第个k+1建筑
// 它将会得到或者失去正比于与H(k+1)与E之差的能量
// 如果 H(k+1) > E 那么机器人就失去H(k+1)-E的能量值，否则它将得到E-H(k+1)的能量值
// 游戏目标是到达第个N建筑，在这个过程中，能量值不能为负数个单位
// 现在的问题是机器人以多少能量值开始游戏，才可以保证成功完成游戏
// 测试链接 : https://www.nowcoder.com/practice/7037a3d57bbd4336856b8e16a9cafd71
#include <iostream>
#include <vector>
using namespace std;
int n;
int arr[100005];
int maxs;//记录数组当中最大值，用于剪枝，防止f函数运行时溢出

bool f(int m) {
    for (int i = 1; i <= n; i++) {
        if (arr[i] > m)
            m -= arr[i] - m;
        else
            m += m - arr[i];

        // 注意！
        // 如果给的能量值很大，那么后续能量增长将非常恐怖
        // 完全有可能超出long的范围
        // 所以要在遍历时，一定要加入m >= max的判断
        // 一旦能量超过高度最大值，后面肯定通关了，可以提前返回了
        // 这里很阴
        if (m >= maxs)
            return true;
        if (m < 0)
            return false;
    }
    return true;
}

// [l,r]通关所需最小能量的范围，不停二分
// max是所有建筑的最大高度
// 时间复杂度O(n * log(max))，额外空间复杂度O(1)
int compute() {
    int l = 0, r = maxs;
    int ans = 0;
    while (l <= r) {
        int m = l + ((unsigned)(r - l) >> 1);
        if (f(m)) {
            r = m - 1;
            ans = m;
        }
        else
            l = m + 1;
    }

    return ans;
}

int main() {
    cin >> n;
    for (int i = 1; i <= n; i++) {
        cin >> arr[i];
        maxs = max(maxs, arr[i]);
    }

    cout << compute();

    return 0;
}




//题目四：
// 找出第K小的数对距离
// 数对 (a,b) 由整数 a 和 b 组成，其数对距离定义为 a 和 b 的绝对差值。
// 给你一个整数数组 nums 和一个整数 k
// 数对由 nums[i] 和 nums[j] 组成且满足 0 <= i < j < nums.length
// 返回 所有数对距离中 第 k 小的数对距离。
// 测试链接 : https://leetcode.cn/problems/find-k-th-smallest-pair-distance/
//
//第 k 小/大问题的通用转化方法：  （非常重要）
//第 k 小等价于：求最小的 x，满足绝对差 ≤x 的数对至少有 k 个。（注意是至少不是恰好）
//第 k 大等价于：求最大的 x，满足绝对差 ≥x 的数对至少有 k 个。

class Solution 
{
public:// 时间复杂度O(n * log(n) + log(max-min) * n)，额外空间复杂度O(1)
    int smallestDistancePair(vector<int>& nums, int k) 
    {
        sort(nums.begin(), nums.end());//将nums数组排好序，便于接下来滑动窗口使用
        int ret = 0;
        for (int l = 0, r = nums.back() - nums.front(); l <= r;)
        {
            int m = l + ((unsigned)(r - l) >> 1);
            int pairs = compute(m, nums);//计算数对距离<=m的数对个数
            if (pairs >= k)
            {
                ret = m;
                r = m - 1;
            }
            else
                l = m + 1;
        }

        return ret;
    }

    int compute(int m, const vector<int>& nums)
    {
        int n = nums.size();
        int ans = 0;
        for (int l = 0, r = 1; l < n; l++)
        {
            while (r < n && nums[r] - nums[l] <= m)
                r++;
            ans += r - l - 1;
            // 以 i 为左端点的有效数对有 (j - i - 1) 个
            //计算ans时，r来到符合要求的位置的下一个位置，因此减1
            //[0...3]
            //[0,1] [0,2]
        }
        return ans;
    }
};




//题目五：
// 同时运行N台电脑的最长时间
// 你有 n 台电脑。给你整数 n 和一个下标从 0 开始的整数数组 batteries
// 其中第 i 个电池可以让一台电脑 运行 batteries[i] 分钟
// 你想使用这些电池让 全部 n 台电脑 同时 运行。
// 一开始，你可以给每台电脑连接 至多一个电池
// 然后在任意整数时刻，你都可以将一台电脑与它的电池断开连接，并连接另一个电池，你可以进行这个操作 任意次
// 新连接的电池可以是一个全新的电池，也可以是别的电脑用过的电池
// 断开连接和连接新的电池不会花费任何时间。
// 注意，你不能给电池充电。
// 请你返回你可以让 n 台电脑同时运行的 最长 分钟数。
// 测试链接 : https://leetcode.cn/problems/maximum-running-time-of-n-computers/
//核心：若碎片电池的总时间 > 所需电池总时间 -->>可以完成
class Solution 
{
public:
    long long maxRunTime(int n, vector<int>& batteries) 
    {
        long long sum = 0;
        for (auto& ele : batteries)
            sum += ele;

        long long ans = 0;
        for (long long l = 0, r = sum; l <= r;)
        {
            long long m = l + (r - l) / 2;
            if (f(m, n, batteries))
            {
                ans = m;
                l = m + 1;
            }
            else
                r = m - 1;
        }

        return ans;
    }

    bool f(long long aim, int nums, const vector<int>& arr)
    {
        long long time = 0;
        for (int i = 0; i < arr.size(); i++)
        {
            if (arr[i] >= aim)
                nums--;//碎片电池所需供应的电脑数目减少
            else
                time += arr[i];

            if (time >= (long long)nums * aim)
                return true;
        }
        return false;
    }
};

//优化： 二分答案法 + 增加分析(贪心)
class Solution
{
public:
    long long maxRunTime(int n, vector<int>& batteries)
    {
        long long sum = 0;
        int maxs = 0;
        for (auto& ele : batteries)
        {
            sum += ele;
            maxs = max(maxs, ele);
        }

        if (sum >= (long long)maxs * n)
            return sum / n;
        // 所有电池的最大电量是max
        // 如果此时sum > (long) max * num，
        // 说明 : 最终的供电时间一定在 >= max，而如果最终的供电时间 >= max
        // 说明 : 对于最终的答案X来说，所有电池都是课上讲的"碎片拼接"的概念
        // 那么寻找 ? * num <= sum 的情况中，尽量大的 ? 即可
        // 即sum / num
            
        //如果没中上面那个if，
        // 最终的供电时间一定在 < max范围上
        // [0, sum]二分范围，可能定的比较粗，虽然不影响，但毕竟是有点慢
        // [0, max]二分范围！更精细的范围，二分次数会变少
        long long ans = 0;
        for (long long l = 0, r = maxs; l <= r;)
        {
            long long m = l + (r - l) / 2;
            if (f(m, n, batteries))
            {
                ans = m;
                l = m + 1;
            }
            else
                r = m - 1;
        }

        return ans;
    }

    //由于优化，下面的数据类型就可以用int整型，但我懒得改了
    bool f(long long aim, int nums, const vector<int>& arr)
    {
        long long time = 0;
        for (int i = 0; i < arr.size(); i++)
        {
            if (arr[i] >= aim)
                nums--;//碎片电池所需供应的电脑数目减少
            else
                time += arr[i];

            if (time >= (long long)nums * aim)
                return true;
        }
        return false;
    }
};




//题目六：
// 计算等位时间
// 给定一个数组arr长度为n，表示n个服务员，每服务一个人的时间
// 给定一个正数m，表示有m个人等位，如果你是刚来的人，请问你需要等多久？
// 假设m远远大于n，比如n <= 10^3, m <= 10^9，该怎么做是最优解？
// 谷歌的面试，这个题连考了2个月
// 找不到测试链接，所以用对数器验证

//法一：堆模拟
// 验证方法，不是重点
// 如果m很大，该方法会超时
// 时间复杂度O(m * log(n))，额外空间复杂度O(n)
class Solution
{
private:
    struct Waiter
    {
        int num;        // 编号（可选）
        int serviceTime; // 服务一个客人的时间（固定）
        int wakeTime;   // 醒来/可用时间（会变化）
    };

public:
    struct WaiterCmp
    {
        // 按照wakeTime（醒来时间）排序，最小堆
        bool operator()(const Waiter& a, const Waiter& b) const
        {
            return a.wakeTime > b.wakeTime;
        }
    };

    // 添加参数m，表示服务次数
    int heap_solve(vector<int>& arr, int m)
    {
        // 最小堆，按照wakeTime排序
        priority_queue<Waiter, vector<Waiter>, WaiterCmp> pq;

        // 初始化所有服务者
        for (int i = 0; i < arr.size(); i++)
        {
            // 初始wakeTime为0，serviceTime为arr[i]
            Waiter tmp = { i, arr[i], 0 };
            pq.push(tmp);
        }

        // 模拟m次服务
        for (int i = 0; i < m; i++)
        {
            Waiter tmp = pq.top();
            pq.pop();

            // 更新醒来时间：当前醒来时间 + 服务时间
            tmp.wakeTime += tmp.serviceTime;

            pq.push(tmp);
        }

        // 返回最早可用的服务者的时间
        return pq.top().wakeTime;
    }
};

//法二：二分答案法
// 最优解
// 时间复杂度O(n * log(min * w))，额外空间复杂度O(1)
class Solution
{
public:
    int Binary_solve(vector<int>& arr)
    {
        int n = arr.size();
        int mins = INT_MAX;
        for (auto& ele : arr)
            mins = min(mins, ele);

        long long ans = 0;
        for (long long l = 0, r = (long long)mins * n; l <= r;)
        {
            long long m = l + (r - l) / 2;
            if (f(m, arr) >= n + 1)
            {
                ans = m;
                r = m - 1;
            }
            else
                l = m + 1;
        }
        return ans;
    }

    int f(int m, const vector<int>& arr)
    {
        int ans = 0;
        for (int i = 0; i < arr.size(); i++)
        {
            ans += m / arr[i] + 1;//刚刚接待的客人也算, (完成 + 开始)服务的人数，需要+1
        }
        return ans;
    }
};

// 完成旅途的最少时间(题目6的在线测试)
// 有同学找到了在线测试链接，和课上讲的题目6几乎是一个意思，但是有细微差别
// 实现的代码，除了一些变量需要改成long类型之外，仅有两处关键逻辑不同，都打上了注释
// 除此之外，和课上讲的题目6的实现，再无区别
// 可以仔细阅读如下测试链接里的题目，重点关注此题和题目6，在题意上的差别
// 测试链接 : https://leetcode.cn/problems/minimum-time-to-complete-trips/
class Solution 
{
public:
    long long minimumTime(vector<int>& time, int totalTrips)
    {
        int mins = INT_MAX;
        for (auto& ele : time)
            mins = min(mins, ele);

        long long l = 0;
        long long r = (long long)mins * totalTrips;
        long long ans = 0;
        while (l <= r)
        {
            long long m = l + (r - l) / 2;
            if (f(m, time) >= totalTrips)
            {
                ans = m;
                r = m - 1;
            }
            else
                l = m + 1;
        }
        return ans;
    }

    long long f(long long m, const vector<int>& time)
    {
        long long ans = 0;
        for (auto& ele : time)
            ans += m / ele; //能完成多少旅行，不需要 + 1
        return ans;
    }
};




//题目七：
// 刀砍毒杀怪兽问题
// 怪兽的初始血量是一个整数hp，给出每一回合刀砍和毒杀的数值cuts和poisons
// 第i回合如果用刀砍，怪兽在这回合会直接损失cuts[i]的血，不再有后续效果
// 第i回合如果用毒杀，怪兽在这回合不会损失血量，但是之后每回合都损失poisons[i]的血量
// 并且你选择的所有毒杀效果，在之后的回合都会叠加
// 两个数组cuts、poisons，长度都是n，代表你一共可以进行n回合
// 每一回合你只能选择刀砍或者毒杀中的一个动作
// 如果你在n个回合内没有直接杀死怪兽，意味着你已经无法有新的行动了
// 但是怪兽如果有中毒效果的话，那么怪兽依然会在血量耗尽的那回合死掉
// 返回至少多少回合，怪兽会死掉
// 数据范围 : 
// 1 <= n <= 10^5
// 1 <= hp <= 10^9
// 1 <= cuts[i]、poisons[i] <= 10^9
// 本题来自真实大厂笔试，找不到测试链接，所以用对数器验证
class Solution
{
public:
    int fast(vector<int>& cut, vector<int>& poison, int hp)
    {
        int ans = 0;
        for (int l = 1, r = hp + 1; l <= r;)
        {
            int m = l + (r - l) / 2;
            if (f(cut, poison, hp, m))
            {
                ans = m;
                r = m - 1;
            }
            else
                l = m + 1;
        }
        return ans;
    }

    bool f(vector<int>& cut, vector<int>& poison, int hp, int limitedRound)
    {
        int poison_hurt = 0;
        int n = min(static_cast<int>(cut.size()), limitedRound);//限制的回合 和 可以行动的回合 取最小值
        //这里用min函数要记得将.size()方法类型转换一下，否则会报错
        for (int i = 0; i < n; i++)
        {
            hp -= poison_hurt;
            if ((long long)poison[i] * (n - i - 1) > cut[i])
                poison_hurt += poison[i];
            else
                hp -= cut[i];

            if (hp <= 0)
                return true;
        }

        return false;
    }
};



// 动态规划方法(只是为了验证)
// 目前没有讲动态规划，所以不需要理解这个函数
// 这个函数只是为了验证二分答案的方法是否正确的
// 纯粹为了写对数器验证才设计的方法，血量比较大的时候会超时
// 这个方法不做要求，此时并不需要理解，可以在学习完动态规划章节之后来看看这个函数

// 辅助递归函数
int f1(vector<int>& cuts, vector<int>& poisons, int i, int r, int p, vector<vector<vector<int>>>& dp) {
    r -= p;
    if (r <= 0) {
        return i + 1;
    }
    if (i == cuts.size()) {
        if (p == 0) {
            return INT_MAX;
        }
        else {
            return cuts.size() + 1 + (r + p - 1) / p;
        }
    }
    if (dp[i][r][p] != 0) {
        return dp[i][r][p];
    }
    int p1 = r <= cuts[i] ? (i + 1) : f1(cuts, poisons, i + 1, r - cuts[i], p, dp);
    int p2 = f1(cuts, poisons, i + 1, r, p + poisons[i], dp);
    int ans = min(p1, p2);
    dp[i][r][p] = ans;
    return ans;
}

// 主函数
int fast1(vector<int>& cuts, vector<int>& poisons, int hp) {
    int sum = 0;
    for (int num : poisons) {
        sum += num;
    }
    // 创建三维dp数组，初始化为0
    vector<vector<vector<int>>> dp(cuts.size(),
        vector<vector<int>>(hp + 1,
            vector<int>(sum + 1, 0)));
    return f1(cuts, poisons, 0, hp, 0, dp);