//********************************************
//	���ຯ��
//********************************************
#pragma once
#include <list>
#include <map>
using namespace std;

#ifndef DELETE_SAFE
#define DELETE_SAFE(x)	do{ delete (x); (x)=NULL; }while(0)
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(x)	if( (x)!=NULL ) { delete (x); (x)=NULL; }
#endif

#ifndef SAFE_DECREF
#define SAFE_DECREF(x)	if( (x)!=NULL ) { (x)->DecRef(); (x)=NULL; }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(x)	if( (x)!=NULL ) { delete[] (x); (x)=NULL; }
#endif

#ifndef SAFE_FREE
#define SAFE_FREE(x)	if( (x)!=NULL ) { free(x); (x)=NULL; }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x)	if( (x)!=NULL ) { (x)->Release(); (x)=NULL; }
#endif

#ifndef SAFE_DEL_RELEASE
#define SAFE_DEL_RELEASE(x)	if( (x)!=NULL ) { (x)->Release(); delete (x); (x)=NULL; }
#endif

//! �ж�InValue��ֵ�Ƿ���MinValue��MaxValue�������ڣ������䣩
#ifndef Included_Close
#define Included_Close(InValue, MinValue, MaxValue)	((MinValue) <= (InValue) && (MaxValue) >= (InValue))
#endif

//! �ж�InValue��ֵ�Ƿ���MinValue��MaxValue�������ڣ������䣩
#ifndef Included_Open
#define Included_Open(InValue, MinValue, MaxValue)	((MinValue) < (InValue) && (MaxValue) > (InValue))
#endif

#define WCHAR unsigned short

// Fox@20081106----------------------------------------------
// ������־���
#define __FILE_DIR__	dirname(__FILE__)
#define __FILE_NAME__
#define DEBUGFMT  "<%s> %s(%d) : %s"
#define DEBUGARGS __FUNCTION__, __LINE__

