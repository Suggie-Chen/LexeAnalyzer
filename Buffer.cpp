#include<vector>
#include "Buffer.h"
using namespace std;
extern FILE* fp;
extern char c;
extern int charNum;
extern int rowsNum;
extern vector<char>token;
void Buffer::beginRead()
{
	errno_t e = fopen_s(&fp, "test.cpp", "r");

	if (e != 0)
		printf("Failed to open the file!\n");
	else
	{
		printf("The file has been opened.\n");
	}
	int count=fread(buffer, 1, MAXBUFLEN / 2 - 1, fp);
	//printf("count==%d\n", count);
	if (feof(fp))
	{
		buffer[lhead + count] = EOF;
	}
	
	
}

int Buffer::forwarding()
{
	pforward++;
	if (buffer[pforward] == EOF)
	{
		if (pforward == lend)//��ǰָ����������յ�
		{
			//��������Ұ���
			int count=fread(&buffer[rhead], 1, MAXBUFLEN / 2 - 1, fp);
			if (feof(fp))//�����ļ�β��
			{
				buffer[rhead+count+1] == EOF;
			}
			pforward++;
		}
		else if (pforward == rend)//��ǰָ�����Ұ����յ�
		{
			//������������
			int count=fread(buffer, 1, MAXBUFLEN / 2 - 1, fp);
			if (feof(fp))//�����ļ�β��
			{
				buffer[lhead + count] == EOF;
			}
			pforward = 0;
		}
		else //��ֹ�ʷ�����
		{
			std::cout << "�ļ��Ѷ���!" << std::endl;
			//exit(6);
		}
	}
	return 0;
}

void Buffer::retract()
{
	pforward = (pforward - 1) % MAXBUFLEN;
	if (pforward == EOF)
	{
		pforward = (pforward - 1) % MAXBUFLEN;
	}

}

void Buffer::get_char()
{
	//����һ���ַ���pforward��ǰ��һ
	c = buffer[pforward];
	charNum++;
	if (c == '\n')
	{
		rowsNum++;
	}
	forwarding();
}

//�����ո�
void Buffer::get_nbc()
{
	while (c == ' ' || c == '\t')
	{
		get_char();
	}
}
