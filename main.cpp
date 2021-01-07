/*

要求：
(1)可以识别出用C语言编写的源程序中的每个单词符号，并以记号的形式输出每个单词符号。
(2)可以识别并跳过源程序中的注释。
(3)可以统计源程序中的语句行数、各类单词的个数、以及字符总数，并输出统计结果。
(4)检查源程序中存在的词法错误，并报告错误所在的位置。   //错误所在的位置即当前所在的行数
(5)对源程序中出现的错误进行适当的恢复，使词法分析可以继续进行，对源程序进行一次扫描，即可检查并报告源程序中存在的所有词法错误。

*/

//首先定义种别码
/*
第一类：标识符   nondigit (nondigit| digit)*  无穷集
第二类：整型和浮点数常量    (digit)+  无穷集
第三类: 字符串常量 String literals ""
第四类：字符常量 ''
第五类：关键字(32)
		auto       break    case     char        const      continue
		default    do       double   else        enum       extern
		float      for      goto     if          int        long
		register   return   short    signed      sizeof     static
		struct     switch   typedef  union       unsigned   void
		volatile    while
第六类：分界符和运算符(标点)  ‘/*’、‘//’、 () { } [ ] " "  <、<=、>、>=、=、+、-、*、/、^、==
*/

#include<iostream>
#include<vector>
#include "def.h"
#include "Buffer.h"
#include "lib.h"
#include "main.h"
using namespace std;


int rowsNum;//语句行数
int idNum;	//各类单词的个数
int constNumNum;	//数字常量个数
int constStringNum;	//字符串常量个数
int constCharNum;	//字符常量个数
int pucNum;	//标点的个数
int charNum;//字符总数

int state ;	//当前状态
char c;			//当前读入的字符
//char token[MAX_TOKEN_LEN];//当前正在识别的单词字符串
vector <char>token;//当前正在识别的单词字符串
Buffer buf;
FILE* fp;	//指向文件的指针
//保留字表
char reservedWords[35][20] = {
   "auto", "break", "case", "char", "const", "continue",
   "default", "do", "double", "else", "enum", "extern",
   "float", "for", "goto", "if", "int", "long",
   "register", "return", "short", "signed", "sizeof", "static",
   "struct", "switch", "typedef", "union", "unsigned", "void",
   "volatile", "while"
};

//界符运算符表,根据需要可以自行增加
char puc[60][10] = {
  "+","-","*","/","<","<=",">",">=","=","==",
  "!=","^","++","--","&","&&","|","||","%","~","<<",">>","!",
  ";",",","\"","\'","#","(",")","[","]","{", "}","\\",".",":"
};

vector<string>table;//用来存放识别出的标识符


