// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once
#pragma warning(disable:4244)
#pragma warning(disable:4018)
#pragma warning(disable:4996)

#ifndef _USE_32BIT_TIME_T
#define _USE_32BIT_TIME_T
#endif

#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����

// TODO: �ڴ˴����ó���Ҫ��ĸ���ͷ�ļ�

#include <Windows.h>
#include <wtypes.h>
#include <assert.h>
#include <time.h>
#include <process.h>

// ��׼ģ���
#include <list>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <deque>
#include <set>

#include "..\public\public.h"
#include "..\public\tools.h"
#include "..\public\readwrite.h"
#include "..\public\debugnew.h"
//#include "..\..\server\logserver\logserver\game.h"

using namespace std;