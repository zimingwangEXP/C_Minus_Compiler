#include<iostream>
#include<set>
#include<string>
#include<map>
#include<iterator>
#include<algorithm>
#include<vector>
#include<ctype.h>
#define Reloacate freopen("./in.txt","r",stdin); freopen("./out.txt","w",stdout)
#define Production pair<char,string>   //ʹ��pair���������ʽ
using namespace std;
set<char> head;     //nonterminal��
vector<Production> PList;  //����ʽ�б�
map<char, set<char> > FirstSet;  //First��
map<char, set<char> > FollowSet; //Follow��
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
    	Merge�������ڽ�����pa��pb�鲢��pc��
	*/
	set_union(pa.begin(), pa.end(), pb.begin(), pb.end(), inserter(target,target.begin()));
}
void GetSingleFirstSet(string work, set<char>& bck)
{
	/*

	  ���룺workΪһ������ʽ����
	        bckΪĳ������ʽ��Ӧ��ͷ��First������
	  ����һ������ʽ��First��
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
	    ���һ������ʽ�б��ɨ���First���ļ��㣬����ֵ��ʾһ�ֹ����Ƿ��з��ս����First�������仯
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
	   ��������ʽ,
	*/
	Production cur;
	while (cin >> cur.first >> cur.second && (cur.first != '0' || cur.second != "0"))
	{
		head.insert(cur.first);
		bool fir = 0;  //������������ʽbody����ߵķ��ս��
		for (auto one : cur.second)
		{
			if (IsNonterminal(one))
			{
				if (!fir)
				{
					fir = 1;
					if (one == cur.first)
					{
						cout << "���������ʽ������ݹ�,�����ķ��Ƿ��Ѿ�����\n";
						exit(2);
					}
				}
			}
			else
			{
				//�������ս��Ҳ����FirstSet
				if (!FirstSet.count(one))
					FirstSet[one] = set<char>();
				//�ս����First��Ϊ�ս������
				FirstSet[one].insert(one);
			}
		}
		PList.push_back(cur);
	}
	/*
	  �������з��ս����First����Follow��Ϊ��
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
	��start symbol���е�������
	*/
	if (!head.count('S'))
	{
		cout << "�ķ������п�ʼ��S������������\n";
		exit(2);
	}
}
void inline GetFirstSet()
{
	/*
	   һ����ɨ��ֱ��û�з��ս����First�������仯
	*/
	while (OnePass());
}
bool GetSingleFollowSet()
{
	/*
		���һ������ʽ�б��ɨ���Follow���ļ��㣬����ֵ��ʾһ�ֹ����Ƿ��з��ս����Follow�������仯
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
	//��������ض���
	Reloacate;
	//��������ʽ
	ReadIn();
	GetFirstSet();
	GetFollowSet();
	for (auto one : head)
	{
		cout << one << ":\n";
		cout << "First����";
		for (auto second : FirstSet[one])
		{
			cout << second << " ";
		}
		cout << endl;
		cout << "Follow����";
		for (auto second : FollowSet[one])
		{
			cout << second << " ";
		}
		cout << endl;

	}
	//system("PAUSE");
	return 0;
}