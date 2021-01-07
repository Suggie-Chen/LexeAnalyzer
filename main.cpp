/*

Ҫ��
(1)����ʶ�����C���Ա�д��Դ�����е�ÿ�����ʷ��ţ����ԼǺŵ���ʽ���ÿ�����ʷ��š�
(2)����ʶ������Դ�����е�ע�͡�
(3)����ͳ��Դ�����е�������������൥�ʵĸ������Լ��ַ������������ͳ�ƽ����
(4)���Դ�����д��ڵĴʷ����󣬲�����������ڵ�λ�á�   //�������ڵ�λ�ü���ǰ���ڵ�����
(5)��Դ�����г��ֵĴ�������ʵ��Ļָ���ʹ�ʷ��������Լ������У���Դ�������һ��ɨ�裬���ɼ�鲢����Դ�����д��ڵ����дʷ�����

*/

//���ȶ����ֱ���
/*
��һ�ࣺ��ʶ��   nondigit (nondigit| digit)*  ���
�ڶ��ࣺ���ͺ͸���������    (digit)+  ���
������: �ַ������� String literals ""
�����ࣺ�ַ����� ''
�����ࣺ�ؼ���(32)
		auto       break    case     char        const      continue
		default    do       double   else        enum       extern
		float      for      goto     if          int        long
		register   return   short    signed      sizeof     static
		struct     switch   typedef  union       unsigned   void
		volatile    while
�����ࣺ�ֽ���������(���)  ��/*������//���� () { } [ ] " "  <��<=��>��>=��=��+��-��*��/��^��==
*/

#include<iostream>
#include<vector>
#include "def.h"
#include "Buffer.h"
#include "lib.h"
#include "main.h"
using namespace std;


int rowsNum;//�������
int idNum;	//���൥�ʵĸ���
int constNumNum;	//���ֳ�������
int constStringNum;	//�ַ�����������
int constCharNum;	//�ַ���������
int pucNum;	//���ĸ���
int charNum;//�ַ�����

int state ;	//��ǰ״̬
char c;			//��ǰ������ַ�
//char token[MAX_TOKEN_LEN];//��ǰ����ʶ��ĵ����ַ���
vector <char>token;//��ǰ����ʶ��ĵ����ַ���
Buffer buf;
FILE* fp;	//ָ���ļ���ָ��
//�����ֱ�
char reservedWords[35][20] = {
   "auto", "break", "case", "char", "const", "continue",
   "default", "do", "double", "else", "enum", "extern",
   "float", "for", "goto", "if", "int", "long",
   "register", "return", "short", "signed", "sizeof", "static",
   "struct", "switch", "typedef", "union", "unsigned", "void",
   "volatile", "while"
};

//����������,������Ҫ������������
char puc[60][10] = {
  "+","-","*","/","<","<=",">",">=","=","==",
  "!=","^","++","--","&","&&","|","||","%","~","<<",">>","!",
  ";",",","\"","\'","#","(",")","[","]","{", "}","\\",".",":"
};

vector<string>table;//�������ʶ����ı�ʶ��


void scanner()
{
	state = 0;
	do
	{
		switch (state)
		{
		case 0://��ʼ״̬
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
			else//error״̬
			{
				state = 11;
			}
			break;
		}
		case 1://��ʶ��״̬
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
				if (flag != "-1")//Ϊ�����ؼ���
				{
					output("reservedWords");
					break;
				}
				else//Ϊ�Զ���ı�ʶ��
				{
					idNum++;
					int pos = table_insert(&token[0]);
					output("identifier");
				}
			}
			break;
		}
		case 2://���ֳ���״̬
		{
			cat();
			buf.get_char();
			if (isDigit(c))
			{
				state = 2; break;
			}
			else if (c == '.')
			{
				state = 6; break;//С����״̬
			}
			else if (c == 'E' || c == 'e')
			{
				state = 7; break;//ָ��״̬
			}
			else//ʶ���������
			{
				buf.retract();
				state = 0;
				constNumNum++;
				output("num");
				break;
			}
			break;
		}
		case 3://�ַ�����״̬
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
		case 4://�ַ�������״̬
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
		case 5://������״̬
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
				if (c == '*')//ע�ʹ���
				{
					state = 10;//ת��/*ע�ʹ���׶�
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
		case 6://С��״̬
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
			else//ʶ���С��
			{
				buf.retract();
				state = 0;
				constNumNum++;
				output("num");
				break;
			}
			break;
		}
		case 7://ָ��״̬
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
		case 8://ʶ��num���м�״̬,�����޷�����
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
		case 10:///*ע�ʹ���׶�
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