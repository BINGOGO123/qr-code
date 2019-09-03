/*1652241 갺��� �����1��*/
#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include"90-b4.h"
#include"cmd_console_tools.h"
#include<stdlib.h>
#include<conio.h>
using namespace std;

/*-------------------------------------------------��ȡ��Ϣ----------------------------------------------------*/

/*������ɶ�ά��ĳ�ʼ��Ϣ*/
char *GetInformation(unsigned int &char_num)
{
	char *xinxi;
	int i = 0;
	char_num = 10;

	/*����һ���ʼ��СΪ10�Ŀռ�*/
	xinxi = (char*)malloc(char_num * sizeof(char));
	if (xinxi == NULL)
		exit(1);

	/*������Ϣ*/
	cout << "�����뽫Ҫ���ɵĶ�ά����Ϣ(1-40�汾����,������Ҫ�������ݶ���)��" << endl;
	for (; xinxi[i - 1] != '\n'||i - 1 == 0; i++)
	{
		if (i == char_num)
		{
			char_num += 10;
			/*����������еĳ�����Ҫ�ӳ�*/
			xinxi = (char*)realloc(xinxi, char_num * sizeof(char));
			if (xinxi == NULL)
				exit(1);
		}
		xinxi[i] = getchar();
	}
	xinxi[i - 1] = '\0';//���س����ǵ�

	return xinxi;
}

/*�ͷ����ɶ�ά��ĳ�ʼ ��Ϣ*/
Status FreeInformation(char *xinxi)
{
	free(xinxi);

	return OK;
}

/*��ȡ��ά��ľ���ȼ�*/
VERSION_CLASS CorrectClass()
{
	for (;;)
	{
		cout << "��������Ҫ����ĵȼ�(L,M,Q,H���������������������洢��ͬ���ݿ���Ҫʹ�ø���İ汾)\n";
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
			cout << "�������\n";
	}
}

/*�ж�Ӧ��ʹ���ĸ��汾*/
int JudgeVersion(char *xinxi, VERSION_CLASS version_class)
{
	int i = 1;
	for (; i <= 40; i++)
		if ((VERSION_DATA[i][version_class][FIRSTPART_DATA] * VERSION_DATA[i][version_class][FIRSTPART_NUM]
			+ VERSION_DATA[i][version_class][SECONDPART_DATA] * VERSION_DATA[i][version_class][SECONDPART_NUM]
			- (VERSION_COUNT[version_class][i] + 4 + 8) / 8) >= int(strlen(xinxi)))//̫����
			break;
	return (i <= 40 ? i : 0);
}

/*ǿ�Ƹ��İ汾��*/
void ChangeVersion(int &version)
{
	cout << "����������ǿ�Ƹ��İ汾��С��"<<version<<"-40�����Ƿ���ģ���Y/N��\n";
	for (char a;;)
	{
		a = _getch();
		if (a == 'Y' || a == 'y')
		{
			int version_linshi;
			cout << "������汾�ţ�\n";
			cin >> version_linshi;
			if (version_linshi < version || version_linshi>40)
				cout << "���İ汾��Ϣʧ�ܣ�ԭ���ڰ汾��Χ�ڣ�\n";
			else
			{
				cout << "�Ӱ汾[" << version << "]���ĵ��汾[" << version_linshi << "]�ɹ�!\n";
				version = version_linshi;
			}
			break;
		}
		else if (a == 'N' || a == 'n')
		{
			cout << "�����İ汾��Ϣ��\n";
			break;
		}
	}
}

/*��ȡ���ں��ֵı��뷽ʽ,ֻҪ���к�����ô��ʹ��utf-8�ķ�ʽ����*/
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

/*-------------------------------------------------���ַ�������--------------------------------------------------*/


