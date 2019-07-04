#include<bits/stdc++.h>
using namespace std;
#define Relocate freopen("./out.txt","r",stdin);freopen("./tree.txt","w",stdout) 

typedef  pair<string, vector<string> >  Production;//使用pair定义的生成式
typedef pair<string, string>  Token;
vector<Production> Plist;
vector<Token> Tlist;
const static int terminal_num = 30;
static  string terminal[] = { "ID","NUM","else","if","int","return","void","while"
,"+", "-","*","/","<","<=",">",">=","==","!=","=",";","," ,"(",")","[","]","{","}","^","#" };
/*
	 在此处单纯的将C_Minus的所有终结符保存了一遍，
	 事实上可以采用两种互斥的方式进行优化
	 1.使用配置文件读写，增强可扩展性
	 2.事实上，非终结符集和生成式的head集应该是完全相等的，因此可以不保存，单纯在ReadIn函数中将生成式读入和
	   First,Follow集的初始化分离，然后在生成式读入阶段完成head集合的初始化，并根据此判定是否为终结符(完成
	   IsTerminal()和IsNoTerminal()函数
*/
bool  inline IsTerminal(string op)
{
	for (int i = 0; i < terminal_num; i++)
	{
		if (terminal[i] == op)
		{
			//cout << terminal[i] << endl;
			return true;
		}
	}
	return false;
}

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
		//cout << sroot->label << endl;
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
				//cout << Plist[Pindex].first<<" "<< sroot->label << endl;
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
			for (int i=sz-1;i>=0;i--)
			{
				auto child = sroot->children_list[i];
				BuildTreeHelp(child);
			}
		}
	}
	void PrintInIndent(Pnode* sroot,int depth)
	{
		for (int i = 0; i < depth; i++)
		{
			cout << "   ";
		}
		if (IsTerminal(sroot->label))
		{
			cout << sroot->label<< ":" << sroot->value << endl;
		}
		else
		{
			cout << "*";
			cout << sroot->label << endl;
			for (auto one : sroot->children_list)
			{
				PrintInIndent(one,depth+1);
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
	void Print()
	{
		PrintInIndent(&root, 0);
	}

};

Token Get(string a)
{
	for (auto& one : a)
	{
		if (one == '|')
			one = ' ';
	}
	stringstream in(a);
	string first, second;
	in >> first; in >> second;
	return Token(first, second);
}
void ReadIn()
{
	/*
	   读入token序列
	*/
	string line,tmp;
	getline(cin, line);
	stringstream tin(line);
	while (tin>>tmp)
	{
		auto one = Get(tmp);
		if(one.first.size()&&one.second.size())
		Tlist.push_back(one);
		//cout << Tlist.back().first << " " << Tlist.back().second << endl;
	}
	/*
	   读入生成式,
	*/
	Production cur;
	while (cin >> cur.first)
	{
		if (cur.first == "0" )
			break;
		if (cur.first == "1")
		{
			cout << "源代码存在语法错误，无法建树\n";
			exit(0);
		}
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
		Plist.push_back(cur);
	}
	reverse(Plist.begin(), Plist.end());
	reverse(Tlist.begin(), Tlist.end());
}
int main(void)
{
	Relocate;
	ReadIn();
	Yacc_Tree tree(Tlist, Plist, Plist[0].first);
	tree.BuildTree();
	tree.PrintInJson();
	tree.Print();
	return 0;
}
