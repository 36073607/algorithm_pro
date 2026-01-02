#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <climits>
#include <unordered_map>
using namespace std;


//滑动窗口：维持左、右边界都不回退的一段范围，来求解很多子数组(串)的相关问题

//滑动窗口的关键：找到 范围 和 答案指标 之间的 单调性关系（类似贪心） -->>[l,r)范围越小/越大。答案会呈现单调趋势

//滑动过程：滑动窗口可以用 简单变量 或者 结构 来 维护信息  -->>用数组、哈希表等都可以维护

//求解大流程：求子数组在 每个位置 开头 或 结尾 情况下的答案（开头还是结尾在于个人习惯、具体题目）

//注意：
//滑动窗口维持最大值 或者 最小值的更新结构，在 单调队列 视频里讲述



//题目一：
// 累加和大于等于target的最短子数组长度
// 给定一个含有 n 个正整数的数组和一个正整数 target
// 找到累加和 >= target 的长度最小的子数组并返回其长度
// 如果不存在符合条件的子数组返回0
// 测试链接 : https://leetcode.cn/problems/minimum-size-subarray-sum/
// 时间复杂度: O(n)
class Solution 
{
public:
    int minSubArrayLen(int target, vector<int>& nums) 
    {
        int ans = INT_MAX;
        for (int l = 0, r = 0, sum = 0; r < nums.size(); r++)
        {
            sum += nums[r];
            while (sum - nums[l] >= target)
            {
                // sum : nums[l....r]
                // 如果l位置的数从窗口出去，还能继续达标，那就出去
                sum -= nums[l++];
            }

            if (sum >= target)
                ans = min(ans, r - l + 1);
        }

        return ans == INT_MAX ? 0 : ans;
    }
};



//题目二：
// 无重复字符的最长子串
// 给定一个字符串 s ，请你找出其中不含有重复字符的 最长子串 的长度。
// 测试链接 : https://leetcode.cn/problems/longest-substring-without-repeating-characters/
class Solution 
{
public:
    int lengthOfLongestSubstring(string s)
    {
        vector<int> last_index(256, -1);//记录每个字母最后一次出现时的下标   直接用哈希表也可以
        //char -> int -> 0~255
        int ans = 0;
        for (int l = 0, r = 0; r < s.size(); r++)
        {
            l = max(l, last_index[s[r]] + 1);
            ans = max(ans, r - l + 1);
            last_index[s[r]] = r;// 更新当前字符上一次出现的位置
        }
        return ans;
    }
};



//题目三：
// 最小覆盖子串
// 给你一个字符串 s 、一个字符串 t 。返回 s 中涵盖 t 所有字符的最小子串
// 如果 s 中不存在涵盖 t 所有字符的子串，则返回空字符串 "" 。
// 测试链接 : https://leetcode.cn/problems/minimum-window-substring/
//核心：构建负债表
class Solution 
{
public:
    string minWindow(string s, string t) 
    {
        unordered_map<char, int> ump;//负债表
        int debt = -t.size();//总负债
        int cnt = INT_MAX;
        string ans("");
        for (int i = 0; i < t.size(); i++)
            ump[t[i]]--;
            
        for (int l = 0, r = 0; r < s.size(); r++)
        {
            if (ump[s[r]]++ < 0)//这一下判断就是一个顺序点,所以每个字符对应的数值都会增加。由于后置++,所以判断完后再++
                debt++;
            
            if (debt == 0)//当“负债”还清时移动左窗口
            {
                while (ump[s[l]] > 0)
                {
                    ump[s[l++]]--;
                    //ump[s[l]]--
                    //l++
                }

                if (r - l + 1 < cnt)
                {
                    cnt = r - l + 1;
                    ans = s.substr(l, cnt);
                }

            }
        }
        return ans;
    }
};



//题目四：
// 加油站
// 在一条环路上有 n 个加油站，其中第 i 个加油站有汽油 gas[i] 升。
// 你有一辆油箱容量无限的的汽车，从第 i 个加油站开往第 i+1 个加油站需要消耗汽油 cost[i] 升
// 你从其中的一个加油站出发，开始时油箱为空。
// 给定两个整数数组 gas 和 cost ，如果你可以按顺序绕环路行驶一周
// 则返回出发时加油站的编号，否则返回 -1
// 如果存在解，则 保证 它是 唯一 的。
// 测试链接 : https://leetcode.cn/problems/gas-station/
class Solution 
{
public:
    int canCompleteCircuit(vector<int>& gas, vector<int>& cost) 
    {
        int n = gas.size();
        // 窗口范围是[l, r)，左闭右开，也就是说窗口是[l..r-1]，r是到不了的位置
        for (int l = 0, r = 0, sum = 0; l < n;l = r + 1,r = l)
        {
            sum = 0;//重置sum,防止某一次while循环退出时sum会影响下一轮判断
            while (sum + gas[r % n] - cost[r % n] >= 0)//可以从一个位置到下一个位置
            {
                // r位置即将右扩，窗口会变大
                if (r - l + 1 == n)// 此时检查是否已经转了一圈
                {
                    return l;
                }
                // r位置进入窗口，累加和加上r位置的余量
                sum += gas[r % n] - cost[r % n];
                r++;
            }
        }

        return -1;
    }
};



