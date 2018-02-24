#define VC_EXTRALEAN
#include <afxwin.h>
#include <afxdisp.h>
#include <afxext.h>
#include <afxcview.h>
#include <afxcmn.h>
#include <mmsystem.h>

#include "resource.h"
#include <string>

using namespace std;

#define ID_TIMER_MAIN 10000
#define ID_TIMER_TAPE 20000

//размер бордюра в знакоместах
#define BORDER_TOP_BOTTOM 2
#define BORDER_LEFT_RIGHT 2