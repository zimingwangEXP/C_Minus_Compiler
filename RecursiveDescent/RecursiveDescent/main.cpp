#include<bits/stdc++.h>
#define Relocate freopen("./in.txt","r",stdin); freopen("./out.txt","w",stdout) 
using namespace std;
typedef  pair<string, string> token;
typedef  pair<string, vector<string> > Production;
const static int terminal_num = 30;
static  string terminal[] = { "ID","NUM","else","if","int","return","void","while"
,"+", "-","*","/","<","<=",">",">=","==","!=","=",";","," ,"(",")","[","]","{","}","^","#" };
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
class RecurDescPaser {
public:
	vector<token> token_list;   //
	vector<Production> FinalPlist;
	int total_pointer;
	Production Bind(string head, string body="")
	{
		Production one;
		one.first = head;
		if(body.size())
		one.second.push_back(body);
		return  one;
	}
	void BackUp(int p_backup, int i_backup)
	{
		while (FinalPlist.size() > p_backup)
		{
			FinalPlist.pop_back();
		}
		total_pointer = i_backup;
	}
	void ThrowException(string reason)
	{
		cout << reason << endl;
		exit(2);
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
		in >> pa >> pb;
		if (pb.empty())
		{
			pb = " ";
		}
		return make_pair(pa, pb);
	}
	RecurDescPaser()
	{
		token_list.clear();
		FinalPlist.clear();
		total_pointer = 0;

	}
	void Load()
	{
		string a;
		while (cin >> a)
		{
			token_list.push_back(Get(a));
		}
		token_list.push_back(make_pair("$", ""));
	}
	bool Match(string expectedToken)
	{
		if (total_pointer<token_list.size()-1&&expectedToken == token_list[total_pointer].first)
		{
			++total_pointer;
			return true;
		}
		else
		{
			return false;
		}
	}
	bool Var()
	{
		int p_backup = FinalPlist.size();
		int i_backup = total_pointer;
		FinalPlist.push_back(Bind("var", "ID"));
		if (!Match("ID"))
		{
			BackUp(p_backup, i_backup);
			cout << "Var " << "match ID" << endl;
			return false;
		}
		if (token_list[total_pointer].first == "[")
		{

			FinalPlist[p_backup].second.push_back("[");
			FinalPlist[p_backup].second.push_back("expression");
			FinalPlist[p_backup].second.push_back("]");
			Match("[");
			if (!Expression())
			{
				/*
				异常
				*/
				BackUp(p_backup, i_backup);
				cout << "Var " << "expression" << endl;
				return false;
			}
			if (!Match("]"))
			{
			/*
			异常
			*/ 
				cout << "Var " << "match ]" << endl;
				BackUp(p_backup, i_backup);
				return false;
			}
		}
		return true;
	}
	bool TypeSpecifier()
	{

		int p_backup = FinalPlist.size();
		int i_backup = total_pointer;
		if (token_list[total_pointer].first == "int")
		{
			FinalPlist.push_back(Bind("type-specifier", "int"));
			if (!Match("int"))
			{
				BackUp(p_backup, i_backup);
				cout << "TypeSpecifier " << "match int" << endl;
				return false;
			}
		}
		else if (token_list[total_pointer].first == "void")
		{
			FinalPlist.push_back(Bind("type-specifier", "void"));
			if (!Match("void"))
			{
				BackUp(p_backup, i_backup);
				cout << "TypeSpecifier " << " match void" << endl;
				return false;
			}
		
		}
		else
		{
			BackUp(p_backup,i_backup);
			cout << "TypeSpecifier " << "total " << endl;
			return false;
			/*
			抛异常
			*/
		}
		return true;
	}
	bool Param()
	{
		int p_backup = FinalPlist.size();
		int i_backup = total_pointer;
		FinalPlist.push_back(Bind("param", "type-specifier"));
		if (!TypeSpecifier())
		{
			BackUp(p_backup, i_backup);
			cout << "Param " << "TypeSpecifier " << endl;

			return false;
		}
		FinalPlist[p_backup].second.push_back("ID");
		if (!Match("ID"))
		{
			BackUp(p_backup, i_backup);
			cout << "Param " << "match ID " << endl;
			return false;
		}
		if (token_list[total_pointer].first == "[")
		{
			FinalPlist[p_backup].second.push_back("[");
			FinalPlist[p_backup].second.push_back("]");
			if (!Match("[") )
			{
				cout << "Param " << "match [" << endl;
				BackUp(p_backup, i_backup);
				return false;
			}
			if (!Match("]"))
			{
				cout << "Param " << "match ]" << endl;
				BackUp(p_backup, i_backup);
				return false;
			}
		}
		return true;
	}
	bool ParamList()
	{
		int p_backup = FinalPlist.size();
		int i_backup = total_pointer;
		FinalPlist.push_back(Bind("param-list", "param"));
		int wp_backup = FinalPlist.size(), wi_backup = total_pointer;
		if (!Param())
		{
			BackUp(p_backup, i_backup);
			cout << "ParamList " << "param" << endl;
			return false;
		}
		while (token_list[total_pointer].first == ",")
		{
			Match(",");
			if (!Param())
			{
				BackUp(wp_backup, wi_backup);
				break;
			}
			FinalPlist[p_backup].second.push_back(",");
			FinalPlist[p_backup].second.push_back("param");
			wp_backup = FinalPlist.size();
			wi_backup = total_pointer;
		}
		return true;
	}
	bool Params()
	{
		/*
			此处if的顺序不能替换，因为这种简单的分支结构仅限于对简单的
			C_Minus文法适用,对于更复杂的文法通用方法是在dfs过程在父节点
			设置变量记录子节点的生成式选择序号，将引用传给子节点，子节点从
			该序号进行最左推导
		*/

		int p_backup = FinalPlist.size();
		int i_backup = total_pointer;
		FinalPlist.push_back(Bind("params","param-list"));
		if(!ParamList())
		{
			BackUp(p_backup, i_backup);
			FinalPlist.push_back(Bind("params", "void"));
			if (!Match("void"))
			{
				BackUp(p_backup, i_backup);
				cout << "Params " << " match void" << endl;
				return false;
			}
		}
		return true;
	}
	bool ArgsList()
	{
		int p_backup = FinalPlist.size();
		int i_backup = total_pointer;
		FinalPlist.push_back(Bind("arg-list", "expression"));
		int wp_backup = FinalPlist.size();
		int wi_back_up = total_pointer;
		if (!Expression())
		{
			BackUp(p_backup, i_backup);
			cout << "ArgsList " << "Expression " << endl;
			return false;
		}
		while (token_list[total_pointer].first == ",")
		{

			Match(",");
			if (!Expression())
			{
				BackUp(wp_backup, wi_back_up);
				break;
			}
			wp_backup = FinalPlist.size();
			wi_back_up = total_pointer;
			FinalPlist[p_backup].second.push_back(",");
			FinalPlist[p_backup].second.push_back("expression");
		}

		return true;
	}
	bool Args()
	{
		int p_backup = FinalPlist.size();
		int i_backup = total_pointer;
		FinalPlist.push_back(Bind("args"));
		if (ArgsList())
		{
			FinalPlist[p_backup].second.push_back("arg-list");
		}
		return true;

	}
	bool Relop()
	{

		int p_backup = FinalPlist.size();
		int i_backup = total_pointer;
		string& cmp = token_list[total_pointer].first;
		FinalPlist.push_back(Bind("relop"));
		if (cmp == "<=")
		{
			FinalPlist[p_backup].second.push_back("<=");
			Match("<=");
		}
		else if (cmp == "<")
		{
			FinalPlist[p_backup].second.push_back("<");
			Match("<");
		}
		else if (cmp == ">")
		{
			FinalPlist[p_backup].second.push_back(">");
			Match(">");
		}
		else if (cmp == ">=")
		{
			FinalPlist[p_backup].second.push_back(">=");
			Match(">=");
		}
		else if (cmp == "==")
		{
			FinalPlist[p_backup].second.push_back("==");
			Match("==");
		}
		else if (cmp == "!=")
		{
			FinalPlist[p_backup].second.push_back("!=");
			Match("!=");
		}
		else
		{
			/*
	   抛出异常
	*/
			BackUp(p_backup, i_backup);
			cout << "Relop " << "total " << endl;

			return false;
	
		}
		return true;
	}
	bool Call()
	{
		int p_backup = FinalPlist.size();
		int i_backup = total_pointer;
		FinalPlist.push_back(Bind("call", "ID"));
		if (!Match("ID"))
		{
			BackUp(p_backup, i_backup);
			cout << "Call " << "ID " << endl;

			return false;
		}
		FinalPlist[p_backup].second.push_back("(");
		if (!Match("("))
		{
			BackUp(p_backup, i_backup);
			cout << "Call " << "match ( " << endl;

			return false;
		}
		FinalPlist[p_backup].second.push_back("args");
		if (!Args())
		{
			BackUp(p_backup, i_backup);
			cout << "Call " << "Args " << endl;

			return false;
		}
		FinalPlist[p_backup].second.push_back(")");
		if (!Match(")"))
		{
			BackUp(p_backup, i_backup);
			cout << "Call " << ") " << endl;

			return false;
		}
		return true;
	}
	bool Factor()
	{
		int p_backup = FinalPlist.size();
		int i_backup = total_pointer;
		string& cmp = token_list[total_pointer].first;
		FinalPlist.push_back(Bind("factor"));
		if (cmp == "(")
		{
			FinalPlist[p_backup].second.push_back("(");
			FinalPlist[p_backup].second.push_back("expression");
			FinalPlist[p_backup].second.push_back(")");
			Match("(");
			if (!Expression())
			{
				BackUp(p_backup, i_backup);
				cout << "Factor " << "Expression " << endl;
				return false;
			}
			if (!Match(")"))
			{
				BackUp(p_backup, i_backup);
				cout << "Factor " << "match ) " << endl;
				return false;
			}
		}
		else if (cmp == "NUM")
		{
			FinalPlist[p_backup].second.push_back("NUM");
			Match("NUM");
		}
		else if (cmp == "ID")
		{
			if (Call())
			{
				FinalPlist[p_backup].second.push_back("call");


			}
			else if (Var())
			{
				FinalPlist[p_backup].second.push_back("var");

			}
			else
			{
				BackUp(p_backup, i_backup);
				cout << "Factor " << "no Var no Call " << endl;

				return false;
				/*
				抛异常
				*/
			}
		}
		else
		{
			BackUp(p_backup, i_backup);
			cout << "Factor " << "total " << endl;
			return false;
			/*
			抛异常
			*/
		}
		return true;
	}
	bool Mulop()
	{
		int p_backup = FinalPlist.size();
		int i_backup = total_pointer;
		string& cmp = token_list[total_pointer].first;
		FinalPlist.push_back(Bind("mulop"));
		if (cmp == "*")
		{
			FinalPlist[p_backup].second.push_back("*");
			Match("*");
		}
		else if (cmp == "/")
		{
			FinalPlist[p_backup].second.push_back("/");
			Match("/");
		}
		else
		{
			BackUp(p_backup, i_backup);
			cout << "Mulop " << "total " << endl;
			return false;
			/*
			抛出异常
			*/
		}
		return true;
	}
	bool Addop()
	{
		int p_backup = FinalPlist.size();
		int i_backup = total_pointer;
		string& cmp = token_list[total_pointer].first;
		FinalPlist.push_back(Bind("addop"));
		if (cmp == "+")
		{
			FinalPlist[p_backup].second.push_back("+");
			Match("+");
		}
		else if (cmp == "-")
		{
			FinalPlist[p_backup].second.push_back("-");
			Match("-");
		}
		else
		{
			BackUp(p_backup, i_backup);
			cout << "Addop " << "total " << endl;
			return false;
			/*
			抛出异常
			*/
		}
		return true;
	}
	bool Term()
	{
		int p_backup = FinalPlist.size();
		int i_backup = total_pointer;
		FinalPlist.push_back(Bind("term", "factor"));
		int wp_backup = FinalPlist.size();
		int wi_backup = total_pointer;
		if (!Factor())
		{
			BackUp(p_backup, i_backup);
			cout << "Term " << "Factor " << endl;
			return false;
		}
		while (Mulop())
		{
			
			if (!Factor())
			{
				/*
				   抛出异常或者设置新的备份点，备份
				*/
				BackUp(wp_backup, wi_backup);
				break;
			}
			FinalPlist[p_backup].second.push_back("mulop");
			FinalPlist[p_backup].second.push_back("factor");
			wp_backup = FinalPlist.size();
			wi_backup = total_pointer;
		}
		return true;
	}
	bool AdditiveExpression()
	{
		int p_backup = FinalPlist.size();
		int i_backup = total_pointer;
		FinalPlist.push_back(Bind("additive-expression", "term"));
		int wp_backup = FinalPlist.size();
		int wi_backup = total_pointer;
		if (!Term())
		{
			BackUp(p_backup, i_backup);
			cout << "AdditiveExpression " << "Term " << endl;
			return false;
		}
		while (Addop())
		{
			if (!Term())
			{
				BackUp(wp_backup, wi_backup);
				break;
			}
			FinalPlist[p_backup].second.push_back("addop");
			FinalPlist[p_backup].second.push_back("term");
			wp_backup = FinalPlist.size();
			wi_backup = total_pointer;
		}
		return true;
	}
	bool SimpleExpression()
	{
		int p_backup = FinalPlist.size();
		int i_backup = total_pointer;
		FinalPlist.push_back(Bind("simple-expression", "additive-expression"));
		if (!AdditiveExpression())
		{
			BackUp(p_backup, i_backup);
			cout << "SimpleExpression " << "AdditiveExpression " << endl;
			return false;
		}
		if (Relop())
		{
			FinalPlist[p_backup].second.push_back("relop");
			FinalPlist[p_backup].second.push_back("additive-expression");
			if (!AdditiveExpression())
			{
				/*
				   抛出异常或者设置新的备份点，备份
				*/
			}
		}
		return true;
	}
	bool Expression()
	{
		int p_backup = FinalPlist.size();
		int i_backup = total_pointer;
		FinalPlist.push_back(Bind("expression", ""));
		int wp_p_backup = p_backup+1, wi_backup = i_backup;
		while (Var())
		{

			if (!Match("="))
			{
				BackUp(wp_p_backup, wi_backup);
				break;
			}
			FinalPlist[p_backup].second.push_back("var");
			FinalPlist[p_backup].second.push_back("=");
			wp_p_backup = FinalPlist.size();
			wi_backup = total_pointer;
		}
		FinalPlist[p_backup].second.push_back("simple-expression");
		if (!SimpleExpression())
		{
			BackUp(p_backup, i_backup);
			cout << "Expression " << "SimpleExpression " << endl;

			return false;
		}
		return true;

	}
	bool  SelectionStmt()
	{
		//selection-stmt → if(expression) statement [ else statement ]
		int p_backup = FinalPlist.size();
		int i_backup = total_pointer;
		FinalPlist.push_back(Bind("selection-stmt", "if"));
		if (!Match("if"))
		{
			BackUp(p_backup, i_backup);
			cout << "SelectionStmt " << "match if " << endl;
			return false;
		}
		FinalPlist[p_backup].second.push_back("(");
		if (!Match("("))
		{
			BackUp(p_backup, i_backup);
			cout << "SelectionStmt " << "match ( " << endl;

			return false;
	    }
		FinalPlist[p_backup].second.push_back("expression");
		if (!Expression())
		{
			BackUp(p_backup, i_backup);
			cout << "SelectionStmt " << "Expression" << endl;

			return false;
		}
		FinalPlist[p_backup].second.push_back(")");
		if (!Match(")"))
		{
			cout << "SelectionStmt " << "match ) " << endl;
			BackUp(p_backup, i_backup);
			return false;
		}
		FinalPlist[p_backup].second.push_back("statement");
		if (!Statement())
		{
			cout << "SelectionStmt " << "Statement " << endl;
			BackUp(p_backup, i_backup);
			return false;
		}
		if (token_list[total_pointer].first == "else")
		{
			Match("else");
			if (!Statement())
			{
				ThrowException("else 后面没有合法的语句\n");
			}
			FinalPlist[p_backup].second.push_back("else");
			FinalPlist[p_backup].second.push_back("statement");

		}
		return true;

	}
	bool IterationStmt()
	{

		int p_backup = FinalPlist.size();
		int i_backup = total_pointer;
		FinalPlist.push_back(Bind("iteration-stmt","while"));
		if (!Match("while"))
		{
			BackUp(p_backup, i_backup);
			cout << "IterationStmt " << "match while " << endl;
			return false;
		}
		FinalPlist[p_backup].second.push_back("(");
		if (!Match("("))
		{
			BackUp(p_backup, i_backup);
			cout << "IterationStmt " << "match ( " << endl;
			return false;
		}
		FinalPlist[p_backup].second.push_back("expression");
		if (!Expression())
		{
			BackUp(p_backup, i_backup);
			cout << "IterationStmt " << "Expression " << endl;
			return false;
		}
		FinalPlist[p_backup].second.push_back(")");
		if (!Match(")"))
		{
			BackUp(p_backup, i_backup);
			cout << "IterationStmt " << "match ) " << endl;

			return false;
		}
		FinalPlist[p_backup].second.push_back("statement");
		if (!Statement())
		{
			cout << "IterationStmt " << "Statement " << endl;

			BackUp(p_backup, i_backup);
			return false;
		}
		return true;
	}
	bool ReturnStmt()
	{
		int p_backup = FinalPlist.size();
		int i_backup = total_pointer;
		FinalPlist.push_back(Bind("return-stmt", "return"));
		if (!Match("return"))
		{
			BackUp(p_backup, i_backup);
			cout << "ReturnStmt " << "return " << endl;
			return false;
		}
		if (Expression())
		{
			FinalPlist[p_backup].second.push_back("expression");
		}
		FinalPlist[p_backup].second.push_back(";");
		if (!Match(";"))
		{
			BackUp(p_backup, i_backup);
			cout << "ReturnStmt " << "match ; " << endl;
			return false;
			/*
			    抛出异常,末尾没有；号
			*/
		}
		return true;
	}
	bool ExpressionStmt()
	{
		int p_backup = FinalPlist.size();
		int i_backup = total_pointer;
		FinalPlist.push_back(Bind("expression-stmt", ""));
		if (Expression()) 
		{
			FinalPlist[p_backup].second.push_back("expression");
		}
		if (Match(";"))
		{
			FinalPlist[p_backup].second.push_back(";");
		}
		else
		{
			BackUp(p_backup, i_backup);
			cout << "ExpressionStmt " << "; match " << endl;
			return false;
		}
		return true;
	}
	bool Statement()
	{
		int p_backup = FinalPlist.size();
		int i_backup = total_pointer;
		FinalPlist.push_back(Bind("statement", ""));
		if (SelectionStmt())
		{
			FinalPlist[p_backup].second.push_back("selection-stmt");
		}
		else if (IterationStmt())
		{
			FinalPlist[p_backup].second.push_back("iteration-stmt");
		}
		else if (CompoundStmt())
		{
			FinalPlist[p_backup].second.push_back("compound-stmt");
		}
		else if (ReturnStmt())
		{
			FinalPlist[p_backup].second.push_back("return-stmt");
		}
		else if (ExpressionStmt())
		{
			FinalPlist[p_backup].second.push_back("expression-stmt");
		}
		else
		{
			BackUp(p_backup, i_backup);
			cout << "Statement " << "total " << endl;

			return false;
			/*
			抛出异常
			*/
		}
		return  true;
	}
	bool LocalDeclarations()
	{
		int p_backup = FinalPlist.size();
		int i_backup = total_pointer;
		FinalPlist.push_back(Bind("local-declarations", ""));
		while (VarDeclaration())
		{
			FinalPlist[p_backup].second.push_back("var-declaration");
		}
		return true;
	}
	bool StatementList()
	{

		int p_backup = FinalPlist.size();
		int i_backup = total_pointer;
		FinalPlist.push_back(Bind("statement-list"));
		while (Statement())
		{
			FinalPlist[p_backup].second.push_back("statement");
		}
		return true;
	}
	bool CompoundStmt()
	{
		int p_backup = FinalPlist.size();
		int i_backup = total_pointer;
		FinalPlist.push_back(Bind("compound-stmt","{"));
		if (!Match("{"))
		{
			BackUp(p_backup, i_backup);
			cout << "CompoundStmt " << "{ " << endl;

			return false;
		}
		FinalPlist[p_backup].second.push_back("local-declarations");
		if (!LocalDeclarations())
		{
			BackUp(p_backup, i_backup);
			cout << "CompoundStmt " << "LocalDeclarations " << endl;

			return false;
		}
		FinalPlist[p_backup].second.push_back("statement-list");
		if (!StatementList())
		{
			BackUp(p_backup, i_backup);
			cout << "CompoundStmt " << "StatementList " << endl;

			return false;
		}
		FinalPlist[p_backup].second.push_back("}");
		if (!Match("}"))
		{
			BackUp(p_backup, i_backup);
			cout << "CompoundStmt " << "match } " << endl;

			return false;
		}
		return true;
	}
	bool FunDeclaration()
	{
		int p_backup = FinalPlist.size();
		int i_backup = total_pointer;
		FinalPlist.push_back(Bind("fun-declaration", "type-specifier"));
		if (!TypeSpecifier())
		{
			BackUp(p_backup, i_backup);
			cout << "FunDeclaration " << "TypeSpecifier" << endl;

			return false;
		}
		FinalPlist[p_backup].second.push_back("ID");
		if (!Match("ID"))
		{
			BackUp(p_backup, i_backup);
			cout << "FunDeclaration " << "match id" << endl;

			return false;
		}
		FinalPlist[p_backup].second.push_back("(");
		if (!Match("("))
		{
			BackUp(p_backup, i_backup);
			cout << "FunDeclaration " << "match (" << endl;

			return false;
		}
		FinalPlist[p_backup].second.push_back("params");
		if (!Params())
		{
			cout << "FunDeclaration " << "Params" << endl;
			BackUp(p_backup, i_backup);
			return false;
		}
		FinalPlist[p_backup].second.push_back(")");
		if (!Match(")"))
		{
			cout << "FunDeclaration " << "match )" << endl;

			BackUp(p_backup, i_backup);
			return false;
		}
		FinalPlist[p_backup].second.push_back("compound-stmt");
		if (!CompoundStmt())
		{
			BackUp(p_backup, i_backup);
			cout << "FunDeclaration " << "CompoundStmt" << endl;
			return false;
		}
		return true;

	}
	bool VarDeclaration()
	{

		int p_backup = FinalPlist.size();
		int i_backup = total_pointer;
		FinalPlist.push_back(Bind("var-declaration", "type-specifier"));
		FinalPlist[p_backup].second.push_back("ID");
		if (!TypeSpecifier())
		{
			BackUp(p_backup, i_backup);
			cout << "VarDeclaration " << "TypeSpecifier" << endl;

			return false;
		}
		if (!Match("ID"))
		{
			BackUp(p_backup, i_backup);
			cout << "VarDeclaration " << "match ID" << endl;

			return false;
		}
		if (token_list[total_pointer].first == "[")
		{
			FinalPlist[p_backup].second.push_back("[");
			if (!Match("["))
			{
				BackUp(p_backup, i_backup);
				cout << "VarDeclaration " << "match [" << endl;
				return false;
			}
			FinalPlist[p_backup].second.push_back("NUM");
			if (!Match("NUM"))
			{
				cout << "VarDeclaration " << "match NUM" << endl;

				BackUp(p_backup, i_backup);
				return false;
			}
			FinalPlist[p_backup].second.push_back("]");
			if (!Match("]"))
			{
				cout << "VarDeclaration " << "match ]" << endl;

				BackUp(p_backup, i_backup);
				return false;
			}
		}
		FinalPlist[p_backup].second.push_back(";");
		if (!Match(";"))
		{
			cout << "VarDeclaration " << "match ;" << endl;

			BackUp(p_backup, i_backup);
			return false;
		}
		return true;
	}
	bool Declaration()
	{

		int p_backup = FinalPlist.size();
		int i_backup = total_pointer;
		FinalPlist.push_back(Bind("declaration", "var-declaration"));
		if (VarDeclaration())
		{
			return true;
		}
		else
		{
			BackUp(p_backup, i_backup);
			FinalPlist.push_back(Bind("declaration", "fun-declaration"));
			if (FunDeclaration())
			{
				return true;
			}
			else
			{
				BackUp(p_backup, i_backup);
				cout << "Declaration " << " total" << endl;

				return false;
			}
		}
	}
	bool DeclarationList()
	{
		int p_backup = FinalPlist.size();
		int i_backup = total_pointer;
		auto one = Bind("declaration-list", "declaration");
		FinalPlist.push_back(one);
		if (!Declaration())
		{
			BackUp(p_backup, i_backup);
			return false;
		}
		while (Declaration())
		{
			FinalPlist[p_backup].second.push_back("declaration");
		}
		return true;
	}
	bool Program()
	{
		/*
		  用于维护Final
		*/
		int p_backup = FinalPlist.size();
		int i_backup = total_pointer;
		FinalPlist.push_back(Bind("program", "declaration-list"));
		if (DeclarationList()&&total_pointer==token_list.size()-1)
		{
			return true;
		}
		else
		{
			BackUp(p_backup, i_backup);
			return false;
		}
	}
	void BuildTree()
	{
		RS_Tree tree(token_list, FinalPlist, "program");
		tree.BuildTree();
		tree.PrintInJson();
	}
	class Pnode {
	public:
		string label;
		string value;
		vector<Pnode*> children_list;
	};
	class RS_Tree {
	private:
		Pnode root;
		vector<pair<string, string> > token_list;
		vector<Production> Plist;
		int Pindex;
		ofstream out;
		int index;
		void BuildTreeHelp(Pnode* sroot)
		{
		//	cout << sroot->label << endl;
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
					out << "匹配错误1\n";
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
		RS_Tree(vector<pair<string, string> > token_list,vector<Production> plist, string start_symbol)
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
		~RS_Tree()
		{
			out.close();

		}
		
	};

};
int main(void)
{
	Relocate;
	RecurDescPaser paser;
	paser.Load();
	if (!paser.Program())
	{
		cout << "Game Over\n";
		return 0;
	}
    paser.BuildTree();
	cout << "使用的生成式如下:\n";
	for (auto one : paser.FinalPlist)
	{
		    cout << one.first << " -> ";
			for (auto two : one.second)
			{
				cout << two << " ";
			}
			cout << endl;
	}
	/*for (auto one : paser.token_list)
	{
		cout << one.first << " " << one.second << endl;
	}*/
	//paser.Program();

	return 0;
}