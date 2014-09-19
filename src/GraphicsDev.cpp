#include "GraphicsDev.h"
#include "MouseDev4.h"
#include "resource.h"
#include <stdio.h>

MouseDev4 * GraphicsDev::m_pgMouse = NULL;

void LLGObj::Add(GraphicsObj *pGObj)
{
	LinkedList::Add((void *) pGObj);
}

GraphicsObj * LLGObj::GetNext()
{
	return (GraphicsObj *) LinkedList::GetNext();
}

GraphicsDev::GraphicsDev(HWND hwnd, COLORREF fore, COLORREF back, COLORREF txt)
{
	m_hwnd = hwnd;

	RECT r;
	GetClientRect(m_hwnd, &r);
	m_width = r.right - r.left;
	m_height = r.bottom - r.top;

	::ShowCursor(TRUE);
	m_bCursorOn = TRUE;

	//m_width = GetSystemMetrics(SM_CXSCREEN);
	//m_height = GetSystemMetrics(SM_CYSCREEN);

	m_fScalingFactorX = ((float) (r.right - r.left)) / GetSystemMetrics(SM_CXVIRTUALSCREEN);
	m_fScalingFactorY = ((float) (r.bottom - r.top)) / GetSystemMetrics(SM_CYVIRTUALSCREEN);


	m_hdc = GetDC(m_hwnd);

	m_dwType = EC_DEVTYPE_DISPLAY;

	if (back == NULL) {
		backBrush = (HBRUSH) CreateSolidBrush(RGB(0,0,0));
		SetBkColor(m_hdc, RGB(0,0,0));
		m_rgbBackground = RGB(0,0,0);
	} else {
		backBrush = (HBRUSH) CreateSolidBrush(back);
		SetBkColor(m_hdc, back);
		m_rgbBackground = back;
	}

	if (txt == NULL)
		SetTextColor(m_hdc, RGB(255, 255, 255));
	else
		SetTextColor(m_hdc, txt);

	if (fore == NULL)
		m_rgbForeground = RGB(255,255,255);
	else
		m_rgbForeground = fore;

	m_mdc = CreateCompatibleDC(m_hdc);
	m_bm = CreateCompatibleBitmap(m_hdc, m_width, m_height);
	SelectObject(m_mdc, m_bm);

	forePen = CreatePen(PS_SOLID, 2, m_rgbForeground);
	foreBrush = CreateSolidBrush(m_rgbForeground);
	backPen = CreatePen(PS_SOLID, 2, m_rgbBackground);

	blkBrush = (HBRUSH) GetStockObject(BLACK_BRUSH);
	whtPen = (HPEN) GetStockObject(WHITE_PEN);
	wPen2 = (HPEN) CreatePen(PS_SOLID, 3, RGB(255,255,255));
	thickBlue = CreatePen(PS_SOLID, 2, RGB(0, 128, 128));
	thickBlk = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	thickRed = CreatePen(PS_SOLID, 2, RGB(128, 0, 0));
	hollow = (HBRUSH) GetStockObject(HOLLOW_BRUSH);
	redBrush = CreateSolidBrush(RGB(255, 0, 0));
	redPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));

	oldBrush = NULL;
	oldPen = NULL;


	InitializeCriticalSection(&m_cSection);
}

GraphicsDev::~GraphicsDev()
{
	if (m_hdc)
		ReleaseDC(m_hwnd, m_hdc);

	if (m_mdc)
		ReleaseDC(m_hwnd, m_mdc);

	if (m_bm)
		DeleteObject(m_bm);

	DeleteObject(forePen);
	DeleteObject(foreBrush);
	DeleteObject(backBrush);
	DeleteObject(backPen);
	DeleteObject(blkBrush);
	DeleteObject(whtPen);
	DeleteObject(thickBlue);
	DeleteObject(thickBlk);
	DeleteObject(thickRed);
	DeleteObject(hollow);
	DeleteObject(redBrush);
	DeleteObject(redPen);

	DeleteCriticalSection(&m_cSection);
}

int GraphicsDev::Alert(char *msg, HDC hdc)
{
	RECT rect;
	
	if (hdc == NULL)
		hdc = m_hdc;

	ClearScreen(hdc);
	rect.left = rect.top = 0;
	rect.right = m_width;
	rect.bottom = m_height;
	//rect.top = rect.bottom = nSystemY;

	Lock();
	::DrawText(hdc, msg, strlen(msg), &rect, DT_CENTER | DT_CALCRECT);
	Unlock();
	rect.left = (m_width - (rect.right - rect.left)) / 2;
	rect.right += rect.left;
	rect.top = (m_height - (rect.bottom - rect.top)) / 2;
	rect.bottom += rect.top;
	Lock();
	::DrawText(hdc, msg, strlen(msg), &rect, DT_CENTER);
	Unlock();

	return 0;
}