/*���ַ�����Ϣ����Ϊ��������Ϣ��ԭ������ѡ��ʹ��gb2312�ķ�ʽ���뺺�֣�*/
/*bool* ProduceStorageCode(char *xinxi, const int version, int &length,const int method)
{
	//����ѡ��Ҫ�ú��ַ�ʽ���룬�����ȫ���������֣����������ģʽ�������������������ĸ������ĸ���֣����������ĸ���ַ�Χ����ַ�
	//��ʹ���ֽڣ�����ʹ�ú��ֵķ���
	//��Щ��������ĸģʽ�е�����Ÿ��ַ�
	char code_apl[9] = { ' ','$','%','*','+','-','.','/',':' };
	//���ڴ�Ŷ���������
	bool*shujuwei;
	//���ﶨ���˻��״̬����ʼ��ֹ���Լ�ģʽѡ��
	int mode_end = 0, mode_start = 0;
	bool mode_num = TRUE, mode_alp = FALSE, mode_byte = FALSE;
	int  count_cur = 0;
	//תutf-8
	char *utf_s;

	for (length=0;; mode_num = TRUE, mode_alp = FALSE, mode_byte = FALSE)
	{
		//���ʹ��gb312���ȵ�����λ��,��ֹ������Ǻ��ֻ�����û���ˣ����ʹ��utf-8��ȫ������
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
			//�ж���һ��ʹ��ʲôģʽ
			for (int j = mode_start; j < mode_end; j++)
				if (!(xinxi[j] >= '0' && xinxi[j] <= '9'))//����в���0-9��Χ�ڵ��ַ�����ô��Ȼ����ʹ������ģʽ�洢����ʱ���ݶ�ʹ��������ĸģʽ
				{
					mode_alp = TRUE;
					if (!(xinxi[j] >= 'A'&&xinxi[j] <= 'Z'))//���ݶ�ʹ��������ĸģʽ��ǰ���£�����в���A-Z֮����ַ�
					{
						int z = 0;
						for (; z < 9; z++)//�жϸ��ַ��Ƿ���������ĸģʽ�������ľŸ��ַ�
							if (code_apl[z] == xinxi[j])//����ǣ���ô����ֹѭ��
								break;
						if (z == 9)//����������ϾŸ��ַ�֮һ����ô�����ֽ�ģʽ����ֹѭ��
						{
							mode_byte = TRUE;
							break;
						}
					}//�ж��Ƿ�����������ģģʽ��Χ
				}//�ж��Ƿ���������ģʽ��Χ

		   //����Ͷ�mode_start��mode_end��Χ�ڵ��ַ�����
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
		//����Ϊ���ֱ���Ĺ���
		ProcessFormHanzi(xinxi, &shujuwei, mode_start, mode_end,length, count_cur, version);
		mode_start = mode_end;
	}

	if (method == 1)
		delete xinxi;//ע���������Ϣ��ת��utf_8����Ϣ������ֻ���β�ֵ�ĸı䣬������ԭ����
	return shujuwei;
}*/

/*���ַ�����Ϣ����Ϊ��������Ϣ*/
bool* ProduceStorageCode(char *xinxi, const int version, int &length, const int method)
{
	/*����к��֣���ôֱ��ʹ��utf-8�������жϾ���ʹ���ֽڻ������ֻ���������ĸ*/
	/*��Щ��������ĸģʽ�е�����Ÿ��ַ�*/
	char code_apl[9] = { ' ','$','%','*','+','-','.','/',':' };
	/*���ڴ�Ŷ���������*/
	bool*shujuwei=NULL;
	/*���ﶨ���˻��״̬����ʼ��ֹ���Լ�ģʽѡ��*/
	int mode_end = 0, mode_start = 0;
	bool mode_num = TRUE, mode_alp = FALSE, mode_byte = FALSE;
	int  count_cur = 0;
	/*תutf-8*/
	char *utf_s;

	length = 0;
	/*���ʹ��gb312���ȵ�����λ��,��ֹ������Ǻ��ֻ�����û���ˣ����ʹ��utf-8��ȫ������*/
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
		/*�ж���һ��ʹ��ʲôģʽ*/
		for (int j = mode_start; j < mode_end; j++)
			if (!(xinxi[j] >= '0' && xinxi[j] <= '9'))//����в���0-9��Χ�ڵ��ַ�����ô��Ȼ����ʹ������ģʽ�洢����ʱ���ݶ�ʹ��������ĸģʽ
			{
				mode_alp = TRUE;
				if (!(xinxi[j] >= 'A'&&xinxi[j] <= 'Z'))//���ݶ�ʹ��������ĸģʽ��ǰ���£�����в���A-Z֮����ַ�
				{
					int z = 0;
					for (; z < 9; z++)//�жϸ��ַ��Ƿ���������ĸģʽ�������ľŸ��ַ�
						if (code_apl[z] == xinxi[j])//����ǣ���ô����ֹѭ��
							break;
					if (z == 9)//����������ϾŸ��ַ�֮һ����ô�����ֽ�ģʽ����ֹѭ��
					{
						mode_byte = TRUE;
						break;
					}
				}//�ж��Ƿ�����������ģģʽ��Χ
			}//�ж��Ƿ���������ģʽ��Χ
	}
    /*����Ͷ�mode_start��mode_end��Χ�ڵ��ַ�����*/
	if (mode_byte == TRUE)
		ProcessFormByte(xinxi, &shujuwei, mode_start, mode_end, length, count_cur, version);
	else if (mode_alp == TRUE)
		ProcessFormAlp(xinxi, &shujuwei, mode_start, mode_end, length, count_cur, version);
	else if (mode_num == TRUE)
		ProcessFormNum(xinxi, &shujuwei, mode_start, mode_end, length, count_cur, version);

	if (method == 1)
		delete xinxi;//ע���������Ϣ��ת��utf_8����Ϣ������ֻ���β�ֵ�ĸı䣬������ԭ����
	return shujuwei;
}

