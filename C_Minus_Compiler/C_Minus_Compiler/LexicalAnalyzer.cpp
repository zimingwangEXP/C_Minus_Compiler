#ifndef LEXICAL_ANALYZER
#define LEXICAL_ANALYZER

#include"Buffer.cpp"

static  string mkeywords[] = { "else","if","int","return","void","while" };
static char special_condition[] = { '+','-','*',';',',','(',')','[',']','{','}' };

class Lexer
{
private:
	Buffer buff;
	map<int, vector<pair<string, int> > > st;
public:
	list<int> token_column;//行号的设置这样子做有问题，最好是使用由语法分析器在调用getToken前访问buffer对象中的
	//column ID的取值范围
	Lexer(Buffer one) 
	{
		this->buff = one;
	}
	//获取一个token
	pair<string, int> getToken()
	{
		string token_name, token_value;
		bool End = 0;//表示是否进入结束态
		do {
			char watched = buff.ReadChar();//读一个字符
			if (isalpha(watched))//如果是字母,就一直读到不是字母为止（c语言就是读到非字母，非数字，非_为止)
			{
				token_value = watched;
				token_name = "ID";
				char op;
				while (!buff.EoF())
				{
					op = buff.ReadChar();
					if (isalpha(op))
					{
						token_value += op;
					}
					else
					{
						buff.TraceBack();
						break;
					}
				}
				//判断标志符是不是关键字
				for (auto one : mkeywords)
				{
					if (one == token_value)
					{
						token_name = one;
						token_value = " ";
						break;
					}
				}
				End = 1;
			}
			else if (isdigit(watched))//如果是数字,就一直读到不是数字为止
			{
				token_value = watched;
				token_name = "NUM";
				char op;
				while (!buff.EoF())
				{
					op = buff.ReadChar();
					if (isdigit(op))
					{
						token_value += op;
					}
					else
					{
						buff.TraceBack();
						break;
					}
				}
				End = 1;
			}
			else if (isspace(watched))//空格由于已经要求预处理,所以现在只有一个
			{
				token_name = "BLANK";
				token_value = " ";
				End = 1;
			}
			else
			{
				bool recongnized = 0;
				for (auto one : special_condition)
				{//对无公共前缀的运算符单独考虑
					if (one == watched)
					{
						token_name = one;
						token_value = "";
						recongnized = 1;
						break;
					}
				}
				if (!recongnized)
				{
					char op;
					//考虑<,<=,>,>=,=,==
					if (watched == '<'|| watched == '>'|| watched == '=')
					{
						if (buff.EoF())
						{
							errors+= "Error:运算符 " + to_string(watched)+ " 后文件结尾\n";
						}
						op = buff.ReadChar();
						if (op == '=')
						{
							token_name =string(1,watched)+ "=";
							token_value = "";
						}
						else
						{
							buff.TraceBack();
							token_name = string(1, watched);
							token_value = "";
						}
					}
                     //!,!=不予上面一起讨论的原因在于！不是一个合法的token
					else if (watched == '!')
					{
						if (buff.EoF())
						{
							errors +="Error:! 后文件结尾\n";
							exit(-1);
						}
						op = buff.ReadChar();
						if (op == '=')
						{
							token_name =  "!=";
							token_value = "";
						}
						else
						{
							errors+= "Error，出现了单独的!符号\n" ;
							exit(2);
						}
					}
					//完成/和/*...*/的区分
					else if (watched == '/')
					{
						if (buff.EoF())
						{
							errors+= "Error:/ 后文件结尾\n";
							exit(2);
						}
						op = buff.ReadChar();
						//注释的区分采用状态转移的方式完成
						if (op=='*')
						{
							token_name = "coment";
							token_value = "/*";
							int status = 1;
							while (status != 3)
							{
								if (buff.EoF())
								{
									errors+="Error:不完整的注释\n";
									exit(-1);
								}
								op = buff.ReadChar();
								token_value += op;
								if (status==1)
								{
									if (op == '*')
									{
										status = 2;
									}
			
								}
								else if (status == 2)
								{
									if (op == '/')
									{
										status = 3;
									}
									else
									{
										status = 1;
									}
								}
							}
						}
						else
						{
							token_name = "/";
							token_value = "";
							buff.TraceBack();
						}
					}
				}
				End = 1;

			}
			if (token_name == "BLANK"||token_name == "coment")
			{
				End = 0;
			}
		} while (!End&&!buff.EoF());
		map<string, string> tmp;
		tmp["token_value"] = token_value;
		ObjList.push_back(tmp);
		return make_pair(token_name, ObjList.size());
	}
	//获取所有token串，并打印
	void GetTokens(int debug=1)
	{
		while (!buff.EoF())
		{
			auto one = getToken();
			if (one.first != "BLANK"&&one.first != "comment")
			{
				if (!st.count(buff.line_id))
					st[buff.line_id] = vector<pair<string, int> >();
				st[buff.line_id].push_back(one);
			}
		}
		if (debug==1)
		{
			int cur = 0;
			for (auto one : st)
			{
				cout << one .first<< ":";
				cur += one.second.size();
				for (auto two : one.second)
				{
					cout << "<\""<<two.first << "\" , \"";
					cout << ObjList[two.second - 1]["token_value"];
					cout << "\" >  ";
					//cout<< ">";
				}
				cout << endl;
			}
			cout << cur << endl;
		}
		else if(debug==2)
		{   
			token_column.clear();
			for (auto one : st)
			{
				for (auto two : one.second)
				{
					token_column.push_back(one.first);
					cout << two.first << "$";
					if(ObjList[two.second - 1]["token_value"]!=" ")
					  cout << ObjList[two.second - 1]["token_value"];
					cout << " ";
				}
				cout << endl;
			}
		}
		else if (debug == 3)
		{
			for (auto one : st)
			{
				for (auto two : one.second)
				{
					cout << one.first << "$";
					cout << two.first << "$";
					if (ObjList[two.second - 1]["token_value"] != " ")
						cout << ObjList[two.second - 1]["token_value"];
					cout << " ";
				}
				cout << endl;
			}
		}
		else
		{
			cout << "Error：GetTokensde的bug参数只能在(1,2,3)中取值\n";
			exit(5);
		}
	}

};

#endif
