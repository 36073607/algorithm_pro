#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
using namespace std;


//单调栈--上
//单调栈最经典的用法是解决如下问题：
//每个位置都求：  -->>严格小于-->大压小(大的在上，小的在下)的单调栈
//0. 当前位置的 左侧 比当前位置的数字 小 ，且距离最近的位置 在哪
//1. 当前位置的 右侧 比当前位置的数字 小 ，且距离最近的位置 在哪
//或者
//每个位置都求：  -->>严格大于-->小压大(小的在上，大的在下)的单调栈
//0. 当前位置的 左侧 比当前位置的数字 大 ，且距离最近的位置 在哪
//1. 当前位置的 右侧 比当前位置的数字 大 ，且距离最近的位置 在哪
//
//用单调栈的方式可以做到：求解过程中，单调栈所有调整的总代价为O(n)，单次操作的均摊代价为O(1)
//值得注意的细节（重要）：当新要入栈的元素与栈顶元素相等时，根据具体情况决定是否将栈顶元素弹出（挺关键的取舍）

//注意：这是单调栈最经典的用法，可以解决很多题目，在 单调栈--下 中将继续介绍其他的用法
//注意：单调栈可以和很多技巧交叉使用！比如：动态规划+单调栈优化


//题目一：单调栈最经典用法的模板
// 单调栈求每个位置左右两侧，离当前位置最近、且值严格小于的位置
// 给定一个可能含有重复值的数组 arr
// 找到每一个 i 位置左边和右边离 i 位置最近且值比 arr[i] 小的位置
// 返回所有位置相应的信息。
// 输入描述：
// 第一行输入一个数字 n，表示数组 arr 的长度。
// 以下一行输入 n 个数字，表示数组的值
// 输出描述：
// 输出n行，每行两个数字 L 和 R，如果不存在，则值为 -1，下标从 0 开始。
// 测试链接 : https://www.nowcoder.com/practice/2a2c00e7a88a498693568cef63a4b7bb
//时间复杂度是O(n)，空间复杂度也是O(n)
#include <iostream>
#include <vector>
using namespace std;

const int MAXSIZE = 1000000;
vector<vector<int>> ans(MAXSIZE, vector<int>(2, 0));
vector<int> stk;

int n;

void compute(vector<int>& arr)
{
    // 清空栈
    stk.clear();

    for (int i = 0; i < n; i++)
    {
        // 当栈不为空且栈顶元素对应的值大于等于当前值时，弹出并结算
        while (!stk.empty() && arr[stk.back()] >= arr[i])
        {
            int idx = stk.back();
            stk.pop_back();

            // 计算左边最近比它小的位置
            ans[idx][0] = !stk.empty() ? stk.back() : -1;
            // 右边最近比它小的位置就是当前i（因为arr[i] <= arr[idx]）
            ans[idx][1] = i;
        }

        stk.push_back(i);
    }

    // 清算栈中剩余元素
    while (!stk.empty())
    {
        int idx = stk.back();
        stk.pop_back();

        // 左边最近比它小的位置
        ans[idx][0] = !stk.empty() ? stk.back() : -1;
        // 右边没有比它小的位置
        ans[idx][1] = -1;
    }

    // 修正阶段：处理相等元素的情况
    // 对于每个位置i，如果右边第一个比它小的位置的值和arr[i]相等，
    // 则需要继续向右查找
    for (int i = n - 2; i >= 0; i--)
    {
        if (ans[i][1] != -1 && arr[ans[i][1]] == arr[i])
        {
            ans[i][1] = ans[ans[i][1]][1];
        }
    }
}

int main()
{
    cin >> n;
    vector<int> arr(n);
    for (int i = 0; i < n; i++)
        cin >> arr[i];

    compute(arr);

    for (int i = 0; i < n; i++)
        cout << ans[i][0] << " " << ans[i][1] << endl;

    return 0;
}




