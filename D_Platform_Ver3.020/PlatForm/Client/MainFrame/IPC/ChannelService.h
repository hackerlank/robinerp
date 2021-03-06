#pragma once

#include "sink.h"
#include "DataStorage.h"
#include "IPCControls.h"

//////////////////////////////////////////////////////////////////////////

//信道模块类
class CChannelService : public CWnd, public IChannelService, public IChannelMessageSink
{
	//变量定义
protected:
	HWND							m_hWndServer;						//接收窗口
	CDataStorage					m_DataStorage;						//数据对象
	CIPCRecvCopyData				m_IPCRecvCopyData;					//IPC 接收

	//接口指针
protected:
	IChannelEventSink				* m_pIChannelEventSink;				//回调接口
	IChannelMessageSink				* m_pIChannelMessageSink;			//回调接口

	//函数定义
public:
	//构造函数
	CChannelService();
	//析构函数
	virtual ~CChannelService();

	//基础接口
public:
	//释放对象
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//是否有效
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CChannelService))?true:false; }
	//接口查询
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//配置接口
public:
	//设置事件
	virtual bool __cdecl SetChannelEventSink(IChannelEventSink * pIUnknownEx);
	//设置消息
	virtual bool __cdecl SetChannelMessageSink(IChannelMessageSink * pIUnknownEx);
	//获取事件
	virtual void * __cdecl GetChannelEventSink(const IID & Guid, DWORD dwQueryVer);
	//获取消息
	virtual void * __cdecl GetChannelMessageSink(const IID & Guid, DWORD dwQueryVer);

	//功能接口
public:
	//是否连接
	virtual bool __cdecl IsConnect(bool bEfficacy);
	//创建函数
	virtual bool __cdecl CreateChannel(HWND hWndServer);
	//关闭函数
	virtual bool __cdecl CloseChannel(bool bNotifyServer, bool bNotifySink);
	//发送数据
	virtual bool __cdecl SendData(WORD wMainCmdID, WORD wSubCmdID);
	//发送数据
	virtual bool __cdecl SendData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize);

	//数据接口
public:
	//信道数据处理
	virtual bool __cdecl OnChannelMessage(const IPC_Head * pHead, const void * pIPCBuffer, WORD wDataSize, HWND hWndSend);

	//消息映射
protected:
	//IPC 消息
	afx_msg LRESULT OnIPCMessage(WPARAM wParam, LPARAM lParam);
	//IPC 消息
	afx_msg BOOL OnCopyData(CWnd * pWnd, COPYDATASTRUCT * pCopyDataStruct);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