/*�ͷ����ɶ�ά��Ķ�������Ϣ*/
Status FreeInformation(bool *shujuwei)
{
	free(shujuwei);

	return OK;
}

/*������ĸ��ϢתΪ������*/
void ProcessFormAlp(char *xinxi, bool **shujuwei, int mode_start, int mode_end,int &length,int &count_cur,const int version)
{
	/*�ӳ�����Ŀռ�*/
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

	/*������ĸģʽ*/
	(*shujuwei)[count_cur + 0] = (*shujuwei)[count_cur + 1] = (*shujuwei)[count_cur + 3] = 0;
	(*shujuwei)[count_cur + 2] = 1;

	/*������ĸģʽ�ļ����ַ�λ��*/
	Fangzhi(*shujuwei, mode_end - mode_start, count_cur + 4, start);

	/*���ַ�λ�Ž�ȥ*/
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

	/*����ʣ����������ֻ���һ�����ֽ��д���*/
	if((count - mode_start) % 2 == 1)
	    Fangzhi(*shujuwei, linshi, start + (count - mode_start) / 2 * 11, start + (count - mode_start) / 2 * 11 + 6);
	count_cur = length;
}

/*�ֽڷ�ʽתΪ������*/
void ProcessFormByte(char *xinxi, bool **shujuwei, int mode_start, int mode_end, int &length, int &count_cur, const int version)
{
	/*�ӳ��ռ�*/
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

	/*�ֽ�ģʽ*/
	(*shujuwei)[count_cur + 0] = (*shujuwei)[count_cur + 2] = (*shujuwei)[count_cur + 3] = 0;
	(*shujuwei)[count_cur + 1] = 1;

	/*�ֽ�ģʽ�ļ����ַ�λ��*/
	Fangzhi(*shujuwei, mode_end - mode_start, count_cur + 4, start);

	/*���ַ�λ�Ž�ȥ*/
	for (count = mode_start; count<mode_end; count++)
	{
		linshi = int(unsigned char(xinxi[count]));
		Fangzhi(*shujuwei, linshi, start + (count - mode_start)  * 8, start + (count - mode_start) * 8 + 8);
	}
	count_cur = length;
}

/*�������ı���Ϣת��Ϊ�����ƴ洢����*/
void ProcessFormNum(char *xinxi, bool **shujuwei, int mode_start, int mode_end, int &length, int &count_cur, const int version)
{
	/*�ӳ��ռ�*/
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
	/*����ģʽ*/
	(*shujuwei)[count_cur + 0] = (*shujuwei)[count_cur + 1] = (*shujuwei)[count_cur + 2] = 0;
	(*shujuwei)[count_cur + 3] = 1;

	/*����ģʽ�ļ����ַ�λ��*/
	Fangzhi(*shujuwei, mode_end - mode_start, count_cur + 4, start);

	/*������λ�Ž�ȥ*/
	for (count = mode_start; count<mode_end; count++)
	{
		linshi = linshi * 10 + *(xinxi + count) - 48;
		if ((count-mode_start) % 3 == 2)
		{
			Fangzhi(*shujuwei, linshi, start + (count-mode_start) / 3 * 10, start + (count - mode_start) / 3 * 10 + 10);
			linshi = 0;
		}
	}

	/*����ʣ����������ֻ���һ�����ֽ��д���*/
	Fangzhi(*shujuwei, linshi, start + (count - mode_start) / 3 * 10, start + (count - mode_start) / 3 * 10 + ((count - mode_start) % 3 + 1) / 2 + (count - mode_start) % 3 * 3);

	count_cur = length;
}