void scanner()
{
	state = 0;
	do
	{
		switch (state)
		{
		case 0://初始状态
		{
			token.clear();
			buf.get_char();
			buf.get_nbc();
			if (isLetter(c))
			{
				state = 1;
			}
			else if (isDigit(c))
			{
				state = 2;
			}
			else if (c == '\'')
			{
				state = 3;
			}
			else if (c == '"')
			{
				state = 4;
			}
			else if (isPuc(c))
			{
				state = 5;
			}
			else if (c == '\n')
			{
				
				state = 0;
			}
			else//error状态
			{
				state = 11;
			}
			break;
		}
		case 1://标识符状态
		{
			cat();
			buf.get_char();
			if (isLetter(c) || isDigit(c))
			{
				state = 1;
			}
			else
			{
				buf.retract();
				state = 0;
				token.push_back('\0');
				char* t = &token[0];
				string flag = searchReserve(t);
				if (flag != "-1")//为保留关键字
				{
					output("reservedWords");
					break;
				}
				else//为自定义的标识符
				{
					idNum++;
					int pos = table_insert(&token[0]);
					output("identifier");
				}
			}
			break;
		}
		case 2://数字常量状态
		{
			cat();
			buf.get_char();
			if (isDigit(c))
			{
				state = 2; break;
			}
			else if (c == '.')
			{
				state = 6; break;//小数点状态
			}
			else if (c == 'E' || c == 'e')
			{
				state = 7; break;//指数状态
			}
			else//识别出整常数
			{
				buf.retract();
				state = 0;
				constNumNum++;
				output("num");
				break;
			}
			break;
		}
		case 3://字符常量状态
		{
			cat();
			buf.get_char();
			while (c != '\'')
			{
				cat();
				buf.get_char();
			}
			cat();
			constCharNum++;
			output("character constant");
			state = 0;
			break;
		}
		case 4://字符串常量状态
		{
			cat();
			buf.get_char();
			while (c != '"')
			{
				cat();
				buf.get_char();
			}
			cat();
			constStringNum++;
			output("String literals");
			state = 0;
			break;
		}
		case 5://各类标点状态
		{
			if (isUnaryOpr(c))
			{
				cat();
				pucNum++;
				output("puctuation");
				state = 0;
				break;
			}
			switch (c)
			{
			case '+':
			{
				cat();
				buf.get_char();
				if (c == '+')
				{
					cat();
					state = 0;
					pucNum++;
					output("puctuation");
					break;
				}
				else
				{
					buf.retract();
					state = 0;
					pucNum++;
					output("puctuation");
					break;
				}
				break;
			}
			case '-':
			{
				cat();
				buf.get_char();
				if (c == '-')
				{
					cat();
					state = 0;
					pucNum++;
					output("puctuation");
					break;
				}
				else
				{
					buf.retract();
					state = 0;
					pucNum++;
					output("puctuation");
					break;
				}
				break;
			}
			case '/':
			{
				cat();
				buf.get_char();
				if (c == '*')//注释处理
				{
					state = 10;//转到/*注释处理阶段
				}
				else if (c == '/')
				{
					buf.get_char();
					while (c != '\n')
					{
						buf.get_char();
					}
					state = 0;
				}
				else
				{
					buf.retract();
					state = 0;
					pucNum++;
					output("puctuation");
					break;
				}
				break;
				break;
			}
			case '<':
			{
				cat();
				buf.get_char();
				if (c == '<')
				{
					cat();
					state = 0;
					pucNum++;
					output("puctuation");
					break;
				}
				else
				{
					buf.retract();
					state = 0;
					pucNum++;
					output("puctuation");
					break;
				}
				break;
			}
			case '>':
			{
				cat();
				buf.get_char();
				if (c == '>')
				{
					cat();
					state = 0;
					pucNum++;
					output("puctuation");
					break;
				}
				else
				{
					buf.retract();
					state = 0;
					pucNum++;
					output("puctuation");
					break;
				}
				break;
			}
			case '=':
			{
				cat();
				buf.get_char();
				if (c == '=')
				{
					cat();
					state = 0;
					pucNum++;
					output("puctuation");
					break;
				}
				else
				{
					buf.retract();
					state = 0;
					pucNum++;
					output("puctuation");
					break;
				}
				break;
			}
			case '!':
			{
				cat();
				buf.get_char();
				if (c == '=')
				{
					cat();
					state = 0;
					pucNum++;
					output("puctuation");
					break;
				}
				else
				{
					buf.retract();
					state = 0;
					pucNum++;
					output("puctuation");
					break;
				}
				break;
			}
			case '&':
			{
				cat();
				buf.get_char();
				if (c == '&')
				{
					cat();
					state = 0;
					pucNum++;
					output("puctuation");
					break;
				}
				else
				{
					buf.retract();
					state = 0;
					pucNum++;
					output("puctuation");
					break;
				}
				break;
			}
			case '|':
			{
				cat();
				buf.get_char();
				if (c == '|')
				{
					cat();
					state = 0;
					pucNum++;
					output("puctuation");
					break;
				}
				else
				{
					buf.retract();
					state = 0;
					pucNum++;
					output("puctuation");
					break;
				}
				break;
			}

			}
			break;
		}
		case 6://小数状态
		{
			cat();
			buf.get_char();
			if (isDigit(c))
			{
				state = 6;
				break;
			}
			else if (c == 'E' || c == 'e')
			{
				state = 7;
				break;
			}
			else//识别出小数
			{
				buf.retract();
				state = 0;
				constNumNum++;
				output("num");
				break;
			}
			break;
		}
		case 7://指数状态
		{
			cat();
			buf.get_char();
			if (isDigit(c))
			{
				state = 8;
			}
			else if (c == '+' || c == '-')
			{
				state = 9;
			}
			else
			{
				buf.retract();
				error();
				state = 0;
				break;
			}
			break;
		}
		case 8://识别num的中间状态,返回无符号数
		{
			cat();
			buf.get_char();
			if (isDigit(c))
			{
				state = 8;
			}
			else
			{
				buf.retract();
				constNumNum++;
				output("num");
				state = 0;
			}
			break;
		}
		case 9:
		{
			cat();
			buf.get_char();
			if (isDigit(c))
			{
				state = 8;
			}
			else
			{
				/*buf.retract();
				state = 0;
				constNumNum++;
				output("num");*/
				error();
			}
			break;
		}
		case 10:///*注释处理阶段
		{
			buf.get_char();
			while (c != '*')
			{
				buf.get_char();
			}
			buf.get_char();
			if (c == '/')
			{
				state = 0;
			}
			else
			{
				state = 10;
			}
			break;
		}
		case 11:
		{
			error();
			break;
		}
		}
		//printInform();
	
	}while (c != EOF);

}

int main()
{
	buf.beginRead();
	scanner();
	fclose(fp);
	printInform();
	return 0;
}