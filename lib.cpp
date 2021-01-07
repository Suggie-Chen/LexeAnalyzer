#include <string.h>
#include <iterator>
#include <vector>
#include "Buffer.h"
#include "def.h"
using namespace std;

extern Buffer buf;
extern char c;
extern vector<char> token;
extern int state;
extern int rowsNum;
extern int charNum;
extern int idNum;	//各类单词的个数
extern int constNumNum;	//数字常量个数
extern int constStringNum;	//字符串常量个数
extern int constCharNum;	//字符常量个数
extern int pucNum;	//标点的个数
extern char puc[50][10];
extern char reservedWords[32][20];
extern vector<string>table;//用来存放识别出的标识符

//判断是否为字母
bool isLetter(char letter)
{
	//注意C语言允许下划线也为标识符的一部分可以放在首部或其他地方
	if ((letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z') || (letter == '_'))
	{
		return true;
	}
	else
	{
		return false;
	}
}


//判断是否为数字
bool isDigit(char digit)
{
	if (digit >= '0' && digit <= '9')
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool isPuc(char c)
{
	char temps[10] = { 0 };
	temps[0] = c;
	for (int i = 0; i < 50; i++)
	{
		if (strcmp(temps,puc[i])==0)
		{
			return true;
		}
	}
	return false;
}

bool isUnaryOpr(char c)
{
	if (c == '*' || c == '%' ||
		c == '^' || c == '~' || c == '?' ||
		c == ',' || c == ':' || c == '.' || c == ';' || c == '#' ||
		c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}'
		)
	{
		return true;
	}
	else
		return false;
}
//查找保留字
string searchReserve(char s[])
{
	for (int i = 0; i < 32; i++)
	{
		if (strcmp(reservedWords[i], s) == 0)
		{
			return reservedWords[i];///若成功查找，则返回标识名
		}
	}
	return "-1";//否则返回"-1"，代表查找不成功，即为标识符
}






//将当前的c接到token后
void cat()
{
	token.push_back(c);
}

//将token中的字符串转化为整数
int string2int()
{
	char* temp = &token[0];
	return atoi(temp);
}

//将token中的字符串转化为浮点数
float string2float()
{
	char* temp = &token[0];
	return atof(temp);

}

//错误处理
void error()
{
	cout << "出错啦！出错行数为" << rowsNum << endl;
	token.clear();	//清空token中的元素
	state = 0;		//转到开始状态
}

void printInform()
{
	printf("***************************\n");
	printf("字符总数:     %d\n", charNum);
	printf("总行数:       %d\n", rowsNum);
	printf("标识符数:     %d\n", idNum);
	printf("数字常量数:   %d\n", constNumNum);
	printf("字符串常量数: %d\n", constStringNum);
	printf("字符常量个数: %d\n", constCharNum);
	printf("标点个数:     %d\n", pucNum);
	printf("***************************\n");
}

void output(string type)
{
	token.push_back('\0');
	char* t = &token[0];

	cout << '<' << type <<','<< t << '>' << endl;
}

int table_insert(string s)
{
	vector<string>::iterator it = find(table.begin(), table.end(), s);

	if (it == table.end())
	{
		table.push_back(s);
		 int pos=std::distance(table.begin(), table.end());
		 
		return pos;///
	}
	else
	{
		int pos = std::distance(table.begin(), it);
		return pos;
	}
}