//题目二：
// 每日温度
// 给定一个整数数组 temperatures ，表示每天的温度，返回一个数组 answer
// 其中 answer[i] 是指对于第 i 天，下一个更高温度出现在几天后
// 如果气温在这之后都不会升高，请在该位置用 0 来代替。
// 测试链接 : https://leetcode.cn/problems/daily-temperatures/
//-->> 即：每一个位置的数[i]，离它右边且最近的、严格大于当前位置的数的最近位置
class Solution 
{
public:
    vector<int> dailyTemperatures(vector<int>& temperatures)
    {
        int n = temperatures.size();
        vector<int> ans(n, 0);
        vector<int> stk;

        for (int i = 0; i < n; i++)
        {
            while (!stk.empty() && temperatures[i] > temperatures[stk.back()])//这里让等于栈顶元素的数据也入栈
            {
                int idx = stk.back();
                stk.pop_back();

                ans[idx] = i - idx;
            }

            stk.push_back(i);//降序依次入栈
        }
        //注意：最后不用清算，因为此时栈中剩下的数据右侧均无大于该位置的值，都是0

        return ans;
    }
};




//题目三：
// 子数组的最小值之和
// 给定一个整数数组 arr，找到 min(b) 的总和，其中 b 的范围为 arr 的每个（连续）子数组。
// 由于答案可能很大，答案对 1000000007 取模
// 测试链接 : https://leetcode.cn/problems/sum-of-subarray-minimums/
// [3 ,..., 5,..., 2]  则5为除2、3外，包含该下标的子数组的最小值
//  2       5      8   -->>包含下标5的子数组一共 (5-2)*(8-5)  -->>从3~5中挑一个*从5~7中挑一个-->C(3,1)*C(3,1)
//
//运用同余原理的细节（重要）：
//不要使用+= : 例如不能写成ans+=... , 应该是 ans= ans+...  ,按照第一种写法会溢出
//原因：对于第一种写法，等价于：
//long temp = ((long)(cur_idx - pre_idx) * (i - cur_idx) * arr[cur_idx]) % MOD;
//ans = ans + temp;   注意：这里没有对 ans 取模！
class Solution 
{
public:
    int sumSubarrayMins(vector<int>& arr) 
    {
        int MOD = 1000000007;
        long long ans = 0;  
        vector<int> stk;// 单调递增栈，存储索引
        for (int i = 0; i < arr.size(); i++)
        {
            // 当当前元素小于等于栈顶元素时，处理栈顶元素
            while (!stk.empty() && arr[i] <= arr[stk.back()])//这里是等号
            {
                int cur_idx = stk.back();
                stk.pop_back();
                int pre_idx = stk.empty() ? -1 : stk.back();

                ans = (ans + (long)(cur_idx - pre_idx) * (i - cur_idx) * arr[cur_idx]) % MOD;
            }

            stk.push_back(i);
        }

        //处理栈中剩余元素
        while (!stk.empty())
        {
            int cur_idx = stk.back();
            stk.pop_back();

            int pre_idx = stk.empty() ? -1 : stk.back();
            ans = (ans + (long)(cur_idx - pre_idx) * (arr.size() - cur_idx) * arr[cur_idx]) % MOD;
        }

        return static_cast<int> (ans);
    }
};



//题目四：
// 柱状图中最大的矩形
// 给定 n 个非负整数，用来表示柱状图中各个柱子的高度
// 每个柱子彼此相邻，且宽度为 1 。求在该柱状图中，能够勾勒出来的矩形的最大面积
// 测试链接：https://leetcode.cn/problems/largest-rectangle-in-histogram
// [小1,...,大,...小2] -->> S_tmp = (小2-小1-1)*大     原理跟题目三类似
class Solution 
{
public:
    int largestRectangleArea(vector<int>& heights) 
    {
        int ans = 0;
        vector<int> stk;
        for (int i = 0; i < heights.size(); i++)
        {
            while (!stk.empty() && heights[i] <= heights[stk.back()])
            {
                int cur_idx = stk.back();
                stk.pop_back();
                int pre_idx = stk.empty() ? -1 : stk.back();

                ans = max(ans, heights[cur_idx] * (i - pre_idx - 1));
            }

            stk.push_back(i);
        }

        while (!stk.empty())
        {
            int cur_idx = stk.back();
            stk.pop_back();
            int pre_idx = stk.empty() ? -1 : stk.back();

            ans = max(ans, heights[cur_idx] * ((int)heights.size() - pre_idx - 1));
        }

        return ans;
    }
};
//ps:之前的接雨水问题也可以用单调栈来做



