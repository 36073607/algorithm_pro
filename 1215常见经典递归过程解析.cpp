#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <climits>
using namespace std;

//任何递归都是dfs且非常灵活
//带路径的递归 vs 不带路径的递归(大部分dp,状态压缩dp认为是路径简化了结构，dp专题后续讲述)
//回溯这个术语并不重要

//个人对于递归的理解：
//1.找到base case，即获取一次答案的条件
//2.找到一个规模最小的子过程，即这个过程是完成这个问题的最小单元，更复杂的情况均有这个子过程推广而来


//题目一：返回字符串全部子序列，子序列要求去重。时间复杂度O(2^n * n)
string path;
unordered_set<string> ust;
void dfs(string s, int index)
{
	if (index == s.size())
	{
		ust.insert(path);
	}
	else
	{
		//选择当前字符
		path.push_back(s[index]);
		dfs(s, index + 1);
		//不选择当前字符
		path.pop_back();
		dfs(s, index + 1);
	}

}

vector<string> generatePermutation(string s)
{
	vector<string> ans;
	
	dfs(s, 0);
	for (auto it = ust.begin(); it != ust.end(); it++)
	{
		ans.push_back(*it);
	}
	return ans;
}



//题目二：返回数组的所有组合，可以无视元素顺序，但要求去重。时间复杂度O(2^n * n)
//法一： 采用上道题思路：要么有、要么没有
set<vector<int>> st;
vector<int> pth;

void dfs(vector<int>& nums, int index)
{
	if (index == nums.size())
	{
		vector<int> sortedPath = pth;
		sort(sortedPath.begin(), sortedPath.end());
		st.insert(sortedPath);
	}
	else
	{
		pth.push_back(nums[index]);
		dfs(nums, index + 1);

		pth.pop_back();
		dfs(nums, index + 1);
	}

}

vector<vector<int>> subsetsWithDup(vector<int>& nums)
{
	vector<vector<int>> ans;
	dfs(nums, 0);
	for (auto it = st.begin(); it != st.end(); it++)
		ans.push_back(*it);

	return ans;
}

//法二：将数据排序，相同元素分组，每组取不同个数的元素，分别调用递归
vector<int> pth;
vector<vector<int>> ans;

void dfs(vector<int>& nums, int index)
{
	if (index == nums.size())
	{
		ans.push_back(pth);
		return;
	}

	int back_index = index;//用于path回溯
	int nextGroupIndex = index;//下一组元素的首下标
	for (; nextGroupIndex < nums.size() - 1 && nums[nextGroupIndex] == nums[nextGroupIndex + 1]; nextGroupIndex++);
	nextGroupIndex += 1;

	//当前组取0个元素：
	dfs(nums, nextGroupIndex);

	//当前组取1~全部元素
	for (; index < nextGroupIndex; index++)
	{
		pth.push_back(nums[index]);
		dfs(nums, nextGroupIndex);
	}

	//回溯: 回溯完毕后，pth数组为空
	for (; back_index < nextGroupIndex; back_index++)
		pth.pop_back();
}

vector<vector<int>> subsetsWithDup(vector<int>& nums)
{
	sort(nums.begin(), nums.end());
	dfs(nums, 0);
	return ans;
}



//题目三：返回没有重复值数组的全部排列  时间复杂度O(n! * n)
//法一：将数组中两个元素交换，再紧接处理下一种情况:
//对于有n个元素的数组，每个元素都可能在数组中任何一个位置，每次通过swap交换某个元素位置后，再递归调用下一个元素即可
vector<vector<int>> ans;

void dfs(vector<int>& nums,int index)
{
	if (index == nums.size())
	{
		ans.push_back(nums);
		return;
	}

	for (int i = index; i < nums.size(); i++)
	{
		swap(nums[index], nums[i]);
		dfs(nums, index + 1);//注意这里是index+1
		swap(nums[index], nums[i]);
	}
}

vector<vector<int>> permute(vector<int>& nums)
{
	dfs(nums, 0);
	return ans;
}

//法二：对于路径数组(path)中的每一个位置，都可以存放nums数组中的每一个元素，对于某次过程中在nums数组中已经存放过的元素，用visited记录
vector<vector<int>> ans;
vector<int> pth;//路径
unordered_map<int, bool> visited;

