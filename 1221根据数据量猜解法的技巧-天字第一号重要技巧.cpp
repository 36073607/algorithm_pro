#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>
#include <vector>
using namespace std;

//一个基本事实：
//C/C++运行时间1s，java/python/go等语言运行时间1s-2s
//对应的常数指令操作量是 10^7 ~ 10^8 ，不管什么测试平台，不管什么cpu，都是这个数量级
//所以可以根据这个基本事实，来猜测自己设计的算法最终有没有可能在规定时间内通过

//运用 根据数据量猜解法技巧 的必要条件：
//1.题目要给定各个入参的范围最大值，正式笔试、比赛的题目一定都会给，面试中要和面试官确认
//2.对于自己设计的算法，时间复杂度要有准确的估计


//题目一：
// 测试链接：https://www.nowcoder.com/practice/d88ef50f8dab4850be8cd4b95514bbbd
// 现在有一个打怪类型的游戏，这个游戏是这样的，你有n个技能
// 每一个技能会有一个伤害，
// 同时若怪物小于等于一定的血量，则该技能可能造成双倍伤害
// 每一个技能最多只能释放一次，已知怪物有m点血量
// 现在想问你最少用几个技能能消灭掉他(血量小于等于0)
// 技能的数量是n，怪物的血量是m
// i号技能的伤害是x[i]，i号技能触发双倍伤害的血量最小值是y[i]
// 1 <= n <= 10
// 1 <= m、x[i]、y[i] <= 10^6
//
//因为 n<=10 -->>全排列 O(n!)
class KillMonsterEverySkillUseOnce
{
    int hurt[12];//每个技能常规伤害
    int blood[12];//每个技能双倍伤害触发血量
    int n, m;

    void my_swap(int i, int j)
    {
        swap(hurt[i], hurt[j]);
        swap(blood[i], blood[j]);
    }

    int dfs(int rest_blood, int index)
    {
        if (rest_blood <= 0)
            return index;

        if (index == n)
            return INT_MAX;

        int ans = INT_MAX;
        for (int j = index; j < n; j++)
        {
            my_swap(index, j);
            ans = min(ans, dfs(rest_blood - (rest_blood <= blood[index] ? 2 * hurt[index] : hurt[index]), index + 1));
            my_swap(index, j);
        }

        return ans;
    }


    int main()
    {
        int T;
        cin >> T;
        for (int i = 0; i < T; i++)
        {
            cin >> n >> m;
            for (int j = 0; j < n; j++)
            {
                cin >> hurt[j] >> blood[j];
            }

            int ans = dfs(m, 0) == INT_MAX ? -1 : dfs(m, 0);
            cout << ans << endl;
        }

        return 0;
    }

};



//题目二：
// 超级回文数
// 如果一个正整数自身是回文数，而且它也是一个回文数的平方，那么我们称这个数为超级回文数。
// 现在，给定两个正整数 L 和 R （以字符串形式表示），
// 返回包含在范围 [L, R] 中的超级回文数的数目。
// 1 <= len(L) <= 18
// 1 <= len(R) <= 18
// L 和 R 是表示 [1, 10^18) 范围的整数的字符串
// 测试链接 : https://leetcode.cn/problems/super-palindromes/
class SuperpalindromesInRange1 
{
public:
    int superpalindromesInRange(string left, string right) 
    {
        long long l = stoll(left);
        long long r = stoll(right);
        long long limit = static_cast<long long>(sqrt(r));
        // seed : 枚举量很小，10^18 -> 10^9 -> 10^5
        // seed : 奇数长度回文、偶数长度回文
        long long seed = 1;
        long long num = 0;// num : 根号x，num^2 -> x
        int ans = 0;
        do {
            //  seed生成偶数长度回文数字
            // 123 -> 123321
            num = evenEnlarge(seed);
            if (num <= limit && safeSquare(num) && check(num * num, l, r)) 
            {
                ans++;
            }

            //  seed生成奇数长度回文数字
            // 123 -> 12321
            num = oddEnlarge(seed);
            if (num <= limit && safeSquare(num) && check(num * num, l, r)) 
            {
                ans++;
            }

            seed++;
        } while (num < limit);

        return ans;
    }

private:
    bool safeSquare(long long num) 
    {
        return num <= static_cast<long long>(sqrt(numeric_limits<long long>::max()));
        //return num <= static_cast<long long>(sqrt(LLONG_MAX));
    }

    long long evenEnlarge(long long seed)// 根据种子扩充到偶数长度的回文数字并返回
    {
        long long ans = seed;
        while (seed != 0)
        {
            ans = ans * 10 + seed % 10;
            seed /= 10;
        }
        return ans;
    }

    long long oddEnlarge(long long seed)// 根据种子扩充到奇数长度的回文数字并返回
    {
        long long ans = seed;
        seed /= 10;
        while (seed != 0) 
        {
            ans = ans * 10 + seed % 10;
            seed /= 10;
        }
        return ans;
    }

