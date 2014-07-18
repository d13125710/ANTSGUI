
// GADlg.h : header file
//

#pragma once

#include "afxwin.h"
#include "TSPalgorithm.h"
#include "CTSPLIBFileReader.h"
#include "afxcmn.h"


// CTwoOptDlg dialog
class CTwoOptDlg : public CDialogEx
{
// Construction
public:
	CTwoOptDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TWOOPT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	void SetControls(BOOL Enabled);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OpenTestProblem();
	afx_msg void RunAlgorithm();

	afx_msg LRESULT UpdateDisplay(WPARAM, LPARAM);
	afx_msg LRESULT UpdateIrreation(WPARAM, LPARAM);
	afx_msg LRESULT verboseMessage(WPARAM, LPARAM lParam);

private:

	CStatusBar m_bar;

	void DrawNodes(int cc1, int cc2 ,int rectx1, int rectx2, int recty2, int recty1, CDC &dc)
	{

		Coords mat = CoordsMatrix_[cc1];

		xc1 = static_cast<int>(mat.GetX());
		yc1 = static_cast<int>(mat.GetY());


		mat = CoordsMatrix_[cc2];
		xc2 = static_cast<int>(mat.GetX());
		yc2 = static_cast<int>(mat.GetY());



		xn1 = (float) ( xc1 - MinX ) / (float) ( MaxX -MinX );
		yn1 = (float) ( yc1 - MinY ) / (float) ( MaxY - MinY );
		xn2 = (float) ( xc2 - MinX ) / (float) ( MaxX - MinX );
		yn2 = (float) ( yc2 - MinY ) / (float) ( MaxY - MinY );

		xcoord1 = rectx1 + (int) (float) ( xn1 * abs( rectx1 - rectx2 ) );
		ycoord1 = recty2 - (int) (float) ( yn1 * abs( recty1 - recty2 ) );
		xcoord2 = rectx1 + (int) (float) ( xn2 * abs( rectx1 - rectx2 ) );
		ycoord2 = recty2 - (int) (float) ( yn2 * abs( recty1 - recty2 ) );

		dc.MoveTo( xcoord1, ycoord1 );
		dc.LineTo( xcoord2, ycoord2 );
	}

	TSPalgorithm alg;
	newTour m_besttour;
	newTour m_lasttour;

	vector<Coords> CoordsMatrix_;

	CTSPLIBFileReader *m_pfileReader;
	std::vector<std::vector<int> > matrix_;

	bool hasRun;

	int cc1, cc2;
	int xc1, xc2;
	int yc1, yc2;
	float xn1, xn2;
	float yn1, yn2;
	int xcoord1, xcoord2;
	int ycoord1, ycoord2;
	int x1, y1;
	int count;
	CStatic distText;
	CStatic iterText;
	CStatic m_FileName;
	LOGFONT lf;
	CFont font;

	int MinX;
	int MinY;
	int MaxX;
	int MaxY;

	int m_AlSelection;


public:
	CEdit CInfoText;
	CEdit m_EditDebug;
	CStatic m_SFileName;

	int m_search;


	afx_msg void OnBnClickedButton1();
	CStatic c_SCurrentIrrr;
	CString m_DebugText;
	CComboBox m_CAlgSelection;
	afx_msg void OnSelchangeAlg();
	CSliderCtrl m_cSliderAlpha;
	CSliderCtrl m_cSliderBeta;
	CSliderCtrl m_cSliderEvap;
	CButton m_ButtonLocalSearch;
	int m_doGreedySearch;
	int m_doOpt2;
	int m_doOpt3;
	CProgressCtrl m_cProgress;
	afx_msg void OnNMCustomdrawSlideralpha(NMHDR *pNMHDR, LRESULT *pResult);
	CStatic m_AlphaValue;
	CStatic m_betaValue;
	CStatic m_QValue;
	afx_msg void OnNMCustomdrawBetaslider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawEvapslider(NMHDR *pNMHDR, LRESULT *pResult);
	CEdit m_EditNoOfAnts;
	afx_msg void OnEnChangeEdit4();
	CSpinButtonCtrl m_cSpinAnts;
	CSpinButtonCtrl c_SpinIrrerations;
//	CSpinButtonCtrl m_cSpinEliteWeight;
	CSliderCtrl m_cSpinXi;
	CSliderCtrl m_cSpinQ0;
	CSliderCtrl m_cSpinR0;
	afx_msg void OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSlider6(NMHDR *pNMHDR, LRESULT *pResult);
	CStatic m_cXiValue;
	CStatic m_Q0Value;
	CStatic m_R0Value;
	CSpinButtonCtrl c_mSpinEliteWeight;
};
