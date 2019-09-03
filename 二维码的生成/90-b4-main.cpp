/*1652241 갺��� �����1��*/
#include<iostream>
#include"90-b4.h"
#include"cmd_console_tools.h"
#include<conio.h>
using namespace std;
int main()
{
	/*����*/
	const HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE); //ȡ��׼����豸��Ӧ�ľ��
	const HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);  //ȡ��׼�����豸��Ӧ�ľ��

	/*���ɶ��ձ�*/
	int form[FORM_LENGTH];
	CreateForm(form);
	for (;;)
	{
		/*������ֱ���*/
		char *information;                        //��ԭʼ��Ϣ��ָ��
		VERSION_CLASS version_class;              //��ž���ȼ�
		int version, length = 0;                  //�汾�ţ���ʼ���ɵ�����λ���ȣ�����ʵ��û����
		unsigned int char_num = 0;                //������ֽ���Ŀ
		bool *shujuwei;                           //��ԭʼ��ϢתΪ���������ݵĵ�ַ
		int **data_part1, **data_part2;           //���ڴ������������Ϣ
		int **code1, **code2;                     //���ڴ�ž���������Ϣ
		Square square = NULL, mark = NULL;        //square���ڴ�Ŵ���ӡ����������Ϣ��markΪ��������Ϣ�ŵ�square��
		int size;                                 //ͼ�ι�ģ
		bool version_inf[18];                     //���ڴ�Ű汾��Ϣ���̶�18λ��
		bool form_inf[15];                        //���ڴ�Ÿ�ʽ��Ϣ������
		bool*code_inf;                            //���ڴ�����յ�������Ϣ
		int mode = 0;                             //��Ĥ��Ϣ
		int method = 0;                           //���뷽��gb312����utf-8

		/*��������ͽ����С*/
		setfontsize(hout, L"������", 20);
		setconsoleborder(hout, 120, 30, 999);

		/*��ó�ʼ��Ϣ*/
		information = GetInformation(char_num);//�˹���������ռ�
		version_class = CorrectClass();
		version = JudgeVersion(information, version_class);
		method = MethodOfHanzi(information);

		/*�����Ҫʹ�õ���Ϣ*/
		if (version)
			cout << "\nĿǰ�Ʋ⽫Ҫʹ�õİ汾�ǰ汾��[" << version << "]\n\n";
		else
		{
			cout << "\n������ַ���̫����40���汾�޷�װ�¡�����\n";
			FreeInformation(information);
			_getch();
			continue;
		}

		/*ǿ�Ƹ��İ汾*/
		ChangeVersion(version);

		/*����ϢתΪ������*/
		shujuwei = ProduceStorageCode(information, version, length, method);
		/*������λ��Ϊ��Ӧ�汾�;���ȼ���Ҫ��λ��*/
		for (; MakeUp(&shujuwei, length, version, version_class) != OK;)
		{
			version++;
			if (version > 40)
			{
				cout << "\n������ַ���̫����40���汾�޷�װ�¡�����\n";
				FreeInformation(information);
				_getch();
				system("cls");
				break;
			}
		}
		if (version > 40)
			continue;
		/*������λת������*/
		ConvertMazi(shujuwei, version, version_class, &data_part1, &data_part2);
		/*���ɾ�������*/
		CorrectCode(data_part1, data_part2, version, version_class, form, &code1, &code2);
		/*����������Ϣ*/
		code_inf = CodeCal(data_part1, data_part2, code1, code2, version, version_class);
		/*���ɰ汾��Ϣ*/
		if (version >= 7)
			VersionCal(version, version_inf);

		/*�ж������Ĥ*/
		size = 21 + (version - 1) * 4;
		for (int score, score_last, mode_com = 0; mode_com < 8; mode_com++)
		{
			CreateSquare(square, size);
			CreateSquare(mark, size);
			FillThreeSquare(square, mark, size);                    //������λͼ
			FillLine(square, mark, size);                       //���Ӷ�λͼ����
			if (version > 1)
				FillAlignment(square, mark, version, size);     //У��ͼ��
			if (version >= 7)
				FillVersionInf(square, mark, version_inf, size);//�汾��Ϣ
			FormCal(version_class, mode_com, form_inf);         //���ɸ�ʽ��Ϣ
			FillFormInf(square, mark, form_inf, size);          //��ʽ��Ϣ
			FillCodeInf(square, mark, code_inf, size, (VERSION_DATA[version][version_class][FIRSTPART_NUM] * VERSION_DATA[version][version_class][FIRSTPART_ALLDATA]
				+ VERSION_DATA[version][version_class][SECONDPART_NUM] * VERSION_DATA[version][version_class][SECONDPART_ALLDATA]) * 8);//������Ϣ

			/*��ͼ����Ĥ����*/
			HideMode(square, mark, mode_com, size);

			/*�жϵ÷�*/
			score = ScoreOfMode(square, size);
			if (mode_com == 0)
				score_last = score;
			else if (mode_com != 0)
				if (score < score_last)
				{
					mode = mode_com;
					score_last = score;
				}

			FreeInformation(square, size);                                                         //������
			FreeInformation(mark, size);                                                           //��������
		}

		/*�γ�������Ϣ*/
		CreateSquare(square, size);
		CreateSquare(mark, size);
		FillThreeSquare(square, mark, size);                //������λͼ
		FillLine(square, mark, size);                       //���Ӷ�λͼ����
		if (version > 1)
			FillAlignment(square, mark, version, size);     //У��ͼ��
		if (version >= 7)
			FillVersionInf(square, mark, version_inf, size);//�汾��Ϣ
		FormCal(version_class, mode, form_inf);
		FillFormInf(square, mark, form_inf, size);          //��ʽ��Ϣ
		FillCodeInf(square, mark, code_inf, size, (VERSION_DATA[version][version_class][FIRSTPART_NUM] * VERSION_DATA[version][version_class][FIRSTPART_ALLDATA]
			+ VERSION_DATA[version][version_class][SECONDPART_NUM] * VERSION_DATA[version][version_class][SECONDPART_ALLDATA]) * 8);//������Ϣ																											/*��ͼ����Ĥ����*/
		HideMode(square, mark, mode, size);                 //��Ĥ����

		/*�����ǰ��Ϣ*/
		cout << "\n\n----------------------------------------------------------------------------------------------\n";
		cout << "����Ϊ�˴ζ�ά����Ϣ��\n";
		cout << "������ֽ���[" << strlen(information) << "]\n��Ϣ�ǣ�[";
		if (char_num < 150)
			cout << information << "]\n";
		else
			cout << "�ַ�������������ʾ]\n";
		cout << "��Ҫʹ�õİ汾�ǣ�[" << version << "]\n";
		cout << "��Ҫʹ�õľ���ȼ��ǣ�[" << version_class << "]\n";
		cout << "��Ĥ�����ǣ�[" << mode << "]\n";
		if (method == 1)
		{
			cout << "���ֵı��뷽ʽ��";
			cout << "utf-8\n";
		}
		if (version >= 20)
			cout << "****ע�⣬��Ҫ���ɵĶ�ά��汾�ϴ�ɨ����ȷ���豸��qrcode����Ͻ�****\n";
		cout << "----------------------------------------------------------------------------------------------\n";
		cout << "�س����ɡ���";
		for (; _getch() != '\r';);

		/*������Ϣ�Ĵ�ӡ*/
		system("cls");
		if (version < 10)
		{
			setfontsize(hout, L"������", 9);
			setconsoleborder(hout, 130, 65, 999);
		}
		else if (version < 20)
		{
			setfontsize(hout, L"������", 7);
			setconsoleborder(hout, 220, 110, 999);
		}
		else
		{
			setfontsize(hout, L"������", 1);
			setconsoleborder(hout, 400, 200, 999);
		}
		FinalPrint(square, size);
		/*�ͷ���Ϣ*/
		FreeInformation(square, size);                                                         //������
		FreeInformation(mark, size);                                                           //��������
		FreeInformation(code_inf);                                                             //�ͷ�����������Ϣ������λ
		FreeInformation(&data_part1, VERSION_DATA[version][version_class][FIRSTPART_NUM]);     //���ֲ���1
		FreeInformation(&data_part2, VERSION_DATA[version][version_class][SECONDPART_NUM]);    //���ֲ���2
		FreeInformation(&code1, VERSION_DATA[version][version_class][FIRSTPART_NUM]);          //�����벿��1
		FreeInformation(&code2, VERSION_DATA[version][version_class][SECONDPART_NUM]);         //�����벿��2
		FreeInformation(information);                                                          //ԭʼ��Ϣ
		FreeInformation(shujuwei);                                                             //ԭʼ����λ

		_getch();
		system("cls");
		setfontsize(hout, L"������", 20);
		setconsoleborder(hout, 120, 30, 999);
		cout << "�����밴Y(�����˳�)������" << endl;
		char c = _getch();
		if (!(c == 'Y' || c == 'y'))
			break;
		getchar();
	}
	return 0;
}