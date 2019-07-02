#ifndef FIRST_FOLLOW_GEN
#define FIRST_FOLLOW_GEN



/*
    ATTENTION !!!!!
	�����ڴ��ļ��м���First,Follow������ʹ���ض����ض���ر�׼�����ʱ��ʹ��Windows
	���еı�׼�����ļ���GEN(�ʴ˴����1.0����ʱ�����ڳ�Windows�������ϵͳ����)
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
			Merge�������ڽ�����pa��pb�鲢��pc��
		*/
		set_union(pa.begin(), pa.end(), pb.begin(), pb.end(), inserter(target, target.begin()));
	}
	void GetSingleFirstSet(vector<string> work, set<string>& bck, int begin = 0)
	{
		/*

		  ���룺workΪһ������ʽ����
				bckΪĳ������ʽ��Ӧ��ͷ��First������
		  ����һ������ʽ��First��
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
			���һ������ʽ�б��ɨ���First���ļ��㣬����ֵ��ʾһ�ֹ����Ƿ��з��ս����First�������仯
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
		   ��������ʽ,
		*/
		Production cur;
		while (cin >> cur.first)
		{
			cur.second.clear();  //���body�б�
			string tmp;
			cin >> tmp;
			assert(tmp == "->"); //����������ɷ��򱨴�һ�������˴�λ
			getline(cin, tmp);
			stringstream in(tmp);
			while (in >> tmp)
			{
				cur.second.push_back(tmp);
			}
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
							/*cout << "���������ʽ������ݹ�,�����ķ��Ƿ��Ѿ�����\n";
							exit(2);*/
						}
					}
				}
				else
				{
					//�������ս��Ҳ����FirstSet
					if (!FirstSet.count(one))
						FirstSet[one] = set<string>();
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
				FirstSet[one] = set<string>();
			}
			if (!FollowSet.count(one))
			{
				FollowSet[one] = set<string>();
			}
		}
		/*
		��start symbol���е�������
		*/
		if (!head.count("S"))
		{
			start_symbol = PList[0].first;
			cout << "ע�⣬����δ��⵽�ķ��� S ���ս�����ʽ� " << start_symbol << " ��Ϊ��ʼ��\n";
		}
		else
			start_symbol = "S";

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
		cout.flush();
		freopen("CON", "r", stdin);  		freopen("CON", "w", stdout);
		//���ļ����ض���ؿ���̨
		cin.clear(); cout.clear();
		//���֮ǰ���µ�EOF�ȱ�־λ
	}
public:
	string grammer_path;//LL1�ķ�·��
	set<string> head;     //nonterminal��
	vector<Production> PList;  //����ʽ�б�
	map<string, set<string> > FirstSet;  //First��
	map<string, set<string> > FollowSet; //Follow��
	string start_symbol; //start_symbol
	FirstFollowGen(string grammer_path)
	{

		//�ض����ļ�����
		this->grammer_path = grammer_path;
		freopen(grammer_path.c_str(), "r", stdin); freopen("./grammer_out.txt", "w", stdout);
		ReadIn(); 
		GetFirstSet();
		GetFollowSet();
		freopen("CON", "r", stdin);  		freopen("CON", "w", stdout);
		//���ļ����ض���ؿ���̨
		cin.clear(); cout.clear();
		//���֮ǰ���µ�EOF�ȱ�־λ
	}

};


#endif // !FIRST_FOLLOW_GEN
