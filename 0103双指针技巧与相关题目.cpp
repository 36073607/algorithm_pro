#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


//设置两个指针的技巧，其实这种说法很宽泛，似乎 没什么可总结的
//1.有时候所谓的双指针技巧，就单纯是代码过程用双指针的形式表达出来而已，没有单调性（贪心）方面的考虑
//2.有时候的双指针技巧包含 单调性（贪心）方面 的考虑，牵扯到可能性的取舍。
//  对分析能力的要求会变高。其实是 先有的思考和优化，然后代码变成了 双指针的形式
//3.所以，双指针这个“皮”不重要，分析题目单调性（贪心）方面的特征，这个能力才重要

//常见的双指针类型：
//1.同向双指针
//2.快慢双指针
//3.从两头往中间的双指针
//4.其他


//题目一：
// 按奇偶排序数组II
// 给定一个非负整数数组 nums。nums 中一半整数是奇数 ，一半整数是偶数
// 对数组进行排序，以便当 nums[i] 为奇数时，i也是奇数
// 当 nums[i] 为偶数时， i 也是 偶数
// 你可以返回 任何满足上述条件的数组作为答案
// 测试链接 : https://leetcode.cn/problems/sort-array-by-parity-ii/
//
//不同的说法，同一个题：给定一个数组arr，请把arr调整成 奇数都在奇数位置 或者 偶数都在偶数位置
class Solution 
{
public:
    vector<int> sortArrayByParityII(vector<int>& nums)
    {
        int look = nums.size() - 1;
        for (int even = 0, odd = 1; odd < nums.size() && even < nums.size();)
        {
            if ((nums[look] & 1) == 0)//偶数
            {
                swap(nums[even], nums[look]);
                even += 2;
            }
            else//奇数
            {
                swap(nums[odd], nums[look]);
                odd += 2;
            }
        }

        return nums;
    }
};



//题目二：
// 寻找重复数
// 给定一个包含 n + 1 个整数的数组 nums ，其数字都在 [1, n] 范围内（包括 1 和 n）
// 可知至少存在一个重复的整数。
// 假设 nums 只有 一个重复的整数 ，返回 这个重复的数 。
// 你设计的解决方案必须 不修改 数组 nums 且只用常量级 O(1) 的额外空间。
// 测试链接 : https://leetcode.cn/problems/find-the-duplicate-number/
//-->>转化成单链表找环口的问题
class Solution
{
public:
    int findDuplicate(vector<int>& nums)
    {
        if (nums.size() < 2)
            return -1;

        int fast = nums[nums[0]];
        int slow = nums[0];
        while (fast != slow)
        {
            fast = nums[nums[fast]];
            slow = nums[slow];
        }

        for (fast = 0; fast != slow;)
        {
            fast = nums[fast];
            slow = nums[slow];
        }

        return slow;
    }
};



//题目三：
// 接雨水
// 给定 n 个非负整数表示每个宽度为 1 的柱子的高度图，计算按此排列的柱子，下雨之后能接多少雨水
// 测试链接 : https://leetcode.cn/problems/trapping-rain-water/
//辅助数组的解法(不是最优解)
//时间复杂度O(n)，额外空间复杂度O(n)
//思路：求出每个位置能接多少雨水，最后累加求和即可
//    对于每个位置，求出该位置左侧最大值、右侧最大值，然后记录二者的最小值，减去该位置自身高度即为可存雨水（若为负数，则为0）
class Solution
{
public:
    int trap(vector<int>& height)
    {
        int n = height.size();
        vector<int> left_max(n);//下标表示0~i范围的最大值
        vector<int> right_max(n);//下标表示i~n范围的最大值
        
        left_max[0] = height[0];
        for (int i = 1; i < n; i++)
            left_max[i] = max(height[i], left_max[i - 1]);

        right_max[n - 1] = height[n - 1];
        for (int i = n - 2; i >= 0; i--)
            right_max[i] = max(height[i], right_max[i + 1]);

        int sum = 0;
        for (int i = 1; i < n - 1; i++)//只有1~n-2下标的位置会有水
        {
            sum += max(0, min(left_max[i - 1], right_max[i + 1]) - height[i]);
        }

        return sum;
    }
};


