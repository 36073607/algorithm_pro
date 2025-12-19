#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
using namespace std;

//解决N皇后问题的时间复杂度是O(n!)，好的方法可以大量剪枝，大量优化常数时间（但复杂度还是O(n!)）

//法一：用数组表示路径实现。经典，但不推荐，常数时间慢
//1.记录之前每一行的皇后放在了什么列
//2.来到第i行的时候，可以根据0..i-1 行皇后的位置，判断能放在哪些列
//3.把能放的列都尝试一遍，每次尝试修改路径数组表示当前的决策（需回溯），后续返回的答案都累加返回
class Solution
{
public:
    int totalNQueens(int n) 
    {
        if (n <= 0)
            return 0;
        vector<int> path(n, -1);
        int ans = dfs(n, 0, path);
        return ans;
    }

    //n代表问题规模：n阶方阵
    //i代表第i行
    //path代表之前皇后的摆放位置，下标i表示行，path[i]表示列
    int dfs(int n, int i, vector<int>& path)
    {
        if (i == n)//成功摆完所有行
            return 1;
        int ans = 0;
        for (int j = 0; j < n; j++)//从第0列枚举
        {
            if (check(i, j, path))
            {
                path[i] = j;
                ans += dfs(n, i + 1, path);
                path[i] = -1;
            }
        }
        return ans;
    }

    //当前在第i行，第j列摆放了一个皇后
    //对于0..i-1行皇后的情况，path[0..i-1]有对应记录
    //返回是否冲突：若不会冲突，有效，返回true；若会冲突，无效，返回false
    bool check(int i, int j, vector<int>& path)
    {
        int ans = true;
        for (int k = 0; k < i; k++)//k循环 0..i-1行
        {
            if (j == path[k] || abs(i - k) == abs(j - path[k]))//abs(i - k) == abs(j - path[k]) 表示|当前行-过去行|==|当前列-过去列| -->>曼哈顿距离
            {
                ans = false;
                break;
            }
        }
        return ans;
    }

};

//法二：用位运算的方法。巧妙，推荐，常数时间快
// 1.int col : 0..i-1 行皇后放置的位置因为正下方向延伸的原因，哪些列不能再放皇后
// 2.int left : 0..i-1 行皇后放置的位置因为左下方向延伸的原因，哪些列不能再放皇后
// 3.int right : 0..i-1 行皇后放置的位置因为右下方向延伸的原因，哪些列不能再放皇后
// 4.根据col,left,right,用位运算快速判断能放哪些列
// 5.把能放的列都尝试一遍，每次尝试都修改3个数字表示当前的决策（这里因为之前放的皇后有持续影响，不回溯），后续返回的答案都累加返回
class Solution 
{
public:
    int totalNQueens(int n) 
    {
        if (n < 1)
            return 0;
        int limit = (1 << n) - 1;//n皇后问题的规模，也是当一个方案成功时其对应的状态
        int ans = dfs(limit, 0, 0, 0);
        return ans;
    }

    //limit:n皇后问题的规模
    //col: 之前皇后的列影响
    //left:之前皇后的 右上 -> 左下 的影响
    //right:之前皇后的 左上 -> 右下 的影响
    int dfs(int limit, int col, int left, int right)
    {
        if (col == limit)//最终列的情况与预期一致-->>所有皇后均放完
            return 1;

        int ans = 0;
        int ban = col | left | right;//总限制
        int candidate = limit & ~ban;//待选的位置：1表示可以选的位置；0表示不能选   记得根据问题的规模取&
        while (candidate != 0)//待选的位置不为空
        {
            int place = candidate & (-candidate);//Brian算法,取出最右边的1-->>待选位置
            candidate ^= place;//把已选的位置结算
            ans += dfs(limit, col | place, (unsigned)(left | place) >> 1, (right | place) << 1);
        }
        return ans;
    }

};
//tips: 写 int limit = (1 << n) - 1; 时，记得加括号精确表述
