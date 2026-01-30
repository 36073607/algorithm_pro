#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
using namespace std;


//洪水填充
//洪水填充是一种很简单的技巧，设置路径信息进行剪枝和统计，类似感染的过程
//路径信息不撤销，来保证每一片的感染过程可以得到区分
//看似是暴力递归过程，其实时间复杂度非常好，遍历次数和样本数量的规模一致

//这节课用并查集也能解决

//题目一：
// 岛屿数量
// 给你一个由 '1'（陆地）和 '0'（水）组成的的二维网格，请你计算网格中岛屿的数量
// 岛屿总是被水包围，并且每座岛屿只能由水平方向和/或竖直方向上相邻的陆地连接形成
// 此外，你可以假设该网格的四条边均被水包围
// 测试链接 : https://leetcode.cn/problems/number-of-islands/
//
//注：并查集中讲过该题
class Solution 
{
public:
    void dfs(int i, int j, vector<vector<char>>& grid)
    {
        if (i<0 || i>=grid.size() || j<0 || j>=grid[0].size() || grid[i][j] != '1')//base case
            return;

        grid[i][j] = '0';
        dfs(i - 1, j, grid);
        dfs(i + 1, j, grid);
        dfs(i, j - 1, grid);
        dfs(i, j + 1, grid);
    }

    int numIslands(vector<vector<char>>& grid)
    {
        int ans = 0;
        for (int i = 0; i < grid.size(); i++)
        {
            for (int j = 0; j < grid[0].size(); j++)
            {
                if (grid[i][j] == '1')
                {
                    ans++;
                    dfs(i, j, grid);
                }
            }
        }
        return ans;
    }
};



//题目二：
// 被围绕的区域
// 给你一个 m x n 的矩阵 board ，由若干字符 'X' 和 'O' ，找到所有被 'X' 围绕的区域
// 并将这些区域里所有的 'O' 用 'X' 填充。
// 测试链接 : https://leetcode.cn/problems/surrounded-regions/
//
//思路：从边界'O'出发，将连通的'O'全都改为'F' ， 然后把矩阵遍历一遍，把剩下的'O'全部改为'X'，最后遍历一遍把'F'还原为'O'
class Solution
{
private:
    void dfs(vector<vector<char>>& board, int i, int j)
    {
        if (i < 0 || i >= board.size() || j < 0 || j >= board[0].size() || board[i][j] == 'X' || board[i][j] == 'F')
            return;

        board[i][j] = 'F';
        dfs(board, i + 1, j);
        dfs(board, i - 1, j);
        dfs(board, i, j + 1);
        dfs(board, i, j - 1);
    }

public:
    void solve(vector<vector<char>>& board) 
    {
        int m = board.size();
        int n = board[0].size();

        for (int i = 0; i < m; i++)//检查竖边界
        {
            if (board[i][0] == 'O')
                dfs(board, i, 0);
            if (board[i][n - 1] == 'O')
                dfs(board, i, n - 1);
        }
        for (int j = 0; j < n; j++)//检查横边界
        {
            if (board[0][j] == 'O')
                dfs(board, 0, j);
            if (board[m - 1][j] == 'O')
                dfs(board, m - 1, j);
        }

        for (int i = 0; i < m; i++)//将 被包围的'O'改为'X'
        {
            for (int j = 0; j < n; j++)
            {
                if (board[i][j] == 'O')
                    board[i][j] = 'X';
            }
        }

        for (int i = 0; i < m; i++)//将边界'O'还原为'X'
        {
            for (int j = 0; j < n; j++)
            {
                if (board[i][j] == 'F')
                    board[i][j] = 'O';
            }
        }
    }
};



//题目三：
// 最大人工岛
// 给你一个大小为 n * n 二进制矩阵 grid 。最多 只能将一格 0 变成 1 。
// 返回执行此操作后，grid 中最大的岛屿面积是多少？
// 岛屿 由一组上、下、左、右四个方向相连的 1 形成
// 测试链接 : https://leetcode.cn/problems/making-a-large-island/
//
//思路：
//先洪水填充，把所有岛屿依次编号
//再依次遍历每个'0'，检验如果把该'0'换为'1'时能产生的岛屿面积
//最大值即为答案
//
//如果用并查集的话，使用洪水填充给岛屿编号的过程可以使用信息数组
class Solution
{
private:
    void dfs(vector<vector<int>>& grid, int i, int j, int ID)
    {
        if (i < 0 || i >= grid.size() || j < 0 || j >= grid[0].size() || grid[i][j] != 1)//越界 或 遇到0或其他岛屿
            return;

        grid[i][j] = ID;
        dfs(grid, i + 1, j, ID);
        dfs(grid, i - 1, j, ID);
        dfs(grid, i, j + 1, ID);
        dfs(grid, i, j - 1, ID);
    }

public:
    int largestIsland(vector<vector<int>>& grid)
    {
        int m = grid.size();
        int n = grid[0].size();
        int islandID = 2;//岛屿编号，从2开始
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (grid[i][j] == 1)
                    dfs(grid, i, j, islandID++);
            }
        }

        vector<int> islandSize(islandID, 0);
        int ans = 0;
        for (int i = 0; i < m; i++)//统计每个岛屿的大小
        {
            for (int j = 0; j < n; j++)
            {
                if (grid[i][j] > 1)
                {
                    islandSize[grid[i][j]]++;
                    ans = max(ans, islandSize[grid[i][j]]);
                }
            }
        }

        vector<bool> isCollect(islandID, false);
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (grid[i][j] == 0)
                {
                    int up, down, left, right;//该点上下左右的情况
                    up = i > 0 ? grid[i - 1][j] : 0;
                    down = i < m - 1 ? grid[i + 1][j] : 0;
                    left = j > 0 ? grid[i][j - 1] : 0;
                    right = j < n - 1 ? grid[i][j + 1] : 0;

                    int curans = 1;
                    curans += islandSize[up];
                    isCollect[up] = true;

                    if (!isCollect[down])
                    {
                        curans += islandSize[down];
                        isCollect[down] = true;
                    }
                    if (!isCollect[left])
                    {
                        curans += islandSize[left];
                        isCollect[left] = true;
                    }
                    if (!isCollect[right])
                    {
                        curans += islandSize[right];
                        isCollect[right] = true;
                    }

                    ans = max(ans, curans);
                    isCollect[up] = false;
                    isCollect[down] = false;
                    isCollect[left] = false;
                    isCollect[right] = false;
                }
            }
        }

        return ans;
    }
};



