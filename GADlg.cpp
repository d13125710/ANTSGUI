string

// GADlg.cpp : implementation file
//

#include "stdafx.h"
#include "GA.h"
#include "GADlg.h"
#include <fstream>
#include <string>
#include "afxdialogex.h"
#include "MakeString.h"


#include "Utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int bottom_limit = 120;

// CTwoOptDlg dialog

static UINT BASED_CODE indicators[] =
{
	ID_INDICATOR_NISH,
	ID_INDICATOR_TIME
};



CTwoOptDlg::CTwoOptDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTwoOptDlg::IDD, pParent)
	, m_DebugText(_T(""))
	, m_doGreedySearch(0)
	, m_doOpt2(0)
	, m_doOpt3(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTwoOptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_DIST, distText);
	DDX_Control(pDX, IDC_STATIC_ITER, iterText);
	DDX_Control(pDX, IDC_EDIT1, m_EditDebug);
	DDX_Control(pDX, IDC_STATICFILENAME, m_SFileName);
	DDX_Control(pDX, IDC_STATICIRREATION, c_SCurrentIrrr);
	DDX_Text(pDX, IDC_EDIT1, m_DebugText);
	DDX_Control(pDX, IDC_ALSELECTIONCOMBO, m_CAlgSelection);
	DDX_Control(pDX, IDC_SLIDERALPHA, m_cSliderAlpha);
	DDX_Control(pDX, IDC_BETASLIDER, m_cSliderBeta);
	DDX_Control(pDX, IDC_EVAPSLIDER, m_cSliderEvap);
	//DDX_Control(pDX, IDC_RADIO5, m_ButtonLocalSearch);
	DDX_Radio(pDX, IDC_RADIO5, m_search);
	DDX_Control(pDX, IDC_PROGRESS1, m_cProgress);
	DDX_Control(pDX, IDC_STATICALPHAVALUE, m_AlphaValue);
	DDX_Control(pDX, IDC_STATICBETAVALUE, m_betaValue);
	DDX_Control(pDX, IDC_STATICQVALUE, m_QValue);
	DDX_Control(pDX, IDC_EDIT4, m_EditNoOfAnts);
	DDX_Control(pDX, IDC_SPIN3, m_cSpinAnts);
	DDX_Control(pDX, IDC_SPIN5, c_SpinIrrerations);
	DDX_Control(pDX, IDC_SLIDER4, m_cSpinXi);
	DDX_Control(pDX, IDC_SLIDER5, m_cSpinQ0);
	DDX_Control(pDX, IDC_SLIDER6, m_cSpinR0);
	DDX_Control(pDX, IDC_XIVALUE, m_cXiValue);
	DDX_Control(pDX, IDC_Q0VALUE, m_Q0Value);
	DDX_Control(pDX, IDC_R0VALUE, m_R0Value);
	DDX_Control(pDX, IDC_ELITEWEIGHT, c_mSpinEliteWeight);
}

BEGIN_MESSAGE_MAP(CTwoOptDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RUN2, &CTwoOptDlg::OpenTestProblem)
	ON_BN_CLICKED(IDC_RUN, &CTwoOptDlg::RunAlgorithm)
	ON_MESSAGE(IDC_STATICFILENAME, UpdateDisplay)
	ON_MESSAGE(IDC_INCITTERATION, UpdateIrreation)
	ON_MESSAGE(IDC_VERBOSE, verboseMessage)

	ON_BN_CLICKED(IDC_BUTTON1, &CTwoOptDlg::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_ALSELECTIONCOMBO, &CTwoOptDlg::OnSelchangeAlg)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDERALPHA, &CTwoOptDlg::OnNMCustomdrawSlideralpha)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_BETASLIDER, &CTwoOptDlg::OnNMCustomdrawBetaslider)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_EVAPSLIDER, &CTwoOptDlg::OnNMCustomdrawEvapslider)
	ON_EN_CHANGE(IDC_EDIT4, &CTwoOptDlg::OnEnChangeEdit4)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER4, &CTwoOptDlg::OnNMCustomdrawSlider4)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER5, &CTwoOptDlg::OnNMCustomdrawSlider5)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER6, &CTwoOptDlg::OnNMCustomdrawSlider6)
END_MESSAGE_MAP()

//LRESULT CTwoOptDlg::verboseMessage(WPARAM, LPARAM lParam)
// CTwoOptDlg message handlers