//题目五：
// 最大矩形
// 给定一个仅包含 0 和 1 、大小为 rows * cols 的二维二进制矩阵
// 找出只包含 1 的最大矩形，并返回其面积
// 测试链接：https://leetcode.cn/problems/maximal-rectangle/
//实际上是：题目四+数组压缩技术
//当来到二维数组某一行，则以该行作为水平线，将水平线上连续且与水平线相接的1累加-->>题目四
class Solution 
{
public:
    int maximalRectangle(vector<vector<char>>& matrix)
    {
        int ans = 0;
        int m = matrix.size();
        int n = matrix[0].size();
        vector<int> heights(n);
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                heights[j] = matrix[i][j] == '0' ? 0 : heights[j] + 1;
            }

            ans = max(ans, largestRectangleArea(heights));
        }
        return ans;
    }

    int largestRectangleArea(vector<int>& heights)
    {
        int ans = 0;
        vector<int> stk;
        for (int i = 0; i < heights.size(); i++)
        {
            while (!stk.empty() && heights[i] <= heights[stk.back()])
            {
                int cur_idx = stk.back();
                stk.pop_back();
                int pre_idx = stk.empty() ? -1 : stk.back();

                ans = max(ans, heights[cur_idx] * (i - pre_idx - 1));
            }

            stk.push_back(i);
        }

        while (!stk.empty())
        {
            int cur_idx = stk.back();
            stk.pop_back();
            int pre_idx = stk.empty() ? -1 : stk.back();

            ans = max(ans, heights[cur_idx] * ((int)heights.size() - pre_idx - 1));
        }

        return ans;
    }
};








//单调栈--下
//除了单调栈最经典的用法之外，在很多问题里单调栈还可以 维持求解答案的可能性
//1.单调栈里的所有对象按照 规定好的单调性来组织
//2.当某个对象进入单调栈时，会从 栈顶开始 依次淘汰单调栈里 对后续求解答案没有帮助 的对象
//3.每个对象从栈顶弹出的时候 结算当前对象参与的答案，随后这个对象 不再参与后续求解答案的过程
//4.其实是 先有对题目的分析！进而发现单调性，然后利用 单调栈的特征 去实现
//注：单调栈和其他技巧的交叉使用在后续会有



//题目一：
// 最大宽度坡
// 给定一个整数数组 A，坡是元组 (i, j)，其中  i < j 且 A[i] <= A[j]
// 这样的坡的宽度为 j - i，找出 A 中的坡的最大宽度，如果不存在，返回 0
// 测试链接 : https://leetcode.cn/problems/maximum-width-ramp/
//
//法一：二分答案+构造最小值数组（重要）
class Solution {
public:
    int maxWidthRamp(vector<int>& nums) 
    {
        int ans = 0;
        for (int l = 0, r = nums.size() - 1; l <= r;) 
        {
            int m = l + (r - l) / 2;
            if (f(m, nums)) 
            {
                ans = m;
                l = m + 1;
            }
            else
                r = m - 1;
        }
        return ans;
    }

    bool f(int m, vector<int>& nums) 
    {
        int n = nums.size();
        vector<int> lmin(n);

        lmin[0] = nums[0];
        for (int i = 1; i < n; i++)
            lmin[i] = min(lmin[i - 1], nums[i]);

        for (int i = m; i < n; i++) 
        {
            int j = i - m;
            if (lmin[j] <= nums[i])
                return true;
        }

        return false;
    }
};
//总结经验：
//for(int i = m, j = i - m;i < n; i++)  -->> 这种写法只会初始化一次j，在之后的循环过程中，j的值不会变化
//因此，需要写成：
//for (int i = m; i < n; i++)
//{
//    int j = i - m;
//    ...
//}


//法二：单调栈
//让符合答案预期的数据入栈，再一一检测这些入栈数据
//单调性：当新入栈的数据大于等于栈顶数据时，即使该数据能得到一个答案，也不如栈顶元素得到的答案更优；
//        若新入栈的数据小于栈顶数据时，该数据得到的答案可能是最优答案
class Solution 
{
public:
    int maxWidthRamp(vector<int>& nums)
    {
        int ans = 0;
        vector<int> stk;
        for (int i = 0; i < nums.size(); i++)//将可能的答案入栈
        {
            if (stk.empty() || nums[i] < nums[stk.back()])// 如果栈为空 或 当前值小于栈顶索引对应的值，则将当前索引入栈
                stk.push_back(i);
        }

        for (int i = nums.size() - 1; i >= 0; i--)//检测栈中答案
        {
            while (!stk.empty() && nums[stk.back()] <= nums[i])
            {
                ans = max(ans, i - stk.back());
                stk.pop_back();
            }
        }

        return ans;
    }
};



