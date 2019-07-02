#ifndef  BUFFER
#define  BUFFER

#include"tools.h"

//将数据从载入buffer
void Buffer::Load()    
{
	string mdata;
	while (!input->eof())
	{
		getline(*input, mdata);
		buffer += mdata + "\n";
	}
	sz = buffer.size();
	//cout << buffer;
	//system("PAUSE");
}
//数据为词法分析预处理
string Buffer::PreProcess(string code)
{
	/*
	    将原所有的除换行外的空白符忽略，对于换行符进行单独处理(分割字符串及行号)
	*/
	string ans;//返回值
	int sz = code.size();
	int id = 0;
	while (id < sz)
	{
		if (isspace(code[id]) && code[id]!='\n')
			ans += " ";
		while (id < sz && isspace(code[id]) && code[id] != '\n')
		{
			id++;
		}
		if (id < sz)
		{
			ans += code[id];
			id++;
		}

	}
	return ans;
}
//默认构造函数
Buffer::Buffer() { 
	index = 0;
    sz = 0;
	line_id = 1;
	buffer = "";
}
//根据读入数据来源构造
Buffer::Buffer(ReadMode m)
{
	index = 0;
	sz = 0;
	line_id = 1;
	if (m == read_from_file)
	{
		input =new ifstream("./src_code.txt");
		if (input->fail())
		{
			cout << "读入文件失败,请检查文件路径\n";
			exit(-1);
		}
	}
    Load();
}
//copy构造函数
Buffer::Buffer(const Buffer& one)
{
	buffer = one.buffer;
	line_id = one.line_id;
	sz = one.sz;
	index = one.index;
	input = one.input;
	input = NULL;   //处于安全性的考虑，不允许其他buffer对象访问输入流
}
//调用preprocess完成预处理
void Buffer::Pre()
{
	buffer = PreProcess(buffer);
	sz = buffer.size();
	index = 0;
	line_id = 1;
	//cout << buffer << endl;
	//cout << endl << endl << buffer << endl;
}
//判断buffer是否已经读完
bool Buffer::EoF()
{
	return index >= sz ;
}
//读一个字符，并完成指针后移
char Buffer::ReadChar()
{
	if (!EoF()&&buffer[index] == '\n')
	{
		line_id++;
	}
	index++;
	return  buffer[index - 1];
}
//指针回退一步
void Buffer::TraceBack()
{
	index--;
	if (buffer[index] == '\n')
		line_id--;
}
void Buffer::Debug()
{
	cout << buffer << endl;
}
//析构函数，主要完成动态创建的流对象的删除
Buffer::~ Buffer()
{
	buffer.clear();
	sz = 0;
	index = 0;
	line_id = 1;
	delete input;
}


#endif 