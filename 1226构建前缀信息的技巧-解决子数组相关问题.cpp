#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

//子数组必须是连续的
//构建某个前缀信息: 最早/晚出现的位置、出现次数... ，是很常见的技巧
//除此之外，还有很多种类的前缀信息可以构建出来，解决很多子数组相关问题
//-->>借助哈希表实现
//---->>一定要记得处理前缀为空时，哈希表的状态！！！ ump[0]=...

//tips:关于前缀和数组的两种方式：
//1. vector<int> sum(n+1) , 下标表示一共i个数字的前缀和，因此需要在sum[0]设置sum[0]=0  eg:sum[2] = arr[0]+arr[1] = sum[1]+arr[1]
//下标为 L~R 上的数组元素之和 = ∑(0~R) - ∑(0~L-1) = sum[R+1] - sum[L]  -->> 0~R 一共(R+1)个数，所以sum[R+1] ; 0~L-1 一共(L)个数，所以sum[L]
//
//2. vector<int> sum(n) , 下标表示 0-i 的前缀和 eg: sum[2] = arr[0] + arr[1] + arr[2] = sum[1]+arr[2]
//下标为 L~R 上的数组元素之和 = ∑(0~R) - ∑(0~L-1) = sum[R] - sum[L-1]  -->>0~R sum[R] ; 0~L-1 sum[L-1]
//-->>但由于(L-1)有数组越界的风险，所以不推荐

//题目一：构建 前缀和数组。快速解决子数组范围求和的问题
// 利用前缀和快速得到区域累加和
// 测试链接 : https://leetcode.cn/problems/range-sum-query-immutable/
//技巧：构建前缀和数组(S)
//      下标为 L~R 上的数组元素之和 = ∑(0~R) - ∑(0~L-1) = S[R+1] - S[L +1 -1](S[L])
class NumArray1
{
private:
    int sum[10001];

public:
    NumArray1(vector<int>& nums) 
    {
        sum[0] = 0;
        for (int i = 0; i < nums.size(); i++)
            sum[i + 1] = sum[i] + nums[i];
    }

    int sumRange(int left, int right)
    {
        return sum[right + 1] - sum[left];
    }
};

class NumArray2
{
public:
    vector<int> sum;

    NumArray2(vector<int>& nums)
    {
        int n = nums.size();
        sum.resize(n + 1);
        sum[0] = 0;
        for (int i = 0; i < n; i++)
            sum[i + 1] = sum[i] + nums[i];
        
    }

    int sumRange(int left, int right)
    {
        return sum[right + 1] - sum[left];
    }
};



//题目二：构建 前缀和 最早出现的位置。返回 无序数组中 累加和为给定值的 最小子数组长度
// 返回无序数组中累加和为给定值的最长子数组长度
// 给定一个无序数组arr, 其中元素可正、可负、可0
// 给定一个整数aim
// 求arr所有子数组中累加和为aim的最长子数组长度
// 测试链接 : https://www.nowcoder.com/practice/36fb0fd3c656480c92b569258a1223d5
int N, k;
vector<int> arr;
unordered_map<int, int> ump;//key:某段前缀和   val:前缀和最早出现的位置
int compute(int k)
{
    int ans = 0;
    ump.clear();
    ump[0] = -1;//前缀和为0在数组中没有数字时便已经达成 （非常重要）
    for (int i = 0, sum = 0; i < N; i++)//以i为下标结尾的子数组有多少个
    {
        sum += arr[i];//前缀和
        int aim = sum - k;
        if (ump.find(aim) != ump.end())// 如果 aim 出现过，说明存在子数组和为k
            ans = max(ans, i - ump[aim]);
        if(ump.find(sum)==ump.end())// 只记录前缀和第一次出现的位置
            ump[sum] = i;
    }

    return ans;
}

int main()
{
    cin >> N >> k;
    arr.resize(N);
    for (int i = 0; i < N; i++)
        cin >> arr[i];
    cout << compute(k);

    return 0;
}