//双指针的解法（最优解）
//时间复杂度O(n)，额外空间复杂度O(1)
//分析单调性：左右两个边界谁小，相应那一侧的水量就可以结算了 -->> 先有的辅助空间解法，再进一步分析单调性，才有的双指针解法
//Q:为什么不结算另一边？
//A:假设lmax < rmax，则已知l位置的左侧真实最大值就是lmax，而l位置右侧的最大值是虚拟最大值，该虚拟最大值一定>=rmax
//  若结算r位置，由于r位置左侧为虚拟最大值，该值仅>=lmax，即使rmax > lmax ，由于r位置并没有完全探明其左侧全部情况，所以虚拟最大值可能>rmax
//  因此需结算l位置
class Solution
{
public:
    int trap(vector<int>& height)
    {
        int n = height.size();
        int lmax = height[0], rmax = height[n - 1];
        int sum = 0;
        for (int l = 1, r = n - 2; l <= r;)
        {
            if (lmax <= rmax)
            {
                sum += max(0, lmax - height[l]);
                lmax = max(lmax, height[l++]);
            }
            else
            {
                sum += max(0, rmax - height[r]);
                rmax = max(rmax, height[r--]);
            }
        }
        return sum;
    }
};




//题目四：
// 救生艇
// 给定数组 people
// people[i]表示第 i 个人的体重 ，船的数量不限，每艘船可以承载的最大重量为 limit
// 每艘船最多可同时载两人，但条件是这些人的重量之和最多为 limit
// 返回 承载所有人所需的最小船数
// 测试链接 : https://leetcode.cn/problems/boats-to-save-people/
//思路（单调/贪心）：让体重小的和体重大的一船，不亏
class Solution 
{
public:
    int numRescueBoats(vector<int>& people, int limit)
    {
        sort(people.begin(), people.end());
        int boat = 0;
        for (int l = 0, r = people.size() - 1; l <= r;)
        {
            if (l == r)
            {
                boat++;
                break;
            }
               
            if (people[l] + people[r] > limit)
            {
                r--;
                boat++;
            }
            else
            {
                l++;
                r--;
                boat++;
            }
        }

        return boat;
    }
};

//扩展：再增加一个要求，如果两人一船那么体重之和必须是偶数，又应该怎么做？（大厂真考过）
//      -->> 奇数+奇数  偶数+偶数
//      -->>把原始数组分成奇数数组和偶数数组，分别对两个数组来一遍上述操作，求和即可




//题目五：
// 盛最多水的容器
// 给定一个长度为 n 的整数数组 height 。有 n 条垂线，第 i 条线的两个端点是 (i, 0) 和 (i, height[i]) 。
// 找出其中的两条线，使得它们与 x 轴共同构成的容器可以容纳最多的水
// 返回容器可以储存的最大水量
// 说明：你不能倾斜容器
// 测试链接 : https://leetcode.cn/problems/container-with-most-water/
class Solution 
{
public:
    int maxArea(vector<int>& height) 
    {
        int ans = 0;
        for (int l = 0, r = height.size() - 1; l < r;)
        {
            ans = max(ans, min(height[l], height[r]) * (r - l));
            if (height[l] <= height[r])//左边高度小于右边就移动左边，寻找更优解
                l++;
            else//右边同理
                r--;
        }

        return ans;
    }
};




//题目六：
// 供暖器
// 冬季已经来临。 你的任务是设计一个有固定加热半径的供暖器向所有房屋供暖。
// 在加热器的加热半径范围内的每个房屋都可以获得供暖。
// 现在，给出位于一条水平线上的房屋 houses 和供暖器 heaters 的位置
// 请你找出并返回可以覆盖所有房屋的最小加热半径。
// 说明：所有供暖器都遵循你的半径标准，加热的半径也一样。
// 测试链接 : https://leetcode.cn/problems/heaters/
class Solution 
{
public:
    int findRadius(vector<int>& houses, vector<int>& heaters)
    {
        sort(houses.begin(), houses.end());
        sort(heaters.begin(), heaters.end());
        int ans = 0;
        for (int house_index = 0, heater_index = 0; house_index < houses.size(); house_index++)
        {
            while (heater_index<heaters.size()-1 &&
                abs(houses[house_index] - heaters[heater_index]) >= abs(houses[house_index] - heaters[heater_index + 1]))//寻找离每个房子最近的暖气
            {
                //记得是 >=
                //当两个暖气离同一个房子距离相等时，
                //选择下标较大的那个暖气
                //便于后续操作
                heater_index++;
            }
            ans = max(ans, abs(houses[house_index] - heaters[heater_index]));
        }
        return ans;
    }
};




