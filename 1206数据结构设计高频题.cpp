#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <map>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <utility>
#include <set>
using namespace std;
//题目一：setAll功能哈希表
//设置全局变量，利用修改时间比较来获得值
#include <unordered_map>
#include <utility>
class MyMap
{
private:
	unordered_map<int, pair<int, int>> mp;
	int setAllValue;//将所有 值 中的value统一改为新值
	int setAllTime = 0;//统一修改的时间
	int cnt = 1;//进行某一次put操作的时间
public:
	void put(int key, int value)
	{
		mp[key] = { value, cnt++ };
	}

	bool containKey(int key)
	{
		if (mp.find(key) != mp.end())
			return true;
		return false;
	}

	int get(int key)
	{
		if (!containKey(key))
			return -1;

		if (mp[key].second < setAllTime)
			return setAllTime;

		return mp[key].first;
	}

	void setAll(int value)
	{
		setAllValue = value;
		setAllTime = cnt++;
	}
};



//题目二：实现LRU结构
//纯容器实现
class LRUCache
{
public:
	LRUCache(int capacity) 
	{
		_capacity = capacity;
	}

	int get(int key) 
	{
		if (mp.find(key) == mp.end())
			return -1;

		auto it = mp.find(key)->second;
		int ans = it->second;// 保存节点值
		k_v_list.erase(it); // 从链表中删除节点
		k_v_list.push_back(make_pair(key, ans));// 重新插入到链表尾部
		mp[key] = prev(k_v_list.end());// 更新map中的迭代器
		return ans;
	}

	void put(int key, int value) 
	{
		if (mp.find(key) != mp.end())//存在
		{
			auto it = mp.find(key)->second;
			k_v_list.erase(it);
			k_v_list.push_back(make_pair(key, value));
			mp[key] = prev(k_v_list.end());
		}
		else//不存在
		{
			if (k_v_list.size() >= _capacity)
			{
				mp.erase(k_v_list.front().first);
				k_v_list.pop_front();
			}
			k_v_list.push_back(make_pair(key, value));
			mp[key] = prev(k_v_list.end());
		}
	}

private:
	int _capacity;
	list<pair<int, int>> k_v_list;
	unordered_map<int, list<pair<int, int>>::iterator> mp;
};

//非纯容器实现
class doubleNode
{
public:
	int _val;
	int _key;
	doubleNode* prev;
	doubleNode* next;

	doubleNode(int key = 0, int val = 0)
	{
		this->prev = nullptr;
		this->next = nullptr;
		_val = val;
		_key = key;
	}
};

class doubleList
{
public:
	doubleNode* head;
	doubleNode* tail;
	int size;

	doubleList()
	{
		head = nullptr;
		tail = nullptr;
		size = 0;
	}

	void pushBack(doubleNode* node)
	{
		if (size == 0)
		{
			head = node;
			tail = node;
		}
		else
		{
			node->prev = tail;
			tail->next = node;
			tail = node;
		}
		size++;
	}

	void moveToTail(doubleNode* node)
	{
		if (tail == node)
			return;

		if (head == node)
		{
			head = head->next;
			if (head)
				head->prev = nullptr;
		}
		else
		{
			if (node->prev != nullptr)
				node->prev->next = node->next;
			if (node->next != nullptr)
				node->next->prev = node->prev;
		}

		node->prev = tail;
		node->next = nullptr;
		tail->next = node;
		tail = node;
	}

	void removeHead()
	{
		if (size == 0)
			return;
		
		doubleNode* tmp = head;
		if (head == tail)
		{
			head = nullptr;
			tail = nullptr;
		}
		else
		{
			head = head->next;
			head->prev = nullptr;
		}
		size--;
	}
};

class LRUCache
{
private:
	doubleList* list;
	int _capacity;
	unordered_map<int, doubleNode*> mp;
public:
	LRUCache(int capacity = 0)
	{
		_capacity = capacity;
		list = new doubleList;
	}

	int get(int key)
	{
		if (mp.find(key) == mp.end())
			return -1;

		list->moveToTail(mp[key]);
		mp[key] = list->tail;
		return list->tail->_val;
	}

	void put(int key, int value)
	{
		if (mp.find(key) != mp.end())//存在
		{
			list->moveToTail(mp[key]);
			list->tail->_val = value;
			mp[key] = list->tail;
		}
		else
		{
			if (_capacity == list->size)
			{
				mp.erase(list->head->_key);
				list->removeHead();
			}

			doubleNode* tmp = new doubleNode;
			list->pushBack(tmp);
			list->tail->_key = key;
			list->tail->_val = value;
			mp[key] = list->tail;

		}
	}

};



