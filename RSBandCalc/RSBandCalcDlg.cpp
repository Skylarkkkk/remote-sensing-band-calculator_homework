
// RSBandCalcDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "RSBandCalc.h"
#include "RSBandCalcDlg.h"
#include "afxdialogex.h"
#include <atlimage.h>
#include"ImageData.h"
#include <vector>
#include <algorithm>
#include <string>
#include<map>

using namespace Gdiplus;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BOOL IsFirstLoad = false;  //  是否为首次加载图片

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();


	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRSBandCalcDlg 对话框



CRSBandCalcDlg::CRSBandCalcDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RSBandCalc_DIALOG, pParent)
	, m_path_show(_T(""))
	, m_result_show(_T(""))
	, m_IsLoadRedImage(TRUE)
	, m_IsLoadGreenImage(TRUE)
	, m_IsLoadBlueImage(TRUE)
	, m_IsLoadRetImage(FALSE)
	, m_radioSelection(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CRSBandCalcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_path_show);
	DDX_Text(pDX, IDC_EDIT1, m_result_show);
	DDX_Control(pDX, IDC_STATIC_rBand, m_pictureCtrl_red);  // 关联 Picture Control
	DDX_Control(pDX, IDC_STATIC_gBand, m_pictureCtrl_green);  // 关联 Picture Control
	DDX_Control(pDX, IDC_STATIC_bBand, m_pictureCtrl_blue);  // 关联 Picture Control
	DDX_Control(pDX, IDC_STATIC_rsgBand, m_pictureCtrl_ret);  // 关联 Picture Control

	DDX_Check(pDX, IDC_CHECK_RED, m_IsLoadRedImage);
	DDX_Check(pDX, IDC_CHECK_GREEN, m_IsLoadGreenImage);
	DDX_Check(pDX, IDC_CHECK_BLUE, m_IsLoadBlueImage);
	DDX_Check(pDX, IDC_CHECK_RET, m_IsLoadRetImage);

	DDX_Radio(pDX, IDC_RADIO2, m_radioSelection);
}

BEGIN_MESSAGE_MAP(CRSBandCalcDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CRSBandCalcDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_CHECK_RED, &CRSBandCalcDlg::OnBnClickedCheckRed)
	ON_BN_CLICKED(IDC_RADIO2, &CRSBandCalcDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CRSBandCalcDlg::OnBnClickedRadio3)

	ON_BN_CLICKED(IDC_CHECK_GREEN, &CRSBandCalcDlg::OnBnClickedCheckGreen)
	ON_BN_CLICKED(IDC_CHECK_BLUE, &CRSBandCalcDlg::OnBnClickedCheckBlue)
	ON_BN_CLICKED(IDC_CHECK_RET, &CRSBandCalcDlg::OnBnClickedCheckRet)
END_MESSAGE_MAP()


// CRSBandCalcDlg 消息处理程序

BOOL CRSBandCalcDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化

	//设置字体等属性
	cfont.CreatePointFont(150, _T("宋体"), NULL);
	GetDlgItem(IDC_STATIC_TITLE)->SetFont(&cfont);

	m_IsLoadRedImage = true;
	m_IsLoadBlueImage = true;
	m_IsLoadGreenImage = true;
	m_IsLoadRetImage = false;

	m_radioSelection = 0;


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRSBandCalcDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRSBandCalcDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		//绘制红绿蓝三个边框
		if (data_loaded)
		{
			if (m_IsLoadRedImage)   draw_bbox(dc, 'r');
			if (m_IsLoadGreenImage) draw_bbox(dc, 'g');
			if (m_IsLoadBlueImage) draw_bbox(dc, 'b');
		}
		CDialogEx::OnPaint();
	}

	if (data_loaded)
	{
		//显示红波段
		CDC* pDC_r = m_pictureCtrl_red.GetDC();
		if (pDC_r && m_IsLoadRedImage)
		{
			CRect rect;
			m_pictureCtrl_red.GetClientRect(&rect);
			m_imagedata->DisplayImage(pDC_r, rect, 'r');
		}

		//显示绿波段
		CDC* pDC_g = m_pictureCtrl_green.GetDC();
		if (pDC_g && m_IsLoadGreenImage)
		{
			CRect rect;
			m_pictureCtrl_green.GetClientRect(&rect);
			m_imagedata->DisplayImage(pDC_g, rect, 'g');
		}

		//显示蓝波段
		CDC* pDC_b = m_pictureCtrl_blue.GetDC();
		if (pDC_b && m_IsLoadBlueImage)
		{
			CRect rect;
			m_pictureCtrl_blue.GetClientRect(&rect);
			m_imagedata->DisplayImage(pDC_b, rect, 'b');
		}
		if (!IsFirstLoad)
		{
			IsFirstLoad = true;
			BandStat(m_imagedata, "first");
		}

		//显示红绿相加或相减图片
		if (m_IsLoadRetImage)
		{
			if (!m_radioSelection)
			{
				CDC* pDC_rag = m_pictureCtrl_ret.GetDC();
				if (pDC_rag)
				{
					CRect rect;
					m_pictureCtrl_ret.GetClientRect(&rect);
					m_imagedata->DisplayImage(pDC_rag, rect, 'a');
				}
			}
			else
			{
				CDC* pDC_rsg = m_pictureCtrl_ret.GetDC();
				if (pDC_rsg && m_radioSelection && m_IsLoadRetImage)
				{
					CRect rect;
					m_pictureCtrl_ret.GetClientRect(&rect);
					m_imagedata->DisplayImage(pDC_rsg, rect, 's');
				}
			}
		}
	}

}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRSBandCalcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRSBandCalcDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	ReadDataAndCalc();
}


