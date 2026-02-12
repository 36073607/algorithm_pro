#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>
using namespace std;



//双向广搜常见用途1：小优化
//bfs的剪枝策略，分两侧展开分支，哪侧数量少就从哪侧展开


//双向广搜常见用途2：重要！本体！用于解决特征很明显的一类问题
//特征：全量样本不允许递归完全展开，但是半量样本可以完全展开
//过程：把数据分成两部分，每部分 各自展开 计算结果，然后设计两部分结果的 整合逻辑
//
//思想类似归并分治，不过最终整合答案时，
// 归并分治是对多次的“折半（二分）”进行整合；折半搜索只需整合一次
//与其说是双向广搜，我觉得更像是 折半DFS



//用途一：
// 
//题目一：
// 单词接龙
// 字典 wordList 中从单词 beginWord 和 endWord 的 转换序列
// 是一个按下述规格形成的序列 beginWord -> s1 -> s2 -> ... -> sk ：
// 每一对相邻的单词只差一个字母。
// 对于 1 <= i <= k 时，每个 si 都在 wordList 中
// 注意， beginWord 不需要在 wordList 中。sk == endWord
// 给你两个单词 beginWord 和 endWord 和一个字典 wordList
// 返回 从 beginWord 到 endWord 的 最短转换序列 中的 单词数目
// 如果不存在这样的转换序列，返回 0 。
// 测试链接 : https://leetcode.cn/problems/word-ladder/
class Solution 
{
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) 
    {
        unordered_set<string> dict;//词典，将wordList转化为哈希表，便于查找
        unordered_set<string> bigLevel;//拥有较大单词数目的层级
        unordered_set<string> smallLevel;//拥有较小单词数目的层级
        unordered_set<string> nextLevel;//smallLevel的下一层级

        dict.insert(wordList.begin(), wordList.end());//转化

        if (!dict.count(endWord)) return 0;//特判

        smallLevel.insert(beginWord);
        bigLevel.insert(endWord);
        int level = 2;
        while (!smallLevel.empty())
        {
            for (auto& word : smallLevel)//删除已经操作过的字符串，避免重复加入层级
            {
                dict.erase(word);
            }

            for (auto& ele : smallLevel)
            {
                for (int i = 0; i < ele.size(); i++)
                {
                    string nextword = ele;
                    for (char ch = 'a'; ch <= 'z'; ch++)
                    {
                        nextword[i] = ch;
                        if (bigLevel.count(nextword))//两个层级相通，说明已经遍历完毕
                            return level;

                        if (nextword != ele && dict.count(nextword))//词典中有该单词
                        {
                            nextLevel.insert(nextword);
                        }
                    }
                }
            }

            if (nextLevel.size() >= bigLevel.size())
            {
                smallLevel = bigLevel;
                bigLevel = nextLevel;
                nextLevel.clear();
            }
            else
            {
                smallLevel = nextLevel;
                nextLevel.clear();
            }

            level++;
        }

        return 0;
    }
};
//
//注：
//STL容器接口的哲学：
//insert()：可以接受来自任何容器的迭代器，因为它只是读取数据。
//erase()：只能接受自身容器的迭代器，因为它需要操作自身的内存布局。



//用途二：
//
//题目二：
// 牛牛的背包问题 & 世界冰球锦标赛
// 牛牛准备参加学校组织的春游, 出发前牛牛准备往背包里装入一些零食, 牛牛的背包容量为w。
// 牛牛家里一共有n袋零食, 第i袋零食体积为v[i]。
// 牛牛想知道在总体积不超过背包容量的情况下,他一共有多少种零食放法(总体积为0也算一种放法)。
// 输入描述：
// 输入包括两行
// 第一行为两个正整数n和w(1 <= n <= 30, 1 <= w <= 2 * 10^9),表示零食的数量和背包的容量
// 第二行n个正整数v[i](0 <= v[i] <= 10^9),表示每袋零食的体积
// 输出描述：
// 输出一个正整数, 表示牛牛一共有多少种零食放法。
// 测试链接 : https://www.luogu.com.cn/problem/P4799
//
//思路：
//如果直接进行搜索(dfs)，每个数据有 (在 / 不在) 两种情况，则时间复杂度为 2^n ，且本题数据量大，不能通过
//所以进行折半搜索，将原始数组分为相等长度的两部分，对两个子部分搜索的时间复杂度为 2^(n/2) ，时间复杂度大大降低
//最后在将两部分的答案整合即可
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
const int MAXN = 40;
//比赛数量最大为40，折半后为20
//对于所有比赛想要完全展开搜索的话，分为 有该比赛 和 无该比赛 两种情况，共有 2 ^ n 种情况
const int MAXM = 1 << 20;
long long n, m;
vector<long long> arr;
//记录arr数组左右两部分的前缀和信息
vector<long long> lsum(MAXM);
vector<long long> rsum(MAXM);

