#include<iostream>
#include<set>
#include<string>
#include<map>
#include<iterator>
#include<algorithm>
#include<vector>
#include<ctype.h>
#define Reloacate freopen("./in.txt","r",stdin); freopen("./out.txt","w",stdout)
#define Production pair<char,string>   //使用pair定义的生成式
using namespace std;
set<char> head;     //nonterminal集
vector<Production> PList;  //生成式列表
map<char, set<char> > FirstSet;  //First集
map<char, set<char> > FollowSet; //Follow集
bool IsTerminal(char op)
{
	return op >= 'a'&&op <= 'z';
}
bool IsNonterminal(char op)
{
	return op >= 'A'&&op <= 'Z';
}
bool ProduceEmpty(char op)
{
	return FirstSet[op].count('^');
}
void Merge(set<char>& pa, set<char>& pb, set<char>& target)
{
	/*
    	Merge函数用于将集合pa，pb归并到pc中
	*/
	set_union(pa.begin(), pa.end(), pb.begin(), pb.end(), inserter(target,target.begin()));
}
void GetSingleFirstSet(string work, set<char>& bck)
{
	/*

	  输入：work为一条生成式的体
	        bck为某个生成式对应的头的First集引用
	  计算一条生成式的First集
	*/
	bool complete = true;
	for (auto one : work)
	{

		auto tmp = FirstSet[one];
		if (tmp.count('^'))
		{
			tmp.erase(tmp.find('^'));
		}
		Merge(bck, tmp, bck);
		if (!ProduceEmpty(one))
		{
			complete = false;
			break;
		}
	}
	if (complete)
	{
		bck.insert('^');
	}
}
bool OnePass()
{
	/*
	    完成一轮生成式列表的扫描和First集的计算，返回值表示一轮过后是否有非终结符的First集发生变化
	*/
	bool changed = false;

	for (auto one : PList)
	{
		auto& cur = FirstSet[one.first];
		int pre = cur.size();
		GetSingleFirstSet(one.second,cur);
		if (pre < cur.size())
		{
			changed = true;
		}
	}
	return changed;
}
void ReadIn()
{
	/*
	   读入生成式,
	*/
	Production cur;
	while (cin >> cur.first >> cur.second && (cur.first != '0' || cur.second != "0"))
	{
		head.insert(cur.first);
		bool fir = 0;  //帮助处理生成式body最左边的非终结符
		for (auto one : cur.second)
		{
			if (IsNonterminal(one))
			{
				if (!fir)
				{
					fir = 1;
					if (one == cur.first)
					{
						cout << "输入的生成式存在左递归,请检查文法是否已经化简\n";
						exit(2);
					}
				}
			}
			else
			{
				//对所有终结符也产生FirstSet
				if (!FirstSet.count(one))
					FirstSet[one] = set<char>();
				//终结符的First集为终结符本身
				FirstSet[one].insert(one);
			}
		}
		PList.push_back(cur);
	}
	/*
	  设置所有非终结符的First集和Follow集为空
	*/
	for (auto one : head)
	{
		if (!FirstSet.count(one))
		{
			FirstSet[one] = set<char>();
		}
		if (!FollowSet.count(one))
		{
			FollowSet[one] = set<char>();
		}
	}
	/*
	对start symbol进行单独分析
	*/
	if (!head.count('S'))
	{
		cout << "文法必须有开始符S，请重新输入\n";
		exit(2);
	}
}
void inline GetFirstSet()
{
	/*
	   一轮轮扫描直到没有非终结符的First集发生变化
	*/
	while (OnePass());
}
bool GetSingleFollowSet()
{
	/*
		完成一轮生成式列表的扫描和Follow集的计算，返回值表示一轮过后是否有非终结符的Follow集发生变化
	*/
	bool changed = false;
	for (auto prod : PList)
	{
		string body = prod.second;
		int sz = body.size();
		for (int i = 0; i < sz; i++)
		{
			if (IsNonterminal(body[i]))
			{
				int len = FollowSet[body[i]].size();
				set<char> tmp;
				GetSingleFirstSet(body.substr(i + 1, sz - i - 1), tmp);
				if (tmp.count('^'))
				{
					tmp.erase(tmp.find('^'));
					Merge(FollowSet[prod.first], FollowSet[body[i]], FollowSet[body[i]]);
				}
				Merge(FollowSet[body[i]],tmp,FollowSet[body[i]]);
				if (FollowSet[body[i]].size() > len)
				{
					changed = true;
				}
			}
		}
	}
	return changed;
}
void GetFollowSet()
{
	FollowSet['S'].insert('#');
	while (GetSingleFollowSet());
}
int main(void)
{
	//输入输出重定向
	Reloacate;
	//读入生成式
	ReadIn();
	GetFirstSet();
	GetFollowSet();
	for (auto one : head)
	{
		cout << one << ":\n";
		cout << "First集：";
		for (auto second : FirstSet[one])
		{
			cout << second << " ";
		}
		cout << endl;
		cout << "Follow集：";
		for (auto second : FollowSet[one])
		{
			cout << second << " ";
		}
		cout << endl;

	}
	//system("PAUSE");
	return 0;
}