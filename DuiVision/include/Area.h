// ����ؼ���������������Ľ���͸���ȣ�������Ӧ���
#pragma once
#include "ControlBase.h"

class CArea : public CControlBase
{
	DUIOBJ_DECLARE_CLASS_NAME(CArea, _T("area"))
public:
	CArea(HWND hWnd, CDuiObject* pDuiObject);
	CArea(HWND hWnd, CDuiObject* pDuiObject, UINT uControlID, CRect rc, int nBeginTransparent = 50, int nEndTransparent = 50, 
		COLORREF clr = RGB(255, 255, 255), BOOL bIsVisible = TRUE);
	virtual ~CArea(void);

protected:
	virtual void DrawControl(CDC &dc, CRect rcUpdate);

public:
	int		m_nBeginTransparent;
	int		m_nEndTransparent;
	COLORREF m_clr;

	DUI_DECLARE_ATTRIBUTES_BEGIN()
		DUI_RGBCOLOR_ATTRIBUTE(_T("color"), m_clr, FALSE)
		DUI_INT_ATTRIBUTE(_T("begin-transparent"), m_nBeginTransparent, FALSE)
		DUI_INT_ATTRIBUTE(_T("end-transparent"), m_nEndTransparent, FALSE)
	DUI_DECLARE_ATTRIBUTES_END()
};
