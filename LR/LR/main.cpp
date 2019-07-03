#include<bits/stdc++.h>
#include"FirstFollowGen.cpp"
#define Relocate freopen("./in.txt","r",stdin);freopen("./out.txt","w",stdout);
using namespace std;
typedef  pair<string, string> token;
typedef  pair<string, vector<string> > Production;

class Item {
public:
	bool extended;
	string head;
	int from;
	vector<string> body;
	int dot;
	Item()
	{
		extended = false;
		dot = 0;
	}
	Item(bool extended, string head, vector<string> body, int dot,int from)
	{
		this->extended = extended;
		this->head = head;
		this->body = body;
		this->dot = dot;
		this->from = from;
	}
	bool IsReduced()//判断此项是否为规约项
	{
		return dot == body.size()||(body.size()==1&&body[0]=="^");
	}
	Item MoveRight()//移进
	{
		if (IsReduced())
		{
			cout << "在规约项调用移进函数，请检查调用前是否检查规约动作\n";
			exit(-1);
		}
		Item bck;
		bck.body = this->body;
		bck.head = this->head;
		bck.dot = this->dot + 1;
		bck.from = this->from;
		return bck;
	}
	bool operator<(const Item & one) const
	{
		if (head == one.head)
		{
			if (body == one.body)
				return  dot < one.dot;
			return body < one.body;
		}
		return head < one.head;
	}
	bool operator==(const Item & one) const
	{
		return head == one.head&&body == one.body&&dot==one.dot;
	}
};

