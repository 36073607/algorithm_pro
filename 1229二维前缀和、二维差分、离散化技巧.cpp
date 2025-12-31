#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


//二维前缀和原理和代码详解
//目的是预处理出一个结构，以后每次查询二维数组任何范围上的累加和都是O(1)的操作
//1.根据原始状况，生成二维前缀和数组sum
//  sum[i][j]: 代表左上角(0,0)到右下角(i,j)这个范围的累加和
//  sum[i][j] += sum[i][j-1] + sum[i-1][j] - sum[i-1][j-1]; -->>左+上-左上+自己
// 
//2.查询左上角(a,b)到右下角(c,d)这个范围的累加和
//  sum[c][d] - sum[c][b-1] - sum[a-1][d] + sum[a-1][b-1];
//
//3.实际过程中往往补第0行、第0列来减少很多条件判断
//  当然也可以不补。根据个人习惯决定      (然鹅我个人更习惯不补0，直接写一个get方法)

//题目一：
// 利用二维前缀和信息迅速得到二维区域和
// 测试链接 : https://leetcode.cn/problems/range-sum-query-2d-immutable/
class NumMatrix1 //这里采用不补第0行，第0列的写法
{
public:
    vector<vector<int>> prefix;
    NumMatrix1(vector<vector<int>>& matrix) 
    {
        if (matrix.empty() || matrix[0].empty())
            return;

        prefix.resize(matrix.size(), vector<int>(matrix[0].size(), 0));
        for (int i = 0; i < matrix.size(); i++)
        {
            for (int j = 0; j < matrix[0].size(); j++)
            {
                prefix[i][j] += matrix[i][j];// 初始化为当前矩阵值
                if (i > 0)// 加上上方的值（如果存在）
                    prefix[i][j] += prefix[i - 1][j];
                if (j > 0) // 加上左方的值（如果存在）
                    prefix[i][j] += prefix[i][j - 1];
                if (i > 0 && j > 0)// 减去重复计算的左上角值（如果存在）
                    prefix[i][j] -= prefix[i - 1][j - 1];
            }
        }
    }

    int sumRegion(int a, int b, int c, int d) 
    {
        int ans = prefix[c][d];
        if (a > 0)
            ans -= prefix[a - 1][d];
        if (b > 0)
            ans -= prefix[c][b - 1];
        if (a > 0 && b > 0)
            ans += prefix[a - 1][b - 1];
        return ans;
    }
};


class NumMatrix2//采用补第0行，第0列的写法
{
private:
    vector<vector<int>> sum;

public:
    NumMatrix2(vector<vector<int>>& matrix) 
    {
        if (matrix.empty() || matrix[0].empty()) return;

        int n = matrix.size();
        int m = matrix[0].size();
        sum.resize(n + 1, vector<int>(m + 1, 0));

        // 构建前缀和数组
        for (int i = 0; i < n; i++) 
        {
            for (int j = 0; j < m; j++)
            {
                // sum[i+1][j+1] 表示从(0,0)到(i,j)的子矩阵和
                sum[i + 1][j + 1] = matrix[i][j] + sum[i][j + 1] + sum[i + 1][j] - sum[i][j];
            }
        }
    }

    int sumRegion(int row1, int col1, int row2, int col2)
    {
        // 转换为sum数组的索引（都加1）
        int r1 = row1 + 1, c1 = col1 + 1;
        int r2 = row2 + 1, c2 = col2 + 1;

        return sum[r2][c2] - sum[r1 - 1][c2] - sum[r2][c1 - 1] + sum[r1 - 1][c1 - 1];
    }
};



//题目二：
// 边框为1的最大正方形
// 给你一个由若干 0 和 1 组成的二维网格 grid
// 请你找出边界全部由 1 组成的最大 正方形 子网格
// 并返回该子网格中的元素数量。如果不存在，则返回 0。
// 测试链接 : https://leetcode.cn/problems/largest-1-bordered-square/
class Solution 
{
private:
    void build(vector<vector<int>>& grid)//为了节省空间，直接复用给过来的数组，无需补行补列
    {
        for (int i = 0; i < grid.size(); i++)
        {
            for (int j = 0; j < grid[0].size(); j++)
            {
                grid[i][j] += get(i, j - 1, grid) + get(i - 1, j, grid) - get(i - 1, j - 1, grid);
            }
        }
    }

    int get(int i, int j, const vector<vector<int>>& grid)
    {
        return (i < 0 || j < 0) ? 0 : grid[i][j];
    }

