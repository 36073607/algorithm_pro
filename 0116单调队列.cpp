#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
using namespace std;



//单调队列--上
//单调队列最经典的用法是解决如下问题：
//滑动窗口在滑动时，r++代表右侧数字进窗口，l++代表左侧数字出窗口
//这个过程中，想随时得到当前滑动窗口的 最大值 或者 最小值
//窗口滑动的过程中，单调队列所有调整的总代价为O(n)，单次操作的均摊代价为O(1)


//注意：这是单调队列最经典的用法，可以解决很多题目，在“单调队列--下”中将继续介绍其他用法
//注意：单调队列可以和很多技巧交叉使用！比如：动态规划+单调队列优化


//题目一：
// 滑动窗口最大值（单调队列经典用法模版）
// 给你一个整数数组 nums，有一个大小为 k 的滑动窗口从数组的最左侧移动到数组的最右侧
// 你只可以看到在滑动窗口内的 k 个数字。滑动窗口每次只向右移动一位。
// 返回 滑动窗口中的最大值 。
// 测试链接 : https://leetcode.cn/problems/sliding-window-maximum/
//这里我用数组模拟双端队列，用stl->deque 也可以
class Solution 
{
private:
    const static int MAXSIZE = 100005;
    int head, tail;
    int deque[MAXSIZE];//形成一个从队头到队尾单调递减的单调队列-->>队头为当前滑动窗口最大值
    //该单调队列中存放的是元素下标

public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) 
    {
        head = 0, tail = 0;//[head, tail)
        vector<int> ans;

        //先形成0~k-2的单调队列
        for (int i = 0; i < k - 1; i++)
        {
            while (head < tail && nums[i] >= nums[deque[tail - 1]])//新来的元素>=队尾元素
                tail--;//弹出队尾元素

            deque[tail++] = i;
        }

        for (int l = 0, r = k - 1; r < nums.size(); l++, r++)//长度为k的窗口向右滑动
        {
            while (head < tail && nums[r] >= nums[deque[tail - 1]])
                tail--;

            deque[tail++] = r;

            ans.push_back(nums[deque[head]]);//收集答案

            if (l == deque[head])
                head++;
        }
        return ans;
    }
};



//题目二：
// 绝对差不超过限制的最长连续子数组
// 给你一个整数数组 nums ，和一个表示限制的整数 limit
// 请你返回最长连续子数组的长度
// 该子数组中的任意两个元素之间的绝对差必须小于或者等于 limit
// 如果不存在满足条件的子数组，则返回 0
// 测试链接 : https://leetcode.cn/problems/longest-continuous-subarray-with-absolute-diff-less-than-or-equal-to-limit/
class Solution 
{
private:
    const static int MAXSIZE = 100005;
    int maxDeque[MAXSIZE];
    int minDeque[MAXSIZE];
    int maxh, maxt;
    int minh, mint;

public:
    int longestSubarray(vector<int>& nums, int limit)
    {
        maxh = maxt = minh = mint = 0;
        int ans = 0;
        for (int l = 0, r = 0; l < nums.size(); l++)//以当前l为左起点，r不断向右滑动，退出循环时，当前以l为左起点的滑动窗口已经审查完毕，准备开启下一轮
        {
            while (r < nums.size() && check(nums, limit, r))
                push(nums, r++);

            ans = max(ans, r - l);

            whether_pop(nums, l);//以l为左下标的子数组结束，检查是否更新单调队列
        }

        return ans;
    }

    bool check(vector<int>& nums, int limit ,int curNum)
    {
        int maxs = maxh < maxt ? max(nums[maxDeque[maxh]], nums[curNum]) : nums[curNum];
        int mins = minh < mint ? min(nums[minDeque[minh]], nums[curNum]) : nums[curNum];

        return maxs - mins <= limit;
    }

    void push(vector<int>& nums, int curNum)
    {
        while (maxh < maxt && nums[maxDeque[maxt - 1]] <= nums[curNum])
            maxt--;
        maxDeque[maxt++] = curNum;

        while (minh < mint && nums[minDeque[mint - 1]] >= nums[curNum])
            mint--;
        minDeque[mint++] = curNum;
    }

    void whether_pop(vector<int>& nums, int curNum)
    {
        if (maxh < maxt && curNum == maxDeque[maxh])
            maxh++;

        if (minh < mint && curNum == minDeque[minh])
            minh++;
    }
};