//题目四：
// 打砖块
// 有一个 m * n 的二元网格 grid ，其中 1 表示砖块，0 表示空白
// 砖块 稳定（不会掉落）的前提是：
// 一块砖直接连接到网格的顶部，或者
// 至少有一块相邻（4 个方向之一）砖块 稳定 不会掉落时
// 给你一个数组 hits ，这是需要依次消除砖块的位置
// 每当消除 hits[i] = (rowi, coli) 位置上的砖块时，对应位置的砖块（若存在）会消失
// 然后其他的砖块可能因为这一消除操作而 掉落
// 一旦砖块掉落，它会 立即 从网格 grid 中消失（即，它不会落在其他稳定的砖块上）
// 返回一个数组 result ，其中 result[i] 表示第 i 次消除操作对应掉落的砖块数目。
// 注意，消除可能指向是没有砖块的空白位置，如果发生这种情况，则没有砖块掉落。
// 测试链接 : https://leetcode.cn/problems/bricks-falling-when-hit/
//
//重要技巧：时光倒流（败者时辰）
//从最终结果倒着往前推
//例如在本题中，将 所有顺时序炮弹产生的破坏效果 转化成 逆时序从破坏完的状态逐步复原到原状态的过程
//应用场景，有些数据结构便于合并，但不便于删除，就可以应用这个技巧
//eg: 并查集便于合并，不便于删除，利用的这一点
class Solution 
{
private:
    //设置为“全局变量”
    int m, n;
    vector<vector<int>> grid;
    vector<vector<int>> hits;

    int dfs(int i, int j)//返回新增的"2"的数目-->>打掉该砖块时砖块掉落的数目
    {
        if (i < 0 || i >= m || j < 0 || j >= n || grid[i][j] != 1)//越界 or 该砖块不是 不坚固的砖块 
            return 0;

        //当前位置是1 -->> 不坚固的砖块/hits数组中打掉的砖块
        grid[i][j] = 2;
        return 1 + dfs(i - 1, j) + dfs(i + 1, j) + dfs(i, j - 1) + dfs(i, j + 1);
    }

    bool worth(int i, int j)
    {
        return grid[i][j] == 1
            &&
            (
                i == 0 ||
                i > 0 && grid[i - 1][j] == 2 ||
                i < m - 1 && grid[i + 1][j] == 2 ||
                j>0 && grid[i][j - 1] == 2 ||
                j < n - 1 && grid[i][j + 1] == 2
             );
    }

public:
    vector<int> hitBricks(vector<vector<int>>& gd, vector<vector<int>>& ht) 
    {
        grid = gd;
        hits = ht;
        m = grid.size();
        n = grid[0].size();

        vector<int> ans(hits.size(), 0);

        for (auto& ele : hits)//所有砖块消除后的最终结果
        {
            grid[ele[0]][ele[1]]--;
        }

        for (int j = 0; j < n; j++)//在最终状态下时，把仍然坚固的砖块都刷成"2" -->> 与坚固砖块断开的仍然是“1”
        {
            dfs(0, j);//这里不需要返回值，仅需把当前坚固砖块都刷成"2", 与此时 不坚固的砖块 区分开
        }

        //时光倒流
        for (int i = hits.size() - 1; i >= 0; i--)
        {
            int row = hits[i][0];
            int col = hits[i][1];
            grid[row][col]++;//将炮弹轰碎的砖块回溯
            if (worth(row, col))//该点是否值得检查，如果没有这个判断，就等同于默认了 轰碎一个砖块一定会导致其他砖块掉落，这是不对的
            {
                int fallingBricks = dfs(row, col) - 1;//轰碎该砖块导致其他砖块掉落的数量
                ans[i] = fallingBricks;
            } 
        }
        return ans;
    }
};


