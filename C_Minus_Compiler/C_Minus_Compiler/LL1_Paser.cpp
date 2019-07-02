#ifndef  PASER_IN_LL1
#define  PASER_IN_LL1

#include "LexicalAnalyzer.cpp"
#include "FirstFollowGenerator.cpp"


static FirstFollowGen Gen("./LL_1_grammer.txt");
static map<string, map<string, set<int> > > driven_table;
class LL1_Paser {
public:
	list<int> colum_id;
	class Pnode {
	public:
		string label;
		string value;
		vector<Pnode*> children_list;
	};
	/*
	 PaserTree可以考虑将递归下降，LL(1)的写法的PaserTree结合起来，设计一个基类
	 程序的可拓展性更强
	*/
	class PaserTree {
	private:

		Pnode root;
		vector<pair<string, string> > token_list;
		ofstream out;
		int index;
		void BuildTreeHelp(Pnode* sroot)
		{
			string op = token_list[index].first;
			if (IsTerminal(sroot->label))
			{
				if (sroot->label != "^")
				{
					if (sroot->label == op)
					{
						sroot->value = token_list[index].second;
						index++;
					}
					else
					{
						out << "建树过程中发生的错误，期望" << sroot->label << "," << "得到了" << op<<"(不应该出现的错误)" << endl;;
						exit(5);
					}
				}

			}
			else
			{
				auto body = Gen.PList[* driven_table[sroot->label][op].begin()].second;
				for (auto one : body)
				{
					Pnode* child = new Pnode{ one,"",vector<Pnode*>() };
					sroot->children_list.push_back(child);
				}
				for (auto child : sroot->children_list)
				{
					BuildTreeHelp(child);
				}
			}
		}
		void PrintInJsonHelp(Pnode* sroot)
		{
			out << "{\n";
			out << "\"name\": \"" << sroot->label;
			if (sroot->children_list.empty())
			{
				out << " : " << sroot->value << "\"}";
				return;
			}
			out << "\",\n\"children\" : [";
			bool fir = 1;
			for (auto child : sroot->children_list)
			{
				if (fir)
					fir = 0;
				else
					out << ",\n";
				PrintInJsonHelp(child);
			}
		    out << "]\n}\n";

		}
	public:
		PaserTree(vector<pair<string,string> > token_list, string start_symbol)
		{
			root.label = start_symbol;
			this->token_list = token_list;
			this->index = 0;
			out = ofstream("./Show/grammer_tree.json");
		}
		void BuildTree()
		{
			BuildTreeHelp(&root);
		}
		void PrintInJson()
		{
			PrintInJsonHelp(&root);
			out.flush();
		}
		~PaserTree()
		{
			out.close();

		}
	};
	LL1_Paser(Lexer & one)
	{
		one.GetTokens(0);
		colum_id = one.token_column;
	}

	void ConstructTable()
	{
		for (auto one : Gen.head)
		{
			for (int i = 0; i < terminal_num; i++)
			{
				driven_table[one][terminal[i]] = set<int>();
			}
		}
		for (int i = 0; i < Gen.PList.size(); i++)
		{
			auto one = Gen.PList[i];
			set<string> tmp = set<string>();
			Gen.GetSingleFirstSet(one.second, tmp);
			for (auto two : tmp)
			{
				if (two != "^")
				{
					driven_table[one.first][two].insert(i);
				}
			}
			if (tmp.count("^"))
			{
				for (auto two : Gen.FollowSet[one.first])
				{
					driven_table[one.first][two].insert(i);
				}
			}
		}
		driven_table["selection-stmt-1"]["else"].erase(driven_table["selection-stmt-1"]["else"].find(37));
		//单独对if语句的二义性处理，处理方式，略微粗糙

		for (auto one : Gen.head)
		{
			for (int i = 0; i < terminal_num; i++)
			{
				if (driven_table[one][terminal[i]].size() > 1)
				{
					cout << one << " " << terminal[i] << endl;
					for (auto two : driven_table[one][terminal[i]])
					{
						cout << two << endl;
					}
					cout << "此文法非LL(1)的文法，请检查后重试\n";
					exit(4);
				}
			}
		}
	}
	pair<string, string> Get(string str)
	{
		for (auto& one : str)
		{
			if (one == '$')
			{
				one = ' ';
			}
		}
		stringstream in(str);
		string pa, pb;
		in >> pa >> pb;
		//cout << pa << " " << pb << endl;
		return make_pair(pa, pb);
	}
	void Infer()
	{
		//将输入重定向到token_in.txt文件
		freopen("./token_seq.txt", "r", stdin);
		freopen("./grammer_out.txt", "w", stdout);
		string token;
		list<pair<string, string > > buff;
		vector<pair<string, string> > buff2;
		list<string> pro;
		pro.push_back(Gen.start_symbol);
		pro.push_back("#");
		colum_id.push_back(colum_id.back()+1);
		while (cin >> token)
		{
			auto one = Get(token);
			buff2.push_back(one);
			buff.push_back(one);
		}
		buff.push_back(make_pair("#", ""));
		buff2.push_back(make_pair("#", ""));
		string  match;  //记录当前已经match到的token序列，方便调试
		while (pro.size() > 1 || buff.size() > 1)
		{
			string peek = buff.front().first;
			string P = pro.front();
			pro.pop_front();
			if (IsTerminal(P))
			{
				if (P == "^")
					continue;
				if (P != peek)
				{

					cout << "Error: syntax error, unexpected " << peek << "," <<
						"expecting " << P << "  encountered at line number:"<<colum_id.front() << endl;
					exit(5);
				}
				match += +" " + P;
				cout << "Match " << P << "{ " << buff.front().second << " } " << endl;
				buff.pop_front();
				colum_id.pop_front();
			}
			else
			{
				auto& items = driven_table[P][peek];
				if (items.empty())
				{

					//cout << match << endl;
				/*	cout << "*********************\n";
					for (auto gg : pro)
					{
						cout << gg << "\n";
					}
					cout << endl;*/
					cout << "Error: there is no thing which could be produced by "<<P<<" -> "<<peek<<" at line number:"<<
						colum_id.front()<<endl ;
					exit(5);
				}
				else
				{
					cout << P << " ->  ";
					auto mlist = Gen.PList[*items.begin()].second;
					for (auto one : mlist)
					{
						cout << one << " ";
					}
					cout << endl;
					while (mlist.size())
					{

						pro.push_front(mlist.back());
						mlist.pop_back();
					}
				}
			}

		}
		if (pro.size() == 1 && buff.size() == 1)
		{
			cout << "Successful\n";
			cout << "完成建树\n";
			PaserTree tree(buff2, Gen.start_symbol);
			tree.BuildTree();
			tree.PrintInJson();
		
		}
		else
		{
			cout << "文法栈未清空或者符号栈未清空\n";
			cout << "文法栈:";
			for (auto a : pro)
			{
				cout << a << " ";
			}
			cout << endl;
			for (auto a : buff)
			{
				cout <<"("<< a.first << ","<<")"<<" ";
			}
			cout << endl;
		}
		cout << endl << endl << endl;
		freopen("CON", "r", stdin);   freopen("CON", "w", stdout);
		//从文件中重定向回控制台
		cin.clear();  cout.clear();
		//清空之前留下的EOF等标志位
		
	}

};



#endif // ! PASER_IN_LL1