    bool check(long long ans, long long l, long long r)// 判断ans是不是属于[l,r]范围的回文数
    {
        return ans >= l && ans <= r && isPalindrome(ans);
    }

    bool isPalindrome(long long num)//是不是回文数字
    {
        long long offset = 1;
        while (num / offset >= 10) // 注意这么写是为了防止溢出
        {
            offset *= 10;
        }
        while (num != 0)
        {
            if (num / offset != num % 10) 
            {
                return false;
            }
            num = (num % offset) / 10;
            offset /= 100;
        }
        return true;
    }
};

class SuperpalindromesInRange2//打表
{
public:
    const vector<long long> record =
    {
       1L, 4L, 9L, 121L, 484L, 10201L, 12321L, 14641L, 40804L, 44944L,
       1002001L, 1234321L, 4008004L, 100020001L, 102030201L, 104060401L,
       121242121L, 123454321L, 125686521L, 400080004L, 404090404L,
       10000200001L, 10221412201L, 12102420121L, 12345654321L,
       40000800004L, 1000002000001L, 1002003002001L, 1004006004001L,
       1020304030201L, 1022325232201L, 1024348434201L, 1210024200121L,
       1212225222121L, 1214428244121L, 1232346432321L, 1234567654321L,
       4000008000004L, 4004009004004L, 100000020000001L, 100220141022001L,
       102012040210201L, 102234363432201L, 121000242000121L,
       121242363242121L, 123212464212321L, 123456787654321L,
       400000080000004L, 10000000200000001L, 10002000300020001L,
       10004000600040001L, 10020210401202001L, 10022212521222001L,
       10024214841242001L, 10201020402010201L, 10203040504030201L,
       10205060806050201L, 10221432623412201L, 10223454745432201L,
       12100002420000121L, 12102202520220121L, 12104402820440121L,
       12122232623222121L, 12124434743442121L, 12321024642012321L,
       12323244744232321L, 12343456865434321L, 12345678987654321L,
       40000000800000004L, 40004000900040004L, 1000000002000000001L,
       1000220014100220001L, 1002003004003002001L, 1002223236323222001L,
       1020100204020010201L, 1020322416142230201L, 1022123226223212201L,
       1022345658565432201L, 1210000024200000121L, 1210242036302420121L,
       1212203226223022121L, 1212445458545442121L, 1232100246420012321L,
       1232344458544432321L, 1234323468643234321L, 4000000008000000004L

    };

    int superpalindromesInRange(string left, string right)
    {
        long long l = stoll(left);
        long long r = stoll(right);

        int i = 0;
        for (; i < record.size() && record[i] < l; i++);
        
        int j = record.size() - 1;
        for (; j >= 0 && record[j] > r; j--);

        return (j >= i) ? j - i + 1 : 0;
    }

private://自己先用private中的函数把所有值打印出来，填到public中revord数组
    vector<long long> collect()
    {
        long long l = 0;
        long long r = LLONG_MAX;
        long long limit = static_cast<long long>(sqrt(r));
        long long seed = 1;
        long long num = 0;
        vector<long long> ans;

        while (1)
        {
            num = evenEnlarge(seed);
            if (security_square(num))
            {
                long long x = num * num;
                if (check(x, l, r))
                    ans.push_back(x);
            }

            num = oddEnlarge(seed);
            if (security_square(num))
            {
                long long x = num * num;
                if (check(x, l, r))
                    ans.push_back(x);
            }

            if (num > limit)
                break;

            seed++;
        }

        return ans;
    }

    long long evenEnlarge(long long seed)
    {
        long long ans = seed;
        while (seed)
        {
            ans = ans * 10 + seed % 10;
            seed /= 10;
        }
        return ans;
    }

    long long oddEnlarge(long long seed)
    {
        long long ans = seed;
        seed /= 10;
        while (seed)
        {
            ans = ans * 10 + seed % 10;
            seed /= 10;
        }
        return ans;
    }

    bool security_square(long long x)
    {
        if (x <= static_cast<long long>(sqrt(LLONG_MAX)))
            return true;
        return false;
    }

    bool check(long long x, long long l, long long r)
    {
        if (x >= l && x <= r && isPalindrome(x))
            return true;
        return false;
    }

    bool isPalindrome(long long x)
    {
        long long offset = 1;
        while (x / offset >= 10)
            offset *= 10;

        while (x)
        {
            if (x / offset != x % 10)
                return false;
            
            x = (x % offset) / 10;
            offset /= 100;
        }
        return true;
    }

    int main()
    {
        vector<long long> ans = collect();
        sort(ans.begin(), ans.end());
        for (auto& it : ans)
            cout << it << endl;

        return 0;
    }

};