﻿
// TaskManagerDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "TaskManager.h"
#include "TaskManagerDlg.h"
#include "afxdialogex.h"

#include "CProcessDlg.h"
#include "CWindowDlg.h"
#include "CVSCleanDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CTaskManagerDlg 对话框



CTaskManagerDlg::CTaskManagerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TASKMANAGER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTaskManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}

BEGIN_MESSAGE_MAP(CTaskManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CTaskManagerDlg::OnSelchangeTab1)
END_MESSAGE_MAP()


// CTaskManagerDlg 消息处理程序

BOOL CTaskManagerDlg::OnInitDialog()
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

	// TODO: 在此添加额外的初始化代码

	// tab 控件添加项
	m_tab.InsertItem(0, L"进程列表");
	m_tab.InsertItem(1, L"窗口列表");
	m_tab.InsertItem(2, L"垃圾清理");
	// 给子窗口指针赋值
	CDialogEx * pSubProcess = new CProcessDlg();
	CDialogEx * pSubWindow = new CWindowDlg();
	CDialogEx * pSubVSClean = new CVSCleanDlg();

	//创建子窗口
	pSubProcess->Create(IDD_DIALOG_PROCESS, &m_tab);
	pSubWindow->Create(IDD_DIALOG_WINDOW, &m_tab);
	pSubVSClean->Create(IDD_DIALOG_VSCLEAN, &m_tab);
	m_tabSubWnd.push_back(pSubProcess);
	m_tabSubWnd.push_back(pSubWindow);
	m_tabSubWnd.push_back(pSubVSClean);
	// 控制两个窗口的大小
	CRect  rec;
	m_tab.GetClientRect(rec);
	rec.DeflateRect(8, 40, 10, 10);
	pSubProcess->MoveWindow(rec);
	pSubWindow->MoveWindow(rec);
	pSubVSClean->MoveWindow(rec);
	//初始显示第一个窗口
	pSubProcess->ShowWindow(SW_SHOW);
	pSubWindow->ShowWindow(SW_HIDE);
	pSubVSClean->ShowWindow(SW_HIDE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTaskManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTaskManagerDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTaskManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTaskManagerDlg::OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	// 隐藏所有
	for (auto &pWnd : m_tabSubWnd){
		pWnd->ShowWindow(SW_HIDE);
	}
	// 显示当前
	int curSel = m_tab.GetCurSel();
	m_tabSubWnd[curSel]->ShowWindow(SW_SHOW);

}