//题目三：插入、删除和获取随机元素O(1)时间的结构
//注意应该用unordered_map，因为map时间复杂度为logn
class RandomizedSet
{
public:
	RandomizedSet()
	{
		srand((unsigned int)time(NULL));
	}

	bool insert(int val)
	{
		if (mp.find(val) != mp.end())
			return false;
		arr.push_back(val);
		mp[val] = arr.size() - 1;
		
		return true;
	}

	bool remove(int val)
	{
		if (mp.find(val) == mp.end())
			return false;
		int pos = mp[val];//要删除的元素的下标
		int end_val = arr.back();//数组中最后一个元素的值
		if (end_val == val)//要删除的元素就是数组中最后一个元素
		{
			arr.pop_back();
			mp.erase(val);
		}
		else
		{
			swap(arr[pos], arr.back());
			arr.pop_back();
			mp.erase(val);
			mp[end_val] = pos;
		}
		
		return true;
	}

	int getRandom()
	{
		int index = rand() % arr.size();
		return arr[index];
	}

private:
	vector<int> arr;
	unordered_map<int, int> mp;//value index
};



//题目四：插入、删除和获取随机元素O(1)时间且允许有重复数字的结构
class RandomizedCollection 
{
public:
	RandomizedCollection() 
	{
		srand((unsigned int)time(NULL));
	}

	bool insert(int val) 
	{
		if (mp.find(val) == mp.end())//不存在
		{
			arr.push_back(val);
			mp[val].insert(arr.size() - 1);
			return true;
		}
		else
		{
			arr.push_back(val);
			mp[val].insert(arr.size() - 1);
			return false;
		}
	}

	bool remove(int val) 
	{
		if (mp.find(val) != mp.end())//存在
		{
			int pos = *mp[val].begin();//要删除元素的下标 取下标集合中的第一个元素
			if (pos == arr.size() - 1)
			{
				mp[val].erase(pos);
				arr.pop_back();

				if (mp[val].empty())//集合为空
				{
					mp.erase(val);
				}
				return true;
			}

			int end_val = arr.back();//原数组中最后一个元素
			swap(arr[pos], arr.back());
			mp[end_val].erase(arr.size() - 1);//换位后把最后一个元素下标集合更改
			arr.pop_back();//数组中删除该元素
			mp[val].erase(pos);//把集合中该下标删除
			mp[end_val].insert(pos);//换位后把最后一个元素下标集合更改

			if (mp[val].empty())//集合为空
			{
				mp.erase(val);
			}
			return true;
		}
		return false;
	}

	int getRandom()
	{
		return arr[rand() % arr.size()];
	}

private:
	vector<int> arr;
	unordered_map<int, unordered_set<int>> mp;//value index_set(下标集合)
	//用集合记录下标
};



//题目五：快速获得数据流的中位数的结构
//纯暴力vectoc再排序会超时
// 
//用两个堆来管理数据，一个大根堆存较小的数据，一个小根堆存较大的数据。
//各自的堆要存一半，所以需要balance操作。
//当一边的size比另外一边大超过or等于2的时候，需要重新balance：从一个堆中弹出堆顶放到另一个堆
//若一共有偶数个数，则两个堆的堆顶的平均数即为中位数；若一共有奇数个数，则存储数据较多的那个堆的堆顶就是中位数
//这样，看两个头就可以获得中位数了。
class MedianFinder
{
private:
	priority_queue<int> maxHeap;
	priority_queue<int, vector<int>, greater<int>> minHeap;
public:
	MedianFinder()
	{

	}

	void balance()
	{
		if (maxHeap.size() - minHeap.size() == 2)
		{
			int tmp = maxHeap.top();
			maxHeap.pop();
			minHeap.push(tmp);
		}
		else if (minHeap.size() - maxHeap.size() == 2)
		{
			int tmp = minHeap.top();
			minHeap.pop();
			maxHeap.push(tmp);
		}
	}

	void addNum(int num)
	{
		if (maxHeap.empty() || num <= maxHeap.top())
			maxHeap.push(num);
		else
		{
			minHeap.push(num);
		}

		balance();
	}

	double findMedian()
	{
		if (minHeap.empty())
			return maxHeap.top();

		if (maxHeap.size() == minHeap.size())
			return (maxHeap.top() + minHeap.top()) * 1.0 / 2;
		else if (maxHeap.size() > minHeap.size())
			return maxHeap.top();
		else
			return minHeap.top();
	}

};



