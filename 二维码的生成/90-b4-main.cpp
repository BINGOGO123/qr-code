/*1652241 臧海彬 计算机1班*/
#include<iostream>
#include"90-b4.h"
#include"cmd_console_tools.h"
#include<conio.h>
using namespace std;
int main()
{
	/*其他*/
	const HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE); //取标准输出设备对应的句柄
	const HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);  //取标准输入设备对应的句柄

	/*生成对照表*/
	int form[FORM_LENGTH];
	CreateForm(form);
	for (;;)
	{
		/*定义各种变量*/
		char *information;                        //放原始信息的指针
		VERSION_CLASS version_class;              //存放纠错等级
		int version, length = 0;                  //版本号，初始生成的数据位长度，长度实际没有用
		unsigned int char_num = 0;                //输入的字节数目
		bool *shujuwei;                           //将原始信息转为二进制数据的地址
		int **data_part1, **data_part2;           //用于存放数据码字信息
		int **code1, **code2;                     //用于存放纠错码字信息
		Square square = NULL, mark = NULL;        //square用于存放待打印的正方形信息，mark为辅助将信息放到square中
		int size;                                 //图形规模
		bool version_inf[18];                     //用于存放版本信息（固定18位）
		bool form_inf[15];                        //用于存放格式信息并放入
		bool*code_inf;                            //用于存放最终的码字信息
		int mode = 0;                             //掩膜信息
		int method = 0;                           //编码方法gb312还是utf-8

		/*更改字体和界面大小*/
		setfontsize(hout, L"新宋体", 20);
		setconsoleborder(hout, 120, 30, 999);

		/*获得初始信息*/
		information = GetInformation(char_num);//此过程有申请空间
		version_class = CorrectClass();
		version = JudgeVersion(information, version_class);
		method = MethodOfHanzi(information);

		/*输出将要使用的信息*/
		if (version)
			cout << "\n目前推测将要使用的版本是版本：[" << version << "]\n\n";
		else
		{
			cout << "\n输入的字符串太长，40个版本无法装下。。。\n";
			FreeInformation(information);
			_getch();
			continue;
		}

		/*强制更改版本*/
		ChangeVersion(version);

		/*将信息转为二进制*/
		shujuwei = ProduceStorageCode(information, version, length, method);
		/*将数据位补为相应版本和纠错等级需要的位数*/
		for (; MakeUp(&shujuwei, length, version, version_class) != OK;)
		{
			version++;
			if (version > 40)
			{
				cout << "\n输入的字符串太长，40个版本无法装下。。。\n";
				FreeInformation(information);
				_getch();
				system("cls");
				break;
			}
		}
		if (version > 40)
			continue;
		/*将数据位转成整型*/
		ConvertMazi(shujuwei, version, version_class, &data_part1, &data_part2);
		/*生成纠错码字*/
		CorrectCode(data_part1, data_part2, version, version_class, form, &code1, &code2);
		/*生成码字信息*/
		code_inf = CodeCal(data_part1, data_part2, code1, code2, version, version_class);
		/*生成版本信息*/
		if (version >= 7)
			VersionCal(version, version_inf);

		/*判断最佳掩膜*/
		size = 21 + (version - 1) * 4;
		for (int score, score_last, mode_com = 0; mode_com < 8; mode_com++)
		{
			CreateSquare(square, size);
			CreateSquare(mark, size);
			FillThreeSquare(square, mark, size);                    //三个定位图
			FillLine(square, mark, size);                       //连接定位图的线
			if (version > 1)
				FillAlignment(square, mark, version, size);     //校正图形
			if (version >= 7)
				FillVersionInf(square, mark, version_inf, size);//版本信息
			FormCal(version_class, mode_com, form_inf);         //生成格式信息
			FillFormInf(square, mark, form_inf, size);          //格式信息
			FillCodeInf(square, mark, code_inf, size, (VERSION_DATA[version][version_class][FIRSTPART_NUM] * VERSION_DATA[version][version_class][FIRSTPART_ALLDATA]
				+ VERSION_DATA[version][version_class][SECONDPART_NUM] * VERSION_DATA[version][version_class][SECONDPART_ALLDATA]) * 8);//码字信息

			/*将图形掩膜操作*/
			HideMode(square, mark, mode_com, size);

			/*判断得分*/
			score = ScoreOfMode(square, size);
			if (mode_com == 0)
				score_last = score;
			else if (mode_com != 0)
				if (score < score_last)
				{
					mode = mode_com;
					score_last = score;
				}

			FreeInformation(square, size);                                                         //主方格
			FreeInformation(mark, size);                                                           //辅助方格
		}

		/*形成最终信息*/
		CreateSquare(square, size);
		CreateSquare(mark, size);
		FillThreeSquare(square, mark, size);                //三个定位图
		FillLine(square, mark, size);                       //连接定位图的线
		if (version > 1)
			FillAlignment(square, mark, version, size);     //校正图形
		if (version >= 7)
			FillVersionInf(square, mark, version_inf, size);//版本信息
		FormCal(version_class, mode, form_inf);
		FillFormInf(square, mark, form_inf, size);          //格式信息
		FillCodeInf(square, mark, code_inf, size, (VERSION_DATA[version][version_class][FIRSTPART_NUM] * VERSION_DATA[version][version_class][FIRSTPART_ALLDATA]
			+ VERSION_DATA[version][version_class][SECONDPART_NUM] * VERSION_DATA[version][version_class][SECONDPART_ALLDATA]) * 8);//码字信息																											/*将图形掩膜操作*/
		HideMode(square, mark, mode, size);                 //掩膜操作

		/*输出当前信息*/
		cout << "\n\n----------------------------------------------------------------------------------------------\n";
		cout << "以下为此次二维码信息：\n";
		cout << "输入的字节数[" << strlen(information) << "]\n信息是：[";
		if (char_num < 150)
			cout << information << "]\n";
		else
			cout << "字符过长，不予显示]\n";
		cout << "将要使用的版本是：[" << version << "]\n";
		cout << "将要使用的纠错等级是：[" << version_class << "]\n";
		cout << "掩膜类型是：[" << mode << "]\n";
		if (method == 1)
		{
			cout << "汉字的编码方式：";
			cout << "utf-8\n";
		}
		if (version >= 20)
			cout << "****注意，您要生成的二维码版本较大，扫码请确保设备与qrcode距离较近****\n";
		cout << "----------------------------------------------------------------------------------------------\n";
		cout << "回车生成。。";
		for (; _getch() != '\r';);

		/*最终信息的打印*/
		system("cls");
		if (version < 10)
		{
			setfontsize(hout, L"点阵体", 9);
			setconsoleborder(hout, 130, 65, 999);
		}
		else if (version < 20)
		{
			setfontsize(hout, L"新宋体", 7);
			setconsoleborder(hout, 220, 110, 999);
		}
		else
		{
			setfontsize(hout, L"新宋体", 1);
			setconsoleborder(hout, 400, 200, 999);
		}
		FinalPrint(square, size);
		/*释放信息*/
		FreeInformation(square, size);                                                         //主方格
		FreeInformation(mark, size);                                                           //辅助方格
		FreeInformation(code_inf);                                                             //释放最终码字信息的数据位
		FreeInformation(&data_part1, VERSION_DATA[version][version_class][FIRSTPART_NUM]);     //码字部分1
		FreeInformation(&data_part2, VERSION_DATA[version][version_class][SECONDPART_NUM]);    //码字部分2
		FreeInformation(&code1, VERSION_DATA[version][version_class][FIRSTPART_NUM]);          //纠错码部分1
		FreeInformation(&code2, VERSION_DATA[version][version_class][SECONDPART_NUM]);         //纠错码部分2
		FreeInformation(information);                                                          //原始信息
		FreeInformation(shujuwei);                                                             //原始数据位

		_getch();
		system("cls");
		setfontsize(hout, L"新宋体", 20);
		setconsoleborder(hout, 120, 30, 999);
		cout << "继续请按Y(否则退出)。。。" << endl;
		char c = _getch();
		if (!(c == 'Y' || c == 'y'))
			break;
		getchar();
	}
	return 0;
}