#include"LexicalAnalyzer.cpp"
#include"LL1_Paser.cpp"
vector<map<string, string> > ObjList = vector<map<string, string> >();  //Obejct��
string errors = "";//Error��ϢͰ

int main(void)
{
	//����ض���
    freopen("./token_seq.txt", "w", stdout);
	Buffer buf(read_from_file);
	//����Ԥ������򣬽�buffer�е�������ϴ
	buf.Pre();
	//����ʷ���������
	Lexer lexer(buf);
	LL1_Paser paser(lexer);
	paser.ConstructTable();
	paser.Infer();
	//lexer.GetTokens(3);
	
		return 0;
}