//题目六：最大频率栈
//设计一个类似堆栈的数据结构，将元素推入堆栈，并从堆栈中弹出出现频率最高的元素。
//使用map（词频表）+最大次记录 注意stack的设计的时候，设计成每一个层都是一个链表，链表存对应词频的数 !
class FreqStack 
{
private:
	int maxFreq;
	unordered_map<int, int> valToFre;//数字 该数字出现频率
	unordered_map<int, vector<int>> freqToVals;//多层链表: 层数(频率) 链表

public:
	FreqStack() 
	{
		maxFreq = 0;
	}

	void push(int val) 
	{
		int freq = ++valToFre[val];
		freqToVals[valToFre[val]].push_back(val);
		maxFreq = max(freq, maxFreq);
	}

	int pop() 
	{
		int theDeleteValue = freqToVals[maxFreq].back();
		freqToVals[maxFreq].pop_back();
		valToFre[theDeleteValue]--;
		if (freqToVals[maxFreq].empty())//最高层为空
		{
			freqToVals.erase(maxFreq);
			maxFreq--;
		}
		if (valToFre[theDeleteValue] == 0)
			valToFre.erase(theDeleteValue);
		
		return theDeleteValue;
	}
};



//题目七：全O(1)的数据结构: 请你设计一个用于存储字符串计数的数据结构，并能够返回计数最小和最大的字符串。
//用一个hash表+一个桶的双向链表
//纯容器实现
class AllOne
{
private:
	list<pair<unordered_set<string>, int>> lst;  // 记录所有字符串，记录出现频率(从小到大)
	unordered_map<string, list<pair<unordered_set<string>, int>>::iterator> mp;

public:
	AllOne() 
	{

	}

	void inc(string key)
	{
		if (mp.find(key) == mp.end())  // 没有
		{
			if (lst.empty() || lst.front().second != 1)  // 双向链表为空或第一个结点出现频率不为1
			{
				unordered_set<string> newSet;
				newSet.insert(key);
				lst.push_front(make_pair(newSet, 1));
				mp[key] = lst.begin();
			}
			else  // 有1的桶
			{
				lst.front().first.insert(key);
				mp[key] = lst.begin();
			}
		}
		else  // 有
		{
			auto it = mp[key];
			int freq = it->second;
			auto next_it = next(it);
			if (next_it != lst.end() && next_it->second == freq + 1)  // 有相邻的桶
			{
				it->first.erase(key);
				next_it->first.insert(key);
				mp[key] = next_it;
			}
			else  // 没有相邻的桶
			{
				unordered_set<string> newSet;
				newSet.insert(key);
				auto newIt = lst.insert(next_it, { newSet, freq + 1 });  // 插入next_it之前，当前桶之后
				it->first.erase(key);
				mp[key] = newIt;
			}

			if (it->first.empty())
				lst.erase(it);
		}
	}

	void dec(string key)
	{
		auto it = mp[key];
		int freq = it->second;
		if (freq == 1)
		{
			it->first.erase(key);
			if (it->first.empty())
			{
				lst.erase(it);
			}
			mp.erase(key);
			return;
		}

		it->first.erase(key);

		// 检查前一个桶是否存在且频率匹配
		if (it != lst.begin())  // 边界检查
		{
			auto prev_it = prev(it);
			if (prev_it->second == freq - 1)  // 有相邻的桶
			{
				prev_it->first.insert(key);
				mp[key] = prev_it;

				// 清理当前桶
				if (it->first.empty())
				{
					lst.erase(it);
				}
				return;
			}
		}

		// 没有相邻的桶（包括it在链表开头，或者前一个桶的频率不匹配）
		unordered_set<string> newSet;
		newSet.insert(key);
		auto newIt = lst.insert(it, { newSet, freq - 1 });
		mp[key] = newIt;

		// 清理当前桶
		if (it->first.empty())
		{
			lst.erase(it);
		}
	}

	string getMaxKey()
	{
		if (lst.empty())
			return "";
		return *lst.back().first.begin();
	}

	string getMinKey()
	{
		if (lst.empty())
			return "";
		return *lst.front().first.begin();
	}
};

//非纯容器实现实在懒得写了，自己手搓一个双向链表，在链表头尾各放一个空桶（head为一个频率为0的桶，tail为一个频率为INT_MAX的桶），方便操作