/*��������Ϣת��Ϊ�����ƴ洢������gb2312��*/
/*void ProcessFormHanzi(char *xinxi, bool **shujuwei, int mode_start, int mode_end, int &length, int &count_cur, const int version)
{
	//����ռ�
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

	//����ģʽ
	(*shujuwei)[count_cur + 0] = (*shujuwei)[count_cur + 1] = (*shujuwei)[count_cur + 3] = (*shujuwei)[count_cur + 7] = 1;
	(*shujuwei)[count_cur + 2] = (*shujuwei)[count_cur + 4] = (*shujuwei)[count_cur + 5] = (*shujuwei)[count_cur + 6] = 0;

	//����ģʽ�ļ����ַ�λ��
	Fangzhi(*shujuwei, (mode_end - mode_start) / 2, count_cur + 4 + 4, start);

	//���ַ�λ�Ž�ȥ
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

/*�ڽ�shuziת��Ϊ�����ƣ�����shujuwei�е���Ӧλ�÷���ȥ������locate_endΪ��ֹ�ĵط�����λ�ò�������*/
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

/*������λ���룬���ж��Ƿ񳬳���ǰ�汾���ơ�����֮ǰ�ж��ܷ�װ�£�������Ȼ�п��ܳ���*/
Status MakeUp(bool**shujuwei,const int length, int version, VERSION_CLASS version_class)
{
	int total_length = (VERSION_DATA[version][version_class][FIRSTPART_NUM] * VERSION_DATA[version][version_class][FIRSTPART_DATA]
		+ VERSION_DATA[version][version_class][SECONDPART_NUM] * VERSION_DATA[version][version_class][SECONDPART_DATA]) * 8;
	if (length <= total_length)
	{
		*shujuwei = (bool*)realloc(*shujuwei, total_length);//����ʹ�õĴ�����ݵĳ���
		if (*shujuwei == NULL)
			return LOVERFLOW;
		int i = 0;
		for (; (i + length < total_length) && i < 4; i++)//4���������ֹ
			(*shujuwei)[length + i] = 0;
		if (i + length != total_length)//����ʣ�µİ�λ
			for (; (i + length) % 8 != 0; i++)
				(*shujuwei)[i + length] = 0;
		if (i + length != total_length)//ʣ�����ְ��ձ�׼ֵ
			for (int j = 0; j + i + length < total_length; j++)
				(*shujuwei)[j + i + length] = COVER[j % 16];
	}
	else
		return ERROR;

	return OK;
}

/*-------------------------------------------���ɾ�����Ĳ���----------------------------------------------------*/

