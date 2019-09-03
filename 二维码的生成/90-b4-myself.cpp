/*1652241 臧海彬 计算机1班*/
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include"90-b4.h"
#include"cmd_console_tools.h"
#include<stdlib.h>
#include<conio.h>
using namespace std;

/*-------------------------------------------------获取信息----------------------------------------------------*/

/*获得生成二维码的初始信息*/
char *GetInformation(unsigned int &char_num)
{
	char *xinxi;
	int i = 0;
	char_num = 10;

	/*申请一块初始大小为10的空间*/
	xinxi = (char*)malloc(char_num * sizeof(char));
	if (xinxi == NULL)
		exit(1);

	/*输入信息*/
	cout << "请输入将要生成的二维码信息(1-40版本均可,容量需要根据内容而定)：" << endl;
	for (; xinxi[i - 1] != '\n'||i - 1 == 0; i++)
	{
		if (i == char_num)
		{
			char_num += 10;
			/*如果超出已有的长度需要加长*/
			xinxi = (char*)realloc(xinxi, char_num * sizeof(char));
			if (xinxi == NULL)
				exit(1);
		}
		xinxi[i] = getchar();
	}
	xinxi[i - 1] = '\0';//将回车覆盖掉

	return xinxi;
}

/*释放生成二维码的初始 信息*/
Status FreeInformation(char *xinxi)
{
	free(xinxi);

	return OK;
}

/*获取二维码的纠错等级*/
VERSION_CLASS CorrectClass()
{
	for (;;)
	{
		cout << "请输入想要纠错的等级(L,M,Q,H纠错能力依次提升，但存储相同数据可能要使用更大的版本)\n";
		char a;
		cin >> a;
		if (a == 'L' || a == 'l')
			return VERSION_L;
		else if (a == 'M' || a == 'm')
			return VERSION_M;
		else if (a == 'H' || a == 'h')
			return VERSION_H;
		else if (a == 'Q' || a == 'q')
			return VERSION_Q;
		else
			cout << "输入错误！\n";
	}
}

/*判断应该使用哪个版本*/
int JudgeVersion(char *xinxi, VERSION_CLASS version_class)
{
	int i = 1;
	for (; i <= 40; i++)
		if ((VERSION_DATA[i][version_class][FIRSTPART_DATA] * VERSION_DATA[i][version_class][FIRSTPART_NUM]
			+ VERSION_DATA[i][version_class][SECONDPART_DATA] * VERSION_DATA[i][version_class][SECONDPART_NUM]
			- (VERSION_COUNT[version_class][i] + 4 + 8) / 8) >= int(strlen(xinxi)))//太长了
			break;
	return (i <= 40 ? i : 0);
}

/*强制更改版本号*/
void ChangeVersion(int &version)
{
	cout << "现在您可以强制更改版本大小（"<<version<<"-40），是否更改？（Y/N）\n";
	for (char a;;)
	{
		a = _getch();
		if (a == 'Y' || a == 'y')
		{
			int version_linshi;
			cout << "请输入版本号：\n";
			cin >> version_linshi;
			if (version_linshi < version || version_linshi>40)
				cout << "更改版本信息失败！原因不在版本范围内！\n";
			else
			{
				cout << "从版本[" << version << "]更改到版本[" << version_linshi << "]成功!\n";
				version = version_linshi;
			}
			break;
		}
		else if (a == 'N' || a == 'n')
		{
			cout << "不更改版本信息。\n";
			break;
		}
	}
}

/*获取对于汉字的编码方式,只要是有汉字那么就使用utf-8的方式编码*/
int MethodOfHanzi(const char *s)
{
	int method = 0;
	for (int i = 0; s[i] != '\0'; i++)
		if (int(unsigned char(s[i])) >= 0xA1)
		{
			method = 1;
			break;
		}
	return method;
}

/*-------------------------------------------------将字符串编码--------------------------------------------------*/


/*将字符串信息编码为二进制信息（原来可以选择使用gb2312的方式编码汉字）*/
/*bool* ProduceStorageCode(char *xinxi, const int version, int &length,const int method)
{
	//首先选择要用何种方式编码，如果是全部都是数字，则采用数字模式，如果既有数字又有字母则用字母数字，如果存在字母数字范围外的字符
	//则使用字节，汉字使用汉字的方法
	//这些是数字字母模式中的另外九个字符
	char code_apl[9] = { ' ','$','%','*','+','-','.','/',':' };
	//用于存放二进制数据
	bool*shujuwei;
	//这里定义了混合状态的起始终止，以及模式选项
	int mode_end = 0, mode_start = 0;
	bool mode_num = TRUE, mode_alp = FALSE, mode_byte = FALSE;
	int  count_cur = 0;
	//转utf-8
	char *utf_s;

	for (length=0;; mode_num = TRUE, mode_alp = FALSE, mode_byte = FALSE)
	{
		//如果使用gb312首先到汉字位置,终止往后就是汉字或者是没有了，如果使用utf-8则全部编码
		if (method == 1)
		{
			utf_s = xinxi;
			xinxi = G2U(utf_s);
			for (; xinxi[mode_end] != '\0'; mode_end++);
		}
		else if (method == 0)
			for (; xinxi[mode_end] != '\0'; mode_end++)
				if (int(unsigned char(xinxi[mode_end])) >= 0xA1)
					break;
		if (mode_start != mode_end)
		{
			//判断这一块使用什么模式
			for (int j = mode_start; j < mode_end; j++)
				if (!(xinxi[j] >= '0' && xinxi[j] <= '9'))//如果有不再0-9范围内的字符，那么必然不能使用数字模式存储，这时先暂定使用数字字母模式
				{
					mode_alp = TRUE;
					if (!(xinxi[j] >= 'A'&&xinxi[j] <= 'Z'))//在暂定使用数字字母模式的前提下，如果有不再A-Z之间的字符
					{
						int z = 0;
						for (; z < 9; z++)//判断该字符是否是数字字母模式中其他的九个字符
							if (code_apl[z] == xinxi[j])//如果是，那么就终止循环
								break;
						if (z == 9)//如果不是以上九个字符之一，那么采用字节模式，终止循环
						{
							mode_byte = TRUE;
							break;
						}
					}//判断是否属于数字子模模式范围
				}//判断是否属于数字模式范围

		   //下面就对mode_start到mode_end范围内的字符编码
			if (mode_byte == TRUE)
				ProcessFormByte(xinxi, &shujuwei, mode_start, mode_end, length, count_cur, version);
			else if (mode_alp == TRUE)
				ProcessFormAlp(xinxi, &shujuwei, mode_start, mode_end, length, count_cur, version);
			else if (mode_num == TRUE)
				ProcessFormNum(xinxi, &shujuwei, mode_start, mode_end, length, count_cur, version);
			mode_start = mode_end;
		}
		if (xinxi[mode_end] == '\0')
			break;
		for (; xinxi[mode_end] != '\0'; mode_end++)
			if (int(unsigned char(xinxi[mode_end])) < 0xA1)
				break;
		//下面为汉字编码的过程
		ProcessFormHanzi(xinxi, &shujuwei, mode_start, mode_end,length, count_cur, version);
		mode_start = mode_end;
	}

	if (method == 1)
		delete xinxi;//注意这里的信息是转成utf_8的信息，而这只是形参值的改变，并不会原变量
	return shujuwei;
}*/