void GraphicsDev::ClearScreen(int nDC)
{
	Lock();
	switch (nDC) {
	case GD_MEMORY :
		ClearScreen(m_mdc);
		break;
	case GD_SCREEN :
		ClearScreen(m_hdc);
		break;
	}
	Unlock();
}

void GraphicsDev::ClearScreen(HDC hdc)
{
	oldBrush = (HBRUSH) SelectObject(hdc, backBrush);
	Lock();
	Rectangle(hdc, 0, 0, m_width, m_height);
	SelectObject(hdc, oldBrush);
	Unlock();
}

int GraphicsDev::Display()
{
	Lock();
	//BitBlt(m_hdc, 0, 0, 1024, 768, m_mdc, 0, 0, SRCCOPY);
	BitBlt(m_hdc, 3, 3, m_width - 6, m_height - 6, m_mdc, 3, 3, SRCCOPY);
	Unlock();
	return 0;
}

int GraphicsDev::DrawBitmap(AOI *pAOI, int nDC)
{
	if (!pAOI)
		return 1;

	if (nDC == GD_SCREEN)
		return DrawBitmap(pAOI, m_hdc);
	else
		return DrawBitmap(pAOI, m_mdc);
}

int GraphicsDev::DrawBitmap(AOI *pAOI, HDC hdc)
{
	static char buf[MAX_PATH+1];
	sprintf(buf, "resource\\%s", pAOI->m_pcRes);
	if (pAOI)
	{
		return DrawBitmap(
			buf,
			pAOI->m_x1,
			pAOI->m_y1,
			pAOI->m_x2,
			pAOI->m_y2,
			hdc);
	}
	else
		return 1;
}

int GraphicsDev::DrawBitmap(char *fname, int x1, int y1, HDC hdc)
{
	return DrawBitmap(fname, x1, y1, x1-1, y1-1, hdc);
}

int GraphicsDev::DrawBitmap(char *fname, int x1, int y1, int nDC, int nFile)
{
	if (nDC == GD_SCREEN) {
		if (nFile)
			return DrawBitmap(fname, x1, y1, x1 - 1, y1 - 1, m_hdc);
		else
			return DrawBitmap(fname, x1, y1, x1 - 1, y1 - 1, m_hdc, 0);
	} else {
		if (nFile)
			return DrawBitmap(fname, x1, y1, x1 - 1, y1 - 1, m_mdc);
		else
			return DrawBitmap(fname, x1, y1, x1 - 1, y1 - 1, m_mdc, 0);
	}

	return 0;
}

int GraphicsDev::DrawBitmap(char *fname, int x1, int y1, int x2, int y2, HDC hdc, int nFile)
{
	int w, h;

	// load in the bitmap
	HBITMAP bm1 = NULL;
	if (nFile)
		bm1 = LoadBitmap(fname, &w, &h);
	else
		bm1 = LoadBitmap(fname, &w, &h, 0);

	if (!bm1) {
		MessageBox(NULL, fname, "Error Loading Bitmap", MB_OK);
	}

	if ((x2 <= x1) || (y2 <= y1)) {
		x2 = x1 + w;
		y2 = y1 + h;
	}
	
	DrawBitmap(bm1, x1, y1, x2, y2, hdc);
	DeleteObject(bm1);

	return 0;
}

int GraphicsDev::DrawBitmap(char *fname, int x1, int y1, int x2, int y2, int nDC) {
	int nResult = 1;
	
	switch (nDC) {
	case GD_MEMORY :
		nResult = DrawBitmap(fname, x1, y1, x2, y2, m_mdc);
		break;
	case GD_SCREEN :
		nResult = DrawBitmap(fname, x1, y1, x2, y2, m_hdc);
		break;
	}

	return nResult;
}

int GraphicsDev::DrawBitmap(HBITMAP hbm, int x1, int y1, int x2, int y2, int nDC) {
	int nResult = 1;
	
	switch (nDC) {
	case GD_MEMORY :
		nResult = DrawBitmap(hbm, x1, y1, x2, y2, m_mdc);
		break;
	case GD_SCREEN :
		nResult = DrawBitmap(hbm, x1, y1, x2, y2, m_hdc);
		break;
	}

	return nResult;
}

int GraphicsDev::DrawBitmapTrans(HBITMAP hbm, int x1, int y1, int x2, int y2, int nDC) {
	int nResult = 1;
	
	switch (nDC) {
	case GD_MEMORY :
		nResult = DrawBitmapTrans(hbm, x1, y1, x2, y2, m_mdc);
		break;
	case GD_SCREEN :
		nResult = DrawBitmapTrans(hbm, x1, y1, x2, y2, m_hdc);
		break;
	}

	return nResult;
}