//题目三：
// 接取落水的最小花盆
// 老板需要你帮忙浇花。给出 N 滴水的坐标，y 表示水滴的高度，x 表示它下落到 x 轴的位置
// 每滴水以每秒1个单位长度的速度下落。你需要把花盆放在 x 轴上的某个位置
// 使得从被花盆接着的第 1 滴水开始，到被花盆接着的最后 1 滴水结束，之间的时间差至少为 D
// 我们认为，只要水滴落到 x 轴上，与花盆的边沿对齐，就认为被接住
// 给出 N 滴水的坐标和 D 的大小，请算出最小的花盆的宽度 W
// 测试链接 : https://www.luogu.com.cn/problem/P2698
//注：本题与题目二极其相似，但多了一个离散化的小技巧，用每个数据的数组下标处理数据，而不是每个数据真正的x坐标

//注意：
//在C++中，二维数组不能直接用于std::sort函数。二维数组int arr[MAXSIZE][2]在传递给sort时会被当作指针数组处理，而二维数组元素（一维数组）不能进行赋值操作。
//解决方法是使用结构体或pair来存储坐标

//注意：
//compute() == INT_MAX ? -1 : compute()， 调用了两次compute()，会导致重复计算且状态被污染，应只调用一次并保存结果。

#include <iostream>
#include <algorithm>
#include <climits>
using namespace std;
const int MAXSIZE = 100005;
int maxDeque[MAXSIZE];
int minDeque[MAXSIZE];
int maxh, maxt;
int minh, mint;
int n, d;

struct Point
{
    int x;
    int y;
};

Point arr[MAXSIZE];//存放雨滴的信息

bool cmp(Point a, Point b)
{
    return a.x < b.x;
}

bool check()
{
    if (maxh == maxt || minh == mint) return false;  // 队列为空
    int maxs = arr[maxDeque[maxh]].y;
    int mins = arr[minDeque[minh]].y;
    return maxs - mins >= d;
}

void push(int curNum)
{
    //维护最大值的单调队列
    while (maxh < maxt && arr[maxDeque[maxt - 1]].y <= arr[curNum].y)
        maxt--;
    maxDeque[maxt++] = curNum;

    //维护最小值的单调队列
    while (minh < mint && arr[minDeque[mint - 1]].y >= arr[curNum].y)
        mint--;
    minDeque[mint++] = curNum;

}

void whether_pop(int curNum)
{
    //维护最大值的单调队列
    if (maxh < maxt && curNum == maxDeque[maxh])
        maxh++;

    //维护最小值的单调队列
    if (minh < mint && curNum == minDeque[minh])
        minh++;
}

int compute()
{
    int ans = INT_MAX;
    sort(arr, arr + n, cmp);//将雨滴x坐标从小到大依次排序,便于滑动窗口计算答案
    // 滑动窗口技巧使用的前提条件包括存在单调性以及滑动窗口不回退，如果不排序，就会出现x坐标回退的情况
    for (int l = 0, r = 0; l < n; l++)
    {
        //因为此题求的是最小值，所以一旦有一个达标就不再进入队列
        while (r < n && !check())
            push(r++);

        if (check())
            ans = min(ans, arr[r - 1].x - arr[l].x);

        whether_pop(l);
    }

    return ans;
}

int main()
{
    cin >> n >> d;
    maxh = maxt = minh = mint = 0;
    for (int i = 0; i < n; i++)
        cin >> arr[i].x >> arr[i].y;

    int ret = compute();
    int ans = (ret == INT_MAX) ? -1 : ret;
    cout << ans;

    return 0;
}

//小结题目二与题目三：
//我们会发现，题目二和题目三其实是一体两面，分别探讨了同一问题下的最大值与最小值
//题目二求最大值，要时刻维持max-min<=limit，所以要先判断下一个值是否符合要求
//题目三求最小值，一旦达到max-min>=d就结算，所以只需关照当前窗口内的情况是否符合

//但实际上，滑动窗口的标准写法还是 先操作，后检查
//题目二的 先操作，后检查 写法：
int longestSubarray(vector<int>& nums, int limit) {
    maxh = maxt = minh = mint = 0;
    int ans = 0;

    for (int l = 0, r = 0; r < nums.size(); r++) {
        // 1. 先加入新元素
        push(nums, r);

        // 2. 检查当前窗口是否满足条件
        // 注意：这里要获取窗口内的最大值和最小值
        int maxVal = maxh < maxt ? nums[maxDeque[maxh]] : nums[r];
        int minVal = minh < mint ? nums[minDeque[minh]] : nums[r];

        // 3. 如果不满足条件，收缩左边界直到满足
        while (maxVal - minVal > limit) {
            pop(nums, l);
            l++;
            // 重新计算当前窗口的最值
            maxVal = maxh < maxt ? nums[maxDeque[maxh]] : nums[r];
            minVal = minh < mint ? nums[minDeque[minh]] : nums[r];
        }

        // 4. 记录最大长度
        ans = max(ans, r - l + 1);
    }

    return ans;
}