void dfs(vector<int>& nums, int index)
{
	if (index == nums.size())
	{
		ans.push_back(pth);
		return;
	}

	for (int i = 0; i < nums.size(); i++)
	{
		if (!visited[nums[i]])
		{
			visited[nums[i]] = true;
			pth.push_back(nums[i]);
			dfs(nums, index + 1);
			visited[nums[i]] = false;
			pth.pop_back();
		}
	}
}

vector<vector<int>> permute(vector<int>& nums)
{
	dfs(nums, 0);
	return ans;
}



//题目四：返回可能有重复值数组的全部排列，排列要求去重。时间复杂度O(n! * n)
//按照上一题的思路，加上剪枝：如果要交换的两个元素相等，则跳过
vector<vector<int>> ans;

void dfs(vector<int>& nums, int index)
{
	if (index == nums.size())
	{
		ans.push_back(nums);
		return;
	}

	unordered_set<int> ust;
	for (int i = index; i < nums.size(); i++)
	{
		if (ust.find(nums[i]) == ust.end())//没有交换过
		{
			ust.insert(nums[i]);
			swap(nums[index], nums[i]);
			dfs(nums, index + 1);
			swap(nums[index], nums[i]);
		}
	}

}

vector<vector<int>> permuteUnique(vector<int>& nums)
{
	dfs(nums, 0);
	return ans;
}



//题目五：用递归逆序一个栈。时间复杂度O(n^2)
//·一个函数弹出栈底，但不改变栈的次序，
//·另一个函数通过获得每次的栈底，逆序
int bottomOut(stack<int>& s)
{
	int ans = s.top();
	s.pop();
	if (s.empty())
	{
		return ans;
	}
	else
	{
		int last = bottomOut(s);
		s.push(ans);
		return last;
	}
}

void reverse(stack<int>& s)
{
	if (s.empty())
		return;

	int tmp = bottomOut(s);
	reverse(s);
	s.push(tmp);
}



//题目六：用递归排序一个栈。时间复杂度O(n^2)
class sortStack
{
public:
	int findDepth(stack<int>& s)
	{
		if (s.empty())
			return 0;

		int nums = s.top();
		s.pop();
		int depth = findDepth(s) + 1;
		s.push(nums);
		return depth;
	}

	int findMax(stack<int>& s, int depth)
	{
		if (depth == 0)
			return INT_MIN;

		int nums = s.top();
		s.pop();
		int maxs = max(nums, findMax(s, depth - 1));
		s.push(nums);
		return maxs;
	}

	int findTimes(stack<int>& s, int maxs, int depth)
	{
		if (depth == 0)
			return 0;

		int nums = s.top();
		s.pop();
		int restTimes = findTimes(s, maxs, depth - 1);
		int times = restTimes + (nums == maxs ? 1 : 0);
		//int times = nums == maxs ? times + 1 : findTimes(s, maxs, depth - 1);  这样写有问题，因为没有事先分配times的内存，导致三目操作符"times+1"这个环节报错
		s.push(nums);
		return times;
	}

	void down(stack<int>& s, int maxs, int times, int depth)
	{
		if (depth == 0)
		{
			for (int i = 0; i < times; i++)
				s.push(maxs);
		}
		else
		{
			int nums = s.top();
			s.pop();
			down(s, maxs, times, depth - 1);
			if (nums != maxs)
				s.push(nums);
		}
	}

	void sort(stack<int>& s)
	{
		int depth = findDepth(s);
		while (depth)
		{
			int maxs = findMax(s, depth);
			int times = findTimes(s, maxs, depth);
			down(s, maxs, times, depth);
			depth -= times;
		}
	}
};



//题目七：打印n层汉诺塔问题的最优移动轨迹。时间复杂度O(2^n)
//思路：
//对于任意一层的元素，把上面的元素先移到other
//把该元素移到to
//把该元素下面的元素移到to
void hanoi_operate(int n, string from, string to, string other)
{
	if (n == 1)
		cout << "移动圆盘 1 从" << from << "到" << to << endl;
	else
	{
		hanoi_operate(n - 1, from, other, to);
		cout << "移动圆盘 " << n << " 从" << from << "到" << to << endl;
		hanoi_operate(n - 1, other, to, from);
	}
}

void hanoi(int n)
{
	if (n > 0)
		hanoi_operate(n, "左", "右", "中");
}

//n层汉诺塔，执行步数为 2^n - 1