/*�����ɵĶ���������תΪһ���������֡�*/
Status ConvertMazi(bool*shujuwei, int version,VERSION_CLASS version_class, int ***mazi_part1,int ***mazi_part2)
{
	/*��������һ������Ϊ��һ���ִ�С�Ķ���ָ��*/
	*mazi_part1 = (int**)malloc(VERSION_DATA[version][version_class][FIRSTPART_NUM]*sizeof(int*));
	if (*mazi_part1 == NULL)
		return LOVERFLOW;
	/*������ÿһ���ִ�С��int�Ϳռ䣬�൱�ڽ�ԭ�������ַָ�Ϊ��ô��Ĳ���*/
	for (int i = 0; i < VERSION_DATA[version][version_class][FIRSTPART_NUM]; i++)
	{
		*(*mazi_part1+i) = (int*)calloc(VERSION_DATA[version][version_class][FIRSTPART_DATA],sizeof(int));
		if (**mazi_part1 == NULL)
			return LOVERFLOW;
	}

	/*ͬ����еڶ����ֵĲ�������Ϊ���ֻ�����࣬�п���ֻ��һ�֣����Եڶ�����ֱ��Ϊ�ռ���*/
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
	
	/*���ˣ����ڷŵ�һ���ֵļ�������*/
	int i = 0;
	for (int j = 0; j < VERSION_DATA[version][version_class][FIRSTPART_NUM]; i++)
	{
		int linshi = shujuwei[i];
		linshi <<= (7 - i % 8);
		(*mazi_part1)[j][(i / 8) % VERSION_DATA[version][version_class][FIRSTPART_DATA]] |= linshi;

		if ((i + 1) % (VERSION_DATA[version][version_class][FIRSTPART_DATA] * 8) == 0)
			j++;
	}

	/*�ŵڶ����ֵļ�������*/
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

/*����������*/
Status CorrectCode(int **part1, int **part2, int version, VERSION_CLASS version_class, int *form, int ***code1, int ***code2)
{
	LinkList L_error, L_code, L_code_act;

	/*����һ������Ϊ��һ���ִ�С�Ķ���ָ��*/
	*code1 = (int**)malloc(VERSION_DATA[version][version_class][FIRSTPART_NUM] * sizeof(int*));
	if (*code1 == NULL)
		return LOVERFLOW;

	/*��һ���ֵľ�����*/
	L_error = ProduceErrorP(VERSION_DATA[version][version_class][FIRSTPART_ALLDATA] - VERSION_DATA[version][version_class][FIRSTPART_DATA], form);//L_error��������
	for (int i = 0; i<VERSION_DATA[version][version_class][FIRSTPART_NUM]; i++)
	{
		L_code = ProduceCodeP(*(part1 + i), VERSION_DATA[version][version_class][FIRSTPART_DATA]);//L_code��������
		MutiplyConst(L_code, &L_code_act, 0, VERSION_DATA[version][version_class][FIRSTPART_ALLDATA] - VERSION_DATA[version][version_class][FIRSTPART_DATA]);
		DestroyLinkList(&L_code);//ɾ����
		*(*code1 + i) = LinkMod(&L_code_act, &L_error, form, VERSION_DATA[version][version_class][FIRSTPART_ALLDATA] - VERSION_DATA[version][version_class][FIRSTPART_DATA]);
		DestroyLinkList(&L_code_act);//ɾ����
	}
	DestroyLinkList(&L_error);//ɾ����

	/*ͬ����еڶ����ֵĲ�������Ϊ���ֻ�����࣬�п���ֻ��һ�֣����Եڶ�����ֱ��Ϊ�ռ���*/
	if (VERSION_DATA[version][version_class][SECONDPART_NUM])
	{
		*code2 = (int**)malloc(VERSION_DATA[version][version_class][SECONDPART_NUM] * sizeof(int*));
		if (*code2 == NULL)
			return LOVERFLOW;
	}
	else
		*code2 = NULL;

	/*�ڶ����ֵľ�����*/
	if (*code2)
	{
		L_error = ProduceErrorP(VERSION_DATA[version][version_class][SECONDPART_ALLDATA] - VERSION_DATA[version][version_class][SECONDPART_DATA], form);//L_error��������
		for (int i = 0; i<VERSION_DATA[version][version_class][SECONDPART_NUM]; i++)
		{
			L_code = ProduceCodeP(*(part2 + i), VERSION_DATA[version][version_class][SECONDPART_DATA]);//L_code��������
			MutiplyConst(L_code, &L_code_act, 0, VERSION_DATA[version][version_class][FIRSTPART_ALLDATA] - VERSION_DATA[version][version_class][FIRSTPART_DATA]);
			DestroyLinkList(&L_code);//ɾ����
			*(*code2 + i) = LinkMod(&L_code_act, &L_error, form, VERSION_DATA[version][version_class][SECONDPART_ALLDATA] - VERSION_DATA[version][version_class][SECONDPART_DATA]);
			DestroyLinkList(&L_code);//ɾ����
		}
		DestroyLinkList(&L_error);//ɾ����
	}

	return OK;
}

/*�ͷŵ�������Ϣ���ж�������*/
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

/*���ж���ʽ1�Ͷ���ʽ2��ȡ��*/
int *LinkMod(LinkList *L1, LinkList *L2, int *form, int code_length)
{
	int num_a, *code;
	LinkList L_con;

	//cout << (*L2)->next->expn << (*L1)->next->expn;
	for (; (*L2)->next->expn <= (*L1)->next->expn;)
	{
		/*�ҵ���������ʽ�е�һ�����ֶ�Ӧ��a�Ĵη���ʾ��ʽ*/
		for (num_a = 0; num_a < FORM_LENGTH; num_a++)
			if (form[num_a] == (*L1)->next->ceof)
				break;
		MutiplyConst(*L2, &L_con, num_a - (*L2)->next->ceof, (*L1)->next->expn - (*L2)->next->expn);//��������½���һ��L_con
		ConvertWithForm(&L_con, form);
		AddPolyn(L1, &L_con);//�˹���L_con������

		InsertInRange(L1, 0, 0);//��Ҫ�ǲ������ȡ��Ĺ������Ƿ�������������������Լ���0��0�η�����Ӱ��֮������㣬�Է�ֹ��һ���жϳ�������
	}
	SuppleLinkList(L1, code_length);//��Ϊϵ��Ϊ0�����ᱻɾ�������Ǿ�����λ��ʹΪ0Ҳ��Ӧ�ñ�ɾ�������Բ���ȥ

									/*��L1�е���Ϣ�ó���������*/
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



/*--------------------------------------------����Ϊ����ʽ�Ļ�������(GF(256))---------------------------------------*/

/* ��ʼ�����Ա� */
Status InitList(LinkList *L)
{
	/* ����ͷ���ռ䣬��ֵ��ͷָ�� */
	*L = (LNode *)malloc(sizeof(LNode));
	if (*L == NULL)
		exit(LOVERFLOW);

	(*L)->next = NULL;
	return OK;
}

/*��������*/
Status DestroyLinkList(LinkList*L)
{
	LinkList p = *L, q;//pָ��ͷ�ڵ�

	for (; p != NULL;)//�����ͷţ�����ͷ�ڵ�
	{
		q = p->next;
		free(p);
		p = q;
	}

	*L = NULL;

	return OK;
}

/*����ָ���Ӵ�С��˳�����Ԫ�أ��������ָ����ͬ���������ӻ��Ǻ���*/
Status InsertInRange(LinkList*L, int ceo, int ex,int method,int *form)
{
	if (*L == NULL)//���ͷָ����NULL���Ǿͷ���
		return ERROR;

	LinkList p = (*L)->next, q = *L, s;//qָ��ͷ��㣬pָ����Ԫ���
	for (; p != NULL;)
		if (p->expn == ex || p->expn<ex)
			break;
		else if (p->expn > ex)
		{
			q = p;
			p = p->next;
		}

	/*����һ��������*/
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
	else if (method == ADD&&p!=NULL&&p->expn == ex)//ע����������ģʽ������������a�Ĵη���ʽ���
	{
		/*����ת��ģʽ*/
		int ceof1, ceof2;
		for (int i = 0; i < FORM_LENGTH; i++)
		{
			if (i == p->ceof)
				ceof1 = form[p->ceof];
			if (i == ceo)
				ceof2 = form[ceo];
		}
		ceof1 ^= ceof2;//���
		/*ת������*/
		for (int i = 0; i < FORM_LENGTH; i++)
			if (form[i] == ceof1)
			{
				p->ceof = i;
				break;
			}
	}

	return OK;
}

/*�µĽ�������Ԫ��㲢����ָ��Ϊ��һ������ָ����һ,���²���Ľ���ָ��Ϊ���*/
Status InsertInRange(LinkList*L, int ceo)
{
	if (*L == NULL)//���ͷָ����NULL���Ǿͷ���
		return ERROR;
	LinkList p = (*L)->next,s;//pָ����Ԫ���

	s = (LinkList)malloc(sizeof(LNode));
	if (s == NULL)
		return LOVERFLOW;
	s->ceof = ceo;

	if (p == NULL)//�����Ԫ��㲻������Ϊ0��
		s->expn = 0;
	else
		s->expn = p->expn + 1;
	s->next = p;

	(*L)->next = s;

	return OK;
}

/*����ʽ�ļӷ�(GF(256))��ͬʱ����L2*/
Status AddPolyn(LinkList *L1, LinkList *L2)
{
	/*��������������һһ��������������δ���������*/
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
			/*ϵ�����,��GF(256)���൱��ȡ���*/
			p->next->ceof ^= q->ceof;
			/*����2�Ƶ���һ����㣬��ǰ��㱻�ͷ�*/
			s = q;
			q = q->next;
			free(s);
			/*��������ϵ��Ϊ0����û�д��ڵı�Ҫ��*/
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
			p = p->next;//pҪ�����ƶ�һ����λ
		}
	}

	/*������������1�ȵ�ͷ����ô��ʣ�µ�����2�Ľ�����ȥ*/
	if (!p->next)
		p->next = q;

	free(*L2);
	*L2 = NULL;

	return OK;
}

