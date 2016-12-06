// MECOToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MECOTool.h"
#include "MECOToolDlg.h"
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMECOToolDlg dialog




CMECOToolDlg::CMECOToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMECOToolDlg::IDD, pParent)
	, dfTargetInc(28.5)
	, dfTargetAlt(250.0)
	, dfTargetLAN(0)
	, dfOrbiterWgt(120000)
	, iAscentMode(0)
	, dfMECOVel(0)
	, dfMECOFPA(0)
	, dfMECOAlt(0)
	, m_OMS1TIG(_T(""))
	, m_OMS2TIG(_T(""))
	, dfOMS1VX(0)
	, dfOMS1VY(0)
	, dfOMS1VZ(0)
	, dfOMS2VX(0)
	, dfOMS2VY(0)
	, dfOMS2VZ(0)
	, m_FileOutput(_T(""))
	, iOutputFormat(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMECOToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TGTINC, m_TargetInc);
	DDX_Control(pDX, IDC_INCSPIN, m_IncSpin);
	DDX_Text(pDX, IDC_TGTINC, dfTargetInc);
	DDV_MinMaxDouble(pDX, dfTargetInc, 28.5, 57.0);
	DDX_Text(pDX, IDC_TGTALT, dfTargetAlt);
	DDV_MinMaxDouble(pDX, dfTargetAlt, 120.0, 650.0);
	DDX_Text(pDX, IDC_TGTLAN, dfTargetLAN);
	DDV_MinMaxDouble(pDX, dfTargetLAN, -180.0, 180.0);
	DDX_Text(pDX, IDC_ORBITERWGT, dfOrbiterWgt);
	DDV_MinMaxDouble(pDX, dfOrbiterWgt, 0, 250000);
	DDX_CBIndex(pDX, IDC_MODE_COMBO, iAscentMode);
	DDX_Text(pDX, IDC_MECOVEL, dfMECOVel);
	DDX_Text(pDX, IDC_MECOFPA, dfMECOFPA);
	DDX_Text(pDX, IDC_MECOALT, dfMECOAlt);
	DDX_Text(pDX, IDC_OMS1TIG, m_OMS1TIG);
	DDX_Text(pDX, IDC_OMS2TIG, m_OMS2TIG);
	DDX_Text(pDX, IDC_OMS1VX, dfOMS1VX);
	DDX_Text(pDX, IDC_OMS1VY, dfOMS1VY);
	DDX_Text(pDX, IDC_OMS1VZ, dfOMS1VZ);
	DDX_Text(pDX, IDC_OMS2VX, dfOMS2VX);
	DDX_Text(pDX, IDC_OMS2VY, dfOMS2VY);
	DDX_Text(pDX, IDC_OMS2VZ, dfOMS2VZ);
	DDX_Text(pDX, IDC_FILEOUTPUT, m_FileOutput);
	DDX_Radio(pDX, IDC_RADIO1, iOutputFormat);
	DDX_Control(pDX, IDC_LANSPIN, m_LANSpin);
	DDX_Control(pDX, IDC_ALTSPIN, m_AltSpin);
	DDX_Control(pDX, IDC_WGTSPIN, m_WgtSpin);
	DDX_Control(pDX, IDC_MODE_COMBO, m_AscentMode);
}

BEGIN_MESSAGE_MAP(CMECOToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CALCULATE, &CMECOToolDlg::OnBnClickedCalculate)
	ON_NOTIFY(UDN_DELTAPOS, IDC_INCSPIN, &CMECOToolDlg::OnDeltaposIncspin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ALTSPIN, &CMECOToolDlg::OnDeltaposAltspin)
END_MESSAGE_MAP()


// CMECOToolDlg message handlers

BOOL CMECOToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_AltSpin.SetPos32(200);
	m_AltSpin.SetRange32(0, 1000);
	m_IncSpin.SetRange32(2850, 5700);
	m_IncSpin.SetPos32(2850);
	m_LANSpin.SetRange32(0, 1000);
	m_AscentMode.SetCurSel(0);
	iOutputFormat = 0;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMECOToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMECOToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMECOToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

double TTA(double fRadius, double fApR, double fPeR)
{
	const double MU_EARTH = 3.986004418E14;
	double fSma = 0.5 * (fApR + fPeR);
	double fEcc = (fApR-fPeR)/(fApR + fPeR);
	double fPhiE = acos((1-fRadius/fSma)/fEcc);
	double ft = sqrt(pow(fSma, 3)/MU_EARTH) * (M_PI - (fPhiE - fEcc * sin(fPhiE)));
	return ft;
}

double OrbitPeriod(double fApR, double fPeR)
{
	const double MU_EARTH = 3.986004418E14;
	double fSma = 0.5 * (fApR + fPeR);

	return sqrt(pow(fSma, 3)/MU_EARTH) * 2.0 * M_PI;
}

double VisViva(double fRadius, double fApR, double fPeR)
{
	const double MU_EARTH = 3.986004418E14;
	return sqrt(MU_EARTH * ((2.0/fRadius) - (2.0/(fApR + fPeR))));
}

double FPA(double fRadius, double fApR, double fPeR)
{
	const double MU_EARTH = 3.986004418E14;
	double fh = 2*fPeR-2*pow(fPeR, 2)/(fApR+fPeR);
	double fx = 2*fRadius - 2*pow(fRadius, 2)/(fApR+fPeR);
	return acos(sqrt(fh/fx));
}

