#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

//差分的个人理解：在目标区域[l...r]最左端(arr[l])设置状态、在最右端的下一位(arr[r+1])设置相反状态，
//                利用前缀和使范围内的数组元素受到改变，且因为arr[r+1]状态的设置，使目标区域以外的元素不受到影响

//差分的作用：实现高效的区间操作
//差分是前缀和的逆运算，是一种对查询少，而区间修改多的数组的一种优化。

//差分与前缀和互为逆运算
//差分总体思想：就是在需要处理的区间范围内增减一个固定值，在影响到的其他范围内需要恢复，即相反操作。
//差分用法：
//用在一维子区间里的数都增减一个固定值，把对于区间内每个数的操作转换为对于差分数组中的端点的操作，时间复杂度降为o(1)。
//用在二维子矩阵里的数都增减一个固定值，把对于子矩阵的每个数的操作转换为对应差分二维数组各个端点的操作。

//tips:关于前缀和数组的两种方式：
//1. vector<int> sum(n+1) , 下标表示一共i个数字的前缀和，因此需要在sum[0]设置sum[0]=0  eg:sum[2] = arr[0]+arr[1] = sum[1]+arr[1]
//2. vector<int> sum(n) , 下标表示 0-i 的前缀和 eg: sum[2] = arr[0] + arr[1] + arr[2] = sum[1]+arr[2]


//一维差分: 不支持一边操作一边查询
//差分数组在左端点增加c之后，会影响以其开始前缀和都增加c。所以为了确保只有LR这段增加c，需要从R+1开始减少c，即差分数组在R+1处减去c。
//L~R范围，+V  <==> [L]+=V , [R+1]-=V

//题目一：
// 航班预订统计
// 这里有 n 个航班，它们分别从 1 到 n 进行编号。
// 有一份航班预订表 bookings ，
// 表中第 i 条预订记录 bookings[i] = [firsti, lasti, seatsi]
// 意味着在从 firsti 到 lasti 
//（包含 firsti 和 lasti ）的 每个航班 上预订了 seatsi 个座位。
// 请你返回一个长度为 n 的数组 answer，里面的元素是每个航班预定的座位总数。
// 测试链接 : https://leetcode.cn/problems/corporate-flight-bookings/
vector<int> corpFlightBookings(vector<vector<int>>& bookings, int n)
{
	vector<int> cnt(n + 2);
	for (int i = 0; i < bookings.size(); i++)
	{
		cnt[bookings[i][0]] += bookings[i][2];
		cnt[bookings[i][1] + 1] -= bookings[i][2];
	}

	for (int i = 1; i < cnt.size(); i++)
	{
		cnt[i] += cnt[i - 1];
	}

	vector<int> ans(n);
	for (int i = 0; i < n; i++)
	{
		ans[i] = cnt[i + 1];
	}

	return ans;
}




//等差数列差分
//等差数列差分问题描述：
//一开始 1~n 范围上的数字都是0.接下来一共有m个操作。
//每次操作：l~r 范围上依次加上首项s、末项e、公差d的数列
//最终 1~n 范围上的每个数字都要正确得到
//
//等差数列差分的过程：
//每个操作调用set方法
//所有操作完成后在arr上生成两遍前缀和，即调用build方法
//arr里就是最终 1~n 范围上的每个数字
vector<int>& arr;
int n;
void set(int l, int r, int s, int e, int d)
{
	arr[l] += s;
	arr[l + 1] += d - s;
	arr[r + 1] -= d + e;
	arr[r + 2] += e;
}

void build()
{
	for (int i = 1; i <= n; i++)
		arr[i] += arr[i - 1];

	for (int i = 1; i <= n; i++)
		arr[i] += arr[i - 1];
}



