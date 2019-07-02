#include"LexicalAnalyzer.cpp"
#include"LL1_Paser.cpp"
vector<map<string, string> > ObjList = vector<map<string, string> >();  //Obejct表
string errors = "";//Error信息桶

int main(void)
{
	//输出重定向
    freopen("./token_seq.txt", "w", stdout);
	Buffer buf(read_from_file);
	//调用预处理程序，将buffer中的数据清洗
	buf.Pre();
	//定义词法分析对象
	Lexer lexer(buf);
	LL1_Paser paser(lexer);
	paser.ConstructTable();
	paser.Infer();
	//lexer.GetTokens(3);
	
		return 0;
}