/*将字符串信息编码为二进制信息*/
bool* ProduceStorageCode(char *xinxi, const int version, int &length, const int method)
{
	/*如果有汉字，那么直接使用utf-8，否则判断究竟使用字节还是数字还是数字字母*/
	/*这些是数字字母模式中的另外九个字符*/
	char code_apl[9] = { ' ','$','%','*','+','-','.','/',':' };
	/*用于存放二进制数据*/
	bool*shujuwei=NULL;
	/*这里定义了混合状态的起始终止，以及模式选项*/
	int mode_end = 0, mode_start = 0;
	bool mode_num = TRUE, mode_alp = FALSE, mode_byte = FALSE;
	int  count_cur = 0;
	/*转utf-8*/
	char *utf_s;

	length = 0;
	/*如果使用gb312首先到汉字位置,终止往后就是汉字或者是没有了，如果使用utf-8则全部编码*/
	if (method == 1)
	{
		utf_s = xinxi;
		xinxi = G2U(utf_s);
		mode_byte = TRUE;
		for (; xinxi[mode_end] != '\0'; mode_end++);
	}
	else if (method == 0)
	{
		for (; xinxi[mode_end] != '\0'; mode_end++);
		/*判断这一块使用什么模式*/
		for (int j = mode_start; j < mode_end; j++)
			if (!(xinxi[j] >= '0' && xinxi[j] <= '9'))//如果有不再0-9范围内的字符，那么必然不能使用数字模式存储，这时先暂定使用数字字母模式
			{
				mode_alp = TRUE;
				if (!(xinxi[j] >= 'A'&&xinxi[j] <= 'Z'))//在暂定使用数字字母模式的前提下，如果有不再A-Z之间的字符
				{
					int z = 0;
					for (; z < 9; z++)//判断该字符是否是数字字母模式中其他的九个字符
						if (code_apl[z] == xinxi[j])//如果是，那么就终止循环
							break;
					if (z == 9)//如果不是以上九个字符之一，那么采用字节模式，终止循环
					{
						mode_byte = TRUE;
						break;
					}
				}//判断是否属于数字子模模式范围
			}//判断是否属于数字模式范围
	}
    /*下面就对mode_start到mode_end范围内的字符编码*/
	if (mode_byte == TRUE)
		ProcessFormByte(xinxi, &shujuwei, mode_start, mode_end, length, count_cur, version);
	else if (mode_alp == TRUE)
		ProcessFormAlp(xinxi, &shujuwei, mode_start, mode_end, length, count_cur, version);
	else if (mode_num == TRUE)
		ProcessFormNum(xinxi, &shujuwei, mode_start, mode_end, length, count_cur, version);

	if (method == 1)
		delete xinxi;//注意这里的信息是转成utf_8的信息，而这只是形参值的改变，并不会原变量
	return shujuwei;
}

/*释放生成二维码的二进制信息*/
Status FreeInformation(bool *shujuwei)
{
	free(shujuwei);

	return OK;
}

/*数字字母信息转为二进制*/
void ProcessFormAlp(char *xinxi, bool **shujuwei, int mode_start, int mode_end,int &length,int &count_cur,const int version)
{
	/*延长申请的空间*/
	if (length == 0)
	{
		length = 4 + (mode_end - mode_start) / 2 * 11 + (mode_end - mode_start) % 2 * 6 + VERSION_COUNT[VERSION_ALP][version];
		*shujuwei = (bool*)malloc(length * sizeof(bool));
	}
	else
	{
		length += (4 + (mode_end - mode_start) / 2 * 11 + (mode_end - mode_start) % 2 * 6 + VERSION_COUNT[VERSION_ALP][version]);
		*shujuwei = (bool*)realloc(*shujuwei, length * sizeof(bool));
	}
	if (*shujuwei == NULL)
		exit(1);

	int start = count_cur + 4 + VERSION_COUNT[VERSION_ALP][version];
	int linshi = 0, count;

	/*数字字母模式*/
	(*shujuwei)[count_cur + 0] = (*shujuwei)[count_cur + 1] = (*shujuwei)[count_cur + 3] = 0;
	(*shujuwei)[count_cur + 2] = 1;

	/*数字字母模式的计数字符位数*/
	Fangzhi(*shujuwei, mode_end - mode_start, count_cur + 4, start);

	/*将字符位放进去*/
	for (count = mode_start; count<mode_end; count++)
	{
		linshi *= 45;
		if (xinxi[count] >= '0'&&xinxi[count] <= '9')
			linshi += *(xinxi + count) - '0';
		else if (xinxi[count] >= 'A'&&xinxi[count] <= 'Z')
			linshi += *(xinxi + count) - 55;
		else if (xinxi[count] == ' ')
			linshi += 36;
		else if (xinxi[count] == '$')
			linshi += 37;
		else if (xinxi[count] == '%')
			linshi += 38;
		else if (xinxi[count] == '*')
			linshi += 39;
		else if (xinxi[count] == '+')
			linshi += 40;
		else if (xinxi[count] == '-')
			linshi += 41;
		else if (xinxi[count] == '.')
			linshi += 42;
		else if (xinxi[count] == '/')
			linshi += 43;
		else if (xinxi[count] == ':')
			linshi += 44;
		if ((count-mode_start) % 2 == 1)
		{
			Fangzhi(*shujuwei, linshi, start + (count-mode_start)/2 * 11, start + (count-mode_start) / 2 * 11 + 11);
			linshi = 0;
		}
	}

	/*对于剩余的两个数字或者一个数字进行处理*/
	if((count - mode_start) % 2 == 1)
	    Fangzhi(*shujuwei, linshi, start + (count - mode_start) / 2 * 11, start + (count - mode_start) / 2 * 11 + 6);
	count_cur = length;
}

