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
	list<int> token_column;//�кŵ������������������⣬�����ʹ�����﷨�������ڵ���getTokenǰ����buffer�����е�
	//column ID��ȡֵ��Χ
	Lexer(Buffer one) 
	{
		this->buff = one;
	}
	//��ȡһ��token
	pair<string, int> getToken()
	{
		string token_name, token_value;
		bool End = 0;//��ʾ�Ƿ�������̬
		do {
			char watched = buff.ReadChar();//��һ���ַ�
			if (isalpha(watched))//�������ĸ,��һֱ����������ĸΪֹ��c���Ծ��Ƕ�������ĸ�������֣���_Ϊֹ)
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
				//�жϱ�־���ǲ��ǹؼ���
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
			else if (isdigit(watched))//���������,��һֱ������������Ϊֹ
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
			else if (isspace(watched))//�ո������Ѿ�Ҫ��Ԥ����,��������ֻ��һ��
			{
				token_name = "BLANK";
				token_value = " ";
				End = 1;
			}
			else
			{
				bool recongnized = 0;
				for (auto one : special_condition)
				{//���޹���ǰ׺���������������
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
					//����<,<=,>,>=,=,==
					if (watched == '<'|| watched == '>'|| watched == '=')
					{
						if (buff.EoF())
						{
							errors+= "Error:����� " + to_string(watched)+ " ���ļ���β\n";
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
                     //!,!=��������һ�����۵�ԭ�����ڣ�����һ���Ϸ���token
					else if (watched == '!')
					{
						if (buff.EoF())
						{
							errors +="Error:! ���ļ���β\n";
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
							errors+= "Error�������˵�����!����\n" ;
							exit(2);
						}
					}
					//���/��/*...*/������
					else if (watched == '/')
					{
						if (buff.EoF())
						{
							errors+= "Error:/ ���ļ���β\n";
							exit(2);
						}
						op = buff.ReadChar();
						//ע�͵����ֲ���״̬ת�Ƶķ�ʽ���
						if (op=='*')
						{
							token_name = "coment";
							token_value = "/*";
							int status = 1;
							while (status != 3)
							{
								if (buff.EoF())
								{
									errors+="Error:��������ע��\n";
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
	//��ȡ����token��������ӡ
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
			cout << "Error��GetTokensde��bug����ֻ����(1,2,3)��ȡֵ\n";
			exit(5);
		}
	}

};

#endif
