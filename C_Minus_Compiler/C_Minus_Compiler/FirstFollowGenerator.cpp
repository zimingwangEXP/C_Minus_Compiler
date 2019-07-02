#ifndef FIRST_FOLLOW_GEN
#define FIRST_FOLLOW_GEN



/*
    ATTENTION !!!!!
	由于在此文件中计算First,Follow过程中使用重定向，重定向回标准输入的时候使用Windows
	特有的标准输入文件名GEN(故此代码的1.0版暂时不能在除Windows外的其他系统运行)
*/


#include"tools.h"



class FirstFollowGen {
public:
	bool ProduceEmpty(string op)
	{
		return FirstSet[op].count("^");
	}
	void Merge(set<string>& pa, set<string>& pb, set<string>& target)
	{
		/*
			Merge函数用于将集合pa，pb归并到pc中
		*/
		set_union(pa.begin(), pa.end(), pb.begin(), pb.end(), inserter(target, target.begin()));
	}
	void GetSingleFirstSet(vector<string> work, set<string>& bck, int begin = 0)
	{
		/*

		  输入：work为一条生成式的体
				bck为某个生成式对应的头的First集引用
		  计算一条生成式的First集
		*/
		bool complete = true;
		for (int i = begin; i < work.size(); i++)
		{
			auto one = work[i];
			auto tmp = FirstSet[one];
			if (tmp.count("^"))
			{
				tmp.erase(tmp.find("^"));
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
			bck.insert("^");
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
			GetSingleFirstSet(one.second, cur);
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
		while (cin >> cur.first)
		{
			cur.second.clear();  //清空body列表
			string tmp;
			cin >> tmp;
			assert(tmp == "->"); //如果不是生成符则报错，一定发生了错位
			getline(cin, tmp);
			stringstream in(tmp);
			while (in >> tmp)
			{
				cur.second.push_back(tmp);
			}
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
							/*cout << "输入的生成式存在左递归,请检查文法是否已经化简\n";
							exit(2);*/
						}
					}
				}
				else
				{
					//对所有终结符也产生FirstSet
					if (!FirstSet.count(one))
						FirstSet[one] = set<string>();
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
				FirstSet[one] = set<string>();
			}
			if (!FollowSet.count(one))
			{
				FollowSet[one] = set<string>();
			}
		}
		/*
		对start symbol进行单独分析
		*/
		if (!head.count("S"))
		{
			start_symbol = PList[0].first;
			cout << "注意，由于未检测到文法有 S 非终结符，故将 " << start_symbol << " 作为开始符\n";
		}
		else
			start_symbol = "S";

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
			auto body = prod.second;
			int sz = body.size();
			for (int i = 0; i < sz; i++)
			{
				if (IsNonterminal(body[i]))
				{
					int len = FollowSet[body[i]].size();
					set<string> tmp;
					GetSingleFirstSet(body, tmp, i + 1);
					if (tmp.count("^"))
					{
						tmp.erase(tmp.find("^"));
						Merge(FollowSet[prod.first], FollowSet[body[i]], FollowSet[body[i]]);
					}
					Merge(FollowSet[body[i]], tmp, FollowSet[body[i]]);
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
		FollowSet[start_symbol].insert("#");
		while (GetSingleFollowSet());
	}
	void PrintSet()
	{
		freopen(grammer_path.c_str(), "r", stdin); 
		freopen("./grammer_out.txt", "w", stdout);
		ReadIn();
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
		cout.flush();
		freopen("CON", "r", stdin);  		freopen("CON", "w", stdout);
		//从文件中重定向回控制台
		cin.clear(); cout.clear();
		//清空之前留下的EOF等标志位
	}
public:
	string grammer_path;//LL1文法路径
	set<string> head;     //nonterminal集
	vector<Production> PList;  //生成式列表
	map<string, set<string> > FirstSet;  //First集
	map<string, set<string> > FollowSet; //Follow集
	string start_symbol; //start_symbol
	FirstFollowGen(string grammer_path)
	{

		//重定向到文件读入
		this->grammer_path = grammer_path;
		freopen(grammer_path.c_str(), "r", stdin); freopen("./grammer_out.txt", "w", stdout);
		ReadIn(); 
		GetFirstSet();
		GetFollowSet();
		freopen("CON", "r", stdin);  		freopen("CON", "w", stdout);
		//从文件中重定向回控制台
		cin.clear(); cout.clear();
		//清空之前留下的EOF等标志位
	}

};


#endif // !FIRST_FOLLOW_GEN
