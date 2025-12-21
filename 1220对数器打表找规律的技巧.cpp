#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <climits>
using namespace std;

//对数器打表找规律的使用场景：输入参数是简单类型，返回值也是简单类型

//对数器打表找规律的过程
//1.可以用最暴力的实现求入参不大情况下的答案，往往只需要最基本的递归能力
//2.打印入参不大情况下的答案，然后观察规律
//3.把规律变成代码，就是最优解了


//题目一：
// 有装下8个苹果的袋子、装下6个苹果的袋子，一定要保证买苹果时所有使用的袋子都装满
// 对于无法装满所有袋子的方案不予考虑，给定n个苹果，返回至少要多少个袋子
// 如果不存在每个袋子都装满的方案返回-1
class AppleMinBags
{
	int bag1(int apple)
	{
		int ans = bag1_operate(apple);
		return ans == INT_MAX ? -1 : ans;
	}

	int bag1_operate(int rest)// 当前还有rest个苹果，使用的每个袋子必须装满，返回至少几个袋子
	{
		if (rest < 0)
			return INT_MAX;
		if (rest == 0)
			return 0;

		int bag_6 = bag1_operate(rest - 6);//使用6规格的袋子，剩余的苹果还需要几个袋子，有可能返回无效解
		int bag_8 = bag1_operate(rest - 8);//使用8规格的袋子，剩余的苹果还需要几个袋子，有可能返回无效解
		bag_6 += bag_6 == INT_MAX ? 0 : 1;
		bag_8 += bag_8 == INT_MAX ? 0 : 1;

		return min(bag_6, bag_8);
	}

	int main()
	{
		for (int i = 0; i < 100; i++)
			cout << i << ":" << " " << bag1(i) << endl;

		return 0;
	}

	//根据暴力实现得到的结果进行找规律->打表
	int bags2(int apple)
	{
		if ((apple & 1) != 0)
		{
			return -1;
		}
		if (apple < 18)
		{
			if (apple == 0)
			{
				return 0;
			}
			if (apple == 6 || apple == 8)
			{
				return 1;
			}
			if (apple == 12 || apple == 14 || apple == 16)
			{
				return 2;
			}
			return -1;
		}
		return (apple - 18) / 8 + 3;
	}
};



//题目二：
// 草一共有n的重量，两只牛轮流吃草，A牛先吃，B牛后吃
// 每只牛在自己的回合，吃草的重量必须是4的幂，1、4、16、64....
// 谁在自己的回合正好把草吃完谁赢，根据输入的n，返回谁赢
class EatGrass
{
	string win1(int n)
	{
		return win1_operate(n, "A");
	}

	// rest : 还剩多少草
	// cur  : 当前选手的名字
	// 返回  : 还剩rest份草，当前选手是cur，按照题目说的，返回最终谁赢
	string win1_operate(int rest, string cur)
	{
		string enemy = cur == "A" ? "B" : "A";
		if (rest < 5)
		{
			//   0  1  2  3  4
			//A    √    √ √
			//B
			if (rest == 0 || rest == 2)
				return enemy;
			return cur;
		}

		int pick = 1;
		while (pick <= rest)
		{
			if (win1_operate(rest - pick, enemy) == cur)
				return cur;
			pick *= 4;
		}
		return enemy;
	}

	int main()
	{
		for (int i = 0; i <= 50; i++)
			cout << i << " " << win1(i) << endl;

		return 0;
	}

	//打表
	string win2(int n) 
	{
		if (n % 5 == 0 || n % 5 == 2)
			return "B";
		else
			return "A";
	}

};



//题目三：判断一个数字是否是若干数量(数量>1)的连续正整数的和
class IsSumOfConsecutiveNumbers
{
	bool is1(int num)
	{
		for (int start = 1; start <= num; start++)
		{
			for (int j = start + 1, sum = start; j <= num; j++)
			{
				if (sum < num)
					sum += j;
				else if (sum == num)
					return true;
				else
					break;
			}
		}
		return false;
	}

	int main()
	{
		for (int i = 0; i < 200; i++)
			cout << i << " " << (is1(i) ? "T" : "F") << endl;

		return 0;
	}

	//打表
	bool is2(int num)
	{
		return num > 0 && num == (num & -num);
		//return (num & (num - 1)) != 0;
	}

};



//题目四：
// 可以用r、e、d三种字符拼接字符串，如果拼出来的字符串中
// 有且仅有1个长度>=2的回文子串，那么这个字符串定义为"好串"
// 返回长度为n的所有可能的字符串中，好串有多少个
// 结果对 1000000007 取模， 1 <= n <= 10^9
// 示例：
// n = 1, 输出0
// n = 2, 输出3
// n = 3, 输出18
class RedPalindromeGoodStrings
{
	int num1(int n)
	{
		if (n <= 1)
			return 0;
		string path(n,'\0');
		return dfs(path, 0);
	}

	int dfs(string& path, int index)
	{
		if (index == path.size())
		{
			int cnt = 0;
			for (int i = 0; i < path.size(); i++)
			{
				for (int j = i + 1; j < path.size(); j++)
				{
					if (check(i, j, path))
						cnt++;
					if (cnt > 1)
						return 0;
				}
			}
			return cnt == 1 ? 1 : 0;
		}

		int ans = 0;
		path[index] = 'r';
		ans += dfs(path, index + 1);

		path[index] = 'e';
		ans += dfs(path, index + 1);

		path[index] = 'd';
		ans += dfs(path, index + 1);

		return ans;
	}

	bool check(int l,int r, string& path)
	{
		while (l <= r)
		{
			if (path[l] != path[r])
				return false;

			l++;
			r--;
		}
		return true;
	}

	int main()
	{
		for (int i = 1; i <= 10; i++)
			cout << "长度为：" << i << " " << "答案：" << num1(i) << endl;

		return 0;
	}	

	int num2(int n)
	{
		if (n == 1)
			return 0;
		if (n == 2)
			return 3;
		if (n == 3)
			return 18;

		return (int)(((long)6 * (n + 1)) % 1000000007);
	}

};