BOOL CTwoOptDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	m_search = -1;
	
	hasRun = false;

	// Creates a 9-point-Arial-font
	font.CreatePointFont(90, _T("Arial"));

	// With a member variable associated to the static control
	distText.SetFont(&font);
	iterText.SetFont(&font);


	// Without a member variable
	GetDlgItem(IDC_STATIC_DIST)->SetFont(&font);	

	m_lasttour.TourLenght =0;
	m_besttour.TourLenght =0;
	m_AlSelection = 0;
	m_CAlgSelection.SetCurSel(m_AlSelection);

	//set alpha between 1 - 5
	m_cSliderAlpha.SetRange(1 , 10);
	m_cSliderAlpha.SetPos(1);
	//set beta between 1 - 5
	m_cSliderBeta.SetRange(1 , 10);
	m_cSliderBeta.SetPos(3);
	m_cSliderEvap.SetRange(0,10);
	m_cSliderEvap.SetPos(5);
	m_cProgress.SetRange(0,100);

	m_cSpinXi.SetRange(1 , 10);
	m_cSpinQ0.SetRange(1 , 100);
	m_cSpinR0.SetRange(1 , 10);

	m_cSpinXi.SetPos(1);
	m_cSpinQ0.SetPos(98);
	m_cSpinR0.SetPos(1);

	c_mSpinEliteWeight.SetRange(1,10);
	c_mSpinEliteWeight.SetPos(5);
	
	c_SpinIrrerations.SetRange(1 , 100);
	c_SpinIrrerations.SetPos(5);


	m_bar.Create(this); //We create the status bar

	m_bar.SetIndicators(indicators,2); //Set the number of panes 

	CRect rect;
	GetClientRect(&rect);
	//Size the two panes
	m_bar.SetPaneInfo(0,ID_INDICATOR_NISH, 	SBPS_NORMAL,rect.Width()-100);      
	m_bar.SetPaneInfo(1,ID_INDICATOR_TIME,SBPS_STRETCH ,0);

	//This is where we actually draw it on the screen
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,ID_INDICATOR_TIME);

	CRect progressRect;
	m_bar.GetItemRect(1, &progressRect);
	//m_cProgress.MoveWindow(progressRect);
	//m_cProgress.Create(WS_CHILD | WS_VISIBLE | SBPS_NOBORDERS | PBS_SMOOTH, progressRect, this, -1);
	m_cProgress.SetRange(0, 100);
	m_cProgress.SetPos(0);
	


	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTwoOptDlg::OnPaint()
{	
	// device context for painting
	
	//CDC memDC ; // buffer context 
	CPaintDC dc(this);   

	if (IsIconic())
	{		
		
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
		if ( CoordsMatrix_.size() < 1 ) return;
	                              // device context for painting


		CPen penDot(PS_DOT,1,RGB(255,0,0));
		CPen penblack(PS_SOLID,3,RGB(0,0,0));

		CRect rect;
		GetClientRect(&rect);
	 

		int rectx1 = rect.left + 20;
		int rectx2 = rect.right  - 170;
		int recty1 = rect.top + 25;
		int recty2 = rect.bottom - bottom_limit;

		
		dc.Rectangle(rectx1-5,recty1-5,rectx2+5,recty2+5);
		// Plot each node
		size_t tour_size = CoordsMatrix_.size();
		for ( count = 0; count < tour_size; count++ )
		{
			Coords mat = CoordsMatrix_[count];
			xc1 =static_cast<int>(mat.GetX());
			yc1 = static_cast<int>(mat.GetY());

			xn1 = (float) ( xc1 - MinX ) / (float) ( MaxX - MinX );
			yn1 = (float) ( yc1 - MinY ) / (float) ( MaxY - MinY );
			xcoord1 = rectx1 + (int) (float) ( xn1 * abs( rectx1 - rectx2 ) );
			ycoord1 = recty2 - (int) (float) ( yn1 * abs( recty1 - recty2 ) );
			dc.SelectObject(&penblack);
			dc.Ellipse( xcoord1 - 2, ycoord1 - 2, xcoord1 + 2, ycoord1 + 2 );	

			//draw lines

			dc.SelectObject(&penDot);

			//draw last tour
			if ( hasRun && count < tour_size - 1 && m_lasttour.Tour.size())
			{
				m_lasttour.Tour[count];
				cc1 = static_cast<int>(m_lasttour.Tour[count]);
				cc2 =  static_cast<int>(m_lasttour.Tour[count +1]);
				DrawNodes(cc1 , cc2, rectx1, rectx2, recty2, recty1, dc);
			}

			dc.SelectObject(&penblack);
			if ( hasRun && count < tour_size - 1   && m_besttour.Tour.size())
			{
				cc1 =  static_cast<int>(m_besttour.Tour[count]);
				cc2 =  static_cast<int>(m_besttour.Tour[count +1]);
				DrawNodes(cc1 , cc2, rectx1, rectx2, recty2, recty1, dc);
			}
		

		}	

		// Draw final link
		if ( hasRun )
		{
			
			if(m_lasttour.Tour.size())
			{
				cc1 =  static_cast<int>(m_lasttour.Tour[tour_size-1]);
				cc2 = static_cast<int>( m_lasttour.Tour[0]);
				dc.SelectObject(&penDot);
				DrawNodes(cc1 , cc2, rectx1, rectx2, recty2, recty1, dc);
			}
			if(m_besttour.Tour.size())
			{
				dc.SelectObject(&penblack);
				cc1 =  static_cast<int>(m_besttour.Tour[tour_size-1]);
				cc2 =  static_cast<int>(m_besttour.Tour[0]);
				DrawNodes(cc1 , cc2, rectx1, rectx2, recty2, recty1, dc);
			}
			
		}
	
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTwoOptDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



// Open the selected test problem
void CTwoOptDlg::OpenTestProblem()
{


	// Open the file opening dialog
	CFileDialog fileDlg( TRUE,  NULL,  NULL,  OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, (LPCTSTR) ".tsp Files(*.tsp)|*.tsp||", this );
	fileDlg.m_ofn.lpstrTitle = (LPCSTR)"Loading TSP Problem";
	INT_PTR result = fileDlg.DoModal();
	CString path = fileDlg.GetPathName();	
	this->m_SFileName.SetWindowText(fileDlg.GetFileName());
	if ( result == IDOK  )
	{
		// Reset matrix data contents		
		CT2CA pszConvertedAnsiString( path );
        std::string path_str( pszConvertedAnsiString );		
	//	mat.Initialize( path_str );		
		hasRun = false;
		//ATL::CSimpleStringT<char,1> = "C:\Users\gareth\Desktop\libaco-thesis\acotsp\trunk\benchmarks\tsplib\ulysses16.tsp"

		m_besttour.Tour.clear();
		m_lasttour.Tour.clear();

		m_pfileReader = new CTSPLIBFileReader(path_str);
		m_pfileReader->Read();
		matrix_ = m_pfileReader->CopyMatrix();
		CoordsMatrix_ = m_pfileReader->CopyCoordsMatrix();

		MinX= m_pfileReader->minx;
		MinY =  m_pfileReader->miny;
		MaxX=  m_pfileReader->maxx;
		MaxY=  m_pfileReader->maxy;
	
		m_cSpinAnts.SetPos( static_cast<int>(CoordsMatrix_.size()));

		delete m_pfileReader;
	}

	// Re-draw the window	
	RedrawWindow( 0, 0, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE );	
}
void CTwoOptDlg::SetControls(BOOL Enabled)
{

	m_CAlgSelection.EnableWindow(Enabled);
	m_cSliderAlpha.EnableWindow(Enabled);
	//set beta between 1 - 5
	m_cSliderBeta.EnableWindow(Enabled);
	m_cSliderEvap.EnableWindow(Enabled);


}



void CTwoOptDlg::RunAlgorithm()
{
	SetControls(false);	
	UpdateData(TRUE);
	//DoDataExchange()
	//std::string path_str( "C:\\Users\\gareth\\Desktop\\libaco-thesis\\acotsp\\trunk\\benchmarks\\tsplib\\ulysses16.tsp" );		
	//	mat.Initialize( path_str );		
	hasRun = false;

	m_besttour.Tour.clear();
	m_lasttour.Tour.clear();

	Parameters par;
	par.ALG = m_CAlgSelection.GetCurSel();
	par.Alpha=m_cSliderAlpha.GetPos();
	par.Beta=m_cSliderBeta.GetPos();
	par.setRho(m_cSliderEvap.GetPos());
	par.Epochs = c_SpinIrrerations.GetPos() *1000;
	par.noAnts = this->m_cSpinAnts.GetPos();

	m_cProgress.SetRange(0,par.Epochs);
	m_cProgress.SetPos(0);
	m_cProgress.SetStep(25);


	par.LocalSearchGreed = (m_search==1) ? true: false;
	par.LocalSearchOpt2 = (m_search==2) ? true : false;
	par.LocalSearchOpt3 = (m_search==3) ? true : false;

	//m_ButtonLocalSearch.

	alg.Run(par , matrix_);
	
	//alg.Run( &mat );
	hasRun = true;	

	// Re-draw the window	
	RedrawWindow( 0, 0, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE );	

	//fix up with thread mesage
	SetControls(true);	
}


// Initialise obersers
//void CTwoOptDlg::Init( TSPalgorithm* tsp )
//{
//	alg.AddObserver( this );  
//}

    

// Tell the GUI to update display
LRESULT CTwoOptDlg::UpdateIrreation(WPARAM, LPARAM lParam)
{
	int i = (int)lParam;
	std::string s = make_string() << "current Iteration: " << i;

	c_SCurrentIrrr.SetWindowText(s.c_str() );
	m_cProgress.StepIt();

	return 0;
}
LRESULT CTwoOptDlg::verboseMessage(WPARAM, LPARAM lParam)
{
	std::string *p = (std::string *)lParam;
	//std::string s = make_string() << "current Iteration: " << *p <<"\r\n";

	// get the initial text length
	int nLength = m_EditDebug.GetWindowTextLength();
	// put the selection at the end of text
	m_EditDebug.SetSel(nLength, nLength);
	// replace the selection

	CString strLine;
	// add CR/LF to text
	strLine.Format(_T("\r\n%s"), p->c_str());

	m_EditDebug.ReplaceSel(strLine);



	//m_DebugText = m_DebugText + s.c_str();
	//UpdateData(FALSE);
	//c_SCurrentIrrr.SetWindowText(s.c_str() );
	return 0;
}


LRESULT CTwoOptDlg::UpdateDisplay(WPARAM, LPARAM lParam  )
{
	newTour *tour = (newTour*)lParam;
	
	
	// Set the new distance text
	m_lasttour = m_besttour;
	m_besttour = *tour;

	double dist = m_besttour.TourLenght;
	std::string s = make_string() << "Best distance: " << dist;
	distText.SetWindowText( s.c_str() );	

	// Set the iterationtext
	std::string i = make_string() << "Found Iteration: " << m_besttour.Irreation;
	iterText.SetWindowText( i.c_str() );	
		
	// Re-draw the window	
	hasRun = true;	
	RedrawWindow( 0, 0, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE );	
	return 0;
}




void CTwoOptDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	alg.stopThread();
}


void CTwoOptDlg::OnSelchangeAlg()
{
	// TODO: Add your control notification handler code here
	m_AlSelection = this->m_CAlgSelection.GetCurSel();
}


void CTwoOptDlg::OnNMCustomdrawSlideralpha(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	if (pNMCD->dwDrawStage == CDDS_PREPAINT)
	{
		std::string s = make_string() << m_cSliderAlpha.GetPos();
		m_AlphaValue.SetWindowText(s.c_str());
		// update static control here
	}

	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CTwoOptDlg::OnNMCustomdrawBetaslider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	std::string s = make_string() << m_cSliderBeta.GetPos();;
	m_betaValue.SetWindowText(s.c_str());
	*pResult = 0;
}


void CTwoOptDlg::OnNMCustomdrawEvapslider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	std::string s = make_string() <<(m_cSliderEvap.GetPos() *0.1);
	m_QValue.SetWindowText(s.c_str());
	*pResult = 0;
}


void CTwoOptDlg::OnEnChangeEdit4()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the __super::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CTwoOptDlg::OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	std::string s = make_string() <<(m_cSpinXi.GetPos() *0.1);
	m_cXiValue.SetWindowText(s.c_str());
	*pResult = 0;
}


void CTwoOptDlg::OnNMCustomdrawSlider5(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	std::string s = make_string() <<(m_cSpinQ0.GetPos() *0.01);
	m_Q0Value.SetWindowText(s.c_str());
	*pResult = 0;
}


void CTwoOptDlg::OnNMCustomdrawSlider6(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here


	std::string s = make_string() <<(m_cSpinR0.GetPos() *0.1);
	m_R0Value.SetWindowText(s.c_str());
	*pResult = 0;
}