//[l, r)
long long dfs(vector<long long>& vec, long long l ,long long r, long long sum, long long limit,long long idx)
{
    if (sum > limit)
        return idx;
    
    if (l == r)//arr半部分搜索完毕
        vec[idx++] = sum;
    else
    {
        idx = dfs(vec, l + 1, r, sum, limit, idx);//不要l位置上的数
        idx = dfs(vec, l + 1, r, sum + arr[l], limit, idx);//要l位置上的数
    }
                      
    return idx;
}

long long compute(long long limit)
{
    long long lsize = dfs(lsum, 0, n / 2, 0, limit, 0);//这个大小即为arr左半部分的最终方法总数
    long long rsize = dfs(rsum, n / 2, n, 0, limit, 0);//这个大小即为arr右半部分的最终方法总数
    //排序，维持单调性，为了使用双指针
    sort(lsum.begin(), lsum.begin() + lsize);
    sort(rsum.begin(), rsum.begin() + rsize);
    //双指针
    // lsize / rsize 中每一个值都代表一种方法
    long long ans = 0;
    for (long long i = lsize - 1, j = 0; i >= 0; i--)
    {
        while (j < rsize && lsum[i] + rsum[j] <= limit)
        {
            j++;
        }
        ans += j;
    }

    return ans;
}

int main()
{
    cin >> n >> m;
    for (int i = 0; i < n; i++)
    {
        long long ipt = 0;
        cin >> ipt;
        arr.push_back(ipt);
    }

    long long ans = compute(m);
    cout << ans;

    return 0;
}



//题目三：
// 最接近目标值的子序列和
// 给你一个整数数组 nums 和一个目标值 goal
// 你需要从 nums 中选出一个子序列，使子序列元素总和最接近 goal
// 也就是说，如果子序列元素和为 sum ，你需要 最小化绝对差 abs(sum - goal)
// 返回 abs(sum - goal) 可能的 最小值
// 注意，数组的子序列是通过移除原始数组中的某些元素（可能全部或无）而形成的数组。
// 数据量描述:
// 1 <= nums.length <= 40
// -10^7 <= nums[i] <= 10^7
// -10^9 <= goal <= 10^9
// 测试链接 : https://leetcode.cn/problems/closest-subsequence-sum/
//
//注意：这里是子序列，不是子数组，所以不要求连续
//思路：依旧折半DFS，分别得到序列和数组；对两个序列和数组排序，利用双指针
class Solution
{
private:
    //static const int MAXSIZE = 40;
    vector<int> lsum;
    vector<int> rsum;
    vector<int> nums;

    //[l, r)
    void dfs(vector<int>& vec, int l, int r, int sum)
    {
        if (l == r)
        {
            vec.push_back(sum);
            return;
        }
            
        int nextGroupIdx = l + 1 >= r ? r : l + 1;
        while (nextGroupIdx < r && nums[l] == nums[nextGroupIdx])
            nextGroupIdx++;

        int sameNum = nextGroupIdx - l;
        for (int i = 0; i <= sameNum; i++)
            dfs(vec, nextGroupIdx, r, sum + i * nums[l]);
    }

public:
    int minAbsDifference(vector<int>& ns, int goal) 
    {
        nums = ns;//把传入的数组放到全局
        int n = nums.size();
        //剪枝1，将数组中大于0和小于0的数分别求和，与goal直接比较
        int maxs = 0, mins = 0;
        for (int i = 0; i < n; i++)
        {
            if (nums[i] > 0)
                maxs += nums[i];
            if (nums[i] < 0)
                mins += nums[i];
        }
        if (maxs < goal)
            return abs(goal - maxs);
        if (mins > goal)
            return abs(goal - mins);

        int ans = abs(goal);//数组中任何一个数字都不选
        //剪枝2：将nums数组排序，采用分组的方式进行dfs
        //eg: 1111 222 33333 ……
        //0个1 -> 调用dfs
        //1个1 -> 调用dfs
        //…………
        sort(nums.begin(), nums.end());
        dfs(lsum, 0, n / 2, 0);
        dfs(rsum, n / 2, n, 0);
        int lsize = lsum.size();
        int rsize = rsum.size();
        //双指针--滑动窗口
        sort(lsum.begin(), lsum.end());
        sort(rsum.begin(), rsum.end());
        for (int i = 0, j = rsize - 1; i < lsize; i++)
        {
            //特别注意：当相等时指针也移动，否则无法遍历所有情况
            while (j > 0 && abs(goal - lsum[i] - rsum[j]) >= abs(goal - lsum[i] - rsum[j - 1]))
                j--;
            ans = min(ans, abs(goal - lsum[i] - rsum[j]));
        }

        return ans;
    }
};
//
//关于“最接近目标值的子序列和”的这一类问题：
//如果数组长度特别大，但是值小，我们可以使用背包问题的方式来解决，
//但是如果数组长度不大，但是数值特别大的话，使用DFS。
