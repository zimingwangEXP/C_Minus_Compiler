#ifndef  BUFFER
#define  BUFFER

#include"tools.h"

//�����ݴ�����buffer
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
//����Ϊ�ʷ�����Ԥ����
string Buffer::PreProcess(string code)
{
	/*
	    ��ԭ���еĳ�������Ŀհ׷����ԣ����ڻ��з����е�������(�ָ��ַ������к�)
	*/
	string ans;//����ֵ
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
//Ĭ�Ϲ��캯��
Buffer::Buffer() { 
	index = 0;
    sz = 0;
	line_id = 1;
	buffer = "";
}
//���ݶ���������Դ����
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
			cout << "�����ļ�ʧ��,�����ļ�·��\n";
			exit(-1);
		}
	}
    Load();
}
//copy���캯��
Buffer::Buffer(const Buffer& one)
{
	buffer = one.buffer;
	line_id = one.line_id;
	sz = one.sz;
	index = one.index;
	input = one.input;
	input = NULL;   //���ڰ�ȫ�ԵĿ��ǣ�����������buffer�������������
}
//����preprocess���Ԥ����
void Buffer::Pre()
{
	buffer = PreProcess(buffer);
	sz = buffer.size();
	index = 0;
	line_id = 1;
	//cout << buffer << endl;
	//cout << endl << endl << buffer << endl;
}
//�ж�buffer�Ƿ��Ѿ�����
bool Buffer::EoF()
{
	return index >= sz ;
}
//��һ���ַ��������ָ�����
char Buffer::ReadChar()
{
	if (!EoF()&&buffer[index] == '\n')
	{
		line_id++;
	}
	index++;
	return  buffer[index - 1];
}
//ָ�����һ��
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
//������������Ҫ��ɶ�̬�������������ɾ��
Buffer::~ Buffer()
{
	buffer.clear();
	sz = 0;
	index = 0;
	line_id = 1;
	delete input;
}


#endif 