/*字节方式转为二进制*/
void ProcessFormByte(char *xinxi, bool **shujuwei, int mode_start, int mode_end, int &length, int &count_cur, const int version)
{
	/*延长空间*/
	if (length == 0)
	{
		length = 4 + (mode_end - mode_start) * 8 + VERSION_COUNT[VERSION_BYTE][version];
		*shujuwei = (bool*)malloc(length * sizeof(bool));
	}
	else
	{
		length += (4 + (mode_end - mode_start) * 8 + VERSION_COUNT[VERSION_BYTE][version]);
		*shujuwei = (bool*)realloc(*shujuwei, length * sizeof(bool));
	}
	if (*shujuwei == NULL)
		exit(1);

	int start = count_cur + 4 + VERSION_COUNT[VERSION_BYTE][version];
	int linshi = 0, count;

	/*字节模式*/
	(*shujuwei)[count_cur + 0] = (*shujuwei)[count_cur + 2] = (*shujuwei)[count_cur + 3] = 0;
	(*shujuwei)[count_cur + 1] = 1;

	/*字节模式的计数字符位数*/
	Fangzhi(*shujuwei, mode_end - mode_start, count_cur + 4, start);

	/*将字符位放进去*/
	for (count = mode_start; count<mode_end; count++)
	{
		linshi = int(unsigned char(xinxi[count]));
		Fangzhi(*shujuwei, linshi, start + (count - mode_start)  * 8, start + (count - mode_start) * 8 + 8);
	}
	count_cur = length;
}

/*将数字文本信息转换为二进制存储起来*/
void ProcessFormNum(char *xinxi, bool **shujuwei, int mode_start, int mode_end, int &length, int &count_cur, const int version)
{
	/*延长空间*/
	if (length == 0)
	{
		length = 4 + (mode_end - mode_start) / 3 * 10 + (mode_end - mode_start) % 3 * 3 + ((mode_end - mode_start) % 3 + 1) / 2 + VERSION_COUNT[VERSION_NUM][version];
		*shujuwei = (bool*)malloc(length * sizeof(bool));
	}
	else
	{
		length += (4 + (mode_end - mode_start) / 3 * 10 + (mode_end - mode_start) % 3 * 3 + ((mode_end - mode_start) % 3 + 1) / 2 + VERSION_COUNT[VERSION_NUM][version]);
		*shujuwei = (bool*)realloc(*shujuwei, length * sizeof(bool));
	}
	if (*shujuwei == NULL)
		exit(1);

	int start = count_cur + 4 + VERSION_COUNT[VERSION_NUM][version];
	int linshi = 0, count;
	/*数字模式*/
	(*shujuwei)[count_cur + 0] = (*shujuwei)[count_cur + 1] = (*shujuwei)[count_cur + 2] = 0;
	(*shujuwei)[count_cur + 3] = 1;

	/*数字模式的计数字符位数*/
	Fangzhi(*shujuwei, mode_end - mode_start, count_cur + 4, start);

	/*将数字位放进去*/
	for (count = mode_start; count<mode_end; count++)
	{
		linshi = linshi * 10 + *(xinxi + count) - 48;
		if ((count-mode_start) % 3 == 2)
		{
			Fangzhi(*shujuwei, linshi, start + (count-mode_start) / 3 * 10, start + (count - mode_start) / 3 * 10 + 10);
			linshi = 0;
		}
	}

	/*对于剩余的两个数字或者一个数字进行处理*/
	Fangzhi(*shujuwei, linshi, start + (count - mode_start) / 3 * 10, start + (count - mode_start) / 3 * 10 + ((count - mode_start) % 3 + 1) / 2 + (count - mode_start) % 3 * 3);

	count_cur = length;
}

/*将汉字信息转换为二进制存储起来（gb2312）*/
/*void ProcessFormHanzi(char *xinxi, bool **shujuwei, int mode_start, int mode_end, int &length, int &count_cur, const int version)
{
	//补充空间
	if (length == 0)
	{
		length = 4 + 4 + (mode_end - mode_start) / 2 * 13 + VERSION_COUNT[VERSION_HANZI][version];
		*shujuwei = (bool*)malloc(length * sizeof(bool));
	}
	else
	{
		length += (4 + 4 + (mode_end - mode_start) / 2 * 13 + VERSION_COUNT[VERSION_HANZI][version]);
		*shujuwei = (bool*)realloc(*shujuwei, length * sizeof(bool));
	}
	if (*shujuwei == NULL)
		exit(1);

	int start = count_cur + 4 + 4 + VERSION_COUNT[VERSION_HANZI][version];
	int linshi = 0, count;

	//汉字模式
	(*shujuwei)[count_cur + 0] = (*shujuwei)[count_cur + 1] = (*shujuwei)[count_cur + 3] = (*shujuwei)[count_cur + 7] = 1;
	(*shujuwei)[count_cur + 2] = (*shujuwei)[count_cur + 4] = (*shujuwei)[count_cur + 5] = (*shujuwei)[count_cur + 6] = 0;

	//数字模式的计数字符位数
	Fangzhi(*shujuwei, (mode_end - mode_start) / 2, count_cur + 4 + 4, start);

	//将字符位放进去
	for (count = mode_start; count<mode_end; count++)
	{
		if ((count - mode_start) % 2 == 0)
		{
			if (int(unsigned char(xinxi[count])) <= 0xAA)
				linshi += (int(unsigned char(xinxi[count])) - 0xA1);
			else if (int(unsigned char(xinxi[count])) >= 0xB0)
				linshi += (int(unsigned char(xinxi[count])) - 0xA6);
		}
		else
		{
			linshi *= 0x60;
			linshi += (int(unsigned char(xinxi[count])) - 0xA1);
			Fangzhi(*shujuwei, linshi, start + (count - mode_start) / 2 * 13, start + (count - mode_start) / 2 * 13 + 13);
			linshi = 0;
		}
	}
	count_cur = length;
}*/

/*在将shuzi转换为二进制，并在shujuwei中的相应位置放上去，其中locate_end为终止的地方，该位置不算在内*/
void Fangzhi(bool *shujuwei, int shuzi, int locate_start, int locate_end)
{
	locate_end--;
	for (; locate_start <= locate_end; locate_end--)
	{
		if (shuzi % 2)
			shujuwei[locate_end] = 1;
		else
			shujuwei[locate_end] = 0;
		shuzi /= 2;
	}
}

/*将数据位补齐，并判断是否超出当前版本限制。尽管之前判断能否装下，但是仍然有可能超出*/
Status MakeUp(bool**shujuwei,const int length, int version, VERSION_CLASS version_class)
{
	int total_length = (VERSION_DATA[version][version_class][FIRSTPART_NUM] * VERSION_DATA[version][version_class][FIRSTPART_DATA]
		+ VERSION_DATA[version][version_class][SECONDPART_NUM] * VERSION_DATA[version][version_class][SECONDPART_DATA]) * 8;
	if (length <= total_length)
	{
		*shujuwei = (bool*)realloc(*shujuwei, total_length);//真正使用的存放数据的长度
		if (*shujuwei == NULL)
			return LOVERFLOW;
		int i = 0;
		for (; (i + length < total_length) && i < 4; i++)//4个零除非终止
			(*shujuwei)[length + i] = 0;
		if (i + length != total_length)//补齐剩下的八位
			for (; (i + length) % 8 != 0; i++)
				(*shujuwei)[i + length] = 0;
		if (i + length != total_length)//剩余码字按照标准值
			for (int j = 0; j + i + length < total_length; j++)
				(*shujuwei)[j + i + length] = COVER[j % 16];
	}
	else
		return ERROR;

	return OK;
}

