#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <cmath>
#include <queue>
#include <vector>
#include <utility>
using namespace std;
//题目一：合并k个有序链表
//法一：可以让所有结点都进入一个容器，然后统一排序，最后统一连起来
//法二：利用小根堆
typedef int ElemType;
struct ListNode
{
	ElemType data;
	ListNode* next;
};

struct CustomCompare 
{
    bool operator()(ListNode* lhs, ListNode* rhs) 
    {
        // 使priority_queue成为一个最小堆  
        return lhs->data > rhs->data;
        // >号表示升序排列，因为优先队列队首的元素优先级最高，优先队列队尾元素的优先级最低
        //所以大于号>就规定了优先队列后面的元素都要大于优先队列前面的元素（尾部优先级小于首部优先级），也就是形成一个小根堆，升序排序，每次权值最小的会被弹出来。
    }
};

ListNode* mergeKLists(vector<ListNode*>& arr)
{
    priority_queue<ListNode*, vector<ListNode*>, CustomCompare> pq;
    for (auto it = arr.begin(); it != arr.end(); it++)
    {
        if (*it != NULL)
            pq.push(*it);
    }
    if (pq.empty())
        return NULL;

    ListNode* head = NULL;//新链表的头结点
    ListNode* curr = NULL;//记录新链表最后一个结点
    head = pq.top();
    curr = head;
    pq.pop();
    if (curr->next != NULL)
        pq.push(curr->next);

    while (!pq.empty())
    {
        ListNode* tmp = pq.top();
        curr->next = tmp;
        curr = curr->next;
        pq.pop();
        if(curr->next!=NULL)
            pq.push(curr->next);
    }

    return head;
}



//题目二：线段最多重合问题
//注：1.任何重合区域的左边界都一定是某一个线段的左边界
//    2.当两个线段仅头尾相接时，不视为重合线段。例如：[1,3]和[3,7]，不是重合线段
//思路：
//1.根据线段开头位置将所有线段从小到大排序
//2.小根堆中存放着线段结束位置
//3.当来一个新线段 [x,y]，让小根堆中<=x的数全走，然后把y放进小根堆，此时小根堆中数的个数就是目前所有线段最多重合的数量
struct cmp_sort
{
    bool operator()(pair<int, int>a, pair<int, int>b)
    {
        return a.first < b.first;
    }
};

struct cmp_pq
{
    bool operator()(int a, int b)
    {
        return a > b;
    }
};

int compute(vector<pair<int, int>>& arr)
{
    sort(arr.begin(), arr.end(), cmp_sort());
    int max_num = 0;
    priority_queue<int, vector<int>, cmp_pq> pq;
    for (int i = 0; i < arr.size(); i++)
    {
        //删除
        while (!pq.empty() && pq.top() <= arr[i].first)
            pq.pop();
        //求重合线段
        pq.push(arr[i].second);
        max_num = max_num > pq.size() ? max_num : pq.size();
    }
    return max_num;
}

int main()
{
    int n;
    cin >> n;
    vector<pair<int, int>> arr;
    for (int i = 0; i < n; i++)
    {
        int first, second;
        cin >> first >> second;
        arr.push_back(make_pair(first, second));
    }
    cout << compute(arr);

    return 0;
}

//复杂度分析：
//时间复杂度：
// 1个线段，进堆一次出堆一次，每一次进堆的时间复杂度是O(logn)
// n个线段，总的时间复杂度是O(nlogn)
//空间复杂度：
// 空间占用就是堆的大小，O(n)



//题目三：让数组整体累加和减半的最少操作次数
// 给你一个正整数数组nums，每一次操作中，你可以从nums中选择任意一个数并将它减少到恰好一半（注：在后续操作中，你可以对减半过的数继续执行操作）
//eg:[20 100 50 50] 累加和==220
//随机选择一个数操作一次(/2)，这里选到100 -->> [20 50 50 50] ,累加和170
//求最少操作几次使得数组累加和为110
//思路：
//贪心：每次将数组中最大的那个数减少一半
int halveArray(int* arr, int length)
{
    priority_queue<double> pq;//大根堆
    double sum = 0.0;//记录起始累加和
    for (int i = 0; i < length; i++)
    {
        pq.push(arr[i]);
        sum += arr[i];
    }
    
    sum /= 2;
    int ans = 0;
    for (double minus = 0, cur; minus < sum; minus += cur, ans++)
    {
        cur = pq.top() / 2;
        pq.pop();
        pq.push(cur);
    }

    return ans;
}
//时间复杂度：O(n*logn)


//优化：1.将数组元素全部扩大，以提升精度（用double存储小数，精度可能不够）
//      2.自己建堆，并从下向上建堆
#define MAXSIZE 100000
int heap[MAXSIZE];
void heapify(int i, int size)
{
    int l = 2 * i + 1;
    while (l < size)
    {
        int r = l + 1;
        int maxnum_index = (r<size && heap[r]>heap[l]) ? r : l;
        maxnum_index = heap[i] > heap[maxnum_index] ? i : maxnum_index;
        if (heap[i] == heap[maxnum_index])
            break;
        swap(heap[i], heap[maxnum_index]);
        i = maxnum_index;
        l = 2 * i + 1;
    }
}

int halveArray_pro(int* arr, int length)
{
    long sum = 0;
    for (int i = length - 1; i >= 0; i++)
    {
        heap[i] = (long)arr[i] << 20;//将arr数组中每个元素扩大2^20倍
        sum += heap[i];
        heapify(i, length);
    }
    //int类型2^31-1，long类型2^63-1
    //若需要更高的精度，可以改成 <<30

    sum /= 2;
    int ans = 0;
    for (long minus = 0; minus < sum; ans++)
    {
        heap[0] /= 2;
        minus += heap[0];
        heapify(0, length);
    }

    return ans;
}