void CMECOToolDlg::OnBnClickedCalculate()
{
	const double RADIUS_EARTH = 6.378E6;
	const double MU_EARTH = 3.986004418E14;
	const double TMECO = 520.0;
	UpdateData(TRUE);
	double fRadiusTargetOrbit = dfTargetAlt * 1000.0 + RADIUS_EARTH;
	//Apogee target for MECO orbit
	double fApogeeMECOOrbit = 110.0E3 + RADIUS_EARTH;
	double fPerigeeMECOOrbit = RADIUS_EARTH;

	double fOMS1Apogee = fRadiusTargetOrbit;
	double fOMS1Perigee = fApogeeMECOOrbit;

	if(iAscentMode == 1)
	{
		//Direct ascent
		fApogeeMECOOrbit = fRadiusTargetOrbit;
		fPerigeeMECOOrbit = 55.0E3 + RADIUS_EARTH;
		//Select an as high as possible periapsis for minimum 
		//fuel consumption
		//
		m_OMS1TIG = _T("000:00:12:00");
		dfOMS1VX = 0.0;
		dfOMS1VY = 0.0;
		dfOMS1VZ = 0.0;

	} else {
		m_OMS1TIG = _T("000:00:12:00");
		dfOMS1VX = 0.0;
		dfOMS1VY = 0.0;
		dfOMS1VZ = 0.0;
	}
	
	m_OMS2TIG = _T("000:00:57:00");
	dfOMS2VX = 0.0;
	dfOMS2VY = 0.0;
	dfOMS2VZ = 0.0;
	
	dfMECOAlt = 105.0;
	double fMECORadius = 1000.0 * dfMECOAlt + RADIUS_EARTH;
	double fSmaMECO = 0.5 * (fApogeeMECOOrbit + fPerigeeMECOOrbit);
	
	dfMECOVel = VisViva(fMECORadius, fApogeeMECOOrbit, fPerigeeMECOOrbit);
	dfMECOFPA = FPA(fMECORadius, fApogeeMECOOrbit, fPerigeeMECOOrbit) * 180.0/M_PI;
	double fTIG1 = TTA(fMECORadius, fApogeeMECOOrbit, fPerigeeMECOOrbit);
	double fTIG_OMS1 = 0.0;
	double fTIG_OMS2 = 0.0;
	double fDV_OMS1 = 0.0;
	double fDV_OMS2 = 0.0;
	double fVTargetOrbit = sqrt(MU_EARTH/fRadiusTargetOrbit);

	if(iAscentMode == 1)
	{
		//Direct ascent
		fTIG_OMS1 = TMECO + 120.0;
		fTIG_OMS2 = TMECO + fTIG1;
		
		fDV_OMS2 = fVTargetOrbit - VisViva(fRadiusTargetOrbit, fApogeeMECOOrbit, fPerigeeMECOOrbit);
	} 
	else {
		//Standard ascent
		fTIG_OMS1 = TMECO + fTIG1;
		fTIG_OMS2 = fTIG_OMS1 + 0.5 * OrbitPeriod(fOMS1Apogee, fOMS1Perigee);
		fDV_OMS1 = VisViva(fOMS1Perigee, fOMS1Apogee, fOMS1Perigee) - VisViva(fApogeeMECOOrbit, fApogeeMECOOrbit, fPerigeeMECOOrbit);
		fDV_OMS2 = fVTargetOrbit - VisViva(fOMS1Apogee, fOMS1Apogee, fOMS1Perigee);
		
	}

	fTIG_OMS1 -= fDV_OMS1/(3 * 0.3048);
	fTIG_OMS2 -= fDV_OMS2/(3 * 0.3048);
	
	dfOMS1VX = fDV_OMS1 / 0.3048;
	dfOMS2VX = fDV_OMS2 / 0.3048;

	int iTIG = (int)(fTIG_OMS1);
	int iDay = iTIG/86400;
	iTIG -= iDay * 86400;
	int iHour = iTIG/3600;
	iTIG -= 3600 * iHour;
	int iMin = iTIG/60;
	int iSec = iTIG - 60 * iMin;

	m_OMS1TIG.Format(L"%03d:%02d:%02d:%02d",
		iDay, iHour, iMin, iSec);

	iTIG = (int)(fTIG_OMS2);
	iDay = iTIG/86400;
	iTIG -= iDay * 86400;
	iHour = iTIG/3600;
	iTIG -= 3600 * iHour;
	iMin = iTIG/60;
	iSec = iTIG - 60 * iMin;

	m_OMS2TIG.Format(L"%03d:%02d:%02d:%02d",
		iDay, iHour, iMin, iSec);

	if(this->iOutputFormat == 0)
	{
		m_FileOutput.Format(L"AUTOPILOT %f %f %f %f %f",
			dfTargetInc, dfTargetLAN, dfMECOAlt*1000.0, 
			dfMECOVel, dfMECOFPA);
	} else {
		m_FileOutput.Format(L"TargetInc=%f\r\nTargetLAN=%f\r\nMECOAlt=%f\r\nMECOVel=%f\r\nMECOFPA=%f",
			dfTargetInc, dfTargetLAN, dfMECOAlt*1000.0, 
			dfMECOVel, dfMECOFPA);
	}

	UpdateData(FALSE);
}

void CMECOToolDlg::OnDeltaposIncspin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	dfTargetInc = 0.01 * pNMUpDown->iPos;
	UpdateData(FALSE);
	
	*pResult = 0;
}

void CMECOToolDlg::OnDeltaposAltspin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	dfTargetAlt = 150 +  0.5 * pNMUpDown->iPos;
	UpdateData(FALSE);
	*pResult = 0;
}