/*-------------------------------------------生成纠错码的操作----------------------------------------------------*/

/*将生成的二进制数据转为一个个的码字。*/
Status ConvertMazi(bool*shujuwei, int version,VERSION_CLASS version_class, int ***mazi_part1,int ***mazi_part2)
{
	/*首先申请一个长度为第一部分大小的二级指针*/
	*mazi_part1 = (int**)malloc(VERSION_DATA[version][version_class][FIRSTPART_NUM]*sizeof(int*));
	if (*mazi_part1 == NULL)
		return LOVERFLOW;
	/*再申请每一部分大小的int型空间，相当于将原来的码字分割为这么多的部分*/
	for (int i = 0; i < VERSION_DATA[version][version_class][FIRSTPART_NUM]; i++)
	{
		*(*mazi_part1+i) = (int*)calloc(VERSION_DATA[version][version_class][FIRSTPART_DATA],sizeof(int));
		if (**mazi_part1 == NULL)
			return LOVERFLOW;
	}

	/*同理进行第二部分的操作，因为最多只有两类，有可能只有一种，所以第二部分直接为空即可*/
	if (VERSION_DATA[version][version_class][SECONDPART_NUM])
	{
		*mazi_part2 = (int**)malloc(VERSION_DATA[version][version_class][SECONDPART_NUM]*sizeof(int*));
		if (*mazi_part2 == NULL)
			return LOVERFLOW;
		for (int i = 0; i < VERSION_DATA[version][version_class][SECONDPART_NUM]; i++)
		{
			*(*mazi_part2+i) = (int*)calloc(VERSION_DATA[version][version_class][SECONDPART_DATA],sizeof(int));
			if (**mazi_part2 == NULL)
				return LOVERFLOW;
		}
	}
	else
		*mazi_part2 = NULL;
	
	/*好了，现在放第一部分的几个分区*/
	int i = 0;
	for (int j = 0; j < VERSION_DATA[version][version_class][FIRSTPART_NUM]; i++)
	{
		int linshi = shujuwei[i];
		linshi <<= (7 - i % 8);
		(*mazi_part1)[j][(i / 8) % VERSION_DATA[version][version_class][FIRSTPART_DATA]] |= linshi;

		if ((i + 1) % (VERSION_DATA[version][version_class][FIRSTPART_DATA] * 8) == 0)
			j++;
	}

	/*放第二部分的几个分区*/
	int k = 0;
	for (int j = 0; j < VERSION_DATA[version][version_class][SECONDPART_NUM]; k++)
	{
		int linshi = shujuwei[k+i];
		linshi <<= (7 - k % 8);
		(*mazi_part2)[j][(k / 8) % VERSION_DATA[version][version_class][SECONDPART_DATA]] |= linshi;
		if ((k + 1) % (VERSION_DATA[version][version_class][SECONDPART_DATA] * 8) == 0)
			j++;
	}

	return OK;
}

/*产生纠错码*/
Status CorrectCode(int **part1, int **part2, int version, VERSION_CLASS version_class, int *form, int ***code1, int ***code2)
{
	LinkList L_error, L_code, L_code_act;

	/*申请一个长度为第一部分大小的二级指针*/
	*code1 = (int**)malloc(VERSION_DATA[version][version_class][FIRSTPART_NUM] * sizeof(int*));
	if (*code1 == NULL)
		return LOVERFLOW;

	/*第一部分的纠错码*/
	L_error = ProduceErrorP(VERSION_DATA[version][version_class][FIRSTPART_ALLDATA] - VERSION_DATA[version][version_class][FIRSTPART_DATA], form);//L_error链表被建立
	for (int i = 0; i<VERSION_DATA[version][version_class][FIRSTPART_NUM]; i++)
	{
		L_code = ProduceCodeP(*(part1 + i), VERSION_DATA[version][version_class][FIRSTPART_DATA]);//L_code链表被建立
		MutiplyConst(L_code, &L_code_act, 0, VERSION_DATA[version][version_class][FIRSTPART_ALLDATA] - VERSION_DATA[version][version_class][FIRSTPART_DATA]);
		DestroyLinkList(&L_code);//删除表
		*(*code1 + i) = LinkMod(&L_code_act, &L_error, form, VERSION_DATA[version][version_class][FIRSTPART_ALLDATA] - VERSION_DATA[version][version_class][FIRSTPART_DATA]);
		DestroyLinkList(&L_code_act);//删除表
	}
	DestroyLinkList(&L_error);//删除表

	/*同理进行第二部分的操作，因为最多只有两类，有可能只有一种，所以第二部分直接为空即可*/
	if (VERSION_DATA[version][version_class][SECONDPART_NUM])
	{
		*code2 = (int**)malloc(VERSION_DATA[version][version_class][SECONDPART_NUM] * sizeof(int*));
		if (*code2 == NULL)
			return LOVERFLOW;
	}
	else
		*code2 = NULL;

	/*第二部分的纠错码*/
	if (*code2)
	{
		L_error = ProduceErrorP(VERSION_DATA[version][version_class][SECONDPART_ALLDATA] - VERSION_DATA[version][version_class][SECONDPART_DATA], form);//L_error链表被建立
		for (int i = 0; i<VERSION_DATA[version][version_class][SECONDPART_NUM]; i++)
		{
			L_code = ProduceCodeP(*(part2 + i), VERSION_DATA[version][version_class][SECONDPART_DATA]);//L_code链表被建立
			MutiplyConst(L_code, &L_code_act, 0, VERSION_DATA[version][version_class][FIRSTPART_ALLDATA] - VERSION_DATA[version][version_class][FIRSTPART_DATA]);
			DestroyLinkList(&L_code);//删除表
			*(*code2 + i) = LinkMod(&L_code_act, &L_error, form, VERSION_DATA[version][version_class][SECONDPART_ALLDATA] - VERSION_DATA[version][version_class][SECONDPART_DATA]);
			DestroyLinkList(&L_code);//删除表
		}
		DestroyLinkList(&L_error);//删除表
	}

	return OK;
}

/*释放掉码字信息，有二次申请*/
Status FreeInformation(int ***mazi, int length)
{
	if (*mazi == NULL)
		return OK;
	for (int i = 0; i < length; i++)
		free(*(*mazi + i));
	free(*mazi);
	*mazi = NULL;
	return OK;
}