class Pnode {
public:
	string label;
	string value;
	vector<Pnode*> children_list;
};
class Yacc_Tree {
private:
	Pnode root;
	vector<pair<string, string> > token_list;
	vector<Production> Plist;
	int Pindex;
	ofstream out;
	int index;
	void BuildTreeHelp(Pnode* sroot)
	{
		cout << sroot->label << endl;
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

					out << "匹配错误1\n";
					exit(5);
				}
			}

		}
		else
		{
			if (Plist[Pindex].first != sroot->label)
			{
				cout << Plist[Pindex].first << " " << sroot->label << endl;

				out << "匹配错误2\n";
				exit(5);
			}
			auto body = Plist[Pindex++].second;
			for (auto one : body)
			{
				Pnode* child = new Pnode{ one,"",vector<Pnode*>() };
				sroot->children_list.push_back(child);
			}
			if (body.empty())
			{
				Pnode* child = new Pnode{ "^","",vector<Pnode*>() };
				sroot->children_list.push_back(child);
			}
			int sz = sroot->children_list.size();
			for (int i = sz - 1; i >= 0; i--)
			{
				auto child = sroot->children_list[i];
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
	Yacc_Tree(vector<pair<string, string> > token_list, vector<Production> plist, string start_symbol)
	{
		root.label = start_symbol;
		this->token_list = token_list;
		this->index = 0;
		this->Pindex = 0;
		this->Plist = plist;
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
	~Yacc_Tree()
	{
		out.close();

	}

};

class TableElem {
public:
	string cat;
	string choice;
	int id;
	TableElem(string cat,string choice, int id)
	{
		this->cat = cat;
		this->choice = choice;
		this->id = id;
	}

};
map<int, map<string, vector<TableElem> > > LRTable;
typedef set<Item> Items;
list<int> column_idx;
vector<Items > items_list;  //节点索引到节点信息的映射
map<int, map<string, int> > graph;  // (节点索引 , 生成式头） --->  节点索引 

FirstFollowGen gen; //First，Follow集生成器
vector<Production> grammer; //文法列表
list<token> token_list;  //要识别的token序列列表
void CalcClosure(Items& cur)
{
	//计算Closure时可以使用暴力搜索,也可以维护一个从head到体列表的map
	//暂时使用暴力搜索的方式完成
	int pre ;
	do
	{
		pre = cur.size();
		Items tmp;
		for (auto one : cur)
		{
			if (!one.IsReduced())
			{
				for(int i=0;i<grammer.size();i++)
				{
					auto second = grammer[i];
					if (second.first == one.body[one.dot])
					{
						Item gama(true, second.first, second.second, 0, i);
						tmp.insert(gama);
					}
				}
			}
		}
		gen.Merge(tmp, cur, cur);
	} while (pre<cur.size());
}
void BuildGraph()
{
	Items start;
	start.insert(Item(false, grammer[0].first, grammer[0].second, 0, 0));
	queue<int> que;
	CalcClosure(start);
	items_list.push_back(start);
	que.push(items_list.size() - 1);
	if (!graph.count(items_list.size() - 1))
		graph[items_list.size() - 1] = map<string, int>();
	while (que.size())
	{
		auto now = que.front();
		auto it = items_list[now];
		que.pop();
		set<string> waiting;
		for (auto one : it)
		{
			if (!one.IsReduced())
			{
				waiting.insert(one.body[one.dot]);
			}
		}
		for (auto i : waiting)
		{
			Items tmp;
			for (auto j : it)
			{
				if (!j.IsReduced()&&j.body[j.dot] == i)
				{
					tmp.insert(j.MoveRight());
				}
			}
			CalcClosure(tmp);
			bool find = false;
			for(int k=0 ; k<items_list.size() ;k++)
			{
				if (items_list[k] == tmp)
				{
					graph[now][i] = k;
					find = true;
					break;
				}
			}
			if (!find)
			{
				items_list.push_back(tmp);
				if (!graph.count(items_list.size() - 1))
				{
					graph[items_list.size() - 1] = map<string, int>();
				}
				graph[now][i] = items_list.size()-1;
				que.push(items_list.size() - 1);
			}
		}
	}
}
void PrintSet()
{
	int id = 1;
	for (auto one : items_list)
	{
		cout <<"Item: " << id << ":\n";
		for (auto second : one)
		{
			cout << second.extended << " ";
			cout << second.head << " -> ";
			for(int i=0;i<second.body.size();i++)
			{
				if (i == second.dot)
					cout << " . ";
				cout << second.body[i] << " ";
			}
			if (second.dot == second.body.size())
			{
				cout << " . ";
			}
			cout << endl;
		}
		cout << endl;
		cout << endl;
		id++;
	}
}
void PrintTable()
{
	
	for (int status = 0; status < items_list.size(); status++)
	{
		for (auto obj : LRTable[status])
		{
			if (obj.second.size() > 1)
			{
				cout << "此文法不是LR(0)的文法\n";
				for (auto ss : obj.second)
				{
					cout <<status<<" "<<obj.first<<" "<< ss.cat<<" "<<ss.choice<<" "<<ss.id << " ";
				}
				cout << endl;
			}
		}
	}
}
void CreateTable()
{
	/*
	 完成GOTO部分表的构建和ACTION移进部分表项的构建
	*/
	for (int status = 0; status < items_list.size(); status++)
	{
		LRTable[status] = map<string, vector<TableElem> >();
		for (auto obj : graph[status])
		{
			if (!LRTable[status].count(obj.first))
			{
				LRTable[status][obj.first] = vector<TableElem>();
			}
			if (IsTerminal(obj.first))
			{
				//cout << status << " "<<" "<<obj.first << "   shift" << " " << obj.second << endl;
				LRTable[status][obj.first].push_back(TableElem{ "ACTION","shift",obj.second });
			}
			else
			{
				//cout << status << " " << " " << obj.first << "  GOTO" << " " << obj.second << endl;
				LRTable[status][obj.first].push_back(TableElem{ "GOTO","",obj.second });
			}
		}

	}
	/*
	 完成ACTION部分规约项的构建，以及终结态(ACC)的构建
	*/
	cout << endl << endl;
	Item Final(false,grammer[0].first,grammer[0].second,1,0);
	for (int status = 0; status < items_list.size(); status++)
	{
		for (auto one : items_list[status])
		{
			if (one == Final)
			{
				bool ok = 1;
				for (auto tt : LRTable[status]["#"])
				{
					if (tt.choice != "reduce")
					{
						ok = 0;
						break;
					}
				}
				if (ok)
				{
					LRTable[status]["#"].clear();
					LRTable[status]["#"].push_back(TableElem("ACTION", "acc", -1));

				}
				else
				{
					cout << "终结符位置发生移进规约冲突\n";
				}
			}
			if (one.IsReduced())
			{
				for (int i=0;i<terminal_num;i++)
				{
					if (gen.FollowSet[one.head].count(terminal[i]))
					{
						//cout << status << " " << " " <<terminal[i] << " ruduce" << " " << one.from << endl;
						if(!LRTable[status].count(terminal[i])||LRTable[status][terminal[i]].empty()|| LRTable[status][terminal[i]][0].choice!="acc")
						LRTable[status][terminal[i]].push_back(TableElem("ACTION", "reduce", one.from));
					}
				}
			}
		}
	}
	/*
	 消除if else移进规约冲突
	 */
	LRTable[97]["else"].pop_back();

}
token Get(string str)
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
	int pc;
	in >> pc >> pa >> pb; 
	column_idx.push_back(pc);
	if (pb.empty())
	{
		pb = " ";
	}
	return make_pair(pa, pb);
}
void LoadToken()
{
	string a;
	while (cin >> a)
	{
		token_list.push_back(Get(a));
	}
	token_list.push_back(make_pair("#", ""));
	column_idx.push_back(column_idx.back() + 1);
}
void LoadGrammer()
{
	/*
	   读入生成式,
	*/
	ifstream in = ifstream("./LR_grammer.txt");
	Production cur;
	while (in >> cur.first)
	{
		cur.second.clear();  //清空body列表
		string tmp;
		in >> tmp;
		assert(tmp == "->"); //如果不是生成符则报错，一定发生了错位
		getline(in, tmp);
		stringstream in(tmp);
		while (in >> tmp)
		{
			cur.second.push_back(tmp);
		}
		grammer.push_back(cur);
	}
	in.close();
}
void Load()
{
	LoadGrammer();
	LoadToken();
}

vector<Production> Plist;
vector<token> Tlist;
void LoadFinalProduction()
{
	Production cur;
	ifstream in = ifstream("./final_production.txt");
	while (in >> cur.first)
	{
		cur.second.clear();  //清空body列表
		string tmp;
		in >> tmp;
		assert(tmp == "->"); //如果不是生成符则报错，一定发生了错位
		getline(in, tmp);
		stringstream sin(tmp);
		while (sin >> tmp)
		{
			cur.second.push_back(tmp);
		}
		Plist.push_back(cur);
	}
	reverse(Plist.begin(), Plist.end());
}
void ShowInJson()
{
	auto backup = token_list;
	backup.pop_back();
	while (backup.size())
	{
		Tlist.push_back(backup.front());
		backup.pop_front();
	}
	reverse(Tlist.begin(), Tlist.end());
	LoadFinalProduction();
	Yacc_Tree tree(Tlist, Plist, Plist[0].first);
	tree.BuildTree();
	tree.PrintInJson();
}

void Infer()
{
	ofstream out = ofstream("./final_production.txt");
	vector<token> obj_stack ;
	list<token> in = token_list;
	vector<int> status_stack;
	status_stack.push_back(0);
	while (true)
	{
		//cout << status_stack.back() << ":\n";
		auto a = status_stack.back();
		auto b = in.front().first;
		if (!LRTable[a].count(b) || LRTable[a][b].empty())
		{
			cout << "Error: syntax error, unexpected " << b << "," <<
				"expecting ";
			for (auto c : LRTable[a])
			{
				if(IsTerminal(c.first))
				cout << c.first << " or ";
			}
			cout << "  encountered at line number:" << column_idx.front() << endl;
			exit(5);

		}
		auto one = LRTable[a][b][0];
		if ( one.choice== "reduce")
		{
			auto pro = grammer[one.id];
			int sz = pro.second.size();
			if (pro.second.size() == 1 && pro.second[0] == "^")
				sz = 0;
			while (sz--)
				obj_stack.pop_back();
			obj_stack.push_back(token{ pro.first,"" });
			while (status_stack.size() > obj_stack.size())
			{
				status_stack.pop_back();
			}
			status_stack.push_back(LRTable[status_stack.back()][obj_stack.back().first][0].id);
			out << pro.first << "  ->";
			for (auto A : pro.second)
			{
				out <<" "<< A <<" ";
			}
			out << endl;
		}
		else if (one.choice == "shift")
		{
			//cout << "shift" << " " << in.front().first << endl;
			obj_stack.push_back(in.front());
			in.pop_front();
			column_idx.pop_front();
			status_stack.push_back(one.id);
		}
		else if(one.choice=="acc")
		{
			//out << grammer[0].first << " -> " << grammer[0].second[0] << endl;
			cout << "Successful Infer\n";
			break;
		}
		else
		{
			cout << "Error!\n";
			exit(5);
		}
	}
	out.close();
}
int main(void)
{
	Relocate;
	Load();
	BuildGraph();
	CreateTable();
	//PrintTable();
	Infer();
	ShowInJson();
	//PrintSet();
	return 0;
}