//题目三：构建 前缀和 出现的次数。返回 无序数组中 累加和为给定值的 子数组数量
// 返回无序数组中累加和为给定值的子数组个数
// 测试链接 : https://leetcode.cn/problems/subarray-sum-equals-k/
int subarraySum(vector<int>& nums, int k)
{
    unordered_map<int, int> mp;//key:前缀和，value:出现的次数
    mp[0] = 1;//前缀和为0在数组为空时便已经成立
    int ans = 0;
    for (int i = 0, sum = 0; i < nums.size(); i++)//以i为下标结尾的子数组有多少个
    {
        sum += nums[i];//前缀和
        int aim = sum - k;
        if (mp.find(aim) != mp.end())
            ans += mp[aim];
        mp[sum]++;
    }
    return ans;
}



//题目四：构建 前缀和 最早出现的位置。返回 无序数组中 正数和负数个数相等的 最长子数组长度
// 返回无序数组中正数和负数个数相等的最长子数组长度
// 给定一个无序数组arr，其中元素可正、可负、可0
// 求arr所有子数组中正数与负数个数相等的最长子数组的长度
// 测试链接 : https://www.nowcoder.com/practice/545544c060804eceaed0bb84fcd992fb
int arr[100001];
unordered_map<int, int> ump;//key:前缀和 ， val:该前缀和最早出现位置
int N;
int operate(int k)
{
    int ans = 0;
    ump[0] = -1;//前缀和为0在数组为空时便已经实现
    for (int i = 0, sum = 0; i < N; i++)
    {
        sum += arr[i];//前缀和
        int aim = sum - k;
        if (ump.find(aim) != ump.end())
            ans = max(ans, i - ump[aim]);
        if (ump.find(sum) == ump.end())
            ump[sum] = i;
    }
    return ans;
}

int main()
{
    cin >> N;
    for (int i = 0; i < N; i++)
    {
        cin >> arr[i];
        if (arr[i] > 0)
            arr[i] = 1;
        else if (arr[i] < 0)
            arr[i] = -1;
    }

    cout << operate(0);

    return 0;
}



//题目五：构建 前缀和 最早出现的位置。表现良好的最长时间段问题
// 给你一份工作时间表 hours，上面记录着某一位员工每天的工作小时数
// 我们认为当员工一天中的工作小时数大于 8 小时的时候，那么这一天就是 劳累的一天
// 所谓 表现良好的时间段 ，意味在这段时间内，「劳累的天数」是严格 大于 不劳累的天数
// 请你返回 表现良好时间段 的最大长度
// 测试链接 : https://leetcode.cn/problems/longest-well-performing-interval/
// 
//法一：O(n^2)的暴力实现
class Solution
{
public:
    int longestWPI(vector<int>& hours) 
    {
        vector<int> invert(hours.size());
        for (int i = 0; i < hours.size(); i++) 
        {
            invert[i] = (hours[i] > 8) ? 1 : -1;
        }

        int ans = 0;
        unordered_map<int, int> ump;
        ump[0] = -1;

        for (int i = 0, sum = 0; i < invert.size(); i++)
        {
            sum += invert[i];

            // 遍历所有已记录的前缀和
            for (auto& ele : ump)
            {
                // 检查是否满足条件：当前前缀和 - 之前的前缀和 > 0
                if (sum - ele.first > 0) 
                {
                    ans = max(ans, i - ele.second);
                }
            }

            if (ump.find(sum) == ump.end())
            {
                ump[sum] = i;
            }
        }

        return ans;
    }
};