/*进行多项式1和多项式2的取余*/
int *LinkMod(LinkList *L1, LinkList *L2, int *form, int code_length)
{
	int num_a, *code;
	LinkList L_con;

	//cout << (*L2)->next->expn << (*L1)->next->expn;
	for (; (*L2)->next->expn <= (*L1)->next->expn;)
	{
		/*找到被除多项式中第一个数字对应的a的次方表示形式*/
		for (num_a = 0; num_a < FORM_LENGTH; num_a++)
			if (form[num_a] == (*L1)->next->ceof)
				break;
		MutiplyConst(*L2, &L_con, num_a - (*L2)->next->ceof, (*L1)->next->expn - (*L2)->next->expn);//这个过程新建了一个L_con
		ConvertWithForm(&L_con, form);
		AddPolyn(L1, &L_con);//此过程L_con被销毁

		InsertInRange(L1, 0, 0);//主要是不清楚在取余的过程中是否会出现整除的情况，所以加上0的0次方，不影响之后的运算，以防止下一步判断出现问题
	}
	SuppleLinkList(L1, code_length);//因为系数为0则该项会被删除，但是纠错码位即使为0也不应该被删除，所以补上去

									/*将L1中的信息拿出来放起来*/
	code = (int*)malloc(code_length * sizeof(int));
	if (code == NULL)
		exit(1);
	LinkList p = (*L1)->next;
	for (int i = 0; i < code_length; i++)
	{
		code[i] = p->ceof;
		p = p->next;
	}

	return code;
}



/*--------------------------------------------以下为多项式的基本操作(GF(256))---------------------------------------*/

/* 初始化线性表 */
Status InitList(LinkList *L)
{
	/* 申请头结点空间，赋值给头指针 */
	*L = (LNode *)malloc(sizeof(LNode));
	if (*L == NULL)
		exit(LOVERFLOW);

	(*L)->next = NULL;
	return OK;
}

/*销毁链表*/
Status DestroyLinkList(LinkList*L)
{
	LinkList p = *L, q;//p指向头节点

	for (; p != NULL;)//依次释放，包括头节点
	{
		q = p->next;
		free(p);
		p = q;
	}

	*L = NULL;

	return OK;
}

/*按照指数从大到小的顺序插入元素，如果存在指数相同项，则考虑是相加还是忽略*/
Status InsertInRange(LinkList*L, int ceo, int ex,int method,int *form)
{
	if (*L == NULL)//如果头指针是NULL，那就返回
		return ERROR;

	LinkList p = (*L)->next, q = *L, s;//q指向头结点，p指向首元结点
	for (; p != NULL;)
		if (p->expn == ex || p->expn<ex)
			break;
		else if (p->expn > ex)
		{
			q = p;
			p = p->next;
		}

	/*申请一个结点插入*/
	if (p == NULL || p->expn<ex)
	{
		s = (LinkList)malloc(sizeof(LNode));
		if (s == NULL)
			return LOVERFLOW;
		s->ceof = ceo;
		s->expn = ex;
		q->next = s;
		s->next = p;
	}
	else if (method == ADD&&p!=NULL&&p->expn == ex)//注意这里的相加模式仅仅限于两个a的次方形式相加
	{
		/*首先转换模式*/
		int ceof1, ceof2;
		for (int i = 0; i < FORM_LENGTH; i++)
		{
			if (i == p->ceof)
				ceof1 = form[p->ceof];
			if (i == ceo)
				ceof2 = form[ceo];
		}
		ceof1 ^= ceof2;//相加
		/*转换回来*/
		for (int i = 0; i < FORM_LENGTH; i++)
			if (form[i] == ceof1)
			{
				p->ceof = i;
				break;
			}
	}

	return OK;
}

/*新的结点放在首元结点并且其指数为下一个结点的指数加一,即新插入的结点的指数为最大*/
Status InsertInRange(LinkList*L, int ceo)
{
	if (*L == NULL)//如果头指针是NULL，那就返回
		return ERROR;
	LinkList p = (*L)->next,s;//p指向首元结点

	s = (LinkList)malloc(sizeof(LNode));
	if (s == NULL)
		return LOVERFLOW;
	s->ceof = ceo;

	if (p == NULL)//如果首元结点不存在则为0次
		s->expn = 0;
	else
		s->expn = p->expn + 1;
	s->next = p;

	(*L)->next = s;

	return OK;
}

/*多项式的加法(GF(256))，同时销毁L2*/
Status AddPolyn(LinkList *L1, LinkList *L2)
{
	/*对于两个链表万一一个或者两个都是未建立的情况*/
	if (*L2 == NULL)
		return OK;
	else if (*L1 == NULL)
	{
		*L1 = *L2;
		*L2 = NULL;
		return OK;
	}

	LinkList p = *L1, q = (*L2)->next, s;
	for (; p->next&&q;)
	{
		if (p->next->expn == q->expn)
		{
			/*系数相加,在GF(256)中相当于取异或*/
			p->next->ceof ^= q->ceof;
			/*链表2移到下一个结点，当前结点被释放*/
			s = q;
			q = q->next;
			free(s);
			/*如果该项的系数为0，就没有存在的必要了*/
			if (p->next->ceof == 0)
			{
				s = p->next;
				p->next = s->next;
				free(s);
			}
		}
		else if (p->next->expn > q->expn)
			p = p->next;
		else if (p->next->expn < q->expn)
		{
			s = q;
			q = q->next;
			s->next = p->next;
			p->next = s;
			p = p->next;//p要向下移动一个单位
		}
	}

	/*如果最后是链表1先到头，那么，剩下的链表2的结点接上去*/
	if (!p->next)
		p->next = q;

	free(*L2);
	*L2 = NULL;

	return OK;
}

/*将多项式中各项的系数由a的次方形式转为int形式*/
Status ConvertWithForm(LinkList *L, int *form)
{
	if (*L == NULL)
		return ERROR;

	LinkList p = (*L)->next;//p指向首元
	for (; p; p = p->next)
	{
		if (p->ceof >= 255)//如果已经超过了255那么要进行变换到小于255
			p->ceof %= 255;
		p->ceof = form[p->ceof];
	}

	return OK;
}

/*将L1链表乘以一个只有一项的多项式并用L2新建一个链表存储其中,显然L的系数必须是a的次方形式*/
Status MutiplyConst(LinkList L1, LinkList *L2,const int a_num,const int a_expn)
{
	if (L1 == NULL)//如果L1还未建立，那么返回错误
		return ERROR;

	InitList(L2);
	LinkList p = L1->next, q = *L2,s;//p指向L1首元结点,q指向L2的头节点
	for (; p;)
	{
		s = (LinkList)malloc(sizeof(LNode));
		if (s == NULL)
			return LOVERFLOW;
		s->ceof = p->ceof + a_num;
		s->expn = p->expn + a_expn;
		q->next = s;
		q = q->next;
		p = p->next;
	}
	q->next = NULL;

	return OK;
}