void CRSBandCalcDlg::ReadDataAndCalc()
{
	// TODO: 在此处添加实现代码.
	CFileDialog cfd(true, _T(".jpg"), NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("Image Files (*.jpg)|*.jpg|All Files (*.*)|*.*||"), this); //选项图片的约定
	if (cfd.DoModal() != IDOK) //判断是否获得图片 
	{
		return;
	}
	m_imgPath = cfd.GetPathName(); //获得图片路径
	m_path_show = m_imgPath;
	m_result_show = "图像长0像素，宽0像素\r\n";

	m_imagedata = new ImageData(m_imgPath);
	m_result_show.Format(_T("图像长%d像素，宽%d像素\r\n"), m_imagedata->GetHeight(), m_imagedata->GetWidth());  //更新显示框内容
	//红绿蓝三个波段统计最大最小值、平均值
	/*BandStat();*/

	//红绿两个波段加减操作
	// 红绿相加
	m_imagedata->RedAddGreen();
	//AfxMessageBox(_T("RedAddGreen has been called"));  //  debug
	// 红绿相减
	m_imagedata->RedSubstractGreen();
	data_loaded = true;

	//刷新
	UpdateData(false);
	Invalidate();
}

/// <summary>
/// 波段统计
/// </summary>
/// <returns></returns>
int CRSBandCalcDlg::BandStat(ImageData* data, string label)
{
	// TODO: 在此处添加实现代码.
	CString max, min, aver;
	
	if (label == "first")
	{
		max.Format(_T("%d"), data->m_max["Red"]);
		min.Format(_T("%d"), data->m_min["Red"]);
		aver.Format(_T("%f"), data->m_mean["Red"]);
		m_result_show = m_result_show + _T("红波段：") + _T("最大值是") + max + _T(", 最小值是") + min + _T(", 平均值是") + aver + _T("\r\n");

		max.Format(_T("%d"), data->m_max["Green"]);
		min.Format(_T("%d"), data->m_min["Green"]);
		aver.Format(_T("%f"), data->m_mean["Green"]);
		m_result_show = m_result_show + _T("绿波段：") + _T("最大值是") + max + _T(", 最小值是") + min + _T(", 平均值是") + aver + _T("\r\n");
		
		max.Format(_T("%d"), data->m_max["Blue"]);
		min.Format(_T("%d"), data->m_min["Blue"]);
		aver.Format(_T("%f"), data->m_mean["Blue"]);
		m_result_show = m_result_show + _T("蓝波段：") + _T("最大值是") + max + _T(", 最小值是") + min + _T(", 平均值是") + aver + _T("\r\n");
	}
	else
	{
		max.Format(_T("%d"), data->m_max[label]);
		min.Format(_T("%d"), data->m_min[label]);
		aver.Format(_T("%f"), data->m_mean[label]);

		if (label == "a")
			m_result_show = m_result_show + _T("红绿相加结果：") + _T("最大值是") + max + _T(", 最小值是") + min + _T(", 平均值是") + aver + _T("\r\n");

		if (label == "s")
			m_result_show = m_result_show + _T("红绿相减结果：") + _T("最大值是") + max + _T(", 最小值是") + min + _T(", 平均值是") + aver + _T("\r\n");
	}
	UpdateData(false);

	return 0;
}





