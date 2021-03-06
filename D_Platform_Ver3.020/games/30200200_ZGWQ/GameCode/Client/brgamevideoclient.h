#if !defined(AFX_BRGAMEVIDEOCLIENT_H__F04582C6_891C_4E47_8A6B_8F81E9472211__INCLUDED_)
#define AFX_BRGAMEVIDEOCLIENT_H__F04582C6_891C_4E47_8A6B_8F81E9472211__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CBRGameVideoClient wrapper class

class CBRGameVideoClient : public CWnd
{
protected:
	DECLARE_DYNCREATE(CBRGameVideoClient)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xf53b5b07, 0xdf60, 0x4df5, { 0x80, 0x35, 0x3a, 0x63, 0xd0, 0x1d, 0x83, 0xca } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd, UINT nID,
		CCreateContext* pContext = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); }

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID,
		CFile* pPersist = NULL, BOOL bStorage = FALSE,
		BSTR bstrLicKey = NULL)
	{ return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); }

// Attributes
public:

// Operations
public:
	void ActiveVideoClient(LPCTSTR authCode, short codeEncType);
	void StartVideoClient(LPCTSTR videoserveripaddr, short videoserverport);
	void Release();
	void RegisterUser(short isVisitor, long userid, LPCTSTR username, LPCTSTR password, short passEncType);
	void UnRegisterUser(long userid);
	void EnterRoom(long roomid, LPCTSTR roompass, short passEncType);
	void LeaveRoom(long roomid);
	void SendVideoControl(short bSend);
	short GetSendVideoState();
	short GetCameraState(long userid);
	void UserCameraControl(long userid, short bOpen, long* phWnd, short left, short top, short right, short bottom);
	short GetSpeakState(long userid);
	void UserSpeakControl(long userid, short bOpen);
	short GetRoomUserCount();
	void SetMessageNotifyHandle(long* phWnd);
	void ShowUserPanel(long userid, short bShow, short left, short top, short right, short bottom);
	void PrepaGetUserList();
	long FetchNextUserID();
	short SetFortuneImage(LPCTSTR lpImageFile);
	void ResetFortuneMenu(long userid);
	void AppendFortuneMenuItem(long userid, LPCTSTR menuText, LPCTSTR toolTip, short menuTag, short imageIndex);
	void SetSnapShotSavePath(LPCTSTR PathStr);
	void ShowUserPanelEx(long userid, short bShow, short left, short top, short right, short bottom, short style, long* pParentWnd);
	void SetRecordFileSavePath(LPCTSTR PathStr);
	short SetVideoFrameStyle(long* lpStyle);
	short SetVideoToolBarStyle(long* lpStyle);
	void SetUserPicture(long userid, LPCTSTR pathname);
	void SendTextMessage(long userid, long bSecret, LPCTSTR lpMsg, long msglen);
	CString GetUserNameByUserID(long userid);
	long IsInvisibleManager();
	void ActiveCallLog(long bActive);
	void CloseUserPanel(long userid);
	long GetUserPanelState(long userid);
	void SetUserPanelTitle(long userid, LPCTSTR lpTitleStr);
	void RefreshUserPanel(long userid);
	void PrepaGetWaitQueueList();
	long FetchNextWaitQueueUserID();
	void AnswerEnterRequest(long userid, long bAccept);
	void WhiteBoardOperateCtrl(long userid, long wbid, long optype, long* lpwbinfo);
	void WhiteBoardDrawData(long userid, long wbid, long drawtype, long* lpbuf, long nsize);
	void SendTextMessageEx(long userid, long bSecret, long* lpMsgBuf, long bufLen);
	void TransBuffer(long userid, long* lpbuf, long nlen);
	void ShowSettingPanel();
	void ChangeChatMode(long chatmode);
	long PrivateChatRequest(long userid);
	long PrivateChatEcho(long userid, long requestid, long bAccept);
	long PrivateChatExit(long userid);
	long GetUserChatMode(long userid);
	void ShowUserPanelEx2(long userid, short bShow, short left, short top, short right, short bottom, short style, long* pParentHandle);
	void AboutBox();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BRGAMEVIDEOCLIENT_H__F04582C6_891C_4E47_8A6B_8F81E9472211__INCLUDED_)
