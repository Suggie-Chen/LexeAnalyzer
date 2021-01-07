int rowsNum;//lines
int lexeNum;//
int charNum;//
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

int main()
{
	/*zhangwan*ting*/
	char temp = 'a';

	char ch= '/a';

	int i = 1;
	i++;
	i--;
	float num = 4.6E+-4;
	if (i != 0)
	{
		;
	}
	else
	{
		exit(9);
	}
	printf("chenshuyi/*//*");
	return 0;
}