int GraphicsDev::DrawBitmap(HBITMAP hbm, int x1, int y1, int x2, int y2, HDC hdc)
{
	Lock();

	if (!hdc)
		hdc = m_hdc;

	HDC hdc1;
	HBITMAP obm;
	hdc1 = CreateCompatibleDC(hdc);

	obm = (HBITMAP) SelectObject(hdc1, hbm);

	BitBlt(hdc, x1, y1, x2 - x1, y2 - y1, hdc1, 0, 0, SRCCOPY);

	SelectObject(hdc1, obm);
	DeleteDC(hdc1);
	Unlock();

	return 0;
}

int GraphicsDev::DrawBitmapTrans(HBITMAP hbm, int x1, int y1, int x2, int y2, HDC hdc)
{
	Lock();

	if (!hdc)
		hdc = m_hdc;

	HDC hdc1;
	HBITMAP obm;
	hdc1 = CreateCompatibleDC(hdc);

	obm = (HBITMAP) SelectObject(hdc1, hbm);

	BitBlt(hdc, x1, y1, x2 - x1, y2 - y1, hdc1, 0, 0, SRCCOPY);

	SelectObject(hdc1, obm);
	DeleteDC(hdc1);
	Unlock();

	return 0;
}


int GraphicsDev::DrawBorder(int x1, int y1, int x2, int y2, HDC tdc)
{
	oldBrush = (HBRUSH) SelectObject(tdc, GetStockObject(NULL_BRUSH));
	oldPen = (HPEN) SelectObject(tdc, whtPen);
	Lock();
	Rectangle(tdc, x1, y1, x2, y2);
	Unlock();
	SelectObject(tdc, oldBrush);
	SelectObject(tdc, oldPen);

	return 0;
}

int GraphicsDev::DrawBorder(int x1, int y1, int x2, int y2, int nWidth)
{
	HPEN hp = CreatePen(PS_SOLID, nWidth, RGB(255,255,0));

	Lock();
	HPEN oldPen = (HPEN) SelectObject(m_hdc, hp);
	HBRUSH oldBrush = (HBRUSH) SelectObject(m_hdc, GetStockObject(NULL_BRUSH));

	Rectangle(m_hdc, x1, y1, x2, y2);

	SelectObject(m_hdc, oldPen);
	SelectObject(m_hdc, oldBrush);
	Unlock();
	DeleteObject(hp);

	return 0;
}

int GraphicsDev::HighlightRegion(int x1, int y1, int x2, int y2, int nMode, int nDC)
{
	HDC tdc;

	if (nDC == GD_SCREEN)
		tdc = m_hdc;
	else
		tdc = m_mdc;

	Lock();
	oldBrush = (HBRUSH) SelectObject(tdc, GetStockObject(NULL_BRUSH));

	if (nMode == GD_HIGHLIGHT_ON)
		oldPen = (HPEN) SelectObject(tdc, thickBlue);
	else
		oldPen = (HPEN) SelectObject(tdc, backPen);

	Rectangle(tdc, x1, y1, x2, y2);

	SelectObject(tdc, oldBrush);
	SelectObject(tdc, oldPen);
	Unlock();

	return 0;
}

int GraphicsDev::HighlightRegion(COLORREF rgb, int x1, int y1, int x2, int y2, int nMode, int nDC)
{
	Lock();
	HDC tdc;
	HPEN pen = CreatePen(PS_SOLID, 2, rgb);

	if (nDC == GD_SCREEN)
		tdc = m_hdc;
	else
		tdc = m_mdc;

	oldBrush = (HBRUSH) SelectObject(tdc, GetStockObject(NULL_BRUSH));

	if (nMode == GD_HIGHLIGHT_ON)
		oldPen = (HPEN) SelectObject(tdc, pen);
	else
		oldPen = (HPEN) SelectObject(tdc, backPen);

	Rectangle(tdc, x1, y1, x2, y2);

	SelectObject(tdc, oldBrush);
	SelectObject(tdc, oldPen);

	DeleteObject(pen);
	Unlock();

	return 0;
}

int GraphicsDev::HighlightRegionRed(int x1, int y1, int x2, int y2, HDC tdc)
{
	Lock();
	oldBrush = (HBRUSH) SelectObject(tdc, GetStockObject(HOLLOW_BRUSH));
	oldPen = (HPEN) SelectObject(tdc, thickRed);
	Rectangle(tdc, x1, y1, x2, y2);
	SelectObject(tdc, oldBrush);
	SelectObject(tdc, oldPen);
	Unlock();

	return 0;
}

int GraphicsDev::DrawHCenteredText(int y, char *txt)
{
	return DrawHCenteredText(y, txt, m_hdc);
}

int GraphicsDev::DrawHCenteredText(int y, char *txt, HDC hdc)
{
	RECT rect;
	
	if (hdc == NULL)
		hdc = m_hdc;

	rect.left = 0;
	rect.top = y;
	rect.right = m_width;
	rect.bottom = m_height;
	int w, h;

	Lock();
	::DrawText(hdc, txt, strlen(txt), &rect, DT_CENTER | DT_CALCRECT);
	w = rect.right - rect.left;
	h = rect.bottom - rect.top;
	rect.left = (m_width - w) / 2;
	rect.right = rect.left + w;
	rect.top = y;
	rect.bottom = rect.top + h;
	::DrawText(hdc, txt, strlen(txt), &rect, DT_CENTER);
	Unlock();

	return 0;	
}