//单调队列--下
//除了单调队列最经典的用法之外，在很多问题里单调队列还可以 维持求解答案的可能性
//1.单调队列里的所有对象按照 规定好的单调性来组织
//2.当某个对象从队尾进入单调队列时，
//  会从 队头 或者 队尾 依次淘汰单调队列里，对后续求解答案没有帮助 的对象
//3.每个对象一旦从单调队列弹出，可以结算此时这个对象参与的答案
//  随后这个对象 不再参与后续求解答案的过程
//4.其实是 先有对题目的分析！ 进而发现单调性，然后利用 单调队列的特征 去实现
//
//注意：单调队列可以和很多技巧交叉使用！比如动态规划+单调队列优化

//头部维持最值优势，尾部维持新旧优势，新元素一定有新旧优势，又新最值又好可以直接淘汰前面又旧最值又差的，扩展时又淘汰了头部过期的元素



//题目一：
// 和至少为K的最短子数组
// 给定一个数组arr，其中的值有可能正、负、0
// 给定一个正数k
// 返回累加和>=k的所有子数组中，最短的子数组长度
// 测试链接 : https://leetcode.cn/problems/shortest-subarray-with-sum-at-least-k/
//法一：前缀和+二分 -->> 失败！！！
//根本缺陷：对于包含负数的数组，子数组长度与和之间没有单调性，所以二分查找无法保证找到最短的满足条件的子数组。
class Solution {
public:
    int shortestSubarray(vector<int>& nums, int k) {
        int ans = INT_MAX;
        int n = nums.size();

        vector<long long> prefix(n + 1, 0); // nums数组前缀和
        for (int i = 1; i <= n; i++)
            prefix[i] = prefix[i - 1] + nums[i - 1];

        for (int l = 1, r = n; l <= r;) {
            int m = l + (r - l) / 2;
            if (f(prefix, k, m)) {
                ans = m;
                r = m - 1;
            }
            else
                l = m + 1;
        }
        return ans == INT_MAX ? -1 : ans;
    }

    bool f(vector<long long>& prefix, int k, int m) {
        int n = prefix.size() - 1;//实际长度
        for (int l = 0; l + m <= n; l++) {
            if (prefix[l + m] - prefix[l] >= k)
                return true;
        }
        return false;
    }
};

//法二：单调队列   时间复杂度：O(n)
//分析单调性：
// 检测方法：由于使用到了 前缀和数组 ，所以固定r，l不断向右移动 ; 即以检测以r为数组末尾的所有子数组
//新的前缀和从小到大不断入队，
// 
//如果新的前缀和 - 队头前缀和 符合要求，则弹出队头，h++，检测新的前缀和与当前队头前缀和是否满足要求
// 为什么要弹出队头？如果下一轮新的前缀和 > 新的前缀和(这一轮)，那么得到的答案数值更大，不符合题目要求；
//                   如果下一轮新的前缀和 <= 新的前缀和 -->>(即下方讨论的 新的前缀和小于等于单调队列队尾前缀和)
//                   -->>(这一轮新的前缀和弹出)
//                     -->>情况1:下一轮新的前缀和 <= 这个队头前缀和 ，也要弹出
//                     -->>情况2:下一轮新的前缀和 >  这个队头前缀和 ，得到的答案数值更大，不符合题目要求
// 
//如果新的前缀和 小于等于 单调队列队尾前缀和，由于检测方法，新的前缀和比旧的前缀和更靠近下一轮的新的前缀和 
//  且 
//  如果 下一轮的新的前缀和 - 新的前缀和（这一轮） 不达标 ，则队尾前缀和一定不达标（队尾前缀和大于这一轮新的前缀和）
//所以单调队列从小到大递增入队
class Solution 
{
private:
    static const int MAXSIZE = 100005;
    int upDeque[MAXSIZE];//单调递增的单调队列，存放几个数（笔记中方式一--前几个数的前缀和）,从数组中第一个数(下标为0)开始一共n个数
    int h, t;

public:
    int shortestSubarray(vector<int>& nums, int k) 
    {
        int ans = INT_MAX;
        h = t = 0;
        int n = nums.size();
        vector<long long> preSum(n + 1, 0);//前n个数的前缀和，eg: preSum[0] -->> 前0个数的前缀和
        for (int i = 1; i <= n; i++)
            preSum[i] = preSum[i - 1] + nums[i - 1];

        for (int i = 0; i <= n; i++)
        {
            while (h < t && preSum[i] - preSum[upDeque[h]] >= k)//符合要求
            {
                // 如果当前的前缀和 - 头前缀和，达标！
                ans = min(ans, i - upDeque[h]);
                h++;
            }

            while (h < t && preSum[i] <= preSum[upDeque[t - 1]])//维护单调队列的单调性
                t--;

            upDeque[t++] = i;
        }
        
        return ans == INT_MAX ? -1 : ans;
    }
};