/*将链表s_expn指数以下的所有项补齐，缺少，则其系数为0*/
Status SuppleLinkList(LinkList *L, int s_expn)
{
	/*链表未建立*/
	if (*L == NULL)
		return ERROR;

	for(int i=0;i<s_expn;i++)
	    InsertInRange(L, 0, i);

	return OK;
}

/*将多项式L1和多项式L2相乘(GF(256))，最后L2被释放，注意：系数必须为a的次方形式*/
Status MutiplyP(LinkList *L1, LinkList *L2, int *form)
{
	if (!(*L1&&*L2))//如果L1或者L2中的一个未不存在的链表则返回
		return ERROR;

	LinkList L3, r;
	InitList(&L3);
	int ceo;

	for (LinkList p = (*L2)->next; p;)
	{
		for (LinkList q = (*L1)->next; q; q = q->next)
		{
			ceo = p->ceof + q->ceof;//确保a的次方小于255
			if (ceo >= 255)
				ceo %= 255;
			InsertInRange(&L3, ceo, p->expn + q->expn, ADD, form);
		}
		r = p;
		p = p->next;
		free(r);
	}
	/*这样生成的L3为按照指数从大到小的顺序排列*/

	free(*L2);//L2被释放
	*L2 = NULL;
	DestroyLinkList(L1);//L1被释放
	*L1 = L3;//头指针更改

	return OK;
}

/*传入纠错码字的个数，返回应该使用的生成多项式*/
LinkList ProduceErrorP(int code_num, int *form)//注意，在使用完成多项式之后应该释放该多项式，尽管在使用之前并没有使用InitList
{
	LinkList L1, L2;
	InitList(&L1);
	InsertInRange(&L1, 0);
	InsertInRange(&L1, 0);
	for (int i = 1; i<code_num; i++)
	{
		InitList(&L2);
		InsertInRange(&L2, i);
		InsertInRange(&L2, 0);
		MutiplyP(&L1, &L2, form);//此过程L2被释放掉，乘法的结果在L1中
	}
	return L1;//L1本来就是一个指针，所以应该是没有问题的，以上用到地址的地方均使用在此函数中的自动变量地址，无影响
}

/*传入一段整型数据，返回一个用整型数据组成的多项式*/
LinkList ProduceCodeP(int *code, int length)
{
	LinkList L;
	InitList(&L);
	for (int i = length - 1; i >= 0; i--)
		InsertInRange(&L, *(code + i));

	return L;//同上一个函数
}



/*--------------------------------------------以下为填充方格的函数------------------------------------------------*/

/*申请一个这样大的方格空间*/
Status CreateSquare(Square&square, const int size)//size=21 + (version - 1) * 4
{
	square = (bool**)malloc(size*sizeof(bool*));
	if (square == NULL)
		return LOVERFLOW;
	for (int i = 0; i < size; i++)
	{
		*(square + i) = (bool*)calloc(size , sizeof(bool));
		if (*(square + i) == NULL)
			return LOVERFLOW;
	}

	return OK;
}

/*释放掉一个方格*/
Status FreeInformation(Square&square, const int size)
{
	if (square == NULL)
		return OK;
	for (int i = 0; i < size; i++)
		free(*(square + i));
	free(square);
	square = NULL;

	return OK;
}

/*在三个角上画上(3，5，7)图形*/
Status FillThreeSquare(Square&square, Square&mark, const int size)
{
	FillInSquare(square, 3, 3, 3, 1);
	FillInSquare(square, 3, 3, 1, 1);
	FillInSquare(square, 3, 3, 0, 1);
	FillInSquare(square, size-4, 3, 3, 1);
	FillInSquare(square, size-4, 3, 1, 1);
	FillInSquare(square, size-4, 3, 0, 1);
	FillInSquare(square, 3, size-4, 3, 1);
	FillInSquare(square, 3, size-4, 1, 1);
	FillInSquare(square, 3, size-4, 0, 1);

	/*在mark上标出*/
	FillInSquare(mark, 3, 3, 3, 1, 1);
	FillInSquare(mark, size - 4, 3, 3, 1, 1);
	FillInSquare(mark, 3, size - 4, 3, 1, 1);

	/*将三个方框和其他部分中间的白线在标上*/
	for (int i = 0; i < 8; i++)
	{
		mark[i][7] = 1;
		mark[i][size-8] = 1;
	}
	for (int j = 0; j < 8; j++)
	{
		mark[7][j] = 1;
		mark[size - 8][j] = 1;
	}

	for (int i = size - 8; i < size; i++)
		mark[i][7] = 1;
	for (int j = size - 8; j < size; j++)
		mark[7][j] = 1;
	return OK;
}

/*以一个点为中心，在半径为r的正方形内改变为相应的值,模式表示是实心还是空心,0表示空心，1表示实心*/
Status FillInSquare(Square&square,const int x,const int y, const int r, const bool value,int mode)
{
	if (mode==1)
		for (int i = -r; i <= r; i++)
			for (int j = -r; j <= r; j++)
				square[x + i][y + j] = value;
	else if(mode==0)
		for (int i = -r; i <= r; i++)
			for (int j = -r; j <= r; j++)
				if (i == r || i == -r || j == r || j == -r)
					square[x + i][y + j] = value;

	return OK;
}

/*把连接三个方框的分割线画出来*/
Status FillLine(Square&square, Square&mark,const int size)
{
	for (int i = 8; i<size-8; i++)
	{
		square[i][6] = ((i + 1) % 2 == 0 ? 0 : 1);
		mark[i][6] = 1;
	}
	for (int j = 8; j<size-8; j++)
	{
		square[6][j] = ((j + 1) % 2 == 0 ? 0 : 1);
		mark[6][j] = 1;
	}

	return OK;
}

/*把校正图形画出来*/
Status FillAlignment(Square&square, Square&mark, const int version,const int size)
{
	int x, y;
	for(int i=0;i<7&&ALIGNMENT_LOCATION[i][version]!=0;i++)
		for (int j = 0; j < 7 && ALIGNMENT_LOCATION[j][version] != 0; j++)
		{
			x = ALIGNMENT_LOCATION[i][version];
			y = ALIGNMENT_LOCATION[j][version];
			if (!(x - 2 <= 7 && y - 2 <= 7) && !(x - 2 <= 7 && y + 2 >= size - 8) && !(x + 2 >= size - 8 && y - 2 <= 7))
			{
				FillInSquare(square, x, y, 0, 1);
				FillInSquare(square, x, y, 2, 1);
				FillInSquare(mark, x, y, 2, 1, 1);
			}
		}

	return OK;
}

