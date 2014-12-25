// MECOToolDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CMECOToolDlg dialog
class CMECOToolDlg : public CDialog
{
// Construction
public:
	CMECOToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MECOTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// Target inclination in degrees
	CEdit m_TargetInc;
	// Spin Control for inclination
	CSpinButtonCtrl m_IncSpin;
	// Target inclination in degrees
	double dfTargetInc;
	// Target altitude in kilometers MSL
	double dfTargetAlt;
	// Target longitude of ascending node in degrees
	double dfTargetLAN;
	// Initial Orbiter weight in kg, used for optimization
	double dfOrbiterWgt;
	// Selected Ascent Mode
	int iAscentMode;
	// velocity at MECO
	double dfMECOVel;
	// Flight Path Angle at MECO
	double dfMECOFPA;
	double dfMECOAlt;
	CString m_OMS1TIG;
	CString m_OMS2TIG;
	double dfOMS1VX;
	double dfOMS1VY;
	double dfOMS1VZ;
	double dfOMS2VX;
	double dfOMS2VY;
	double dfOMS2VZ;
	CString m_FileOutput;
	int iOutputFormat;
	CSpinButtonCtrl m_LANSpin;
	CSpinButtonCtrl m_AltSpin;
	CSpinButtonCtrl m_WgtSpin;
	afx_msg void OnBnClickedCalculate();
	afx_msg void OnDeltaposIncspin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposAltspin(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox m_AscentMode;
};