/*������ʽ�и����ϵ����a�Ĵη���ʽתΪint��ʽ*/
Status ConvertWithForm(LinkList *L, int *form)
{
	if (*L == NULL)
		return ERROR;

	LinkList p = (*L)->next;//pָ����Ԫ
	for (; p; p = p->next)
	{
		if (p->ceof >= 255)//����Ѿ�������255��ôҪ���б任��С��255
			p->ceof %= 255;
		p->ceof = form[p->ceof];
	}

	return OK;
}

/*��L1�������һ��ֻ��һ��Ķ���ʽ����L2�½�һ������洢����,��ȻL��ϵ��������a�Ĵη���ʽ*/
Status MutiplyConst(LinkList L1, LinkList *L2,const int a_num,const int a_expn)
{
	if (L1 == NULL)//���L1��δ��������ô���ش���
		return ERROR;

	InitList(L2);
	LinkList p = L1->next, q = *L2,s;//pָ��L1��Ԫ���,qָ��L2��ͷ�ڵ�
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

/*������s_expnָ�����µ�������룬ȱ�٣�����ϵ��Ϊ0*/
Status SuppleLinkList(LinkList *L, int s_expn)
{
	/*����δ����*/
	if (*L == NULL)
		return ERROR;

	for(int i=0;i<s_expn;i++)
	    InsertInRange(L, 0, i);

	return OK;
}

/*������ʽL1�Ͷ���ʽL2���(GF(256))�����L2���ͷţ�ע�⣺ϵ������Ϊa�Ĵη���ʽ*/
Status MutiplyP(LinkList *L1, LinkList *L2, int *form)
{
	if (!(*L1&&*L2))//���L1����L2�е�һ��δ�����ڵ������򷵻�
		return ERROR;

	LinkList L3, r;
	InitList(&L3);
	int ceo;

	for (LinkList p = (*L2)->next; p;)
	{
		for (LinkList q = (*L1)->next; q; q = q->next)
		{
			ceo = p->ceof + q->ceof;//ȷ��a�Ĵη�С��255
			if (ceo >= 255)
				ceo %= 255;
			InsertInRange(&L3, ceo, p->expn + q->expn, ADD, form);
		}
		r = p;
		p = p->next;
		free(r);
	}
	/*�������ɵ�L3Ϊ����ָ���Ӵ�С��˳������*/

	free(*L2);//L2���ͷ�
	*L2 = NULL;
	DestroyLinkList(L1);//L1���ͷ�
	*L1 = L3;//ͷָ�����

	return OK;
}

/*����������ֵĸ���������Ӧ��ʹ�õ����ɶ���ʽ*/
LinkList ProduceErrorP(int code_num, int *form)//ע�⣬��ʹ����ɶ���ʽ֮��Ӧ���ͷŸö���ʽ��������ʹ��֮ǰ��û��ʹ��InitList
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
		MutiplyP(&L1, &L2, form);//�˹���L2���ͷŵ����˷��Ľ����L1��
	}
	return L1;//L1��������һ��ָ�룬����Ӧ����û������ģ������õ���ַ�ĵط���ʹ���ڴ˺����е��Զ�������ַ����Ӱ��
}