//题目二：
// 去除重复字母保证剩余字符串的字典序最小
// 给你一个字符串 s ，请你去除字符串中重复的字母，使得每个字母只出现一次
// 需保证 返回结果的字典序最小
// 要求不能打乱其他字符的相对位置
// 测试链接 : https://leetcode.cn/problems/remove-duplicate-letters/
//由于要使字典序最小，所以构建大压小的单调栈
class Solution 
{
public:
    string removeDuplicateLetters(string s) 
    {
        string stk;//用作单调栈
        vector<int> umap(26, 0);//用作哈希表，统计字符串中每个字符的个数
        vector<bool> existInStack(26, false);//记录每个字符是否在单调栈中

        for (auto& ele : s)
            umap[ele - 'a']++;

        for (int i = 0; i < s.size(); i++)
        {
            if (!existInStack[s[i] - 'a'])//栈中没有该字符
            {
                while (!stk.empty() && (s[i] - 'a') < (stk.back() - 'a') && umap[stk.back() - 'a'] > 0)//新的字符的字典序小于栈顶字符字典序 且 哈希表中该字符还有次数
                {
                    existInStack[stk.back() - 'a'] = false;
                    stk.pop_back();
                }
                existInStack[s[i] - 'a'] = true;//设置为在栈中
                stk.push_back(s[i]);
            }
            umap[s[i] - 'a']--;
        }

        return stk;
    }
};



//第三题：
// 大鱼吃小鱼问题
// 给定一个数组arr，每个值代表鱼的体重
// 每一轮每条鱼都会吃掉右边离自己最近比自己体重小的鱼，每条鱼向右找只吃一条
// 但是吃鱼这件事是同时发生的，也就是同一轮在A吃掉B的同时，A也可能被别的鱼吃掉
// 如果有多条鱼在当前轮找到的是同一条小鱼，那么在这一轮，这条小鱼同时被这些大鱼吃
// 请问多少轮后，鱼的数量就固定了
// 比如 : 8 3 1 5 6 7 2 4
// 第一轮 : 8吃3；3吃1；5、6、7吃2；4没有被吃。数组剩下 8 5 6 7 4
// 第二轮 : 8吃5；5、6、7吃4。数组剩下 8 6 7
// 第三轮 : 8吃6。数组剩下 8 7
// 第四轮 : 8吃7。数组剩下 8。
// 过程结束，返回4
// 测试链接 : https://www.nowcoder.com/practice/77199defc4b74b24b8ebf6244e1793de
// 测试链接 : https://leetcode.cn/problems/steps-to-make-array-non-decreasing/
class Solution 
{
public:
    int totalSteps(vector<int>& nums)
    {
        int ans = 0;
        vector<vector<int>> stk(0, vector<int>(2, 0));//[鱼的下标][完成后续事件的轮数]
        for (int i = nums.size() - 1; i >= 0; i--)
        {
            int times = 0;
            while (!stk.empty() && nums[i] > nums[stk.back()[0]])
            {
                times = max(times + 1, stk.back()[1]);//奖池还在累加
                stk.pop_back();
            }
            stk.push_back({ i,times });
            ans = max(ans, times);
        }
        return ans;
    }
};