/*把版本信息加进去*/
Status FillVersionInf(Square&square, Square&mark, bool*version_inf,const int size)
{
	for (int i = 0; i < 6; i++)
    	for(int j=size-11;j<=size-9;j++)
		{
			square[i][j] = version_inf[i * 3 + j - size + 11];
			mark[i][j] = 1;
		}

	for(int j=0;j<6;j++)
		for (int i = size - 11; i <= size - 9; i++)
		{
			square[i][j] = version_inf[j * 3 + i - size + 11];
			mark[i][j] = 1;
		}

	return OK;
}

/*把格式信息加进去*/
Status FillFormInf(Square&square, Square&mark, bool*form_inf,const int size)
{
	for (int i = 0; i < 6; i++)
	{
		square[i][8] = form_inf[i];
		mark[i][8] = 1;
	}
	square[7][8] = form_inf[6];
	mark[7][8] = 1;
	square[8][8] = form_inf[7];
	mark[8][8] = 1;
	square[8][7] = form_inf[8];
	mark[8][7] = 1;
	for (int j = 5; j >= 0; j--)
	{
		square[8][j] = form_inf[9 + 5 - j];
		mark[8][j] = 1;
	}

	for (int j = 0; j < 8; j++)
	{
		square[8][size - 1 - j] = form_inf[j];
		mark[8][size - 1 - j] = 1;
	}
	square[size - 8][8] = 1;
	mark[size - 8][8] = 1;
	for (int i = 8; i < 15; i++)
	{
		square[size - 7 + i - 8][8] = form_inf[i];
		mark[size - 7 + i - 8][8] = 1;
	}
	return OK;
}

/*将码字信息加进去*/
Status FillCodeInf(Square&square, Square&mark, bool*code_inf,const int size,const int num)
{
	int count = 0;
	for (int j = size - 1; j >= 1; j -= 2)//列从右边往左边移动，每次移动两个单位。
	{
		if (j == 6)//如果移动到那条（连接两个正方形边框的那条线）的话，额外移动一个格
			j--;
		if ((size - 1 - j) / 2 % 2)//如果说现在是j减少了奇数次的话，应该是从上往下移动
			for (int i = 0; i < size; i++)
			{
				/*先右后左的顺序*/
				if (!mark[i][j])
				{
					square[i][j] = code_inf[count];
					count++;
					if (count == num)
						return OK;
				}
				if (!mark[i][j - 1])
				{
					square[i][j - 1] = code_inf[count];
					count++;
					if (count == num)
						return OK;
				}
			}
		else//偶数次则是从下往上移动
			for (int i = size - 1; i >= 0; i--)
			{
				/*先右后左的顺序*/
				if (!mark[i][j])
				{
					square[i][j] = code_inf[count];
					count++;
					if (count == num)
						return OK;
				}
				if (!mark[i][j - 1])
				{
					square[i][j - 1] = code_inf[count];
					count++;
					if (count == num)
						return OK;
				}
			}
	}
	/*如果有剩下的位置本来是使用calloc申请的，所以已经自动填写为0*/

	return OK;
}

/*加上掩膜信息*/
Status HideMode(Square&square, Square&mark, int hide_mode,int size)
{
    if (hide_mode == 0)
	{
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				if ((i + j) % 2 == 0 && mark[i][j] == 0)
					square[i][j] = !square[i][j];
	}
	else if (hide_mode == 1)
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
			{
				if (i % 2 == 0 && mark[i][j] == 0)
					square[i][j] = !square[i][j];
			}
	else if (hide_mode == 2)
	{
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				if (j % 3 == 0 && mark[i][j] == 0)
					square[i][j] = !square[i][j];
	}
	else if (hide_mode == 3)
	{
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				if ((i + j) % 3 == 0 && mark[i][j] == 0)
					square[i][j] = !square[i][j];
	}
	else if (hide_mode == 4)
	{
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				if ((i / 2 + j  / 3) % 2 == 0 && mark[i][j] == 0)
					square[i][j] = !square[i][j];
	}
	else if (hide_mode == 5)
	{
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
			{
				if (((i * j) % 2 + (i *j) % 3 == 0) && mark[i][j] == 0)
					square[i][j] = !square[i][j];
			}
	}
	else if (hide_mode == 6)
	{
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
			{
				if (((i * j) % 2 + (i * j) % 3) % 2 == 0 && mark[i][j] == 0)
					square[i][j] = !square[i][j];
			}
	}
	else if (hide_mode == 7)
	{
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
			{
				if (((i  + j ) % 2 + (i * j) % 3) % 2 == 0 && mark[i][j] == 0)
					square[i][j] = !square[i][j];
			}
	}
	return OK;
}

/*计算当前方格的罚分状况*/
int ScoreOfMode(Square&square,const int size)
{
	int score = 0, num_dark = 0;
	/*第一个罚分，连续五个以上的同色块*/
	for (int i = 0; i < size; i++)
		for (int j = 0, num_same = 1, init_col = 2; j < size; j++)
			if (int(square[i][j]) == init_col)
			{				
				num_same++;
				if (num_same == 5)
					score += 3;
				else if (num_same > 5)
					score++;
			}
			else
			{
				init_col = square[i][j];
				num_same = 1;
			}
	for (int j = 0; j < size; j++)
		for (int i = 0, num_same = 1, init_col = 2; i < size; i++)
			if (int(square[i][j]) == init_col)
			{
				num_same++;
				if (num_same == 5)
					score += 3;
				else if (num_same > 5)
					score++;
			}
			else
			{
				init_col = square[i][j];
				num_same = 1;
			}
	/*第二个罚分*/
	for (int i = 0; i < size - 1; i++)
		for (int j = 0; j < size - 1; j++)
			if (square[i][j] == square[i][j + 1] && square[i][j] == square[i + 1][j] && square[i][j] == square[i + 1][j + 1])
				score += 3;
	/*第三部分罚分*/
	for (int i = 0; i < size; i++)
		for (int j = 0, num_same = 1; j < size; j++)
			if (square[i][j] == 0)
			{
				num_same++;
				if (num_same == 4)
				{
					if (j + 7 < size&&square[i][j + 1] == 1 && square[i][j + 2] == 0 && square[i][j + 3] == 1 && square[i][j + 4] == 1 && square[i][j + 5] == 1 && square[i][j + 6] == 0 && square[i][j + 7] == 1)
						score += 40;
					if (j - 10 >= 0&&square[i][j - 4] == 1 && square[i][j - 5] == 0 && square[i][j - 6] == 1 && square[i][j - 7] == 1 && square[i][j - 8] == 1 && square[i][j - 9] == 0 && square[i][j - 10] == 1)
						score += 40;	
					j -= 3;
					num_same = 0;
				}
			}
			else
				num_same = 0;
	for (int j = 0; j < size; j++)
		for (int i = 0, num_same = 1; i < size; i++)
			if (square[i][j] == 0)
			{
				num_same++;
				if (num_same == 4)
				{
					if (i + 7 < size&&square[i + 1][j] == 1 && square[i + 2][j] == 0 && square[i + 3][j] == 1 && square[i + 4][j] == 1 && square[i + 5][j] == 1 && square[i + 6][j] == 0 && square[i + 7][j] == 1)
						score += 40;
					if (i - 10 >= 0 && square[i - 4][j] == 1 && square[i - 5][j] == 0 && square[i - 6][j] == 1 && square[i - 7][j] == 1 && square[i - 8][j] == 1 && square[i - 9][j] == 0 && square[i - 10][j] == 1)
						score += 40;
					i -= 3;
					num_same = 0;
				}
			}
			else
				num_same = 0;
	/*第四部分罚分*/
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			if (square[i][j] == 1)
				num_dark++;
	int ratial = num_dark * 100 / (size*size);
	if (ratial >= 50)
		score += ((ratial - 50) / 5 * 10);
	else if (ratial < 50)
		score += ((50 - ratial) / 5 * 10);

	return score;
}

