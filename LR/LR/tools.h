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

typedef  pair<string, vector<string> >  Production;//ʹ��pair���������ʽ
extern vector<map<string, string> > ObjList;  //Obejct��
extern string errors;//Error��ϢͰ

const static int terminal_num = 30;
static  string terminal[] = { "ID","NUM","else","if","int","return","void","while"
,"+", "-","*","/","<","<=",">",">=","==","!=","=",";","," ,"(",")","[","]","{","}","^","#" };
/*
	 �ڴ˴������Ľ�C_Minus�������ս��������һ�飬
	 ��ʵ�Ͽ��Բ������ֻ���ķ�ʽ�����Ż�
	 1.ʹ�������ļ���д����ǿ����չ��
	 2.��ʵ�ϣ����ս����������ʽ��head��Ӧ������ȫ��ȵģ���˿��Բ����棬������ReadIn�����н�����ʽ�����
	   First,Follow���ĳ�ʼ�����룬Ȼ��������ʽ����׶����head���ϵĳ�ʼ���������ݴ��ж��Ƿ�Ϊ�ս��(���
	   IsTerminal()��IsNoTerminal()����
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
enum ReadMode //����buffer���뷽ʽ����ǿ�ɶ���
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
};//����Դ�����ļ��Ķ�д

#endif