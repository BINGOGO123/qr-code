#include<iostream>
using namespace std;
int main2()
{
	int shuzu[301] = { 0 }, shuzu_linshi[301] = { 0 };
	shuzu[300] = 1;
	bool panduan = 0;
	int shuzu_chu[9] = { 1,1,0,1,1,0,0,0,1 };
	for (;;)
	{
		for (int i = 300; i >= 0; i--)
			if (shuzu[i] == 1)
			{
				for (int j = 0; j < 9; j++)
					shuzu_linshi[j + i - 8] = shuzu_chu[j];
				for (int z = 0; z < 301; z++)
					if (shuzu_linshi[z] + shuzu[z] == 1)
						shuzu[z] = 1;
					else
						shuzu[z] = 0;
				break;
			}

		for (int i = 300; i >= 0; i--)
		{
			if (i <= 7)
			{
				panduan = 1;
				cout << shuzu[i] << ' ';
				continue;
			}
			if (shuzu[i] == 1)
				break;
		}

		if (panduan == 1)
			break;

		for (int i = 0; i < 301; i++)
			shuzu_linshi[i] = 0;
	}

	return 0;
}

int main1()
{
	int shuzu[46] = { 0 }, shuzu_linshi[46] = { 0 };
	shuzu[45] = 1;
	bool panduan = 0;
	int shuzu_chu[9] = { 1,1,0,1,1,0,0,0,1 };
	for (;;)
	{
		for (int i = 45; i >= 0; i--)
			if (shuzu[i] == 1)
			{
				for (int j = 0; j < 9; j++)
					shuzu_linshi[j + i - 8] = shuzu_chu[j];
				for (int z = 0; z < 46; z++)
					if (shuzu_linshi[z] + shuzu[z] == 1)
						shuzu[z] = 1;
					else
						shuzu[z] = 0;
				break;
			}

		for (int i = 45; i >= 0; i--)
		{
			if (i <= 7)
			{
				panduan = 1;
				cout << shuzu[i] << ' ';
				continue;
			}
			if (shuzu[i] == 1)
				break;
		}

		if (panduan == 1)
			break;

		for (int i = 0; i < 46; i++)
			shuzu_linshi[i] = 0;
	}

	return 0;
}

/*这个证明了a45和a300确实是相等的啊，那么之前的运算到底哪里出了问题呢*/


int main3()
{
	char a, b;
	for (;;)
	{
		a = getchar();
		b = getchar();
		getchar();
		cout << int(a) << int(b)<<endl;
	}

	return 0;
}

void shabi(bool**shujuewi)
{

}
int main5()
{
	char a[3] = "我";
	cout << int(unsigned char(a[0])) << ' ' << int(unsigned char(a[1])) << endl;
	
	return 0;
}