int GraphicsDev::DrawFixationX(int x, int y)
{
	// default parameters: middle of screen
	if (x < 0) {
		x = m_width / 2;
		y = m_height / 2;
	}

	Lock();
	oldPen = (HPEN) SelectObject(m_hdc, redPen);

	MoveToEx(m_hdc, x - 10, y - 10, NULL);
	LineTo(m_hdc, x + 10, y + 10);
	MoveToEx(m_hdc, x + 10, y - 10, NULL);
	LineTo(m_hdc, x - 10, y + 10);

	SelectObject(m_hdc, oldPen);
	Unlock();

	return 0;
}

int GraphicsDev::DrawFixationCross(int x, int y)
{
	// default parameters: middle of screen
	if (x < 0) {
		x = m_width / 2;
		y = m_height / 2;
	}

	Lock();
	oldPen = (HPEN) SelectObject(m_hdc, whtPen);

	MoveToEx(m_hdc, x, y - 10, NULL);
	LineTo(m_hdc, x, y + 10);
	MoveToEx(m_hdc, x - 10, y, NULL);
	LineTo(m_hdc, x + 10, y);

	SelectObject(m_hdc, oldPen);
	Unlock();

	return 0;
}

HBITMAP GraphicsDev::LoadBitmap(char *fname, int *px, int *py, int nFile)
{
	char strTmp[256];
	BITMAP bminf1;
	HBITMAP bm1;
	DWORD dwError = 0;
	

	if (nFile)
		bm1 = (HBITMAP) LoadImage(NULL, fname, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	
	dwError = GetLastError();

	if (dwError != ERROR_SUCCESS) {
		switch (dwError) {
			case ERROR_NOT_ENOUGH_MEMORY :
			{
				MessageBox(NULL, "Error: not enough memory to load the bitmap.", fname, MB_OK);
			}
			break;
		case ERROR_FILE_NOT_FOUND :
			{
				MessageBox(NULL, "Error: file not found.", fname, MB_OK);
			}
			break;
		default :
			{
				MessageBox(NULL, "An unknown error occurred\n while loading the bitmap.", fname, MB_OK);
			}
		}
	}
	//else
		//bm1 = (HBITMAP) LoadImage(m_hInstance, MAKEINTRESOURCE(IDB_FIXPT), IMAGE_BITMAP, 0, 0, 0);
	
	if (bm1 == NULL) {
		// try to load it again, adding the three character extension.
		sprintf(strTmp, "%s.bmp", fname);
		bm1 = (HBITMAP) LoadImage(NULL, strTmp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if (bm1 == NULL) {
			DWORD dwError = GetLastError();
			return NULL;
		}
	}

	// figure out its dimensions
	GetObject(bm1, sizeof(bminf1), &bminf1);
	*px = bminf1.bmWidth;
	*py = bminf1.bmHeight;

	return bm1;
}

int GraphicsDev::Display(int x1, int y1, int w, int h, HDC hdc)
{
	Lock();
	BitBlt(m_hdc, x1, y1, w, h, hdc, 0, 0, SRCCOPY);
	Unlock();

	return 0;
}

void GraphicsDev::DrawGrid(int x, int y, Grid *pGrid, int nDC)
{
	if (nDC == GD_SCREEN)
		DrawGrid(x, y, pGrid, m_hdc);
	else
		DrawGrid(x, y, pGrid, m_mdc);
}

void GraphicsDev::DrawGrid(int x, int y, Grid *pGrid, HDC hdc)
{
	int i;
	
	if (pGrid->m_rgb == NULL)
	{
		pGrid->m_rgb = m_rgbForeground;
	}

	if (hdc == NULL)
	{
		// default is screen DC
		hdc = m_hdc;
	}

	HPEN NewPen = CreatePen(PS_SOLID, 3, pGrid->m_rgb);
	Lock();
	oldPen = (HPEN) SelectObject(hdc, NewPen);
	HBRUSH oldBrush = (HBRUSH) SelectObject(hdc, GetStockObject(HOLLOW_BRUSH));

	Rectangle(hdc, x, y, x + pGrid->m_nCols * pGrid->m_nColWidth,
		y + pGrid->m_nRows * pGrid->m_nRowHeight);

	for (i = 1; i < pGrid->m_nRows; i++)
	{
		MoveToEx(
			hdc,
			x,
			y + i * pGrid->m_nRowHeight,
			NULL);

		LineTo(
			hdc,
			x + (pGrid->m_nCols * pGrid->m_nColWidth),
			y + i * pGrid->m_nRowHeight);
	}

	for (i = 1; i < pGrid->m_nCols; i++)
	{
		MoveToEx(
			hdc,
			x + (i * pGrid->m_nColWidth),
			y,
			NULL);
		LineTo(
			hdc,
			x + (i * pGrid->m_nColWidth),
			y + (pGrid->m_nRows * pGrid->m_nRowHeight) );
	}

	DeleteObject(SelectObject(hdc, oldPen));
	SelectObject(hdc, oldBrush);
	Unlock();
}

void GraphicsDev::ClearRegion(int x1, int y1, int x2, int y2, int nDC)
{
	HDC hdc;

	if (nDC == GD_SCREEN)
		hdc = m_hdc;
	else
		hdc = m_mdc;

	Lock();
	oldBrush = (HBRUSH) SelectObject(hdc, backBrush);
	oldPen = (HPEN) SelectObject(hdc, GetStockObject(NULL_PEN));

	Rectangle(hdc, x1, y1, x2, y2);

	SelectObject(hdc, oldBrush);
	SelectObject(hdc, oldPen);
	Unlock();
}

void GraphicsDev::DrawAllObj(int nMode)
{
	if (nMode == GD_MEMORY)
		DrawAllObj(m_mdc);
	else
		DrawAllObj(m_hdc);
}

void GraphicsDev::DrawAllObj(HDC hdc)
{
	GraphicsObj *pGObj;

	m_llGObj.GotoHead();
	while ( (pGObj = m_llGObj.GetNext()) )
		pGObj->Draw(hdc);
}

void GraphicsDev::FlipBitmap(GBitmap *pGBm)
{
	int i;
	HDC mdc;
	HBITMAP obm;
	int x = pGBm->m_x;
	int y = pGBm->m_y;
	int w = pGBm->m_width;
	int h = pGBm->m_height;

	mdc = CreateCompatibleDC(m_hdc);
	obm = (HBITMAP) SelectObject(mdc, pGBm->m_bm);

	// BLIT IT TO THE SCREEN UPSIDE DOWN
	Lock();
	for (i = 0; i < h; i++)
		BitBlt(m_hdc, x, y + i, w, 1, mdc, 0, h - i - 1, SRCCOPY);

	// NOW, REVERSE IT BACK INTO MEMORY
	BitBlt(mdc, 0, 0, w, h, m_hdc, x, y, SRCCOPY);

	SelectObject(mdc, obm);
	DeleteDC(mdc);
	Unlock();
}

void GraphicsDev::FlipRegion(int x1, int y1, int x2, int y2)
{
	int i;
	HDC mdc;
	HBITMAP obm;

	int w = x2 - x1;
	int h = y2 - y1;

	mdc = CreateCompatibleDC(m_hdc);
	Lock();
	HBITMAP bm = CreateCompatibleBitmap(m_hdc, w, h);

	obm = (HBITMAP) SelectObject(mdc, bm);

	// BLIT IT TO MEMORY UPSIDE DOWN
	for (i = 0; i < h; i++)
		BitBlt(mdc, 0, i, w, 1, m_hdc, x1, y2 - i - 1, SRCCOPY);

	// NOW, COPY IT BACK ONTO SCREEN
	BitBlt(m_hdc, x1, y1, w, h, mdc, 0, 0, SRCCOPY);

	SelectObject(mdc, obm);
	DeleteObject(bm);
	DeleteDC(mdc);
	Unlock();
}

void GraphicsDev::DrawText(char *str, int x1, int y1, int x2, int y2, int nFlags)
{
	RECT rect;
	rect.left = x1;
	rect.top = y1;
	rect.right = x2;
	rect.bottom = y2;
	Lock();
	::DrawText(m_hdc, str, strlen(str), &rect, nFlags);
	Unlock();
}


void GraphicsDev::Lock()
{
	EnterCriticalSection(&m_cSection);
	if (m_pgMouse) {
		if (m_pgMouse->m_dwCursorActive) {
			DrawMouseCursor(m_pgMouse->m_x, m_pgMouse->m_y); // erase the old
		}
	}
}

void GraphicsDev::Unlock()
{
	if (m_pgMouse) {
		if (m_pgMouse->m_dwCursorActive) {
			DrawMouseCursor(m_pgMouse->m_x, m_pgMouse->m_y); // redraw
		}
	}
	LeaveCriticalSection(&m_cSection);
}

void GraphicsDev::LockNoMouseRedraw()
{
	EnterCriticalSection(&m_cSection);
}

void GraphicsDev::UnlockNoMouseRedraw()
{
	LeaveCriticalSection(&m_cSection);
}


void GraphicsDev::DrawMouseCursor(int x, int y)
{
	SetROP2(m_hdc, R2_NOT);

	HPEN OldPen = (HPEN) SelectObject(m_hdc, GetStockObject(WHITE_PEN));
	HBRUSH OldBrush = (HBRUSH) SelectObject(m_hdc, GetStockObject(HOLLOW_BRUSH));
	
	Ellipse(m_hdc, x - 12, y - 12, x + 12, y + 12);
	MoveToEx(m_hdc, x, y - 14, NULL);
	LineTo(m_hdc, x, y + 14);
	MoveToEx(m_hdc, x - 14, y, NULL);
	LineTo(m_hdc, x + 14, y);
	
	DeleteObject(SelectObject(m_hdc, OldPen));
	DeleteObject(SelectObject(m_hdc, OldBrush));
	SetROP2(m_hdc, R2_COPYPEN);
}

int GraphicsDev::CalcTextRect(RECT *pRect, char *pc)
{
	Lock();
	::DrawText(m_hdc, pc, strlen(pc), pRect, DT_CALCRECT | DT_LEFT);
	Unlock();

	return 0;
}

void GraphicsDev::DrawCalibGrid()
{
	HPEN hp = CreatePen(PS_SOLID, 8, RGB(255,255,0));
	Lock();
	oldPen = (HPEN) SelectObject(m_hdc, hp);
	oldBrush = (HBRUSH) SelectObject(m_hdc, GetStockObject(NULL_BRUSH));

	Rectangle(m_hdc, 0, 0, 1024, 768);
	SelectObject(m_hdc, oldPen);
	SelectObject(m_hdc, oldBrush);
	DeleteObject(hp);
	Unlock();

	DrawCalibCross(152,127);
	DrawCalibCross(872,127);
	DrawCalibCross(512,384);
	DrawCalibCross(152,621);
	DrawCalibCross(872,621);
}

void GraphicsDev::DrawCalibCross(int x, int y)
{
	HPEN hp = CreatePen(PS_SOLID, 5, RGB(255,255,255));
	HPEN oldPen;

	Lock();

	oldPen = (HPEN) SelectObject(m_hdc, hp);

	MoveToEx(m_hdc, x, y - 10, NULL);
	LineTo(m_hdc, x, y + 10);
	MoveToEx(m_hdc, x - 10, y, NULL);
	LineTo(m_hdc, x + 10, y);

	DeleteObject(SelectObject(m_hdc, oldPen));
	Unlock();
}

void GraphicsDev::DrawEyeXHair(int x1, int y1)
{
	int x = 0;
	int y = 0;

	//Lock();
	SetROP2(m_hdc, R2_NOT);

	x = gx(x1);
	y = gy(y1);

	MoveToEx(m_hdc, x, y - 5, NULL);
	LineTo(m_hdc, x, y + 5);
	MoveToEx(m_hdc, x - 5, y, NULL);
	LineTo(m_hdc, x + 5, y);

	SetROP2(m_hdc, R2_COPYPEN);
	//Unlock();
}

int GraphicsDev::gx(int x)
{
	return (int) (m_fScalingFactorX * x);
}

int GraphicsDev::gy(int y)
{
	return (int) (m_fScalingFactorY * y);
}

int GraphicsDev::DrawBitmapStretch(char * fname, int x1, int y1, int x2, int y2, HDC hdc)
{
	int w, h;

	// load in the bitmap
	HBITMAP bm1 = NULL;
	bm1 = LoadBitmap(fname, &w, &h);

	HDC hdc1;
	HBITMAP obm;
	hdc1 = CreateCompatibleDC(hdc);

	obm = (HBITMAP) SelectObject(hdc1, bm1);

	int w2 = gx(w);
	int h2 = gy(h);

	StretchBlt(hdc, gx(x1), gy(y1), gx(w), gy(h), hdc1, 0, 0, w, h, SRCCOPY);
	//BitBlt(hdc, x1, y1, x2 - x1, y2 - y1, hdc1, 0, 0, SRCCOPY);

	SelectObject(hdc1, obm);
	DeleteDC(hdc1);

	return 0;
}

int GraphicsDev::GetWidth()
{
	RECT r;

	GetClientRect(m_hwnd, &r);
	return (r.right - r.left);
}

int GraphicsDev::GetHeight()
{
	RECT r;

	GetClientRect(m_hwnd, &r);
	return (r.bottom - r.top);
}

void GraphicsDev::Line(int x0, int y0, int x1, int y1, BOOL bErase)
{
	HPEN oldpen;

	Lock();
	if (bErase) {
		oldpen = (HPEN) SelectObject(m_hdc, backPen);
	} else {
		oldpen = (HPEN) SelectObject(m_hdc, forePen);
	}

	MoveToEx(m_hdc, x0, y0, NULL);
	LineTo(m_hdc, x1, y1);

	SelectObject(m_hdc, oldpen);

	Unlock();
}

void GraphicsDev::Circle(int x, int y, int r, BOOL bErase)
{
	HPEN oldpen;
	HBRUSH oldbrush;
	
	Lock();
	if (bErase) {
		oldpen = (HPEN) SelectObject(m_hdc, backPen);
	} else {
		oldpen = (HPEN) SelectObject(m_hdc, forePen);
	}
	oldbrush = (HBRUSH) SelectObject(m_hdc, hollow);

	Ellipse(m_hdc, x - r, y - r, x + r, y + r);

	SelectObject(m_hdc, oldpen);
	SelectObject(m_hdc, oldbrush);

	Unlock();
}

void GraphicsDev::CircleFill(int x, int y, int r, BOOL bErase)
{
	HPEN oldpen;
	HBRUSH oldbrush;
	
	Lock();
	if (bErase) {
		oldpen = (HPEN) SelectObject(m_hdc, backPen);

	} else {
		oldpen = (HPEN) SelectObject(m_hdc, forePen);
	}
	oldbrush = (HBRUSH) SelectObject(m_hdc, backBrush);
	
	Ellipse(m_hdc, x - r, y - r, x + r, y + r);

	SelectObject(m_hdc, oldpen);
	SelectObject(m_hdc, oldbrush);

	Unlock();
}

void GraphicsDev::RectFill(int x0, int y0, int x1, int y1, BOOL bErase)
{
	HPEN oldpen;
	HBRUSH oldbrush;
	
	Lock();
	if (bErase) {
		oldpen = (HPEN) SelectObject(m_hdc, backPen);

	} else {
		oldpen = (HPEN) SelectObject(m_hdc, forePen);
	}
	oldbrush = (HBRUSH) SelectObject(m_hdc, backBrush);
	
	Rectangle(m_hdc, x0, y0, x1, y1);

	SelectObject(m_hdc, oldpen);
	SelectObject(m_hdc, oldbrush);

	Unlock();

}

HBITMAP GraphicsDev::LoadBitmapStretch(char *pcFile, int * pw, int * ph)
{
	if (!pcFile)
		return NULL;

	// load in the bitmap
	HBITMAP bm1 = NULL;
	HBITMAP bm2 = NULL;
	HDC hdc1, hdc2;
	HBITMAP obm1, obm2;
	int w, h;

	bm1 = LoadBitmap(pcFile, &w, &h);
	*pw = gx(w);
	*ph = gy(h);

	bm2 = CreateCompatibleBitmap(m_hdc, *pw, *ph);

	hdc1 = CreateCompatibleDC(m_hdc);
	hdc2 = CreateCompatibleDC(m_hdc);

	obm1 = (HBITMAP) SelectObject(hdc1, bm1);
	obm2 = (HBITMAP) SelectObject(hdc2, bm2);

	StretchBlt(hdc2, 0, 0, *pw, *ph, hdc1, 0, 0, w, h, SRCCOPY);
	//BitBlt(hdc, x1, y1, x2 - x1, y2 - y1, hdc1, 0, 0, SRCCOPY);

	SelectObject(hdc1, obm1);
	SelectObject(hdc2, obm2);
	
	DeleteDC(hdc1);
	DeleteDC(hdc2);

	DeleteObject(bm1);

	return bm2;
}

void GraphicsDev::SwapRegions(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
	int w = (x4 - x3);
	int h = (y4 - y3);

	HDC mdc1 = CreateCompatibleDC(m_hdc);
	HDC mdc2 = CreateCompatibleDC(m_hdc);
	HBITMAP mbm1 = CreateCompatibleBitmap(m_hdc, w, h);
	HBITMAP mbm2 = CreateCompatibleBitmap(m_hdc, w, h);

	HBITMAP obm1 = (HBITMAP) SelectObject(mdc1, mbm1);
	HBITMAP obm2 = (HBITMAP) SelectObject(mdc2, mbm2);

	BitBlt(mdc1, 0, 0, w, h, m_hdc, x3, y3, SRCCOPY);
	BitBlt(mdc2, 0, 0, w, h, m_hdc, x1, y1, SRCCOPY);
 
	BitBlt(m_hdc, x3, y3, w, h, mdc2, 0, 0, SRCCOPY);
	BitBlt(m_hdc, x1, y1, w, h, mdc1, 0, 0, SRCCOPY);
	
	DeleteObject(SelectObject(mdc2, obm2));
	DeleteObject(SelectObject(mdc1, obm1));
	//ReleaseDC(mdc);
	DeleteDC(mdc1);
	DeleteDC(mdc2);
}

void GraphicsDev::HighlightRegion(AOI *pAOI, COLORREF rgb, int nMode)
{
	HighlightRegion(rgb, pAOI->m_x1-5, pAOI->m_y1-5, pAOI->m_x2+5, pAOI->m_y2+5, nMode);
}

HBITMAP GraphicsDev::MakeDot(AOI *pAOI, int x1, int y1, int r)
{

	if (!pAOI)
		return NULL;

	// load in the bitmap
	HBITMAP bm1 = NULL;
	HBITMAP bm2 = NULL;
	HDC hdc1, hdc2;
	HBITMAP obm1, obm2;
	HPEN oldpen1;
	HBRUSH oldbrush1;
	int w, h;

	bm1 = LoadBitmap(pAOI->m_pcRes, &w, &h);
	bm2 = CreateCompatibleBitmap(m_hdc, 2 * r, 2 * r);

	hdc1 = CreateCompatibleDC(m_hdc);
	hdc2 = CreateCompatibleDC(m_hdc);

	obm1 = (HBITMAP) SelectObject(hdc1, bm1);
	obm2 = (HBITMAP) SelectObject(hdc2, bm2);

	//StretchBlt(hdc2, 0, 0, *pw, *ph, hdc1, 0, 0, w, h, SRCCOPY);
	BitBlt(hdc2, 0, 0, 2 * r, 2 * r, hdc1, x1 - pAOI->m_x1, y1 - pAOI->m_y1, SRCCOPY);
	oldpen1 = (HPEN) SelectObject(hdc2, GetStockObject(WHITE_PEN));
	oldbrush1 = (HBRUSH) SelectObject(hdc2, GetStockObject(BLACK_BRUSH));
	Ellipse(hdc2, 0, 0, 2 * r, 2 * r);
	SelectObject(hdc2, oldpen1);
	SelectObject(hdc2, oldbrush1);

	SelectObject(hdc1, obm1);
	SelectObject(hdc2, obm2);
	
	DeleteDC(hdc1);
	DeleteDC(hdc2);

	DeleteObject(bm1);

	return bm2;
}

int GraphicsDev::ShowCursor(bool bShow /* = TRUE */)
{
	int n = 0;

	if (bShow) {
		if (!m_bCursorOn) {
			do {
				n = ::ShowCursor(TRUE);
			} while (n < 0);
			m_bCursorOn = TRUE;
		}
	} else {
		if (m_bCursorOn) {
			do {
				n = ::ShowCursor(FALSE);
			} while (n >= 0);
			m_bCursorOn = FALSE;
		}
	}

	return 0;
}

int GraphicsDev::ShowSoftBreak(void)
{
	int xTop = (int) GetSystemMetrics(SM_CXVIRTUALSCREEN) - 20;
	int yTop = (int) GetSystemMetrics(SM_CYVIRTUALSCREEN) - 20;

	Lock();
	HBRUSH oldbrush;
	HPEN oldpen;
	oldpen = (HPEN) SelectObject(m_hdc, forePen);
	oldbrush = (HBRUSH) SelectObject(m_hdc, backBrush);

	MoveToEx(m_hdc, xTop, yTop, NULL);
	LineTo(m_hdc, xTop+15, yTop+15);
	MoveToEx(m_hdc, xTop+15, yTop, NULL);
	LineTo(m_hdc, xTop, yTop+15);

	SelectObject(m_hdc, oldpen);
	SelectObject(m_hdc, oldbrush);
	Unlock();

	return 0;
}

int GraphicsDev::Overlay(HBITMAP hbmSource, HBITMAP hbmDest)
{
	int i;
	HDC mdcDest, mdcSource;
	HBITMAP obmDest, obmSource;
	BITMAP bminf1;

	GetObject(hbmSource, sizeof(bminf1), &bminf1);
	int width = bminf1.bmWidth;
	int height = bminf1.bmHeight;

	mdcDest = CreateCompatibleDC(m_hdc);
	mdcSource = CreateCompatibleDC(m_hdc);
	obmDest = (HBITMAP) SelectObject(mdcDest, hbmDest);
	obmSource = (HBITMAP) SelectObject(mdcSource, hbmSource);

	TransparentBlt(mdcDest, 0, 0, width, height,
		mdcSource, 0, 0, width, height, RGB(0,0,0));

	SelectObject(mdcDest, obmDest);
	SelectObject(mdcSource, obmSource);
	DeleteDC(mdcDest);
	DeleteDC(mdcSource);
	//	DeleteObject(bminf1);

	return 0;
}

int GraphicsDev::Replace(HBITMAP hbmSource, HBITMAP hbmDest)
{
	HDC mdcDest, mdcSource;
	HBITMAP obmDest, obmSource;
	BITMAP bminf1;

	GetObject(hbmSource, sizeof(bminf1), &bminf1);
	int width = bminf1.bmWidth;
	int height = bminf1.bmHeight;

	mdcDest = CreateCompatibleDC(m_hdc);
	mdcSource = CreateCompatibleDC(m_hdc);
	obmDest = (HBITMAP) SelectObject(mdcDest, hbmDest);
	obmSource = (HBITMAP) SelectObject(mdcSource, hbmSource);

	BitBlt(mdcDest, 0, 0, width, height,
		mdcSource, 0, 0, SRCCOPY);

	SelectObject(mdcDest, obmDest);
	SelectObject(mdcSource, obmSource);
	DeleteDC(mdcDest);
	DeleteDC(mdcSource);
	//	DeleteObject(bminf1);

	return 0;
}