/*����һ���������ݣ�����һ��������������ɵĶ���ʽ*/
LinkList ProduceCodeP(int *code, int length)
{
	LinkList L;
	InitList(&L);
	for (int i = length - 1; i >= 0; i--)
		InsertInRange(&L, *(code + i));

	return L;//ͬ��һ������
}



/*--------------------------------------------����Ϊ��䷽��ĺ���------------------------------------------------*/

/*����һ��������ķ���ռ�*/
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

/*�ͷŵ�һ������*/
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

/*���������ϻ���(3��5��7)ͼ��*/
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

	/*��mark�ϱ��*/
	FillInSquare(mark, 3, 3, 3, 1, 1);
	FillInSquare(mark, size - 4, 3, 3, 1, 1);
	FillInSquare(mark, 3, size - 4, 3, 1, 1);

	/*��������������������м�İ����ڱ���*/
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

/*��һ����Ϊ���ģ��ڰ뾶Ϊr���������ڸı�Ϊ��Ӧ��ֵ,ģʽ��ʾ��ʵ�Ļ��ǿ���,0��ʾ���ģ�1��ʾʵ��*/
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

/*��������������ķָ��߻�����*/
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

/*��У��ͼ�λ�����*/
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

/*�Ѱ汾��Ϣ�ӽ�ȥ*/
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

