////////////////////////////////////////////////////////////////
// MSDN -- September 2000
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual C++ 6.0, runs on Windows 98 and probably NT too.
//
#pragma once

//////////////////
// Get NONCLIENTMETRICS info: ctor calls SystemParametersInfo.
//
class CNonClientMetrics : public NONCLIENTMETRICS {
public:
	CNonClientMetrics() {
		cbSize = sizeof(NONCLIENTMETRICS);
		SystemParametersInfo(SPI_GETNONCLIENTMETRICS,0,this,0);
	}
};

//////////////////
// Popup text window, like tooltip.
// Can be right or left justified relative to creation point.
//
class CPopupText : public CWnd {
public:
	CSize m_szMargins;		// extra space around text: change if you like
	enum {JUSTIFYLEFT=0, JUSTIFYRIGHT};
	CPopupText();
	virtual ~CPopupText();
	BOOL Create(CPoint pt, CWnd* pParentWnd, UINT nStyle=0, UINT nID=0);
	void ShowDelayed(UINT msec);
	void Cancel();

protected:
	CFont	m_font;				// font to use (same as tooltips)
	UINT	m_nStyle;			// style (see enum below)

	virtual void PostNcDestroy();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnSetText(WPARAM wp, LPARAM lp);
	DECLARE_DYNAMIC(CPopupText);
	DECLARE_MESSAGE_MAP();
};
