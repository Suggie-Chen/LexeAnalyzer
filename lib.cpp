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
extern int idNum;	//���൥�ʵĸ���
extern int constNumNum;	//���ֳ�������
extern int constStringNum;	//�ַ�����������
extern int constCharNum;	//�ַ���������
extern int pucNum;	//���ĸ���
extern char puc[50][10];
extern char reservedWords[32][20];
extern vector<string>table;//�������ʶ����ı�ʶ��

//�ж��Ƿ�Ϊ��ĸ
bool isLetter(char letter)
{
	//ע��C���������»���ҲΪ��ʶ����һ���ֿ��Է����ײ��������ط�
	if ((letter >= 'a' && letter <= 'z') || (letter >= 'A' && letter <= 'Z') || (letter == '_'))
	{
		return true;
	}
	else
	{
		return false;
	}
}


//�ж��Ƿ�Ϊ����
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
//���ұ�����
string searchReserve(char s[])
{
	for (int i = 0; i < 32; i++)
	{
		if (strcmp(reservedWords[i], s) == 0)
		{
			return reservedWords[i];///���ɹ����ң��򷵻ر�ʶ��
		}
	}
	return "-1";//���򷵻�"-1"��������Ҳ��ɹ�����Ϊ��ʶ��
}






//����ǰ��c�ӵ�token��
void cat()
{
	token.push_back(c);
}

//��token�е��ַ���ת��Ϊ����
int string2int()
{
	char* temp = &token[0];
	return atoi(temp);
}

//��token�е��ַ���ת��Ϊ������
float string2float()
{
	char* temp = &token[0];
	return atof(temp);

}

//������
void error()
{
	cout << "����������������Ϊ" << rowsNum << endl;
	token.clear();	//���token�е�Ԫ��
	state = 0;		//ת����ʼ״̬
}

void printInform()
{
	printf("***************************\n");
	printf("�ַ�����:     %d\n", charNum);
	printf("������:       %d\n", rowsNum);
	printf("��ʶ����:     %d\n", idNum);
	printf("���ֳ�����:   %d\n", constNumNum);
	printf("�ַ���������: %d\n", constStringNum);
	printf("�ַ���������: %d\n", constCharNum);
	printf("������:     %d\n", pucNum);
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