/*�Ѹ�ʽ��Ϣ�ӽ�ȥ*/
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

/*��������Ϣ�ӽ�ȥ*/
Status FillCodeInf(Square&square, Square&mark, bool*code_inf,const int size,const int num)
{
	int count = 0;
	for (int j = size - 1; j >= 1; j -= 2)//�д��ұ�������ƶ���ÿ���ƶ�������λ��
	{
		if (j == 6)//����ƶ����������������������α߿�������ߣ��Ļ��������ƶ�һ����
			j--;
		if ((size - 1 - j) / 2 % 2)//���˵������j�����������εĻ���Ӧ���Ǵ��������ƶ�
			for (int i = 0; i < size; i++)
			{
				/*���Һ����˳��*/
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
		else//ż�������Ǵ��������ƶ�
			for (int i = size - 1; i >= 0; i--)
			{
				/*���Һ����˳��*/
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
	/*�����ʣ�µ�λ�ñ�����ʹ��calloc����ģ������Ѿ��Զ���дΪ0*/

	return OK;
}

/*������Ĥ��Ϣ*/
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

/*���㵱ǰ����ķ���״��*/
int ScoreOfMode(Square&square,const int size)
{
	int score = 0, num_dark = 0;
	/*��һ�����֣�����������ϵ�ͬɫ��*/
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
	/*�ڶ�������*/
	for (int i = 0; i < size - 1; i++)
		for (int j = 0; j < size - 1; j++)
			if (square[i][j] == square[i][j + 1] && square[i][j] == square[i + 1][j] && square[i][j] == square[i + 1][j + 1])
				score += 3;
	/*�������ַ���*/
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
	/*���Ĳ��ַ���*/
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

/*��ӡ���ն�ά�루�ڰ����հ棩*/
void FinalPrint(Square&square, const int size)
{
	const HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE); //ȡ��׼����豸��Ӧ�ľ��
	const HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);  //ȡ��׼�����豸��Ӧ�ľ��
	for (int i = 0; i < size + 8; i++)
		for (int j = 0; j < size + 8; j++)
			if (i < 4 || i >= size + 4 || j < 4 || j >= size + 4)
				showch(hout, j * 2, i, ' ', COLOR_HWHITE, COLOR_BLACK, 2);
			else
				showch(hout, j * 2, i, ' ', square[i - 4][j - 4] == 0 ? COLOR_HWHITE : COLOR_BLACK, COLOR_BLACK, 2);
	setcolor(hout, COLOR_BLACK, COLOR_WHITE);
}

/*------------------------------����Ϊ���������ݵļ���--------------------------------------*/

/*����汾��Ϣ���ܼ�18λ*/
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

/*�����ʽ��Ϣ���ܼ�15λ*/
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

/*�������ֵĶ�������Ϣ*/
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
	/*������������*/
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
	/*���þ�������*/
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

/*-----------------------------------����-----------------------------------------*/

/*��num�Ķ������м�λ*/
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

/*���ɶ��ձ�*/
Status CreateForm(int *form)//��Ϊ255
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