//题目五：
// 替换子串得到平衡字符串
// 有一个只含有 'Q', 'W', 'E', 'R' 四种字符，且长度为 n 的字符串。
// 假如在该字符串中，这四个字符都恰好出现 n/4 次，那么它就是一个「平衡字符串」。
// 给你一个这样的字符串 s，请通过「替换一个子串」的方式，使原字符串 s 变成一个「平衡字符串」。
// 你可以用和「待替换子串」长度相同的 任何 其他字符串来完成替换。
// 请返回待替换子串的最小可能长度。
// 如果原字符串自身就是一个平衡字符串，则返回 0。
// 测试链接 : https://leetcode.cn/problems/replace-the-substring-for-balanced-string/

//核心：转化问题并构建 debt表
class Solution
{
public:
    int balancedString(string s)
    {
        int n = s.size();
        unordered_map<char, int> ump;
        for (int i = 0; i < n; i++)
            ump[s[i]]++;
        int debt = 0;//总债务
        for (auto& it : ump)//出现次数多于n/4的字母“负债”
        {
            if (it.second > n / 4)
            {
                it.second = -(it.second - n / 4);
                debt -= it.second;//减负的，变成正
            }
            else
                it.second = 0;
        }

        int ans = INT_MAX;
        for (int l = 0, r = 0; r < n; r++)//找到长度最小的、可以清空“负债”的子串（让该子串变成剩余字母即可满足题意）
        {
            if (ump[s[r]]++ < 0)
            {
                debt--;
            }

            if (debt == 0)
            {
                while (ump[s[l]] > 0)
                {
                    ump[s[l++]]--;
                }
                ans = min(ans, r - l + 1);
            }
        }
        return ans == INT_MAX ? 0 : ans;
    }
};



//题目六：
// K个不同整数的子数组
// 给定一个正整数数组 nums和一个整数 k，返回 nums 中 「好子数组」 的数目。
// 如果 nums 的某个子数组中不同整数的个数恰好为 k
// 则称 nums 的这个连续、不一定不同的子数组为 「好子数组 」。
// 例如，[1,2,3,1,2] 中有 3 个不同的整数：1，2，以及 3。
// 子数组 是数组的 连续 部分。
// 测试链接 : https://leetcode.cn/problems/subarrays-with-k-different-integers/
//
// 题目要求 严格等于k 没有“单调性”，因此需要转化：ans:<=k - ans:<=(k-1)
//随着范围扩大，就越可能超过要求的种类-->>形成一种弱单调性
class Solution 
{
public:
    int subarraysWithKDistinct(vector<int>& nums, int k) 
    {
        return numsOfMostKinds(nums, k) - numsOfMostKinds(nums, k - 1);
    }

    int numsOfMostKinds(const vector<int>& nums, int k)
    {
        int ans = 0;
        unordered_map<int, int> ump;
        //int ump[20001]={0};直接用数组模拟哈希表也可以
        for (int l = 0, r = 0, collect_kind = 0; r < nums.size(); r++)
        {
            if (++ump[nums[r]] == 1)//第一次加入
                collect_kind++;//收集到的种类增加

            while (collect_kind > k)
            {
                if (--ump[nums[l++]] == 0)
                    collect_kind--;
            }
            ans += r - l + 1;//收集到以r为结尾、满足要求的所有子串
        }
        return ans;
    }

};



//题目七：
// 至少有K个重复字符的最长子串
// 给你一个字符串 s 和一个整数 k ，请你找出 s 中的最长子串
// 要求该子串中的每一字符出现次数都不少于 k 。返回这一子串的长度
// 如果不存在这样的子字符串，则返回 0。
// 测试链接 : https://leetcode.cn/problems/longest-substring-with-at-least-k-repeating-characters/
class Solution 
{
public:
    int longestSubstring(string s, int k)
    {
        int ans = 0;
        for (int require = 1; require <= 26; require++)
        {
            vector<int> hash(256, 0);//记录每个字符出现的次数
            //collect_kind:窗口中一共收集到的种类数
            //aim:窗口中达标的种类数(次数>=k)
            for (int l = 0, r = 0, collect_kind = 0, aim = 0; r < s.size(); r++)
            {
                if (hash[s[r]] == 0)
                    collect_kind++;//收集到的种类增加
                if (hash[s[r]] == k - 1)
                    aim++;//某个种类字符出现次数达到要求，aim增加
                hash[s[r]]++;

                // l....r 种类超了！
                // l位置的字符，窗口中吐出来！
                while (collect_kind > require)
                {
                    if (hash[s[l]] == 1)
                        collect_kind--;
                    if (hash[s[l]] == k)
                        aim--;

                    hash[s[l++]]--;
                }
               
                if (require == aim)
                    ans = max(ans, r - l + 1);
            }
        }
        return ans;
    }
};