    int sum(int a, int b, int c, int d, const vector<vector<int>>& grid)
    {
        return grid[c][d] - get(c, b - 1, grid) - get(a - 1, d, grid) + get(a - 1, b - 1, grid);
    }

public:
    int largest1BorderedSquare(vector<vector<int>>& grid)
    {
        build(grid);
        int m = grid.size();
        int n = grid[0].size();
        if (sum(0, 0, m - 1, n - 1, grid) == 0)
            return 0;

        int ans = 1;//若矩阵不全为零，则必定存在边长为1的答案
        for (int a = 0; a < m; a++)
        {
            for (int b = 0; b < n; b++)
            {
                // (a,b)所有左上角点
                // (c,d)更大边长的右下角点
                //  k是当前尝试的边长
                for (int c = a + ans, d = b + ans, k = ans + 1; c < m && d < n; c++, d++, k++)
                {
                    if (sum(a, b, c, d, grid) - sum(a + 1, b + 1, c - 1, d - 1, grid) == (k - 1) << 2)
                    {
                        ans = k;
                    }
                }
            }
        }

        return ans * ans;
    }
};




//二维差分的原理和代码详解
//
//在二维数组中，如果经历如下的过程：
//1.批量的做如下的操作，每个操作都有独立的a,b,c,d,v
//  void add(a,b,c,d,v): 左上角(a,b)到右下角(c,d)范围上，每个数字+v,怎么快速处理？
//2.操作做完后，如何正确得到二维数组中每个位置的值？
//
//这就是二维差分的主要工作，add时候快速处理，最后build得到每个位置的值，修改操作必须集中在一起，不能边修改边查询：
//1.add方法实现，和一维差分类似，在特定位置设置状态，最后通过前缀和操作处理
//2.build方法实现，和处理前缀和类似
//3.真实数据用一圈0包裹起来，可以减少很多边界讨论-->>避免add方法越界   
vector<vector<int>> diff;
int n, m;
void add(int a, int b, int c, int d, int v)
{
    diff[a][b] += v;
    diff[c + 1][b] -= v;
    diff[a][d + 1] -= v;
    diff[c + 1][d + 1] += v;
}

void build()
{
    for (int i = 1; i <= m; i++)
    {
        for (int j = 1; j <= n; j++)
            diff[i][j] += diff[i][j - 1] + diff[i - 1][j] - diff[i - 1][j - 1];
    }
}

//题目三：
// 二维差分模版(洛谷)
// 测试链接 : https://www.luogu.com.cn/problem/P3397
// 二维差分模版(牛客)
// 测试链接 : https://www.nowcoder.com/practice/50e1a93989df42efb0b1dec386fb4ccc
//
//洛谷题解：
int n, m;
int arr[2005][2005];

void add(int a, int b, int c, int d, int k)
{
    arr[a][b] += k;
    arr[c + 1][b] -= k;
    arr[a][d + 1] -= k;
    arr[c + 1][d + 1] += k;
}

void build()
{
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
            arr[i][j] += arr[i][j - 1] + arr[i - 1][j] - arr[i - 1][j - 1];
    }
}

int main()
{
    cin >> n >> m;
    for (int i = 0; i < m; i++)
    {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        add(x1, y1, x2, y2, 1);
    }

    build();

    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}

//牛客题解与其类似，但要注意，若矩阵有初始值，则可以将初始值视为单个位置的差分进行差分处理，正常的差分正常进行，最终统一build()
//eg: 矩阵有(i,j)位置的初始值为a，可以进行差分：add(i,j,i,j,a);



//题目四：
// 用邮票贴满网格图
// 给你一个 m * n 的二进制矩阵 grid
// 每个格子要么为 0 （空）要么为 1 （被占据）
// 给你邮票的尺寸为 stampHeight * stampWidth
// 我们想将邮票贴进二进制矩阵中，且满足以下 限制 和 要求 ：
// 覆盖所有空格子，不覆盖任何被占据的格子
// 可以放入任意数目的邮票，邮票可以相互有重叠部分
// 邮票不允许旋转，邮票必须完全在矩阵内
// 如果在满足上述要求的前提下，可以放入邮票，请返回 true ，否则返回 false
// 测试链接 : https://leetcode.cn/problems/stamping-the-grid/
class Solution
{
private:
    int prefixSum(int a, int b, int c, int d, const vector<vector<int>>& preSum)
    {
        return preSum[c][d] - get(c, b - 1, preSum) - get(a - 1, d, preSum) + get(a - 1, b - 1, preSum);
    }

    void add(int a, int b, int c, int d, vector<vector<int>>& arr)
    {
        arr[a][b] += 1;
        arr[a][d + 1] -= 1;
        arr[c + 1][b] -= 1;
        arr[c + 1][d + 1] += 1;
    }

    int get(int i, int j, const vector<vector<int>>& arr)
    {
        return (i < 0 || j < 0) ? 0 : arr[i][j];
    }

    void build(vector<vector<int>>& arr)
    {
        for (int i = 0; i < arr.size(); i++)
        {
            for (int j = 0; j < arr[0].size(); j++)
            {
                arr[i][j] += get(i, j - 1, arr) + get(i - 1, j, arr) - get(i - 1, j - 1, arr);
            }
        }
    }

public:
    bool possibleToStamp(vector<vector<int>>& grid, int stampHeight, int stampWidth) 
    {
        int m = grid.size();
        int n = grid[0].size();
        vector<vector<int>> preSum(m, vector<int>(n, 0));//grid的前缀和数组
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
                preSum[i][j] = grid[i][j];
        }

