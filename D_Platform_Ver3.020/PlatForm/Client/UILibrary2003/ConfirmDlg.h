#pragma once
#ifndef AFCMESSAGEB0X_HEAD_H
#define AFCMESSAGEB0X_HEAD_H

#define IDQUXIAO                        2

#include "Stdafx.h"
#include "afcdevhead.h"
#include "Resource.h"
#include "CconfigDlg.h"


enum MB_AFC_STYLE          //KT消息框类型
{
    AFC_CONFIRM = 500,       //确认对话框
    AFC_YESNO,               //提示“是”、“否”对话框
    AFC_WARN                 //警告对话框
};

class _declspec(dllexport) ConfirmDlg : public CDialog
{
	DECLARE_DYNAMIC(ConfirmDlg)

	HBRUSH	m_bkBrush;		///< 用于背景色的刷子
public:
	ConfirmDlg(CString strInfo,CString strTitle = "系统提示",UINT style = AFC_CONFIRM,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ConfirmDlg();

// 对话框数据
	enum { IDD = IDD_CONFIRMDLG };
	DWORD m_wuiHandle;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    //CString					m_backPicFile;
    CString					m_infoBmpFile;
    CBitmap					m_infoBmp;     //提示图片
    CRect					m_rectTitle;     //标题栏区域
    CNormalBitmapButtonEx   m_confirmBtn;
    CNormalBitmapButtonEx   m_cancelBtn;
    CNormalBitmapButtonEx   m_closeBtn;

    CString     m_infoStr;     //提示信息
    CString     m_titelStr;    //标题
    UINT        m_msgBoxStyle; //消息框样式，为MB_AFC_STYLE值
    CGameImage  m_backPic;     //背景图
    CRect       m_rectInfoPic; //提示图片显示区域
    CRect       m_rectTitleText;   //标题文字区域

	CconfigDlg  m_configDlg; //加载底图类. 2012.10.17 yyf
    CFont m_fontCaption;     //标题栏字体
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnBnClickedOk();
    afx_msg void OnBnClickedNo();
    afx_msg void OnBnClickedClosebtn();

public:
	static void SetSkinKey(const CString& strkey); 
	  // 设置窗口圆角 2012.10.15 yyf
	  void SetWndRgn(void);
private:
	static CString m_skinKey;
};


class TimerDlg :
	public ConfirmDlg
{
private:
	int m_sec;
public:
	TimerDlg(int sec, CString strInfo,CString strTitle = "系统提示",UINT style = AFC_CONFIRM,CWnd* pParent = NULL);
public:
	~TimerDlg(void);
public:
	virtual BOOL OnInitDialog();
public:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	virtual void OnOK();
};


#endif