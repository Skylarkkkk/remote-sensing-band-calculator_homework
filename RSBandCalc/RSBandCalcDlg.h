
// RSBandCalcDlg.h: 头文件
//

#pragma once
#include <atlimage.h>
#include"ImageData.h"
#include <string>

// CRSBandCalcDlg 对话框
class CRSBandCalcDlg : public CDialogEx
{
	// 构造
public:
	CRSBandCalcDlg(CWnd* pParent = nullptr);	// 标准构造函数

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RSBandCalc_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


	// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CFont cfont;

	//这些是图片显示框（picture control）
	CStatic m_pictureCtrl_red;
	CStatic m_pictureCtrl_green;
	CStatic m_pictureCtrl_blue;
	CStatic  m_pictureCtrl_ret;

	CString m_imgPath;     //存图片的路径
	CString m_path_show;   //文件路径显示框的内容
	CString m_result_show; //结果显示框的内容

	ImageData* m_imagedata;  //ImageData类指针

	bool data_loaded = FALSE; //数据是否载入进来

	//这些是右侧控制是否显示的状态变量
	BOOL m_IsLoadRedImage;
	BOOL m_IsLoadGreenImage;
	BOOL m_IsLoadBlueImage;
	BOOL m_IsLoadRetImage;

	int m_radioSelection; // 表示 Radio 选择状态

public:
	//这些都是按钮关联函数（消息传递）
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCheckRed();

	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();

	void ReadDataAndCalc();  //载入数据和统计函数

	int CRSBandCalcDlg::BandStat(ImageData* data, string label); //波段统计函数

	afx_msg void OnBnClickedCheckGreen();
	afx_msg void OnBnClickedCheckBlue();
	afx_msg void OnBnClickedCheckRet();

	void draw_bbox(CPaintDC& dc, char label); //绘制边框函数
};
