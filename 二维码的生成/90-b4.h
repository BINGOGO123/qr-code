/*1652241 臧海彬 计算机1班*/
#define LOVERFLOW -2
#define OK 1
#define LERROR -1
#define MAX_NUM_PER_LINE 15

/*加罗华域的运算需要用到的常量*/
#define FORM_LENGTH 255
#define AND_XOR 285
#define FORM_INFORMATION 21522
#define VERSION_CAL 7973
#define FORM_CAL 1335

/*在插入元素的时候究竟要忽略重复元素还是相加*/
#define IGNORE 0
#define ADD 1

typedef int Status;

typedef struct MyData
{
	int ceof;
	int expn;
	struct MyData *next;
}LNode, *LinkList;

typedef bool **Square;

/*纠错等级变量*/
enum VERSION_CLASS { VERSION_L, VERSION_M, VERSION_Q, VERSION_H };

/*获取信息*/
char *GetInformation(unsigned int &char_num);
VERSION_CLASS CorrectClass();
int JudgeVersion(char *xinxi, VERSION_CLASS version_class);
void ChangeVersion(int &version);
int MethodOfHanzi(const char *s);

/*数据处理*/
bool* ProduceStorageCode(char *xinxi, const int version, int &total_length,int method);
void ProcessFormNum(char *xinxi, bool **shujuwei, int mode_start, int mode_end, int &length, int &count_cur, const int version);
void ProcessFormAlp(char *xinxi, bool **shujuwei, int mode_start, int mode_end, int &length, int &count_cur, const int version);
void ProcessFormByte(char *xinxi, bool **shujuwei, int mode_start, int mode_end, int &length, int &count_cur, const int version);
//void ProcessFormHanzi(char *xinxi, bool **shujuwei, int mode_start, int mode_end, int &length, int &count_cur, const int version);
void Fangzhi(bool *shujuwei, int shuzi, int locate_start, int locate_end);
Status MakeUp(bool**shujuwei, const int length, int version, VERSION_CLASS version_class);
Status ConvertMazi(bool*shujuwei, int version, VERSION_CLASS version_class, int ***mazi_part1, int ***mazi_part2);
Status CorrectCode(int **part1, int **part2, int version, VERSION_CLASS version_class, int *form, int ***code1, int ***code2);
char* G2U(const char* gb2312);

/*填方格函数*/
Status CreateSquare(Square&square, const int size);
Status FillInSquare(Square&square, const int x, const int y, const int r, const bool value, int mode = 0);
Status FillThreeSquare(Square&square, Square&mark, const int size);
Status FillLine(Square&square, Square&mark, const int size);
Status FillAlignment(Square&square, Square&mark, const int version, const int size);
Status FillVersionInf(Square&square, Square&mark, bool*version_inf, const int size);
Status FillFormInf(Square&square, Square&mark, bool*form_inf, const int size);
Status FillCodeInf(Square&square, Square&mark, bool*code_inf, const int size, const int num);
Status HideMode(Square&square, Square&mark, int hide_mode, int size);
int ScoreOfMode(Square&square, const int size);
void FinalPrint(Square&square, const int size);

/*计算码字和格式版本*/
void VersionCal(int version, bool*version_inf);
void FormCal(VERSION_CLASS version_class, int hide_mode, bool*form_inf);
bool *CodeCal(int **part1, int **part2, int **code1, int **code2, int version, VERSION_CLASS version_class);

/*与多项式链表有关的函数*/
Status InsertInRange(LinkList*L, int ceo, int ex, int xuanxiang = IGNORE,int *form=NULL);
Status DestroyLinkList(LinkList*L);
Status InsertInRange(LinkList*L, int ceo);
Status InitList(LinkList *L);
Status AddPolyn(LinkList *L1, LinkList *L2);
Status ConvertWithForm(LinkList *L, int *form);
Status MutiplyConst(LinkList L1, LinkList *L2, const int a_num, const int a_expn);
Status MutiplyP(LinkList *L1, LinkList *L2, int *form);
Status SuppleLinkList(LinkList *L, int s_expn);
LinkList ProduceCodeP(int *code, int length);
int *LinkMod(LinkList *L1, LinkList *L2, int *form, int code_length);
LinkList ProduceErrorP(int code_num, int *form);