//题目二：
// 一开始1~n范围上的数字都是0，一共有m个操作，每次操作为(l,r,s,e,d)
// 表示在l~r范围上依次加上首项为s、末项为e、公差为d的数列
// m个操作做完之后，统计1~n范围上所有数字的最大值和异或和
// 测试链接 : https://www.luogu.com.cn/problem/P4231
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
typedef long long ll;
ll n, m;
ll arr[10000005];
ll ans_eor, maxs;
void set(ll l, ll r, ll s, ll e, ll d)
{
	arr[l] += s;
	arr[l + 1] += d - s;
	arr[r + 1] -= e + d;
	arr[r + 2] += e;
}

void build()
{
	for (int i = 1; i <= n; i++)//题目要求1~n个柱子，即柱子下标是1~n，因此是<=n
		arr[i] += arr[i - 1];
	for (int i = 1; i <= n; i++)
		arr[i] += arr[i - 1];
}

int main()
{
	// 提高输入输出效率
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> n >> m;
	for (int i = 0; i < m; i++)
	{
		ll l, r, s, e;
		cin >> l >> r >> s >> e;
		ll d = (e - s) / (r - l);
		set(l, r, s, e, d);
	}

	build();

	for (int i = 1; i <= n; i++)//柱子下标是1~n
	{
		ans_eor ^= arr[i];
		maxs = max(maxs, arr[i]);
	}
	cout << ans_eor << " " << maxs;
	return 0;
}




//题目三：
// 一群人落水后求每个位置的水位高度
// 问题描述比较复杂，见测试链接
// 测试链接 : https://www.luogu.com.cn/problem/P5026
#include <iostream>
using namespace std;
int n, m;

// 题目说了m <= 10^6，代表湖泊宽度
// 这就是MAXSIZE的含义，湖泊最大宽度的极限
const int MAXSIZE = 1000005;

// 数值保护，因为题目中v最大是10000
// 所以左侧影响最远的位置到达了x - 3 * v + 1
// 所以右侧影响最远的位置到达了x + 3 * v - 1
// x如果是正式的位置(1~m)，那么左、右侧可能超过正式位置差不多30000的规模
// 这就是OFFSET的含义
const int OFFSET = 30001;//用于将整体数组偏移，防止去写复杂的边界判断    （神来之笔）

int arr[OFFSET + MAXSIZE + OFFSET];

void set(int l, int r, int s, int e, int d)
{
	// 为了防止x - 3 * v + 1出现负数下标，进而有很多很烦的边界讨论
	// 所以任何位置，都加上一个较大的数字(OFFSET)
	// 这样一来，所有下标就都在0以上了，省去了大量边界讨论
	// 这就是为什么arr在初始化的时候要准备OFFSET + MAXN + OFFSET这么多的空间
	arr[l + OFFSET] += s;
	arr[l + 1 + OFFSET] += d - s;
	arr[r + 1 + OFFSET] -= e + d;
	arr[r + 2 + OFFSET] += e;
}

void build()
{
	//这里直接将m+OFFSET范围上进行前缀和操作
	for (int i = 1; i <= m + OFFSET; i++)
		arr[i] += arr[i - 1];
	for (int i = 1; i <= m + OFFSET; i++)
		arr[i] += arr[i - 1];
}

void putToArr(int v, int x)//一个朋友跳水会导致四段区域出现等差变化
{
	set(x - 3 * v + 1, x - 2 * v, 1, v, 1);
	set(x - 2 * v + 1, x, v - 1, -v, -1);
	set(x + 1, x + 2 * v, -v + 1, v, 1);
	set(x + 2 * v + 1, x + 3 * v - 1, v - 1, 1, -1);
}

int main()
{
	// 提高输入输出效率
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	cin >> n >> m;
	for (int i = 0; i < n; i++)
	{
		int v, x;
		cin >> v >> x;
		putToArr(v, x);
	}

	build();

	// 0...OFFSET这些位置是辅助位置，为了防止越界设计的
	// 从OFFSET+1开始往下数m个，才是正式的位置1...m
	// 打印这些位置，就是返回正式位置1...m的水位
	for (int i = 1 + OFFSET; i <= m + OFFSET; i++)
		cout << arr[i] << " ";

	return 0;
}