        build(preSum);

        vector<vector<int>> diff(m + 1, vector<int>(n + 1, 0));//差分数组
        // 差分矩阵
        // 当贴邮票的时候，不再原始矩阵里贴，在差分矩阵里贴
        // 原始矩阵就用来判断能不能贴邮票，不进行修改
        // 每贴一张邮票都在差分矩阵里修改

        //遍历，将矩阵每个点作为左上角点，通过邮票的长和宽便能计算出右下角点，再通过前缀和进一步判断
        //贪心，能贴就贴
        for (int a = 0; a < m; a++)
        {
            for (int b = 0; b < n; b++)
            {
                int c = a + stampHeight - 1;
                int d = b + stampWidth - 1;

                if (c >= m || d >= n)//防止越界
                    continue;

                if (prefixSum(a, b, c, d, preSum) == 0)//可以贴
                {
                    add(a, b, c, d, diff);
                }
            }
        }

        build(diff);

        // 检查所有的格子！
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                // 原始矩阵里：grid[i][j] == 0，说明是个洞
                // 差分矩阵里：diff[i + 1][j + 1] == 0，说明洞上并没有邮票
                // 此时返回false
                if (grid[i][j] == 0 && diff[i][j] == 0)
                    return false;
            }
        }

        return true;
    }
};




//离散化技巧
//为了防止开辟的空间太大，可以将需要的空间重新编号
//让数组的大小跟力场的个数有关，而不是跟实际数值有关
// 
// 最强祝福力场
// 小扣在探索丛林的过程中，无意间发现了传说中"落寞的黄金之都"
// 而在这片建筑废墟的地带中，小扣使用探测仪监测到了存在某种带有「祝福」效果的力场
// 经过不断的勘测记录，小扣将所有力场的分布都记录了下来
// forceField[i] = [x,y,side] 
// 表示第 i 片力场将覆盖以坐标 (x,y) 为中心，边长为 side 的正方形区域。
// 若任意一点的 力场强度 等于覆盖该点的力场数量
// 请求出在这片地带中 力场强度 最强处的 力场强度
// 注意：力场范围的边缘同样被力场覆盖。
// 测试链接 : https://leetcode.cn/problems/xepqZ5/
class Solution
{
private:
    void add(int a, int b, int c, int d, vector<vector<int>>& diff)
    {
        diff[a][b] += 1;
        diff[c + 1][b] -= 1;
        diff[a][d + 1] -= 1;
        diff[c + 1][d + 1] += 1;
    }

    int get(int i, int j, vector<vector<int>>& diff)
    {
        return (i < 0 || i >= (int)diff.size() || j < 0 || j >= (int)diff[0].size()) ? 0 : diff[i][j];
    }

public:
    int fieldOfGreatestBlessing(vector<vector<int>>& forceField)
    {
        int n = forceField.size();
        if (n == 0) return 0;

        vector<long long> px, py;

        // 坐标离散化：将所有关键位置存入px和py
        for (int i = 0; i < n; i++)
        {
            long long x = forceField[i][0];
            long long y = forceField[i][1];
            long long side = forceField[i][2];

            // 将坐标乘以2，避免小数处理
            px.push_back(2 * x - side);
            px.push_back(2 * x + side);
            py.push_back(2 * y - side);
            py.push_back(2 * y + side);
        }

        // 排序并去重
        sort(px.begin(), px.end());
        sort(py.begin(), py.end());
        px.erase(unique(px.begin(), px.end()), px.end());
        py.erase(unique(py.begin(), py.end()), py.end());

        // 创建差分数组
        vector<vector<int>> diff(px.size() + 1, vector<int>(py.size() + 1, 0));

        // 对每片力场进行离散化处理
        for (int i = 0; i < n; i++)
        {
            long long x = forceField[i][0];
            long long y = forceField[i][1];
            long long side = forceField[i][2];

            // 找到矩形四个角在离散化坐标中的位置
            int x_min = lower_bound(px.begin(), px.end(), 2 * x - side) - px.begin();
            int x_max = lower_bound(px.begin(), px.end(), 2 * x + side) - px.begin();
            int y_min = lower_bound(py.begin(), py.end(), 2 * y - side) - py.begin();
            int y_max = lower_bound(py.begin(), py.end(), 2 * y + side) - py.begin();

            // 在差分数组中标记矩形范围
            add(x_min, y_min, x_max, y_max, diff);
            //注意：这里的x_min、x_max、y_min、y_max均是下标
            //      所以在差分数组中从左上角到右下角即为(x_min, y_min)->(x_max, y_max)
        }

        // 还原差分数组，计算力场强度
        int maxStrength = 0;
        for (int i = 0; i < (int)diff.size(); i++)
        {
            for (int j = 0; j < (int)diff[0].size(); j++)
            {
                diff[i][j] += get(i - 1, j, diff) + get(i, j - 1, diff) - get(i - 1, j - 1, diff);
                maxStrength = max(maxStrength, diff[i][j]);
            }
        }

        return maxStrength;
    }
};