//题目七：
// 缺失的第一个正数
// 给你一个未排序的整数数组 nums ，请你找出其中没有出现的最小的正整数。
// 请你实现时间复杂度为 O(n) 并且只使用常数级别额外空间的解决方案。
// 测试链接 : https://leetcode.cn/problems/first-missing-positive/

//这段代码有问题：if判断的先后顺序有误，会导致数组越界：比如arr[l] = 0
class Solution 
{
public:
    int firstMissingPositive(vector<int>& nums)
    {
        //我们期望nums[i]的位置的数为i+1
        //r下标管理“垃圾”区域
        int l, r;
        // l的左边，都是做到i位置上放着i+1的区域
        // 永远盯着l位置的数字看，看能不能扩充(l++)
        //
        // [r....]垃圾区
        // 最好的状况下，认为1~r是可以收集全的，每个数字收集1个，不能有垃圾
        // 有垃圾呢？预期就会变差(r--)
        for (l = 0, r = nums.size(); l < r;)//这里不取等，l==r说明l到达“垃圾区”，没必要往下看
        {
            if (nums[l] <= l || nums[l] > r || nums[nums[l] - 1] == nums[l])
            {
            // 当前元素是垃圾：
            // 1. arr[l] <= l：元素值太小（小于等于左边界）
            // 2. arr[l] > r：元素值太大（大于期望的最大值）
            // 3. arr[arr[l] - 1] == arr[l]：目标位置已经有正确值了（重复）
                swap(nums[l], nums[--r]);
            }
            else if (nums[l] == l + 1)//符合要求
                l++;
            else//这个数没踩雷也没中标，换到期望的位置上，接下来检查换过来的数
            {
            // 当前元素应该在正确范围内，但不在正确位置
            // 将其交换到正确位置 arr[l] - 1
                swap(nums[l], nums[nums[l] - 1]);
            }
        }
        return l + 1;
    }
};

//正确写法:
//1.改变判断顺序：
int firstMissingPositive(vector<int>& arr) {
    // 特殊情况：空数组
    if (arr.empty()) return 1;

    int l = 0;
    int r = arr.size();

    while (l < r) {
        if (arr[l] == l + 1) {
            // 当前元素已经在正确位置
            l++;
        }
        else if (arr[l] <= l || arr[l] > r || arr[arr[l] - 1] == arr[l]) {
            // 当前元素是垃圾：
            // 1. arr[l] <= l：元素值太小（小于等于左边界）
            // 2. arr[l] > r：元素值太大（大于期望的最大值）
            // 3. arr[arr[l] - 1] == arr[l]：目标位置已经有正确值了（重复）
            swap(arr[l], arr[--r]);
        }
        else {
            // 当前元素应该在正确范围内，但不在正确位置
            // 将其交换到正确位置 arr[l] - 1
            swap(arr[l], arr[arr[l] - 1]);
        }
    }

    // 返回第一个缺失的正整数
    return l + 1;
}

//2.显示地避免越界：
int firstMissingPositive(vector<int>& nums) {
    if (nums.empty()) return 1;

    int l = 0;
    int r = nums.size();

    while (l < r) {
        if (nums[l] == l + 1) {
            l++;
        }
        else if (nums[l] <= 0 || nums[l] > r) {
            // 明显无效的值：负数或大于可能的最大值
            swap(nums[l], nums[--r]);
        }
        else if (nums[l] != nums[nums[l] - 1]) {
            // 需要交换到正确位置，且目标位置不是相同的值
            swap(nums[l], nums[nums[l] - 1]);
        }
        else {
            // 重复值，放在当前位置没有意义，放到垃圾区
            swap(nums[l], nums[--r]);
        }
    }

    return l + 1;
}

