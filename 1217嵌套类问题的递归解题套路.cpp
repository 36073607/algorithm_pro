#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cctype>
#include <vector>
#include <string>
#include <map>
using namespace std;

//嵌套类问题的解题套路：（递归版本思路）
//大概过程：
//1.定义全局变量 int where
//2.递归函数f(i): s[i..]，从i位置出发开始解析，遇到 字符串终止 或 嵌套条件终止 就返回
//3.返回值是f(i)负责这一段的结果
//4.f(i)在返回前更新全局变量where，让上级函数通过where知道解析到了什么位置，进而继续
//执行细节：
//1.如果f(i)遇到 嵌套条件开始 ，就调用下级递归去处理嵌套，下级会负责嵌套部分的计算结果
//2.f(i)下级处理完成后，f(i)可以根据下级更新的全局变量where，知道该从什么位置继续解析


//题目一：含有嵌套的表达式求值。时间复杂度O(n)
class Solution
{
	int where = 0;//全局变量where
	int caculate(string s, int index)
	{
		vector<int> nums;
		vector<char> ops;
		int cur = 0;
		while (index < s.size() && s[index] != ')')
		{
			if (isdigit(s[index]))//数字
			{
				cur = cur * 10 + (s[index++] - '0');
			}
			else if (s[index] != '(')//运算符
			{
				push(nums, ops, cur, s[index++]);
				cur = 0;
			}
			else//左括号
			{
				cur = caculate(s, index + 1);
				index = where + 1;
			}
		}
		//最后一个数由于字符串终止或嵌套条件终止没加上，这里把最后一个数字加上
		push(nums, ops, cur, '+');
		where = index;// 记录当前位置，用于返回给上一层

		return compute(nums, ops);
	}

	void push(vector<int>& nums, vector<char>& ops, int cur, char ch)
	{
		if (!ops.empty())
		{
			char topOp = ops.back();
			if (topOp == '*')
			{
				ops.pop_back();//把*弹出
				int topNum = nums.back();
				nums.pop_back();
				nums.push_back(topNum * cur);
				ops.push_back(ch);
				return;
			}
			else if (topOp == '/')
			{
				ops.pop_back();
				int topNum = nums.back();
				nums.pop_back();
				nums.push_back(topNum / cur);
				ops.push_back(ch);
				return;
			}
		}
		//对于+ - 或 空栈
		nums.push_back(cur);
		ops.push_back(ch);
	}

	int compute(vector<int>& nums, vector<char>& ops)
	{
		if (nums.empty())
			return 0x3f3f3f3f;

		// 移除最后多余的'+'
		if (!ops.empty() && ops.back() == '+')
			ops.pop_back();

		int ans = nums[0];
		for (int i = 1; i < nums.size(); i++)
		{
			char ch = ops[i - 1];
			if (ch == '+')
			{
				ans = ans + nums[i];
			}
			else if (ch == '-')
			{
				ans = ans - nums[i];
			}
		}
		return ans;
	}

};



//题目二：含有嵌套的字符串解码。时间复杂度O(n)
class Solution 
{
public:
	int where = 0;

	string decodeString(string s) 
	{
		if (s.empty())
			return s;

		return f(s, 0);
	}

	string f(string s, int i)
	{
		int curTimes = 0;
		string path;

		while (i < s.size() && s[i] != ']')
		{
			if (isdigit(s[i]))
			{
				curTimes = curTimes * 10 + s[i++] - '0';
			}
			else if (s[i] == '[')
			{
				string tmp = f(s, i + 1);
				for (int k = 0; k < curTimes; k++)
					path += tmp;
				i = where + 1;//返回后记得更新i
				curTimes = 0;
			}
			else
				path += s[i++];
		}

		where = i;
		return path;
	}
};



//题目三：含有嵌套的分子式求原子数量
//核心：遇到 大写字母 和 左括号 就结算
class Solution
{
public:
	int where = 0;

	string countOfAtoms(string formula)
	{
		string ans;
		if (formula.empty())
			return ans;

		map<string, int> mp = f(formula, 0);
		
		for (auto& it : mp)
		{
			ans += it.first;
			if (it.second > 1)
				ans += to_string(it.second);
		}

		return ans;
	}

	map<string, int> f(string& s, int i)
	{
		map<string, int> curLevelAns;//本层结果
		string path;//记录历史存储
		map<string, int> prev;//记录历史存储
		int cnt = 0;//记录倍数
		while (i < s.size() && s[i] != ')')
		{
			if (isupper(s[i]) || s[i] == '(')
			{
				fill(curLevelAns, path, prev, cnt);
				path.clear();
				prev.clear();
				cnt = 0;

				if (isupper(s[i]))
				{
					path += s[i++];
				}
				else//左括号
				{
					prev = f(s, i + 1);
					i = where + 1;//更新下标
				}
			}
			else if (islower(s[i]))
				path += s[i++];
			else//数字
				cnt = cnt * 10 + (s[i++] - '0');
		}

		fill(curLevelAns, path, prev, cnt);
		where = i;
		return curLevelAns;
	}

	void fill(map<string, int>& ans, string& path, map<string, int>& prev, int cnt)
	{
		cnt = cnt == 0 ? 1 : cnt;
		if (!path.empty())
		{
			ans[path] += cnt;
		}
		
		if (!prev.empty())
		{
			for (auto& ele : prev)
			{
				ans[ele.first] += ele.second * cnt;
			}
		}
	}
};