//题目二：
// 满足不等式的最大值
// 给你一个数组 points 和一个整数 k
// 数组中每个元素都表示二维平面上的点的坐标，并按照横坐标 x 的值从小到大排序
// 也就是说 points[i] = [xi, yi]
// 并且在 1 <= i < j <= points.length 的前提下，xi < xj 总成立
// 请你找出 yi + yj + |xi - xj| 的 最大值，
// 其中 |xi - xj| <= k 且 1 <= i < j <= points.length
// 题目测试数据保证至少存在一对能够满足 |xi - xj| <= k 的点。
// 测试链接 : https://leetcode.cn/problems/max-value-of-equation/
// 
// -->> 后面点(x+y) + 前面点(y-x) 
//注意操作顺序：先移除过期的队首元素，然后计算当前点与队首元素的值，最后将当前点加入队列并维护单调性。
class Solution 
{
private:
    static const int MAXSIZE = 100005;
    int deque[MAXSIZE][2];//记录 [x坐标][该点 y-x 的值]   ， 从大到小
    int h, t;

public:
    int findMaxValueOfEquation(vector<vector<int>>& points, int k)
    {
        h = t = 0;
        int n = points.size();
        int ans = INT_MIN;
        for (int i = 0; i < n; i++)
        {
            int cur_x = points[i][0];
            int cur_y = points[i][1];
            int curPointDiff = cur_y - cur_x;

            while (h < t && cur_x - deque[h][0] > k)// |xi - xj| <= k 不成立
                h++;

            if (h < t)
                ans = max(ans, deque[h][1] + cur_x + cur_y);

            while (h < t && curPointDiff >= deque[t - 1][1])//当前点y-x的差值 >= 队尾记录的差值 ，由于当前点更优(x最新且y-x大)，则队尾出队
                t--;

            deque[t][0] = cur_x;
            deque[t++][1] = curPointDiff;
        }

        return ans;
    }
};



//题目三：
// 你可以安排的最多任务数目
// 给你 n 个任务和 m 个工人。每个任务需要一定的力量值才能完成
// 需要的力量值保存在下标从 0 开始的整数数组 tasks 中，
// 第i个任务需要 tasks[i] 的力量才能完成
// 每个工人的力量值保存在下标从 0 开始的整数数组workers中，
// 第j个工人的力量值为 workers[j]
// 每个工人只能完成一个任务，且力量值需要大于等于该任务的力量要求值，即workers[j]>=tasks[i]
// 除此以外，你还有 pills 个神奇药丸，可以给 一个工人的力量值 增加 strength
// 你可以决定给哪些工人使用药丸，但每个工人 最多 只能使用 一片 药丸
// 给你下标从 0 开始的整数数组tasks 和 workers 以及两个整数 pills 和 strength
// 请你返回 最多 有多少个任务可以被完成。
// 测试链接 : https://leetcode.cn/problems/maximum-number-of-tasks-you-can-assign/
//
//这道题对于单调队列的单调性分析没有那么强烈，单调队列在其中更多的充当一个窗口,是一个简单的双端队列使用
class Solution
{
private:
    vector<int> tasks;
    vector<int> workers;
    static const int MAXSIZE = 50005;
    int deque[MAXSIZE];  // 存储任务索引
    int h, t;

public:
    int maxTaskAssign(vector<int>& ts, vector<int>& ws, int pills, int strength)
    {
        tasks = ts;
        workers = ws;

        sort(tasks.begin(), tasks.end());
        sort(workers.begin(), workers.end());

        int ans = 0;
        int n = tasks.size(), m = workers.size();
        for (int l = 0, r = min(n, m), mid; l <= r;)
        {
            mid = (l + r) / 2;
            if (f(mid, pills, strength, n, m))
            {
                ans = mid;
                l = mid + 1;
            }
            else
                r = mid - 1;
        }
        return ans;
    }

