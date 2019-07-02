#ifndef TOOLS_H
#define TOOLS_H


#include<fstream>
#include<iostream>
#include<set>
#include<string>
#include<map>
#include<list>
#include<iterator>
#include<algorithm>
#include<sstream>
#include<vector>
#include<assert.h>
#include<ctype.h>

using namespace std;

typedef  pair<string, vector<string> >  Production;//使用pair定义的生成式
extern vector<map<string, string> > ObjList;  //Obejct表
extern string errors;//Error信息桶

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
bool  inline IsNonterminal(string op)
{
	return !IsTerminal(op);
}
enum ReadMode //定义buffer读入方式，增强可读性
{
	read_from_console,
	read_from_file
};

class Buffer {
private:
	string buffer;
	int sz;
	int index;
	inline void Load();
	istream* input = &cin;
	inline string PreProcess(string code);
public:
	int line_id;
	inline Buffer();
	inline Buffer(ReadMode m);
	inline Buffer(const Buffer& one);
	inline void Pre();
	inline bool EoF();
	inline char ReadChar();
	inline void Debug();
	inline void TraceBack();
	inline ~Buffer();
};//用于源代码文件的读写

#endif