void CRSBandCalcDlg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	// 点击红绿波段相加
	m_radioSelection = 0; // 0表示选中Radio_Rag按钮
	if (m_IsLoadRetImage)
	{
		BandStat(m_imagedata, "a");
	}
	UpdateData();
	Invalidate();
}


void CRSBandCalcDlg::OnBnClickedRadio3()
{
	// TODO: 在此添加控件通知处理程序代码
	// 点击红绿波段相减
	m_radioSelection = 1; // 1表示选中Radio_Rsg按钮
	if (m_IsLoadRetImage)
	{
		BandStat(m_imagedata, "s");
	}
	UpdateData();
	Invalidate();
}


// 勾选红波段
void CRSBandCalcDlg::OnBnClickedCheckRed()
{
	// TODO: 在此添加控件通知处理程序代码
	m_IsLoadRedImage = !m_IsLoadRedImage;

	UpdateData();
	Invalidate();

}

// 勾选绿波段
void CRSBandCalcDlg::OnBnClickedCheckGreen()
{
	// TODO: 在此添加控件通知处理程序代码
	m_IsLoadGreenImage = !m_IsLoadGreenImage;

	UpdateData();
	Invalidate();
}

// 勾选蓝波段
void CRSBandCalcDlg::OnBnClickedCheckBlue()
{
	// TODO: 在此添加控件通知处理程序代码
	m_IsLoadBlueImage = !m_IsLoadBlueImage;

	//AfxMessageBox(_T("蓝波段被选中")); //  debug

	UpdateData();
	Invalidate();
}

// 勾选计算结果
void CRSBandCalcDlg::OnBnClickedCheckRet()
{
	// TODO: 在此添加控件通知处理程序代码
	m_IsLoadRetImage = !m_IsLoadRetImage;
	if (m_IsLoadRetImage)
	{

		if (!m_radioSelection)
		{
			BandStat(m_imagedata, "a");
		}
		else
		{
			BandStat(m_imagedata, "s");
		}
	}
	UpdateData();
	Invalidate();
}

void CRSBandCalcDlg::draw_bbox(CPaintDC& dc, char label)
{
	CWnd* pWnd = nullptr;
	switch (label)
	{
	case 'r':
		pWnd = GetDlgItem(IDC_STATIC_rBand);//红波段的控件
		if (pWnd)
		{
			// 获取控件的屏幕坐标
			CRect rect;
			pWnd->GetWindowRect(&rect);
			// 将屏幕坐标转换为客户端坐标
			ScreenToClient(&rect);
			// 设置画笔颜色
			CPen pen(PS_SOLID, 12, RGB(255, 0, 0));
			CPen* pOldPen = dc.SelectObject(&pen);
			// 绘制矩形外框
			dc.Rectangle(rect);
			// 恢复旧的画笔
			dc.SelectObject(pOldPen);
		}
		break;
	case 'g':
		pWnd = GetDlgItem(IDC_STATIC_gBand);//绿波段的控件
		if (pWnd)
		{
			// 获取控件的屏幕坐标
			CRect rect;
			pWnd->GetWindowRect(&rect);
			// 将屏幕坐标转换为客户端坐标
			ScreenToClient(&rect);
			// 设置画笔颜色
			CPen pen(PS_SOLID, 12, RGB(0, 255, 0));
			CPen* pOldPen = dc.SelectObject(&pen);
			// 绘制矩形外框
			dc.Rectangle(rect);
			// 恢复旧的画笔
			dc.SelectObject(pOldPen);
		}
		break;
	case 'b':
		pWnd = GetDlgItem(IDC_STATIC_bBand);//蓝波段的控件
		if (pWnd)
		{
			// 获取控件的屏幕坐标
			CRect rect;
			pWnd->GetWindowRect(&rect);
			// 将屏幕坐标转换为客户端坐标
			ScreenToClient(&rect);
			// 设置画笔颜色
			CPen pen(PS_SOLID, 12, RGB(0, 0, 255));
			CPen* pOldPen = dc.SelectObject(&pen);
			// 绘制矩形外框
			dc.Rectangle(rect);
			// 恢复旧的画笔
			dc.SelectObject(pOldPen);
		}
		break;
	}
}