//法二：优化版本, O(n)
int longestWPI(vector<int>& hours)
{
    int ans = 0;
    unordered_map<int, int> ump;//key:前缀和 ， val:第一次出现的位置
    ump[0] = -1;//当数组为空时，其前缀和便是0
    for (int i = 0, prefix = 0; i < hours.size(); i++)
    {
        prefix += (hours[i] > 8) ? 1 : -1;
        if (prefix > 0)
            ans = max(ans, i + 1);
        else
        {
            if (ump.find(prefix - 1) != ump.end())//因为转化后的hours数组都是由1/-1构成，只能+1，-1改变前缀和，因此只需查找是否有prefix-1即可
                ans = max(ans, i - ump[prefix - 1]);
        }

        if (ump.find(prefix) == ump.end())
            ump[prefix] = i;
    }

    return ans;
}



//题目六：构建 前缀和余数 最早出现的位置。移除的最短子数组长度，使得剩余元素的累加和能被p整除
// 给你一个正整数数组 nums，请你移除 最短 子数组（可以为 空）
// 使得剩余元素的 和 能被 p 整除。 不允许 将整个数组都移除。
// 请你返回你需要移除的最短子数组的长度，如果无法满足题目要求，返回 -1 。
// 子数组 定义为原数组中连续的一组元素。
// 测试链接 : https://leetcode.cn/problems/make-sum-divisible-by-p/
//
//记[0..i]范围的元素之和模p = cur, 整个数组前缀和余数为mod
// 则只需把 %p=mod 的这一段子数组删掉即可
//1.cur>=mod  find = cur-mod
//2.cur<mod  find = (cur+p-mod)
//-->>统一为：find = (cur+p-mod)
int minSubarray(vector<int>& nums, int p)
{
    //先求整体的前缀和余数
    int mod_all = 0;
    for (int i = 0; i < nums.size(); i++)
        mod_all = (mod_all + nums[i]) % p;//同余定理
    if (mod_all == 0)
        return 0;

    int ans = nums.size();
    unordered_map<int, int> ump;//key: 前缀和 %p 的余数  val:最晚出现的位置
    ump[0] = -1;  // 重要：余数为0在位置-1处出现（表示从头开始）
    for (int i = 0, prefix_mod = 0; i < nums.size(); i++)
    {
        prefix_mod = (prefix_mod + nums[i]) % p;//记录子数组前缀和余数
        int findAim = (prefix_mod - mod_all + p) % p;
        // 我们需要：(当前前缀和 - 某个前缀和) % p == mod_all
        // 所以：某个前缀和 ≡ (当前前缀和 - mod_all) mod p

        if (ump.find(findAim) != ump.end())
            ans = min(ans, i - ump[findAim]);

        ump[prefix_mod] = i;//更新最晚出现的位置
    }
    return ans == nums.size() ? -1 : ans;
}



//题目七：构建 前缀奇偶状态 最早出现位置。每个元音包含偶数次的 最长子串长度
// 给你一个字符串 s ，请你返回满足以下条件的最长子字符串的长度
// 每个元音字母，即 'a'，'e'，'i'，'o'，'u'
// 在子字符串中都恰好出现了偶数次。
// 测试链接 : https://leetcode.cn/problems/find-the-longest-substring-containing-vowels-in-even-counts/
int moveBit(char ch)
{
    switch (ch)
    {
    case 'a':
        return 0;
    case 'e':
        return 1;
    case 'i':
        return 2;
    case 'o':
        return 3;
    case 'u':
        return 4;
    default:
        return -1;
    }
}

int findTheLongestSubstring(string s)
{
    unordered_map<int, int> ump;//key:前缀状态(aeiou的奇偶状态) , val:最早出现位置
    ump[0] = -1;//aeiou全为偶的状态在字符串为空便已经存在
    int ans = 0;
    for (int i = 0, prefix_status = 0; i < s.size(); i++)
    {
        //u o i e a
        //4 3 2 1 0
        int m = moveBit(s[i]);
        if (m != -1)
            prefix_status ^= (1 << m);//前缀-元音字母状态
        if (ump.find(prefix_status) != ump.end())
            ans = max(ans, i - ump[prefix_status]);

        if (ump.find(prefix_status) == ump.end())//更新最早出现位置
            ump[prefix_status] = i;
    }

    return ans;
}