/*打印最终二维码（黑白最终版）*/
void FinalPrint(Square&square, const int size)
{
	const HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE); //取标准输出设备对应的句柄
	const HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);  //取标准输入设备对应的句柄
	for (int i = 0; i < size + 8; i++)
		for (int j = 0; j < size + 8; j++)
			if (i < 4 || i >= size + 4 || j < 4 || j >= size + 4)
				showch(hout, j * 2, i, ' ', COLOR_HWHITE, COLOR_BLACK, 2);
			else
				showch(hout, j * 2, i, ' ', square[i - 4][j - 4] == 0 ? COLOR_HWHITE : COLOR_BLACK, COLOR_BLACK, 2);
	setcolor(hout, COLOR_BLACK, COLOR_WHITE);
}

/*------------------------------以下为二进制数据的计算--------------------------------------*/

/*计算版本信息，总计18位*/
void VersionCal(int version,bool*version_inf)
{
	int _version = version,version_cal;
	_version <<= 12;
	for (; BitNum(_version) >= BitNum(VERSION_CAL);)
	{
		version_cal = VERSION_CAL << (BitNum(_version) - BitNum(VERSION_CAL));
		_version ^= version_cal;
	}
	for (int i = 0; i < 12; i++)
	{
		version_inf[i] = (_version % 2 == 0 ? 0 : 1);
		_version /= 2;
	}
	for (int i = 12; i < 18; i++)
	{
		version_inf[i] = (version % 2 == 0 ? 0 : 1);
		version /= 2;
	}
}

/*计算格式信息，总计15位*/
void FormCal(VERSION_CLASS version_class, int hide_mode, bool*form_inf)
{
	int data_ori,data_cor,form_cal;

	if (version_class == VERSION_L)
		data_ori = 1;
	else if (version_class == VERSION_M)
		data_ori = 0;
	else if (version_class == VERSION_Q)
		data_ori = 3;
	else if (version_class == VERSION_H)
		data_ori = 2;
	data_ori <<= 3;
	data_ori |= hide_mode;
	data_cor = data_ori << 10;
	for (; BitNum(data_cor) >= BitNum(FORM_CAL);)
	{
		form_cal = FORM_CAL << (BitNum(data_cor) - BitNum(FORM_CAL));
		data_cor ^= form_cal;
	}
	data_ori <<= 10;
	data_ori |= data_cor;
	data_ori ^= FORM_INFORMATION;
	for (int i = 0; i < 15; i++)
	{
		form_inf[i] = (data_ori % 2 == 0 ? 0 : 1);
		data_ori /= 2;
	}
}

/*计算码字的二进制信息*/
bool *CodeCal(int **part1, int **part2, int **code1, int **code2,int version,VERSION_CLASS version_class)
{
	bool *code_inf;
	code_inf = (bool*)malloc((VERSION_DATA[version][version_class][FIRSTPART_NUM]* VERSION_DATA[version][version_class][FIRSTPART_ALLDATA]
		+ VERSION_DATA[version][version_class][SECONDPART_NUM] * VERSION_DATA[version][version_class][SECONDPART_ALLDATA]) * 8 * sizeof(bool));
	if (code_inf == NULL)
		exit(1);

	int locate = 0,
		num1 = VERSION_DATA[version][version_class][FIRSTPART_DATA],
		num2 = VERSION_DATA[version][version_class][SECONDPART_DATA],
		num1_num = VERSION_DATA[version][version_class][FIRSTPART_NUM],
		num2_num = VERSION_DATA[version][version_class][SECONDPART_NUM];
	/*放置数据码字*/
	for (int j = 0; j<(num1>num2 ? num1 : num2); j++)
		for (int i = 0; i<num1_num + num2_num; i++)
		{
			if (j >= num1 && i < num1_num)
			{
				i = num1_num - 1;
				continue;
			}
			if (j >= num2 && i >= num1_num)
				break;
			if (i < num1_num)
				Fangzhi(code_inf, part1[i][j], locate, locate + 8);
			else
				Fangzhi(code_inf, part2[i - num1_num][j], locate, locate + 8);
			locate += 8;
		}
	/*放置纠正码字*/
	num1 = VERSION_DATA[version][version_class][FIRSTPART_ALLDATA] - VERSION_DATA[version][version_class][FIRSTPART_DATA],
		num2 = VERSION_DATA[version][version_class][SECONDPART_ALLDATA] - VERSION_DATA[version][version_class][SECONDPART_DATA];
	for (int j = 0; j<(num1>num2 ?num1:num2); j++)
		for (int i = 0; i<num1_num + num2_num; i++)
		{
			if (j >= num1 && i < num1_num)
			{
				i = num1_num - 1;
				continue;
			}
			if (j >= num2 && i >= num1_num)
				break;
			if (i < num1_num)
				Fangzhi(code_inf, code1[i][j], locate, locate + 8);
			else
				Fangzhi(code_inf, code2[i - num1_num][j], locate, locate + 8);
			locate += 8;
		}

	return code_inf;
}

/*-----------------------------------其他-----------------------------------------*/

/*求num的二进制有几位*/
int BitNum(int num)
{
	int bit_num = 0;

	for (; num;)
	{
		bit_num++;
		num /= 2;
	}

	return bit_num;
}

/*生成对照表*/
Status CreateForm(int *form)//表长为255
{
	form[0] = 1;
	for (int i = 1; i < FORM_LENGTH; i++)
	{
		form[i] = 2 * form[i - 1];
		if (form[i] > FORM_LENGTH)
			form[i] ^= AND_XOR;
	}

	return OK;
}