    // 判断是否能完成aim个任务
    bool f(int aim, int pills, int strength, int n, int m)
    {
        if (aim == 0) return true;

        h = t = 0;
        // 任务范围：最小aim个任务
        int taskStart = 0, taskEnd = aim - 1;
        // 工人范围：最大aim个工人
        int workerStart = m - aim, workerEnd = m - 1;
        int usedPills = 0;

        for (int i = workerStart, j = taskStart; i <= workerEnd; i++)
        {
            // 将当前工人不吃药就能完成的任务加入队列
            for (; j <= taskEnd && tasks[j] <= workers[i]; j++)
            {
                deque[t++] = j;  // 存储任务索引
            }

            // 不吃药就能完成任务
            if (h < t && tasks[deque[h]] <= workers[i])
            {
                h++;
            }
            else  // 需要吃药
            {
                // 将吃药后能完成的任务加入队列
                for (; j <= taskEnd && tasks[j] <= workers[i] + strength; j++)
                {
                    deque[t++] = j;
                }

                if (h < t)  // 有任务可以分配
                {
                    usedPills++;
                    t--;  // 从队列尾部取最困难的任务
                    if (usedPills > pills) return false;
                }
                else  // 没有任务可以分配
                {
                    return false;
                }
            }
        }
        return true;
    }
};
//每来到一个工人解锁任务, 意思是当前工人以及后面工人一定可以完成的任务放到队列里去, 然后考察当前工人能不能完成队头的任务(需要能力值最小的); 如果能够完成, 直接去下一个工人, 
//否则,说明队列里没有我解锁的任务, 或者当队列里的任务是我前面的兄弟嗑药解锁的, 我要完成也得嗑药;
//所以进入嗑药的逻辑, 我嗑药后能解锁的加入队列, 我完成一个最大的(为后面的哥们减轻负担), 如果我嗑药后队列任然为空 (--若不为空则说明是我解锁的或者是我前面的兄弟(不管磕没嗑药)解锁的任务,我嗑药是一定可以完成的--) ,说明我无法完成当前任务,宣告失败..........
//所以最终说来, 队列里面解锁的是我以及后面的工人能够完成的任务, 如果我嗑药解锁的, 那么后面的工人大不了也嗑药完成

//经验总结：
//先判断边界再访问
//eg: while (ts_l <= ts_r && workers[ws_l] >= tasks[ts_l])



//java转化成cpp
class Solution {
private:
    vector<int> tasks;
    vector<int> workers;
    int deque[50005];
    int h, t;

    // 在药的数量不超情况下，能不能每个工人都做一个任务
    bool f(int tl, int tr, int wl, int wr, int s, int pills) {
        h = t = 0;
        int cnt = 0; // 已经使用的药的数量

        for (int i = wl, j = tl; i <= wr; i++) {
            // i : 工人的编号
            // j : 任务的编号

            // 工人不吃药的情况下，去解锁任务
            for (; j <= tr && tasks[j] <= workers[i]; j++) {
                deque[t++] = j;
            }

            if (h < t && tasks[deque[h]] <= workers[i]) {
                h++;
            }
            else {
                // 吃药之后的逻辑
                for (; j <= tr && tasks[j] <= workers[i] + s; j++) {
                    deque[t++] = j;
                }
                if (h < t) {
                    cnt++;
                    t--;
                }
                else {
                    return false;
                }
            }
        }
        return cnt <= pills;
    }

public:
    int maxTaskAssign(vector<int>& ts, vector<int>& ws, int pills, int strength) {
        tasks = ts;
        workers = ws;
        sort(tasks.begin(), tasks.end());
        sort(workers.begin(), workers.end());

        int tsize = tasks.size();
        int wsize = workers.size();
        int ans = 0;

        // [0, Math.min(tsize, wsize)]
        int left = 0, right = min(tsize, wsize);
        while (left <= right) {
            int mid = (left + right) / 2;
            // tasks[0...mid-1]需要力量最小的几个任务
            // workers[wsize-mid...wsize-1]力量值最大的几个工人
            if (f(0, mid - 1, wsize - mid, wsize - 1, strength, pills)) {
                ans = mid;
                left = mid + 1;
            }
            else {
                right = mid - 1;
            }
        }

        return ans;
    }
};