#define PutoutLog(FILE_NAME, TYPE, FORMAT, ...) \
{ \
	char szLogType0x[32]; \
	LOG_TYPE type0x = TYPE; \
	switch( type0x ) \
{ \
	case LT_WARNING:	strcpy_s(szLogType0x, 32, "WARNING"); break; \
	case LT_ERROR:		strcpy_s(szLogType0x, 32, "ERROR"); break; \
	case LT_FATAL:		strcpy_s(szLogType0x, 32, "FATAL"); break; \
	default:			strcpy_s(szLogType0x, 32, "INFO"); break; \
} \
	char szLog0xFmt[512]; \
	sprintf_s(szLog0xFmt, 512, FORMAT,##__VA_ARGS__); \
	char szLog0x[2048]; \
	sprintf_s(szLog0x, 512, DEBUGFMT, \
	szLogType0x, DEBUGARGS, szLog0xFmt); \
	PutStringToFile(FILE_NAME, szLog0x); \
}

enum LOG_TYPE
{
	LT_DEFAULT		= 0,
	LT_WARNING		= 1,
	LT_ERROR		= 2,
	LT_FATAL		= 4
};
// Fox@20081106----------------------------------------------

//���ͷ�����
//exp: Do( m=MessageBox( NULL, _pTemp, "test", MB_OK ), Ini.ReadText("index", "test") );
//extern char *g_pTemp;
//#define Do(line, param) {g_pTemp=param; line; SAFE_FREE(g_pTemp);}

int random(int nMax);						//�����
int Round(float f);							//��������
int Round(double f);						//��������
void Delay(long time);						//��ʱ
int GetFileLength(const char *name);		//ȡ�ļ�����
bool CheckFile(const char *name);			//�ж��ļ��Ƿ����
void ShowMessage(const char *msg,...);		//��ʾ��Ϣ
void InitialDebugFileName(bool bIsTimed = false,bool bIsClear = true);	// �Ƿ񰴵�ǰʱ�������������ļ�
void PutDebugString(const char *msg);		//���������Ϣ
const char* GetDebugFileName();
void PutWarDebugString(const char *msg);	//�����ս������Ϣ
void PutStringToFile(const char* filename, const char *msg);	//����ļ���Ϣ
BOOL OverloadFile(const char* filename, const char *msg);		//! ����һ���ļ�����
void PutLogInfo(const char *msg);			//���log��Ϣ
char *GetFilePath(const char *filename, char* out);	//���ļ�����ȡ�ļ�·��
char *GetFileExtName(const char *filename, char* out);	//���ļ�����ȡ�ļ���չ��
WCHAR* WChar(char *str);					//char to wchar
char* Char(WCHAR* str);						//wchar to char
void DeleteDirectory(const char* strDir);	//ɾ��Ŀ¼
bool GetFileDialog(bool Load, const char* currentPath, char*, const char* strFilter="All Files\0*.*\0");	// ��ȡ�ļ�
bool GetPathDialog(const char* strRoot, char*);					// ��ȡ·��
void ReplaceLine(char* str);		// �滻STR�е�'\\'Ϊ'/';
bool GetString(const char* sour, char* dest, int num);	// ��ȡһ�����ַ����еĵ�NUM�������ո�Ϊ���

bool CreateDir(const char* dir); 
void FindFile(const char* strFile, const char* strExt, list<string>* plistFileName);	// ��ȡ���е�ָ����չ�����ļ�
void FindFile(const char* strFile, list<string>* plistFileName);	// ��ȡ���е��ļ�

// ����ļ���Ϣ������ָ����Ŀ¼�±����ļ�,���һ��������ʾ�Ƿ���ݵ�ǰϵͳʱ�����´����ļ���false�������룩
void PutStrToFileSaveToDir(const char* dir,const char* filename,const char *msg,bool IsRePut=false);

inline bool IsInRect(long x, long y, RECT& rect)
{
	if( x > rect.left && x < rect.right && y > rect.top && y < rect.bottom )
	{
		return true;
	}
	return false;
}

// �ж����������Ƿ��ཻ
// ��Դ���ε�����һ�㱻Ŀ����ΰ����ˣ����ཻ
inline bool IsInstersect(RECT sRect, RECT dRect)
{
	//���������ν���:
	//���󽻼������Ͻ�����,�����������ε����Ͻ�����֮�нϴ��һ��
	//���󽻼������½�����,�����������ε����½�����֮�н�С��һ��
	long  lx,ly,rx,ry;
	//�ཻ��������Ͻ�����
	lx = max(sRect.left,dRect.left);
	ly = max(sRect.top,dRect.top);
	//�ཻ��������½�����
	rx = min(sRect.right,dRect.right);
	ry = min(sRect.bottom,dRect.bottom);
	//�ཻ����Ŀ��͸߶�����0��ʾ�ཻ
	return ((rx - lx > 0) && (ry - ly > 0));
}

bool FormatMoneyNumber(char *str,const int size);//��ʽ����Ǯ��ʾ,ÿ����֮���һ��","

template<class T>
void QuickSort(T* a, int begin, int end);	// ��������

long Distance(long sx,long sy,long dx,long dy);
float Distance(float sx,float sy,float dx,float dy);
long GetLineDir(long sx, long sy, long dx, long dy);		// ��ȡ��(sx,sy)��(dx,dy)�ķ���(0-7)
float GetRadianDir(float sx,float sy,float dx,float dy);
//ͨ������õ��߶������е�
long GetLinePointByDir(list<POINT>& listPos,long lX,long lY,long lDir,long lDistance);
//�õ�ͨ��Ŀ���������߶��ϵĵ�
long GetLinePointToDest(list<POINT>& listPos,long lX,long lY,long lDestX,long lDestY,long lDistance);
//�õ�һ��Բ������е�
long GetCircPoint(list<POINT>& listPos,long lCenterX,long lCenterY,long lRadius);
//�õ�һ��������������е�
long GetSquarePoint(list<POINT>& listPos,long lCenterX,long lCenterY,long lOffLen);
//�õ�Դ��Ŀ��ֱ��һ��Դ��Զͬʱ��Ŀ������ĵ�����
void GetLineBack(long &x,long &y,long lX,long lY,long lDestX,long lDestY,long lFigure);
//�õ�Դ��Ŀ��ֱ��һ��Դ���ͬʱ��Ŀ������������
void GetLineFront(long &x,long &y,long lSrcX,long lSrcY,long lDestX,long lDestY,long lDistance);
//ȥ���ַ����ұߵĿո�
char* TrimRight(char* pszStr);


// �߾���ʱ�亯��,����ϵͳ�Կ��������ĺ�����,��RDTSCָ���ȡCPU�Կ���������ָ�����ڣ�����CPUƵ�ʵõ�
DWORD GetCurTickCount();
DWORD GetCurTime();

void IniEncoder(const char* pstrIn, char* pstrOut);		// ����
void IniDecoder(const char* pstrIn, char* pstrOut, int len);		// ����

#ifdef _RUNSTACKINFO_
//�������ջ��Ϣ
void StartStackInfo();
void OutputStackInfo(const char* pszInfo);
void EmptyStackInfo();
void EndStackInfo();
#endif


// �ṩ��hashʹ��
class point_compare
{
public:
	bool operator( )( const POINT& Key1, const POINT& Key2 ) const
	{
		if(Key1.x < Key2.x)
			return true;
		else if(Key1.x == Key2.x)
			if(Key1.y < Key2.y)
				return true;
		return false;
	}

};
extern bool g_bOutPut;	// �Ƿ����

///
/// calculate the rest seconds in this day.
///
unsigned long GetRestSeconds();

///
/// Calculate the elapsed day between two time values.
///
unsigned long GetElapsedDay( time_t time1, time_t time2 );


/* 
*���ܣ����ɼ����ַ���
*ϸ�������� ��Դ�ַ��� ����ָ�����Ƚضϣ����������� ʡ�Է��ű�ʾ
*������ szString	Դ�ַ���
*		iLen		ָ������
*����ֵ�� std::string
*���ӣ�����	
*/
std::string	GetShortString( LPCTSTR szString, int iLen );

/* 
*���ܣ��ַ����ַ����滻
*ϸ�������� ��Դ�ַ���source��ָ���ַ���sub�滻Ϊrep
*������ source	Դ�ַ���
		sub ָ���ַ���
*		rep		ָ�������滻�ַ���
*����ֵ�� ������ַ���
*���ӣ����Ƚ�
*/
char *Replace(char *source, char *sub, char *rep);


// SYSTEMTIME to time_t
time_t SystemtimeToTime(const SYSTEMTIME &st);