/*释放空间*/
Status FreeInformation(bool *xinxi);
Status FreeInformation(char *xinxi);
Status FreeInformation(int ***mazi, int length);
Status FreeInformation(Square&square, const int size);

/*其他*/
Status CreateForm(int *form);
int BitNum(int num);

/*字符计数指示符的位数*/
#define VERSION_NUM 0
#define VERSION_ALP 1
#define VERSION_BYTE 2
#define VERSION_HANZI 3
const int VERSION_COUNT[4][40 + 1]=
{
	{0, 10,10,10,10,10,10,10,10,10,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,14,14,14,14,14,14,14,14,14,14,14,14,14,14},
	{0, 9, 9, 9, 9, 9, 9, 9, 9, 9,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,13,13,13,13,13,13,13,13,13,13,13,13,13,13},
	{0, 8, 8, 8, 8, 8, 8, 8, 8, 8,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16},
	{0, 8, 8, 8, 8, 8, 8, 8, 8, 8,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,12,12,12,12,12,12,12,12,12,12,12,12,12,12}
};

/*每个版本、纠错等级对应的部分数目（最多两部分），每部分分组个数，每组的总码字，数据码字（纠错码字为两者之差）*/
#define FIRSTPART_NUM 0
#define FIRSTPART_ALLDATA 1
#define FIRSTPART_DATA 2
#define SECONDPART_NUM 3
#define SECONDPART_ALLDATA 4
#define SECONDPART_DATA 5
const int  VERSION_DATA[40 + 1][4][6] =
{
	{0},//为了使用方便
	{{1,26,19},{1,26,16},{1,26,13},{1,26,9}},//1
	{{1,44,34},{1,44,28},{1,44,22},{1,44,16}},//2
	{{1,70,55},{1,70,44},{2,35,17},{2,35,13}},//3
	{{1,100,80},{2,50,32},{2,50,24},{4,25,9}},//4
	{{1,134,108},{2,67,43},{2,33,15,2,34,16},{2,33,11,2,34,12}},//5
	{{2,86,68},{4,43,27},{4,43,19},{4,43,15}},//6
	{{2,98,78},{4,49,31},{2,32,14,4,33,15},{4,39,13,1,40,14} },//7
	{{2,121,97},{2,60,38,2,61,39},{4,40,18,2,41,19},{4,40,14,2,41,15}},//8
	{{2,146,116},{3,58,36,2,59,37},{4,36,16,4,37,19},{4,36,12,4,37,13}},//9
	{{2,86,68,2,87,69},{4,69,43,1,70,44},{6,43,19,2,44,20},{6,43,15,2,44,16}},//10
	{{4,101,81},{1,80,50,4,81,51},{4,50,22,4,51,23},{3,36,12,8,37,13}},//11
	{{2,116,92,2,117,93},{6,58,36,2,59,37},{4,46,20,6,47,21},{7,42,14,4,43,15}},//12
	{{4,133,107},{8,59,37,1,60,38},{8,44,20,4,45,21},{12,33,11,4,34,12}},//13
	{{3,145,115,1,146,116},{4,64,40,5,65,41},{11,36,16,5,37,17},{11,36,12,5,37,13}},//14
	{{5,109,87,1,110,88},{5,65,41,5,66,42},{5,54,24,7,55,25},{11,36,12,7,37,13}},//15
	{{5,122,98,1,123,99},{7,73,45,3,74,46},{15,43,19,2,44,20},{3,45,15,13,46,16}},//16
	{{1,135,107,5,136,108},{10,74,46,1,75,47},{1,50,22,15,51,23},{2,42,14,17,43,15}},//17
	{{5,150,120,1,151,121},{9,69,43,4,70,44},{17,50,22,1,51,23},{2,42,14,19,43,15}},//18
	{{3,141,113,4,142,114},{3,70,44,11,71,45},{17,47,21,4,48,22},{9,39,13,16,40,14}},//19
	{{3,135,107,5,136,108},{3,67,41,13,68,42},{15,54,24,5,55,25},{15,43,15,10,44,16}},//20
	{{4,144,116,4,145,117},{17,68,42},{17,50,22,6,51,23},{19,46,16,6,47,17}},//21
	{{2,139,111,7,140,112},{17,74,46},{7,54,24,16,55,25},{34,37,13}},//22
	{{4,151,121,5,152,122},{4,75,47,14,76,48},{11,54,24,14,55,25},{16,45,15,14,46,16}},//23
	{{6,147,117,4,148,118},{6,73,45,14,74,46},{11,54,24,16,55,25},{30,46,16,2,47,17}},//24
	{{8,132,106,4,133,107},{8,75,47,13,76,48},{7,54,24,22,55,25},{22,45,15,13,46,16}},//25
	{{10,142,114,2,143,115},{19,74,16,4,75,47},{28,50,22,6,51,23},{33,46,16,4,47,17}},//26
	{{8,152,122,4,153,123},{22,73,45,3,74,46},{8,53,23,26,54,24},{12,45,15,28,46,16}},//27
	{{3,147,117,10,148,118},{3,73,45,23,74,46},{4,54,24,31,55,25},{11,45,15,31,46,16}},//28
	{{7,146,116,7,147,117},{21,73,45,7,74,46},{1,53,23,37,54,24},{19,45,15,26,46,16}},//29
	{{5,145,115,10,146,116},{19,75,47,10,76,48},{15,54,24,25,55,25},{23,45,15,25,46,16}},//30
	{{13,145,115,3,146,116},{2,74,46,29,75,47},{42,54,24,1,55,25},{23,45,15,28,46,16}},//31
	{{17,145,115},{10,74,46,23,75,47},{10,54,24,35,55,25},{19,45,15,35,46,16}},//32
	{{17,145,115,1,146,116},{14,74,46,21,75,47},{29,54,24,19,55,25},{11,45,15,46,46,16}},//33
	{{13,145,115,6,146,116},{14,74,46,23,75,47},{44,54,24,7,55,25},{59,46,16,1,47,17}},//34
	{{12,151,121,7,152,122},{12,75,47,26,76,48},{39,54,24,14,55,25},{22,45,15,41,46,16}},//35
	{{6,151,121,14,152,122},{6,75,47,34,76,48},{46,54,24,10,55,25},{2,45,15,64,46,16}},//36
	{{17,152,122,4,153,123},{29,74,46,14,75,47},{49,54,24,10,55,25},{24,45,15,46,46,16}},//37
	{{4,152,122,18,153,123},{13,74,46,32,75,47},{48,54,24,14,55,25},{42,45,15,32,46,16}},//38
	{{20,147,117,4,148,118},{40,75,47,7,76,48},{43,54,24,22,55,25},{10,45,15,67,46,16}},//39
	{{19,148,118,6,149,119},{18,75,47,31,76,48},{34,54,24,34,55,25},{20,45,15,61,46,16}}//40
};

/*在数据位不足以达到指定位数时补充的位*/
const bool COVER[16] = { 1,1,1,0,1,1,0,0,0,0,0,1,0,0,0,1 };

/*40个版本的校正图形位置*/
const int ALIGNMENT_LOCATION[7][40 + 1]=//这样写短扁一点
{
	{0,0,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
	{0,0,18,22,26,30,34,22,24,26,28,30,32,34,26,26,26,30,30,30,34,28,26,30,28,32,30,34,26,30,26,30,34,30,34,30,24,28,32,26,30},
	{0,0,0,0,0,0,0,38,42,46,50,54,58,62,46,48,50,54,56,58,62,50,50,54,54,58,58,62,50,54,52,56,60,58,62,54,50,54,58,54,58},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,66,70,74,78,82,86,90,72,74,78,80,84,86,90,74,78,78,82,86,86,90,78,76,80,84,82,86},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,94,98,102,106,110,114,118,98,102,104,108,112,114,118,102,102,106,110,110,114},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,122,126,130,134,138,142,146,126,128,132,136,138,142},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,150,154,158,162,166,170}
};