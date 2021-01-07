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
		if (pforward == lend)//向前指针在左半区终点
		{
			//重新填充右半区
			int count=fread(&buffer[rhead], 1, MAXBUFLEN / 2 - 1, fp);
			if (feof(fp))//读到文件尾了
			{
				buffer[rhead+count+1] == EOF;
			}
			pforward++;
		}
		else if (pforward == rend)//向前指针在右半区终点
		{
			//重新填充左半区
			int count=fread(buffer, 1, MAXBUFLEN / 2 - 1, fp);
			if (feof(fp))//读到文件尾了
			{
				buffer[lhead + count] == EOF;
			}
			pforward = 0;
		}
		else //终止词法分析
		{
			std::cout << "文件已读完!" << std::endl;
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
	//读入一个字符，pforward向前加一
	c = buffer[pforward];
	charNum++;
	if (c == '\n')
	{
		rowsNum++;
	}
	forwarding();
}

//跳过空格
void Buffer::get_nbc()
{
	while (c == ' ' || c == '\t')
	{
		get_char();
	}
}