//第四题：
// 统计全1子矩形的数量
// 给你一个 m * n 的矩阵 mat，其中只有0和1两种值
// 请你返回有多少个 子矩形 的元素全部都是1
// 测试链接 : https://leetcode.cn/problems/count-submatrices-with-all-ones/
//注意：单调栈--上，“柱状图中最大的矩形”+“全是1的最大矩形”
//法一：二分+前缀和 -->> O(n^3*logn)
//
//法二：单调栈
// 比如
    //              1
    //              1
    //              1         1
    //    1         1         1
    //    1         1         1
    //    1         1         1
    //             
    //    3  ....   6   ....  8
    //   left      cur        i
    // 如上图，假设6位置从栈中弹出，6位置的高度为6(上面6个1)
    // 6位置的左边、离6位置最近、且小于高度6的是3位置(left)，3位置的高度是3
    // 6位置的右边、离6位置最近、且小于高度6的是8位置(i)，8位置的高度是4
    // 此时我们求什么？
    // 1) 求在4~7范围上必须以高度6作为高的矩形有几个？
    // 2) 求在4~7范围上必须以高度5作为高的矩形有几个？
    // 也就是说，<=4的高度一律不求，>6的高度一律不求！
    // 其他位置也会从栈里弹出，等其他位置弹出的时候去求！
    // 那么在4~7范围上必须以高度6作为高的矩形有几个？如下：
    // 4..4  4..5  4..6  4..7
    // 5..5  5..6  5..7
    // 6..6  6..7
    // 7..7
    // 10个！什么公式？
    // 4...7范围的长度为4，那么数量就是 : 4*5/2
    // 同理在4~7范围上，必须以高度5作为高的矩形也是这么多
    // 所以cur从栈里弹出时产生的数量 : 
    // (cur位置的高度-Max{left位置的高度,i位置的高度}) * ((i-left-1)*(i-left)/2)
class Solution 
{
public:
    int numSubmat(vector<vector<int>>& mat) 
    {
        int m = mat.size();
        int n = mat[0].size();
        int ans = 0;
        vector<int> heights(n, 0);

        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                heights[j] = mat[i][j] == 0 ? 0 : heights[j] + 1;
            }
            ans += compute(heights);
        }
        return ans;
    }

    int compute(vector<int>& heights)
    {
        int ans = 0;
        vector<int> stk;
        for (int i = 0; i < heights.size(); i++)
        {
            while (!stk.empty() && heights[i] <= heights[stk.back()])//新入栈的元素 <= 栈顶元素
            {
                if (heights[i] < heights[stk.back()])
                {
                    int cur_idx = stk.back();
                    stk.pop_back();
                    int pre_idx = stk.empty() ? -1 : stk.back();
                    int length = i - pre_idx - 1;
                    int compute_level = heights[cur_idx] - max((pre_idx == -1 ? 0 : heights[pre_idx]), heights[i]);//计算1要统计的层数为多少
                    ans += compute_level * (length * (length + 1) / 2);
                }
            }
            stk.push_back(i);
        }

        while (!stk.empty())//处理剩余元素
        {
            int cur_idx = stk.back();
            stk.pop_back();
            int pre_idx = stk.empty() ? -1 : stk.back();
            int length = heights.size() - pre_idx - 1;
            int compute_level = heights[cur_idx] - (pre_idx == -1 ? 0 : heights[pre_idx]);//计算1要统计的层数为多少
            ans += compute_level * (length * (length + 1) / 2);
        }

        return ans;
    }
};



#include <vector>
#include <stack>
#include <algorithm>
#include <cstring>
using namespace std;

class Solution {
public:
    int numSubmat(vector<vector<int>>& mat) {
        int n = mat.size();
        int m = mat[0].size();
        vector<int> height(m, 0);
        int ans = 0;

        for (int i = 0; i < n; i++) {
            // 更新高度数组
            for (int j = 0; j < m; j++) {
                height[j] = mat[i][j] == 0 ? 0 : height[j] + 1;
            }
            ans += countFromBottom(height, m);
        }
        return ans;
    }

private:
    int countFromBottom(vector<int>& height, int m) {
        stack<int> st;
        int ans = 0;

        for (int i = 0; i < m; i++) {
            while (!st.empty() && height[st.top()] >= height[i]) {
                int cur = st.top();
                st.pop();

                if (height[cur] > height[i]) {
                    int left = st.empty() ? -1 : st.top();
                    int len = i - left - 1;
                    int bottom = max((left == -1 ? 0 : height[left]), height[i]);
                    ans += (height[cur] - bottom) * len * (len + 1) / 2;
                }
            }
            st.push(i);
        }

        // 处理栈中剩余元素
        while (!st.empty()) {
            int cur = st.top();
            st.pop();

            int left = st.empty() ? -1 : st.top();
            int len = m - left - 1;
            int bottom = (left == -1) ? 0 : height[left];
            ans += (height[cur] - bottom) * len * (len + 1) / 2;
        }

        return ans;
    }
};
