#include "StdAfx.h"
#include "GameSet.h"
#include ".\clientgamedlg.h"
#include "playcardsound.h"
#include <math.h>
#include <shlobj.h>

#ifdef  SUPER_VERSION
//#include "..\..\客户端组件\界面库.2003\resource.h"
#include "platform/CardSet.h"
#endif

//#include "..\..\游戏开发开发库\include\客户端\界面库\resource.h"
//#pragma comment(lib,"vfw32.lib")

//#define SAIZI_IMAGE      "awardpoint.bmp"
//#define SAIZI_IMAGEA     "1.bmp"

///判断是否为空闲状态
///@param a 返回值

#define IsStation(a); \
if (GetStationParameter()==GS_WAIT_SETGAME || GetStationParameter()==GS_WAIT_ARGEE || GetStationParameter()==GS_WAIT_NEXT) \
{ \
	return a; \
}
BEGIN_MESSAGE_MAP(CClientGameDlg, CLoveSendClassInExe)
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(IDM_BEGIN,OnHitBegin)			//点击牌
	ON_MESSAGE(IDM_LEFT_HITCARD,OnHitCard)			//点击牌
	ON_MESSAGE(IDM_HAVE_THING,OnHitHaveThing)		//有事要走
	ON_MESSAGE(IDM_SEND_THING,OnSendHaveThing)		//有事要走
	ON_MESSAGE(IDM_LOOK_LAST,OnLastTurn)
	ON_MESSAGE(IDM_AUTOCARD,OnAuto)					//托管
	ON_MESSAGE(IDM_COUNTER ,OnCounter)           ///点击记牌器
	ON_MESSAGE(IDM_DIALECT ,OnDialect)        ///点击方音

	ON_MESSAGE(IDM_ARGEE_LEAVE,OnArgeeUserLeft)		//同意用户离开
	ON_MESSAGE(IDM_RIGHT_HITCARD,OnRightHitCard)	//右键出牌
	ON_MESSAGE(IDM_CALL_SCORE,OnCallScore)				//叫分
	ON_MESSAGE(IDM_SORT,OnHitSortCard)				//排序

	ON_MESSAGE(IDM_PASS,OnHitPassCard)				//不出
	ON_MESSAGE(IDM_CUE,OnCue)					//提示
	ON_MESSAGE(IDM_OUT_CARD,OnHitOutCard)			//出牌

	ON_MESSAGE(IDM_ROB_NT,OnRobNT)					//抢地主
	ON_MESSAGE(IDM_ADD_DOUBLE,OnAddDouble)					//加棒
	ON_MESSAGE(IDM_SHOW_CARD,OnShowCard)					//亮牌
	ON_MESSAGE(IDM_CHUIYES,OnChuiYes)					//锤
	ON_MESSAGE(IDM_CHUINO,OnChuiNo)					//不锤
#ifdef _USE_VIDEO
	ON_MESSAGE(WM_GV_ENTERROOM,&CClientGameDlg::OnGVClientEnterRoom)
	ON_MESSAGE(WM_GV_USERATROOM,&CClientGameDlg::OnGVClientUserAtRoom)
	ON_MESSAGE(WM_GV_ONLINEUSER,&CClientGameDlg::OnGVClientOnlineUser)
	ON_MESSAGE(WM_GV_LOGINSYSTEM,&CClientGameDlg::OnGVClientLogin)
#endif
	ON_WM_TIMER()	

END_MESSAGE_MAP()

//构造函数
CClientGameDlg::CClientGameDlg() : CLoveSendClassInExe(&m_PlayView)
{
	//c++ test
	m_bPlayFlashVideo = false;
	m_iPlayCount = 0;
	m_iRoomBasePoint = 1;
	m_iRunPublish = 0;
	::ZeroMemory(m_Card,sizeof(m_Card));
	::ZeroMemory(m_CardCount,sizeof(m_CardCount));
	::ZeroMemory(m_iBackCard,sizeof(m_iBackCard));
	::ZeroMemory(m_DeskCard,sizeof(m_DeskCard));
	::ZeroMemory(m_iBaseCard,sizeof(m_iBaseCard));
	::ZeroMemory(m_iLastOutCard,sizeof(m_iLastOutCard));
	::ZeroMemory(m_CueMessage,sizeof(m_CueMessage));
	::ZeroMemory(&colorresource,sizeof(colorresource));

	::ZeroMemory(m_bConnectFlash,sizeof(m_bConnectFlash));
	::ZeroMemory(m_strFlash,sizeof(m_strFlash));

	//已初始化

	//::ZeroMemory(m_iLastCardCount,sizeof(m_iLastCardCount));
	//::ZeroMemory(m_iDeskCardCount,sizeof(m_iDeskCardCount));
	//::ZeroMemory(m_iMeCardList,sizeof(m_iMeCardList));

	bInitFinish = false; //防止旁观崩溃
	m_bAutoCard=false;
	m_LastOneCard=false;

	m_iDialectType = 0 ;

	//	m_iToux=FALSE;
	m_iVersion=DEV_HEIGHT_VERSION;
	m_iVersion2=DEV_LOW_VERSION;
	m_iThinkTime=0;	
	m_iBeenPlayGame=0;
	////	m_iCardCount=0;
	m_iMeCardCount=0;
	m_bShowIngLast=FALSE;
	m_bShowLast=FALSE;
	m_iAfterWaitAction = 2;		//初始化为自动离开 090311a1 JJ
	::memset(m_iLastCardCount,0,sizeof(m_iLastCardCount));

	//	m_bisexit=false;
	m_iNowOutPeople=-1;		//当前出牌者
	m_bCurrentOperationStation = 255;//当前叫分者
	m_iFirstOutPeople=-1;
	m_iBigOutPeople = -1;
	m_iNtPeople=-1;
	m_ResultCallScore=-1;			//初始化
	m_btAutoTime=0;
	m_bSortCard=0;
	m_bTimeOutCount=0;
	//::memset(m_iBackCard,0,sizeof(m_iBackCard));
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	::memset(m_iMeCardList,0,sizeof(m_iMeCardList));
	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		m_PlayView.SetAuto(i,false);
		m_PlayView.SetAwardPoint(i,0);
		m_iAIStation[i] = 0;
		m_PlayView.m_HandCard[i].SetCard(0,NULL,0);
		m_PlayView.m_UserCard[i].SetCard(0,NULL,0);
	}

	m_PlayView.m_MeBackCard.SetCard(0,NULL,0);

	m_hMciWnd1=NULL;
	m_hMciWnd2=NULL;
	m_hMciWnd3=NULL;
	m_bPlay= FALSE;

	memset(m_bIsFirstOne,0,sizeof(m_bIsFirstOne));
	memset(m_bIsFirstTow,0,sizeof(m_bIsFirstTow));
}

void CClientGameDlg::SetColor()				//设置当前颜色
{	
	colorresource.m_crDeskTxColor=RGB(240,240,240);
	colorresource.m_crListBkColor=RGB(240,240,250);
	colorresource.m_crListTxColor=RGB(0,0,0);	

	colorresource.m_crNormalTxColor=RGB(50,255,50);
	colorresource.m_crSysHeadTxColor=RGB(255,0,0);
	colorresource.m_crSystemTxColor=RGB(255,255,255);


	colorresource.m_crTalkBkColor=RGB(233,233,233);
	colorresource.m_crTalkNameColor=RGB(255,255,0);
	colorresource.m_crTalkTxColor=RGB(255,255,255);
}
//析构函数
CClientGameDlg::~CClientGameDlg()
{
	if(m_hMciWnd1)
	{
		MCIWndStop(m_hMciWnd1);
		MCIWndDestroy(m_hMciWnd1);
		m_hMciWnd1=NULL;
	}

	if(m_hMciWnd2)
	{
		MCIWndStop(m_hMciWnd2);
		MCIWndDestroy(m_hMciWnd2);
		m_hMciWnd2=NULL;
	}	

	if(m_hMciWnd3)
	{
		MCIWndStop(m_hMciWnd3);
		MCIWndDestroy(m_hMciWnd3);
		m_hMciWnd3=NULL;
	}
}

//数据绑定函数
void CClientGameDlg::DoDataExchange(CDataExchange * pDX)
{
	CLoveSendClassInExe::DoDataExchange(pDX);
	DoDataExchangeWebVirFace(pDX);
}

//初始化函数
BOOL CClientGameDlg::OnInitDialog()
{
	CLoveSendClassInExe::OnInitDialog();
	m_PlayView.m_showcoredlg.m_ucomtype=GetComType();
	//更新标题
	SetDialogIcon(LoadIcon(GetModuleHandle(GET_CLIENT_DLL_NAME(szTempStr)),MAKEINTRESOURCE(IDI_CLIENT_ROOM)),TRUE);
	m_bRightPower = GetProfileInt(TEXT("RightPower"),TRUE);
	/////////////////090311a1 JJ 添加读取配置文件来设置开场倒计时结束后是自动开始还是自动离开
	CString path = CBcfFile::GetAppPath();
	CString s = _T(path+GET_CLIENT_BCF_NAME(szTempStr));	//本地路径
	if (CBcfFile::IsFileExist(s))
	{
		CBcfFile f(s);
		m_iAfterWaitAction = f.GetKeyVal(_T("config"), _T("AfterWaitAction"), 2);
		if (m_iAfterWaitAction != 1 && m_iAfterWaitAction != 2)
		{
			m_iAfterWaitAction = 2;
		}
		m_bPlayFlashVideo = f.GetKeyVal(_T("config"),_T("PlayFlashVideo"),0)>0?true:false;
	}
	m_PlayView.m_Result.m_nPowerOfGold=Glb().m_nPowerOfGold;

#ifdef _USE_VIDEO
	CString  strVideoCfg=CINIFile::GetAppPath()+TEXT("VideoCfg.ini");
	if(!CINIFile::IsFileExist(strVideoCfg))
	{
		return 0;
	}
	CINIFile iniFile(strVideoCfg);
	CString serverip = iniFile.GetKeyVal(TEXT("VideoCfg"),TEXT("VideoServerIP"), "127.0.0.1");
	int port = iniFile.GetKeyVal(TEXT("VideoCfg"),TEXT("VideoServerPort"), 8906);

	m_GameVideoSet = MyGameVideo(serverip,port);

	if(m_pBRGameVideoSDK || 
		m_PlayView.GetSafeHwnd() == NULL)
		return 0;

	m_pBRGameVideoSDK = new CBRGameVideoSDK;
	if(m_pBRGameVideoSDK->InitGameVideoSDK(this))
	{
		return 0;
	}
	else
	{
		m_pBRGameVideoSDK->CloseGameVideoSDK();
		delete m_pBRGameVideoSDK;
		m_pBRGameVideoSDK = NULL;
	}
#endif
	//////////////////////////////////////////////////////////////////////////////////////////
	//if(m_pGameInfo->bEnableSound)
	//	PlayBackgroundMusic();

	return TRUE;
}

//设置游戏状态
bool CClientGameDlg::SetGameStation(void * pStationData, UINT uDataSize)
{
	bInitFinish = true;//初始化完成
	CreateTrustTxt();
	//Msg("Data_游戏状态 %d__玩家状态%d",GetStationParameter(),m_pUserInfo[GetMeUserInfo()->bDeskStation]->GameUserInfo.bUserState);
	//SetPlaySound(SOUND_WARNING);
	//if(NULL !=GetMeUserInfo())
	//{		
	//	PlayFlashVideo(GetMeUserInfo()->bDeskStation,m_pUserInfo[GetMeUserInfo()->bDeskStation],true);
	//}

#ifdef  VIDEO
	for ( int i = 0; i<PLAY_COUNT; i ++)
	{
		if (NULL == m_pUserInfo[i])
		{
			continue;
		}
		PlayFlashVideo(i,m_pUserInfo[i],GetMeUserInfo()->dwUserID == m_pUserInfo[i]->GameUserInfo.dwUserID);
	}
	SetTimer(ID_CONNECT_FLASH_TIME,100,NULL);
#endif
	
#ifdef POINT5_D
	m_PlayView.m_btChangeShin.ShowWindow(SW_SHOW);
	m_PlayView.m_btChangeShin.EnableWindow(TRUE);
#endif
		
	switch (GetStationParameter())
	{
	case GS_WAIT_SETGAME:	//游戏没有开始状态
	case GS_WAIT_ARGEE:	//等待同意状态
		{
			GameStation_2 * pGameStation=(GameStation_2 *)pStationData;
			//版本核对
			CheckVersion(pGameStation->iVersion,pGameStation->iVersion2);			
			//设置数据
			m_iBeginTime = pGameStation->iBeginTime;
			m_iThinkTime = pGameStation->iThinkTime;
			m_iGiveBackTime = pGameStation->iGiveBackTime;
			m_iCallScoreTime = pGameStation->iCallScoreTime;
			m_iAddDoubleTime = pGameStation->iAddDoubleTime;
			//运行时态消息
			m_iPalyCard = pGameStation->iPlayCard;
			m_iPlayCount = pGameStation->iPlayCount;
			m_iBackCount = pGameStation->iBackCount;

			m_iRoomBasePoint = pGameStation->iRoomBasePoint ;//房间倍数
			m_iRunPublish = pGameStation->iRunPublish ;		//逃跑扣分

			m_PlayView.SetRoomMultiple(m_iRoomBasePoint);
			m_PlayView.m_MeCard.SetShowCard(!m_bWatchMode||m_bWatchOther);
			//显示设置窗口
			m_PlayView.m_btAuto.ShowWindow(!m_bWatchMode);
			m_PlayView.m_btThing.ShowWindow(!m_bWatchMode);
			m_PlayView.m_btSortCard.ShowWindow(!m_bWatchMode);
		    m_PlayView.m_btCounter.ShowWindow(!m_bWatchMode);
		    m_PlayView.m_btdialect.ShowWindow(!m_bWatchMode);
			m_PlayView.m_btdialect.EnableWindow(true);

			m_Logic.SetCardShape(pGameStation->iCardShape);

			//CString s;
			//s.Format("CUsingSkinClass size in game:%d" ,sizeof(CUsingSkinClass));
			//AfxMessageBox(s);
			if(!m_bWatchMode)
			{
				///lym100115
				if (!IsQueueGameRoom()) 
				{ 
					m_PlayView.m_btContinue.ShowWindow(!m_bWatchMode);
					if(m_PlayView.m_btLeave.m_hWnd)
						m_PlayView.m_btLeave.ShowWindow(!m_bWatchMode);
					m_PlayView.m_btContinue.SetFocus();
					SetGameTimer(GetMeUserInfo()->bDeskStation,m_iBeginTime,ID_BEGIN_TIME);

				} 
				else
				{
					KillAllTimer();
					m_PlayView.m_btContinue.ShowWindow(SW_HIDE);
					if(m_PlayView.m_btLeave.m_hWnd)
						m_PlayView.m_btLeave.ShowWindow(SW_HIDE);
					m_PlayView.SetOrientation(-1);
					ResetGameStation(RS_GAME_BEGIN);

					if (USER_SITTING!=m_pUserInfo[GetMeUserInfo()->bDeskStation]->GameUserInfo.bUserState)
					{
						m_PlayView.m_btContinue.ShowWindow(!m_bWatchMode);
						if(m_PlayView.m_btLeave.m_hWnd)
							m_PlayView.m_btLeave.ShowWindow(!m_bWatchMode);
						m_PlayView.m_btContinue.SetFocus();
						SetGameTimer(GetMeUserInfo()->bDeskStation,m_iBeginTime,ID_BEGIN_TIME);
					}
				}			

#ifdef AI_MACHINE
				SetGameTimer(GetMeUserInfo()->bDeskStation,rand()% GAME_RAND_TIME +1,ID_BEGIN_TIME);
#endif			
				m_PlayView.SetOrientation(ViewStation(GetMeUserInfo()->bDeskStation));
			}
			return TRUE;
		}
	case GS_SEND_CARD:	//发牌状态
	case GS_WAIT_BACK:	//等待埋底牌状态
		{
			GameStation_3 * pGameStation=(GameStation_3 *)pStationData;
			//版本核对
			CheckVersion(pGameStation->iVersion,pGameStation->iVersion2);			

			//设置数据
			m_iBeginTime = pGameStation->iBeginTime;
			m_iThinkTime = pGameStation->iThinkTime;
			m_iGiveBackTime = pGameStation->iGiveBackTime;
			m_iCallScoreTime = pGameStation->iCallScoreTime;
			m_iAddDoubleTime = pGameStation->iAddDoubleTime;
			//运行时态消息
			m_iPalyCard = pGameStation->iPlayCard;
			m_iPlayCount = pGameStation->iPlayCount;
			m_iBackCount = pGameStation->iBackCount;

			m_iRoomBasePoint = pGameStation->iRoomBasePoint ;//房间倍数
			m_iRunPublish = pGameStation->iRunPublish ;		//逃跑扣分
			m_Logic.SetCardShape(pGameStation->iCardShape);

			m_PlayView.SetRoomMultiple(m_iRoomBasePoint);
			//设置界面
			m_PlayView.m_btContinue.ShowWindow(SW_HIDE);
			if(m_PlayView.m_btLeave.m_hWnd)
				m_PlayView.m_btLeave.ShowWindow(SW_HIDE);

			m_PlayView.m_bCanLeave = pGameStation->bCanleave[GetMeUserInfo()->bDeskStation];
			m_PlayView.m_btThing.EnableWindow(!m_bWatchMode &&(GetComType()!=2) && m_PlayView.m_bCanLeave);

			m_PlayView.m_btSortCard.ShowWindow(!m_bWatchMode);
			m_PlayView.m_btSortCard.EnableWindow(!m_bWatchMode);
			m_PlayView.m_btThing.ShowWindow(!m_bWatchMode);
			m_PlayView.m_btAuto.ShowWindow(!m_bWatchMode);
			m_PlayView.m_btAuto.EnableWindow(!m_bWatchMode);
			m_PlayView.m_btLastTurn.EnableWindow(!m_bWatchMode);
			m_PlayView.m_btLastTurn.ShowWindow(!m_bWatchMode);
			m_PlayView.m_btLastTurn.EnableWindow(false);

			m_PlayView.m_btCounter.ShowWindow(!m_bWatchMode);
			m_PlayView.m_btCounter.EnableWindow(!m_bWatchMode) ; 

			m_PlayView.m_btdialect.ShowWindow(!m_bWatchMode);
			m_PlayView.m_btdialect.EnableWindow(true);

			m_PlayView.m_btShowCard.ShowWindow(SW_HIDE);
			m_PlayView.m_btShowCard2.ShowWindow(SW_HIDE);

			//设置当前托管情况
			for(int i = 0; i < PLAY_COUNT; i ++)
			{
				m_PlayView.SetAuto(ViewStation(i),pGameStation->bAuto[i]);
				m_PlayView.SetChuiData(ViewStation(i),pGameStation->byUserChui[i]);	//锤一锤数据
			}

			if(!m_bWatchMode)
				m_bAutoCard = pGameStation->bAuto[GetMeUserInfo()->bDeskStation];
			//修改各用户手中的牌
			int iPos = 0;
			for(int i = 0;i < PLAY_COUNT; i ++)
			{
				m_CardCount[ViewStation(i)] = pGameStation->iUserCardCount[i];
				::CopyMemory(m_Card[ViewStation(i)],&pGameStation->iUserCardList[iPos],sizeof(BYTE)*m_CardCount[ViewStation(i)]);
				iPos+=m_CardCount[ViewStation(i)];
				m_Logic.SortCard(m_Card[ViewStation(i)],NULL,m_CardCount[ViewStation(i)]);

				m_PlayView.m_HandCard[ViewStation(i)].SetCard(m_Card[ViewStation(i)],NULL,m_CardCount[ViewStation(i)]);

				if(i == GetMeUserInfo()->bDeskStation)
				{
					::CopyMemory(m_iMeCardList,m_Card[ViewStation(i)],m_CardCount[ViewStation(i)]);
					m_iMeCardCount=m_CardCount[ViewStation(i)];
					m_Logic.SortCard(m_iMeCardList,NULL,m_iMeCardCount);
					m_PlayView.m_MeCard.SetCard(m_Card[ViewStation(i)],NULL,m_CardCount[ViewStation(i)]);
				}				
			}
			m_PlayView.m_MeCard.SetCard(m_iMeCardList,NULL,m_iMeCardCount);
			m_PlayView.m_MeCard.SetUseMouse(!m_bWatchMode);
			m_PlayView.m_MeCard.SetShowCard(!m_bWatchMode||m_bWatchOther);
			switch(pGameStation->iGameFlag)
			{
			case GS_FLAG_CALL_SCORE:
				{
					SetGameTimer(pGameStation->iCallScorePeople,m_iThinkTime,ID_CALL_SCORE_TIME);		//叫分时间设置
					m_PlayView.SetOrientation(ViewStation(pGameStation->iCallScorePeople));

					for(int i = 0;i < PLAY_COUNT; i ++)
						m_PlayView.SetCallScoreValue(ViewStation(i),pGameStation->iCallScore[i]);
					if(pGameStation->iCallScorePeople == GetMeUserInfo()->bDeskStation&&!m_bWatchMode)
						ShowCallScoreBt(pGameStation->iCallScoreResult);
					break;
				}
			case GS_FLAG_ROB_NT:
				{
					break;
				}
			case GS_FLAG_ADD_DOUBLE:
				{
					break;
				}
			case GS_FLAG_SHOW_CARD:
				{
					break;
				}
			}

			//设置地主
			m_PlayView.SetNTStation(ViewStation(pGameStation->iUpGradePeople));			
			m_PlayView.SetCallScoreResult(pGameStation->iCallScoreResult);		//设置叫分结果
			//底牌
			int iBackCount = 3;
			BYTE bTempBackCard[5] = {0};
			::CopyMemory(bTempBackCard,&pGameStation->iUserCardList[iPos-2],iBackCount);
			m_Logic.SortCard(bTempBackCard,NULL,sizeof(BYTE)*iBackCount);
			m_PlayView.m_MeBackCard.SetCard(&pGameStation->iUserCardList[iPos],NULL,sizeof(BYTE)*3);
			if (pGameStation->iGameFlag > GS_FLAG_ADD_DOUBLE)//加棒
			{
				m_PlayView.m_MeBackCard.SetShowCard(true);
			}
			else
			{
				m_PlayView.m_MeBackCard.SetShowCard(false);
			}
			m_PlayView.m_MeBackCard.ShowWindow(SW_SHOW);
			//加倍情况
			m_PlayView.SetRoomMultiple(m_iRoomBasePoint);
			m_PlayView.SetGameMultiple((int)pow(2.0,pGameStation->iBase));
			if (GetMeUserInfo()->bDeskStation != m_iNtPeople)
			{
				m_PlayView.m_MeBackCard.SetShowCard(true);
			}

			return TRUE;
		}
	case GS_PLAY_GAME:	//游戏进行中
		{
			GameStation_4 * pGameStation=(GameStation_4 *)pStationData;
			//版本核对
			CheckVersion(pGameStation->iVersion,pGameStation->iVersion2);
			//设置数据
			m_iBeginTime = pGameStation->iBeginTime;
			m_iThinkTime = pGameStation->iThinkTime;
			m_iGiveBackTime = pGameStation->iGiveBackTime;
			m_iCallScoreTime = pGameStation->iCallScoreTime;
			m_iAddDoubleTime = pGameStation->iAddDoubleTime;
			//运行时态消息
			m_iPalyCard = pGameStation->iPlayCard;
			m_iPlayCount = pGameStation->iPlayCount;
			m_iBackCount = pGameStation->iBackCount;

			//	m_iBeginNt = pGameStation->iBeginNt;
			//m_iEndNt = pGameStation->iEndNt;
			//设置游戏信息
			//m_Logic.SetTwoIsNT(pGameStation->bTwoIsNt);
			//m_Logic.SetHaveKing(pGameStation->bHaveKing);
			m_iRoomBasePoint = pGameStation->iRoomBasePoint ;//房间倍数
			m_iRunPublish = pGameStation->iRunPublish ;		//逃跑扣分

			m_PlayView.SetRoomMultiple(m_iRoomBasePoint);
			m_PlayView.SetGameMultiple((int)pow(2.0,pGameStation->iBase));

			m_iNowOutPeople = pGameStation->iOutCardPeople;
			m_iFirstOutPeople = pGameStation->iFirstOutPeople;
			m_iBigOutPeople = pGameStation->iBigOutPeople;
			m_Logic.SetCardShape(pGameStation->iCardShape);
			//设置界面按钮
			m_PlayView.m_btContinue.ShowWindow(SW_HIDE);
			if(m_PlayView.m_btLeave.m_hWnd)			
				m_PlayView.m_btLeave.ShowWindow(SW_HIDE);
			m_PlayView.m_btShowCard.ShowWindow(SW_HIDE);
			m_PlayView.m_btShowCard2.ShowWindow(SW_HIDE);

			m_PlayView.m_bCanLeave = pGameStation->bCanleave[GetMeUserInfo()->bDeskStation];
			m_PlayView.m_btThing.EnableWindow(!m_bWatchMode &&(GetComType()!=2) && m_PlayView.m_bCanLeave);

			m_PlayView.m_btSortCard.ShowWindow(!m_bWatchMode);
			m_PlayView.m_btSortCard.EnableWindow(!m_bWatchMode);
			m_PlayView.m_btThing.ShowWindow(!m_bWatchMode);
			m_PlayView.m_btAuto.ShowWindow(!m_bWatchMode);
			m_PlayView.m_btAuto.EnableWindow(!m_bWatchMode);
			m_PlayView.m_btLastTurn.EnableWindow(!m_bWatchMode);
			m_PlayView.m_btLastTurn.ShowWindow(!m_bWatchMode);

			m_PlayView.m_btCounter.ShowWindow(!m_bWatchMode);
			m_PlayView.m_btCounter.EnableWindow(!m_bWatchMode) ;

			m_PlayView.m_btdialect.ShowWindow(!m_bWatchMode);
			m_PlayView.m_btdialect.EnableWindow(true);

			//设置地主
			m_PlayView.SetNTStation(ViewStation(pGameStation->iUpGradePeople));
			//设置托管情况
			for(int i = 0; i < PLAY_COUNT; i ++)
			{
				CString s;
				s.Format("lbw setauto %d is %d", i, pGameStation->bAuto[i]);
				OutputDebugString(s.GetBuffer());

				m_PlayView.SetAuto(ViewStation(i),pGameStation->bAuto[i]);				//托管
				m_PlayView.SetAwardPoint(ViewStation(i),pGameStation->iAwardPoint[i]);	//将分
				m_PlayView.SetPeopleMultiple(ViewStation(i),pGameStation->iPeopleBase[i]);
			}
			if(!m_bWatchMode)
				m_bAutoCard = pGameStation->bAuto[GetMeUserInfo()->bDeskStation];

			m_PlayView.SetCallScoreResult(pGameStation->iCallScoreResult);		//设置叫分结果

			//设置扑克
			memset(m_CardCount,0,sizeof(m_CardCount));
			int iPos = 0;
			for(int i = 0; i < PLAY_COUNT; i ++)
			{	//其他用户牌
				if (NULL==m_pUserInfo[i])
				{
					continue;
				}
				m_CardCount[ViewStation(i)] = pGameStation->iUserCardCount[i];
				//::CopyMemory(m_Card[ViewStation(i)],&pGameStation->iUserCardList[iPos],sizeof(BYTE)*m_CardCount[ViewStation(i)]);
				::CopyMemory(m_Card[ViewStation(i)],&pGameStation->iUserCardList[iPos],sizeof(BYTE)*pGameStation->iUserCardCount[i]);
				iPos += m_CardCount[ViewStation(i)];
				m_Logic.SortCard(m_Card[ViewStation(i)],NULL,m_CardCount[ViewStation(i)]);
				m_PlayView.m_HandCard[ViewStation(i)].SetCard(m_Card[ViewStation(i)],NULL,m_CardCount[ViewStation(i)]);
				//自己牌
				if(i == GetMeUserInfo()->bDeskStation)
				{
					::CopyMemory(m_iMeCardList,m_Card[ViewStation(i)],m_CardCount[ViewStation(i)]);
					m_iMeCardCount = m_CardCount[ViewStation(i)];
					m_Logic.SortCard(m_iMeCardList,NULL,m_iMeCardCount);

					m_PlayView.m_MeCard.SetCard(m_Card[ViewStation(i)],NULL,m_CardCount[ViewStation(i)]);
				}
				//桌面上牌
				m_iDeskCardCount[i] = pGameStation->iDeskCardCount[i];
				::CopyMemory(m_DeskCard[i],&pGameStation->iUserCardList[iPos],sizeof(BYTE)*m_iDeskCardCount[i]);
				iPos += m_iDeskCardCount[i];
				m_Logic.SortCard(m_DeskCard[i],NULL,m_iDeskCardCount[i]);
				m_PlayView.m_UserCard[ViewStation(i)].SetCard(m_DeskCard[i],NULL,m_iDeskCardCount[i]);

				// 上一轮数据
				::CopyMemory(m_iLastCardCount, &pGameStation->iLastCardCount, sizeof(BYTE)*PLAY_COUNT );
				::CopyMemory(m_iLastOutCard[i], pGameStation->iLastOutCard[i], sizeof(BYTE)*pGameStation->iLastCardCount[i] );

				if ( ( pGameStation->bIsPass&(1<<i) ) == (1<<i) )
				{					
					if (i == m_iNowOutPeople)
					{
						m_PlayView.SetPass(ViewStation(i),0);
					}
					else
					{			
						m_PlayView.SetPass(ViewStation(i),1);
					}
					
				}
				if (!pGameStation->bIsLastCard)
				{
					m_PlayView.m_btLastTurn.EnableWindow(false);
				}
			}
			//地主亮牌状态跟踪
			if(0<=pGameStation->iUpGradePeople && PLAY_COUNT>pGameStation->iUpGradePeople && \
				GetMeUserInfo()->bDeskStation != pGameStation->iUpGradePeople)
			{
				m_PlayView.m_HandCard[ViewStation(pGameStation->iUpGradePeople)].SetShowCard(pGameStation->iRobNT[pGameStation->iUpGradePeople] >= 2);
			}
			//保存最后一手中最大牌
			m_iBaseCount = pGameStation->iBaseOutCount;
			::CopyMemory(m_iBaseCard,&pGameStation->iUserCardList[iPos],sizeof(BYTE)*m_iBaseCount);
			iPos += m_iBaseCount;

			::CopyMemory(m_PlayView.m_CounterDlg.m_iHistoryCard , pGameStation->iHistroyInfoData , sizeof(m_PlayView.m_CounterDlg.m_iHistoryCard));
			//底牌
			int iBackCount = pGameStation->iBackCardCount;
			BYTE bTempBackCard[5] = {0};
			::CopyMemory(bTempBackCard,&pGameStation->iUserCardList[iPos],iBackCount);
			m_Logic.SortCard(bTempBackCard,NULL,sizeof(BYTE)*iBackCount);
			m_PlayView.m_MeBackCard.SetCard(&pGameStation->iUserCardList[iPos],NULL,sizeof(BYTE)*pGameStation->iBackCardCount);
			m_PlayView.m_MeBackCard.SetShowCard(true);
			m_PlayView.m_MeBackCard.ShowWindow(SW_SHOW);
			
			m_Logic.SetSortCardStyle(0);
			m_Logic.SortCard(m_iMeCardList,NULL,m_iMeCardCount);
			m_PlayView.m_MeCard.SetCard(m_iMeCardList,NULL,m_iMeCardCount);

			m_PlayView.m_MeCard.SetUseMouse(!m_bWatchMode);
			m_PlayView.m_MeCard.SetShowCard(!m_bWatchMode||m_bWatchOther);

			//计时器
			if (m_iNowOutPeople != -1)
			{
				SetGameTimer(m_iNowOutPeople,m_iThinkTime,ID_OUT_CARD);
				m_PlayView.SetOrientation(ViewStation(m_iNowOutPeople));
				// add at 0526 出牌人面前的牌清空
				m_PlayView.m_UserCard[ViewStation(m_iNowOutPeople)].SetCard(NULL,NULL,0);;	//自己面前牌清0
			}
			//判断是否自己出牌	
			if (!m_bWatchMode && (m_iNowOutPeople == GetMeUserInfo()->bDeskStation))
			{
				//自己面前的牌清掉
				m_PlayView.m_UserCard[1].SetCard(0,NULL,0);
				m_PlayView.m_btPass.ShowWindow(m_iNowOutPeople != m_iBigOutPeople);
				m_PlayView.m_btPass.EnableWindow(m_iNowOutPeople != m_iBigOutPeople);
				m_PlayView.SetPass(ViewStation(GetMeUserInfo()->bDeskStation),0);

				m_PlayView.m_btOutCard.ShowWindow(SW_SHOW);
				m_PlayView.m_btCue.ShowWindow(SW_SHOW);
				m_PlayView.m_btCue.EnableWindow(TRUE);
				OnHitCard(0,0);
				if(m_bAutoCard )
					SetGameTimer(m_iNowOutPeople,GAME_AUTO_TIME,ID_OUT_CARD);

#ifdef AI_MACHINE
				SetGameTimer(m_iNowOutPeople,(rand() % GAME_RAND_TIME)+1,ID_OUT_CARD);
#endif
			}
			else if (m_iNowOutPeople == GetMeUserInfo()->bDeskStation)
			{
				// add at 0526 出牌人面前的牌清空
				m_PlayView.m_UserCard[1].SetCard(0,NULL,0);	//自己面前牌清0
			}

			return TRUE;
		}
	case GS_WAIT_NEXT:		//等待下一盘开始
		{
			GameStation_5 * pGameStation=(GameStation_5 *)pStationData;
			//版本核对
			CheckVersion(pGameStation->iVersion,pGameStation->iVersion2);			
			//设置数据
			m_iBeginTime = pGameStation->iBeginTime;
			m_iThinkTime = pGameStation->iThinkTime;
			m_iGiveBackTime = pGameStation->iGiveBackTime;
			m_iCallScoreTime = pGameStation->iCallScoreTime;
			m_iAddDoubleTime = pGameStation->iAddDoubleTime;

			//运行时态消息
			m_iPalyCard = pGameStation->iPlayCard;
			m_iPlayCount = pGameStation->iPlayCount;
			m_iBackCount = pGameStation->iBackCount;
			//	m_iBeginNt = pGameStation->iBeginNt;
			//m_iEndNt = pGameStation->iEndNt;
			//设置游戏信息
			//m_Logic.SetTwoIsNT(pGameStation->bTwoIsNt);
			//m_Logic.SetHaveKing(pGameStation->bHaveKing);
			m_iRoomBasePoint = pGameStation->iRoomBasePoint ;//房间倍数
			m_iRunPublish = pGameStation->iRunPublish ;		//逃跑扣分
			m_Logic.SetCardShape(pGameStation->iCardShape);

			m_PlayView.SetRoomMultiple(m_iRoomBasePoint);
			m_PlayView.m_MeCard.SetShowCard(!m_bWatchMode||m_bWatchOther);
			//显示设置窗口
			m_PlayView.m_btAuto.ShowWindow(!m_bWatchMode);
			m_PlayView.m_btThing.ShowWindow(!m_bWatchMode);
			m_PlayView.m_btSortCard.ShowWindow(!m_bWatchMode);
			m_PlayView.m_btCounter.ShowWindow(!m_bWatchMode);

			m_PlayView.m_btdialect.ShowWindow(!m_bWatchMode);
			m_PlayView.m_btdialect.EnableWindow(true);
			

			m_PlayView.m_btShowCard.ShowWindow(SW_HIDE);
			m_PlayView.m_btShowCard2.ShowWindow(SW_HIDE);

			m_PlayView.SetGameMultiple(1);

			if(!m_bWatchMode)
			{
				///lym100115
				if (!IsQueueGameRoom()) 
				{ 
					m_PlayView.m_btLeave.ShowWindow(!m_bWatchMode);
					//显示开始按钮 
					if(!m_bWatchMode)
					{
						m_PlayView.m_btContinue.ShowWindow(!m_bWatchMode);
						if(m_PlayView.m_btLeave.m_hWnd)m_PlayView.m_btLeave.ShowWindow(!m_bWatchMode);
						m_PlayView.m_btContinue.SetFocus();
						//比赛场自动开始			
						//if(2==GetComType())
						//{
						//	SetGameTimer(GetMeUserInfo()->bDeskStation,GAME_AUTO_TIME,ID_BEGIN_TIME);
						//}
						//else
						SetGameTimer(GetMeUserInfo()->bDeskStation,m_iBeginTime,ID_BEGIN_TIME);
#ifdef AI_MACHINE
						SetGameTimer(GetMeUserInfo()->bDeskStation,(rand() % GAME_RAND_TIME)+1,ID_BEGIN_TIME);
#endif
						m_PlayView.SetOrientation(ViewStation(GetMeUserInfo()->bDeskStation));
					}

				} 
				else
				{
					KillAllTimer();
					m_PlayView.m_btContinue.ShowWindow(SW_HIDE);
					if(m_PlayView.m_btLeave.m_hWnd)
						m_PlayView.m_btLeave.ShowWindow(SW_HIDE);
					m_PlayView.SetOrientation(-1);
					ResetGameStation(RS_GAME_BEGIN);
				}
			}

			return TRUE;
		}
	}
	return false;
}

//游戏消息处理函数
bool CClientGameDlg::HandleGameMessage(NetMessageHead * pNetHead, void * pNetData, UINT uDataSize)
{
    //CString str;

	if (pNetHead->bMainID==MDM_GM_GAME_NOTIFY&&bInitFinish)
	{
		switch (pNetHead->bAssistantID)
		{
		case ASS_UG_USER_SET:		//用户设置牌局
			{
				if (uDataSize!=sizeof(UserArgeeGame)) 
					return FALSE;
				SetStationParameter(GS_WAIT_ARGEE);
				UserArgeeGame * pGameSet=(UserArgeeGame *)pNetData;
				return TRUE;
			}
#ifdef SHAO_YANG
		case ASS_CHUI:
			{
				if(uDataSize !=  sizeof(ChuiStruct))
				{
					return FALSE;
				}

				ChuiStruct * pChuiSet=(ChuiStruct *)pNetData;

				if(pChuiSet->bDeskStation == GetMeUserInfo()->bDeskStation&&!m_bWatchMode)
				{
					m_PlayView.m_btChuiYes.ShowWindow(SW_SHOW);
					m_PlayView.m_btChuiYes.EnableWindow(TRUE);

					m_PlayView.m_btChuiNo.ShowWindow(SW_SHOW);
					m_PlayView.m_btChuiNo.EnableWindow(TRUE);

					SetGameTimer(GetMeUserInfo()->bDeskStation,10,TIME_CHUI);	//设置锤定时器
				}

				char cTemp[255] = {0};
				sprintf(cTemp,"选择锤将收取%d费用,结算时翻倍.不锤不收取费用,结算无影响.",pChuiSet->iFee);
				m_MessageHandle.InsertNormalMessage(cTemp);

				break;
			}
		case ASS_CHUI_RESULT:
			{
				if(uDataSize !=  sizeof(ChuiStruct))
				{
					return TRUE;
				}

				ChuiStruct * pChuiSet=(ChuiStruct *)pNetData;

				m_PlayView.SetChuiData(ViewStation(pChuiSet->bDeskStation),pChuiSet->bIsChui?2:0);	//设置锤一锤数据

				if(pChuiSet->bDeskStation == GetMeUserInfo()->bDeskStation&&!m_bWatchMode)
				{

					m_PlayView.m_btChuiYes.ShowWindow(SW_HIDE);	//锤
					m_PlayView.m_btChuiNo.ShowWindow(SW_HIDE);	//不锤
					KillAllTimer();	//杀死记死器
					m_PlayView.UpdateViewFace(NULL);

					char cTemp[255] = {0};
					if(pChuiSet->bIsChui)
					{
						sprintf(cTemp,"恭喜您加锤成功!");
					}
					else
					{
						sprintf(cTemp,"您已取消加锤!");
					}

					m_MessageHandle.InsertNormalMessage(cTemp);
				}

				break;
			}
#endif
		case ASS_GAME_BEGIN:		//游戏开始
			{				
				if (uDataSize != sizeof(BeginUpgradeStruct))
					return FALSE;
				::ZeroMemory(m_iAIStation,sizeof(m_iAIStation));//清空代打情况；20090218 by Shao
				BeginUpgradeStruct * pBegin=(BeginUpgradeStruct *)pNetData;
				m_bTimeOutCount = 0;
				m_btAutoTime = 0;
				m_iBeenPlayGame ++;
				SetStationParameter(GS_SEND_CARD);
				m_PlayView.m_GameNoteView.SetMessage(NULL);

				if (m_bWatchMode == TRUE) 
				{
					KillAllTimer();
					ResetGameStation(RS_GAME_BEGIN);
				}

				m_PlayView.m_bCanLeave = true;
				m_PlayView.m_btAuto.EnableWindow(!m_bWatchMode);
				m_PlayView.m_btThing.EnableWindow(!m_bWatchMode);
				m_PlayView.m_btSortCard.EnableWindow(!m_bWatchMode);
				m_PlayView.m_btCounter.EnableWindow(!m_bWatchMode) ; 

				m_PlayView.m_btdialect.ShowWindow(!m_bWatchMode);
				m_PlayView.m_btdialect.EnableWindow(true);

				m_PlayView.m_MeCard.SetUseMouse(!m_bWatchMode);
				m_PlayView.m_MeCard.SetShowCard(!m_bWatchMode||m_bWatchOther);

				memset(m_bIsFirstOne,0,sizeof(m_bIsFirstOne));
				memset(m_bIsFirstTow,0,sizeof(m_bIsFirstTow));

				m_PlayView.m_btChuiYes.ShowWindow(SW_HIDE);	//锤
				m_PlayView.m_btChuiNo.ShowWindow(SW_HIDE);	//不锤

				///20100115
				m_PlayView.SetGameMultiple(1);
				m_PlayView.UpdateViewFace(NULL);
				
				if(m_pGameInfo->bEnableSound)		//声音
				{
					//SetPlaySound(-1,SOUND_OUT_CARD);
				
					VoiceOut(255,"GAME_START.WAV");
				}

		
				return TRUE;
			}
		case ASS_GAME_PREPARE:	//翻开一张牌
			{
				if (uDataSize!=sizeof(PrepareStruct)) 
					return FALSE;
				PrepareStruct *pPrepare = (PrepareStruct *)pNetData;

				m_PlayView.m_MeBackCard.SetShowCard(TRUE);
				m_PlayView.m_MeBackCard.ShowWindow(SW_SHOW);
				
				return TRUE;
			}
		case ASS_SEND_CARD:			//服务器发牌
			{
				if (uDataSize!=sizeof(SendCardStruct)) 
					return FALSE;
				SendCardStruct *pSendCard=(SendCardStruct *)pNetData;

				

				int desk = ViewStation(pSendCard->bDeskStation);

				if(pSendCard->bDeskStation != GetMeUserInfo()->bDeskStation)
				{
					m_Card[desk][m_CardCount[desk]++] = pSendCard->bCard;
					m_Logic.SortCard(m_Card[desk],NULL,m_CardCount[desk]);
					m_PlayView.m_HandCard[desk].SetCard(m_Card[desk],NULL,m_CardCount[desk]);
				    return true;
				}
				m_iMeCardList[m_iMeCardCount++] = pSendCard->bCard;
				m_Logic.SortCard(m_iMeCardList,NULL,m_iMeCardCount);
				m_PlayView.m_MeCard.SetCard(m_iMeCardList,NULL,m_iMeCardCount);

				m_PlayView.UpdateViewFace(NULL);

				if(m_pGameInfo->bEnableSound)		//声音
				{
					//SetPlaySound(-1,SOUND_OUT_CARD);
					VoiceOut(255,"OUT_CARD.wav");
				}
				return TRUE;
			}
		case ASS_SEND_ALL_CARD:			//一次将一个人的牌全部发完
			{
				if (uDataSize!=sizeof(SendAllStruct)) 
					return FALSE;
				SendAllStruct * pSendAll=(SendAllStruct *)pNetData;
				int iPos = 0;
				for(int i = 0;i < PLAY_COUNT; i ++)
				{
					int desk = ViewStation(i);
					m_CardCount[desk] = pSendAll->iUserCardCount[i];
					::CopyMemory(m_Card[desk],&pSendAll->iUserCardList[iPos],sizeof(BYTE)*m_CardCount[desk]);
					iPos += m_CardCount[desk];
					m_Logic.SortCard(m_Card[desk],NULL,m_CardCount[desk]);
					m_PlayView.m_HandCard[desk].SetCard(m_Card[desk],NULL,m_CardCount[desk]);

					if(i == GetMeUserInfo()->bDeskStation)
					{
						::CopyMemory(m_iMeCardList,m_Card[desk],m_CardCount[desk]);
						m_iMeCardCount = m_CardCount[desk];
						m_Logic.SortCard(m_iMeCardList,NULL,m_iMeCardCount);
						m_PlayView.m_MeCard.SetCard(m_Card[desk],NULL,m_CardCount[desk]);
					}
				}

				if(m_pGameInfo->bEnableSound)		//声音
					SetPlaySound(SOUND_SEND_CARD);
				return TRUE;
			}
		case ASS_SEND_CARD_MSG://发牌消息
			{
				if(uDataSize!=sizeof(SendCardStruct))
					return FALSE;
				SendCardStruct *pCardMsg = (SendCardStruct*)pNetData;
				pCardMsg->bDeskStation ;
				wsprintf(m_CueMessage,"玩家 %s 最先叫地主",m_pUserInfo[pCardMsg->bDeskStation ]->GameUserInfo.nickName);
				m_MessageHandle.InsertNormalMessage(m_CueMessage);
			//	m_PlayView.m_UserCard[ViewStation(pCardMsg->bDeskStation)].SetCard(&pCardMsg->bCard,NULL,sizeof(BYTE)*1);
				return TRUE;

			}
		case ASS_SEND_FINISH:		//发牌完成
			{
				// 2010-04-29 dxh 修改发牌后，不显示3张底牌
				byte card[]={0x00,0x00,0x00};		//3张
				m_PlayView.m_MeBackCard.SetShowCard(0);	
				m_PlayView.m_MeBackCard.SetCard(card,NULL,/*m_iBackCount*/3);
				m_PlayView.m_MeBackCard.ShowWindow(1);
				for(int i = 0; i < PLAY_COUNT; i ++)
					m_PlayView.m_UserCard[i].SetCard(NULL,NULL,0);
				if(m_pGameInfo->bEnableSound)
					SetPlaySound(SOUND_START_DEF);
				return TRUE;
			}
		case ASS_CALL_SCORE:			//叫分
			{	
				SetStationParameter(GS_WAIT_BACK);
				CallScoreStruct *score=(CallScoreStruct *)pNetData;	
				m_bCurrentOperationStation = score->bDeskStation;
				if(m_bCurrentOperationStation != 255)
				{
					SetGameTimer(m_bCurrentOperationStation,m_iCallScoreTime,ID_CALL_SCORE_TIME);
					m_PlayView.SetOrientation(ViewStation(m_bCurrentOperationStation));
				}

				if(!m_bWatchMode && GetMeUserInfo()->bDeskStation == score->bDeskStation )
				{
					ShowCallScoreBt(score->iValue);
					if(m_pGameInfo->bEnableSound)		//声音
						SetPlaySound(SOUND_CALL_SCORE);
					if(m_bAutoCard)
						SetGameTimer(m_bCurrentOperationStation,GAME_AUTO_TIME,ID_CALL_SCORE_TIME);
#ifdef AI_MACHINE
					int result = rand()%4;
					if(result > score->iCallScore && result > 0 && result <= 3)
						;
					else 
						result = 0;
					OnCallScore(result,result);
#endif 
				}
				return true;
			}
		case ASS_CALL_SCORE_RESULT:			//叫分结果
			{
				if (uDataSize != sizeof(CallScoreStruct)) 
					return FALSE;
				KillTimer(ID_CALL_SCORE_TIME);
				HideButton();
				CallScoreStruct *score=(CallScoreStruct*)pNetData;
				m_PlayView.SetCallScoreValue(ViewStation(score->bDeskStation),score->iValue);		//显示各叫分状况
				if(m_pGameInfo->bEnableSound)
				{
					if (NULL == m_pUserInfo[score->bDeskStation])
					{
						return true;
					}					                    
					SetSceneSound(SOUND_CALL_SCORE,*this,m_pUserInfo[score->bDeskStation]->GameUserInfo.bBoy,score->iValue,m_iDialectType);
				}
				return true;
			}
		case ASS_CALL_SCORE_FINISH:
			{
				if (uDataSize != sizeof(CallScoreStruct)) 
					return FALSE;
				KillTimer(ID_CALL_SCORE_TIME);
				HideButton();

				//清除桌面叫分情况
				for(int i = 0; i < PLAY_COUNT; i ++)
					m_PlayView.SetCallScoreValue(i, -1);		

				m_bCurrentOperationStation = 255;

				CallScoreStruct *score=(CallScoreStruct*)pNetData;

				int iViewStation = ViewStation(score->bDeskStation);

				m_PlayView.SetNTStation(iViewStation);
				m_PlayView.SetCallScoreResult(score->iValue);

				////在左框顯示
				//if(m_pUserInfo[score->bDeskStation] != NULL)
				//{
				//	TCHAR szMessage[100];
				//	wsprintf(szMessage,TEXT("[提示]叫分结果: 地主为%s,地主叫 %d 分!"),m_pUserInfo[score->bDeskStation]->GameUserInfo.nickName,score->iValue);
				//	m_MessageHandle.InsertNormalMessage(szMessage);
				//}
				return true;
			}
		case ASS_ROB_NT:	//抢地主
			{
				if (uDataSize != sizeof(RobNTStruct)) 
					return FALSE;
				RobNTStruct * pRobNT=(RobNTStruct *)pNetData;
				m_bCurrentOperationStation = pRobNT->bDeskStation ;

				if(m_bCurrentOperationStation != 255)
				{
					SetGameTimer(m_bCurrentOperationStation,m_iCallScoreTime,ID_ROB_NT_TIME);

					m_PlayView.SetOrientation(ViewStation(m_bCurrentOperationStation));
				}

				//本方抢地主
				if(GetMeUserInfo()->bDeskStation == pRobNT->bDeskStation )
				{
					m_PlayView.m_btRobNT.ShowWindow(!m_bWatchMode);
					m_PlayView.m_btRobNT.EnableWindow(!m_bWatchMode);
					m_PlayView.m_btRobNT2.ShowWindow(!m_bWatchMode);
					m_PlayView.m_btRobNT2.EnableWindow(!m_bWatchMode);
#ifdef AI_MACHINE
					SetGameTimer(m_bCurrentOperationStation,(rand() % GAME_RAND_TIME)+1,ID_ROB_NT_TIME);
#endif			//if(m_bAutoCard)
					//	SetGameTimer(m_bCurrentOperationStation,GAME_AUTO_TIME,ID_ROB_NT_TIME);
				}

				return TRUE;
			}
		case ASS_ROB_NT_RESULT://抢地主结果
			{
				if (uDataSize != sizeof(RobNTStruct)) 
					return FALSE;

				RobNTStruct * pRobNTResult=(RobNTStruct *)pNetData;
				//str.Format("Settest ROB_NT_RESULT iValue = %d\n", pRobNTResult->iValue);

				//OutputDebugString(str);

				if(m_pGameInfo->bEnableSound && pRobNTResult->iValue > 0)		//声音
				{


				//	SetPlaySound(SOUND_ROB_NT,m_pUserInfo[pRobNTResult->bDeskStation]->GameUserInfo.bBoy);
					//MessageBox("NI");
				//	OutputDebugString("Settest SOUND_ROB_NT\n");
					int irand = rand()%3;
                    TCHAR chtemp[40];
					ZeroMemory(chtemp,sizeof(chtemp));
					wsprintf(chtemp,"human_ROB_NT%d.wav",irand);

					if(m_pGameInfo->bEnableSound)
					{
						VoiceOut(pRobNTResult->bDeskStation,chtemp);
					}
				}
				if(m_pGameInfo->bEnableSound && pRobNTResult->iValue == 0)
				{
					if(m_pGameInfo->bEnableSound)
					{
						VoiceOut(pRobNTResult->bDeskStation,"human_NOROB_NT.wav");
					}
				}

				/*if(m_pUserInfo[pRobNTResult->bDeskStation] != NULL&& pRobNTResult->iValue)
				{
				TCHAR szMessage[200];
					wsprintf(szMessage,"[提示] %s 抢地主!",m_pUserInfo[pRobNTResult->bDeskStation]->GameUserInfo.nickName);
					m_MessageHandle.InsertNormalMessage(szMessage);
				}*/	
				return TRUE;
			}
		case ASS_GAME_MULTIPLE:
			{
				if (uDataSize!=sizeof(AwardPointStruct)) 
					return FALSE;
				AwardPointStruct *awardpoint=(AwardPointStruct*)pNetData;

				m_PlayView.SetGameMultiple((int)pow(2.0,awardpoint->iBase));
				return TRUE;
			}
		case ASS_ROB_NT_FINISH:		//抢地主结束
			{
				if (uDataSize!=sizeof(RobNTStruct)) 
					return FALSE;

				RobNTStruct * pRobNTResult=(RobNTStruct *)pNetData;

				if(m_pUserInfo[pRobNTResult->bDeskStation] != NULL)
				{
					TCHAR szMessage[200];
					wsprintf(szMessage,"[提示] %s 成功抢到地主!",m_pUserInfo[pRobNTResult->bDeskStation]->GameUserInfo.nickName);
					m_MessageHandle.InsertNormalMessage(szMessage);
					m_PlayView.SetNTStation(ViewStation(pRobNTResult->bDeskStation));
				}
			
				return true;
			}
		case ASS_BACK_CARD_EX:		//扩展底牌数据
			{
				if (uDataSize!=sizeof(BackCardExStruct)) 
					return FALSE;
				BackCardExStruct * pBackCard=(BackCardExStruct *)pNetData;
				//顯示底牌
				m_PlayView.m_MeBackCard.ShowWindow(SW_SHOW);		//显示底牌
				m_Logic.SortCard(pBackCard->iBackCard,NULL,pBackCard->iBackCardCount);
				m_PlayView.m_MeBackCard.SetCard(pBackCard->iBackCard,NULL,pBackCard->iBackCardCount);

				//设置数据
				m_iNtPeople = pBackCard->iGiveBackPeople;
				if(m_iNtPeople != GetMeUserInfo()->bDeskStation)
				{
					int desk = ViewStation(m_iNtPeople);
					::CopyMemory(&m_Card[desk][m_CardCount[desk]],pBackCard->iBackCard,sizeof(BYTE)*(pBackCard->iBackCardCount));
					m_CardCount[desk] += pBackCard->iBackCardCount;
					m_Logic.SortCard(m_Card[desk],NULL,m_CardCount[desk]);
					::memset(m_Card[desk],0,sizeof(m_Card[desk]));	//清空玩家牌数据
					m_PlayView.m_HandCard[desk].SetCard(m_Card[desk],NULL,m_CardCount[desk]);
					return true;
				}				
				//设置界面
				m_PlayView.m_MeCard.SetUseMouse(!m_bWatchMode);
				if (/*!m_bWatchMode && */GetMeUserInfo()->bDeskStation == pBackCard->iGiveBackPeople)
				{
					BYTE bUp[45];
					::memset(bUp,0,sizeof(bUp));
					::CopyMemory(&m_iMeCardList[m_iMeCardCount],pBackCard->iBackCard,sizeof(BYTE)*pBackCard->iBackCardCount);
					m_iMeCardCount += pBackCard->iBackCardCount;
					m_Logic.SortCard(m_iMeCardList,bUp,m_iMeCardCount);
					m_PlayView.m_MeCard.SetCard(m_iMeCardList,bUp,m_iMeCardCount);
				}
				return TRUE;
			}

		case ASS_ADD_DOUBLE://加棒
			{
				if (uDataSize!=sizeof(AddDoubleStruct)) 
					return FALSE;
				AddDoubleStruct * pAddDouble=(AddDoubleStruct *)pNetData;

				SetGameTimer(GetMeUserInfo()->bDeskStation,m_iAddDoubleTime,ID_ADD_DOUBLE_TIME);

				//加棒
				if(GetMeUserInfo()->bDeskStation != pAddDouble->bDeskStation &&!m_bWatchMode)
				{
					m_PlayView.m_btAddDouble.ShowWindow(!m_bWatchMode);
					m_PlayView.m_btAddDouble2.ShowWindow(!m_bWatchMode);
					m_PlayView.m_btAddDouble.EnableWindow(!m_bWatchMode);
					m_PlayView.m_btAddDouble2.EnableWindow(!m_bWatchMode);

					m_PlayView.SetOrientation(ViewStation(GetMeUserInfo()->bDeskStation));
#ifdef AI_MACHINE
					OnAddDouble(rand()%2,rand()%2);
#endif			
				}				
				return TRUE;
			}
		case ASS_ADD_DOUBLE_RESULT://加棒结果
			{
				if (uDataSize!=sizeof(AddDoubleStruct)) 
					return FALSE;
				KillTimer(ID_ADD_DOUBLE_TIME);
				AddDoubleStruct * pAddDoubleResult=(AddDoubleStruct *)pNetData;
				//str.Format("Settest ADD_DOUBLE_RESULT iValue = %d\n", pAddDoubleResult->iValue);
				//增加个人倍数
				m_PlayView.SetPeopleMultiple(ViewStation(pAddDoubleResult->bDeskStation),pAddDoubleResult->iValue);
				//OutputDebugString(str);
				//设置加棒声音
				//if(m_pGameInfo->bEnableSound )		//声音
				{
					//if ( pAddDoubleResult->iValue == 2)
					{
						//VoiceOut(pAddDoubleResult->bDeskStation,"human_JIABEI.wav");
					}
				/*	else
					{
						VoiceOut(pAddDoubleResult->bDeskStation,"human_BUJIABEI.wav");
					}*/
				}

				m_PlayView.m_btAddDouble.ShowWindow(SW_HIDE);
				m_PlayView.m_btAddDouble2.ShowWindow(SW_HIDE);

				return true;
			}
		case ASS_ADD_DOUBLE_FINISH:
			{
				m_PlayView.m_MeBackCard.SetShowCard(true);
				KillTimer(ID_ADD_DOUBLE_TIME);
				m_PlayView.m_btAddDouble.ShowWindow(SW_HIDE);
				m_PlayView.m_btAddDouble2.ShowWindow(SW_HIDE);
				return true;
			}
		case ASS_SHOW_CARD://亮牌
			{
				if (uDataSize!=sizeof(ShowCardStruct)) 
					return FALSE;
				ShowCardStruct  * pShow = (ShowCardStruct*)pNetData;
				SetGameTimer(pShow->bDeskStation,m_iAddDoubleTime,ID_SHOW_CARD);
				m_PlayView.SetOrientation(ViewStation(pShow->bDeskStation));
				
				m_bCurrentOperationStation = pShow->bDeskStation ;

				if(!m_bWatchMode && GetMeUserInfo()->bDeskStation == pShow->bDeskStation )
				{
					m_PlayView.m_btShowCard.ShowWindow(!m_bWatchMode);
					m_PlayView.m_btShowCard2.ShowWindow(!m_bWatchMode);
					m_PlayView.m_btShowCard.EnableWindow(!m_bWatchMode);
					m_PlayView.m_btShowCard2.EnableWindow(!m_bWatchMode);
				}
				return TRUE;
			}
		case ASS_SHOW_CARD_RESULT:
			{
				if (uDataSize!=sizeof(ShowCardStruct)) 
					return FALSE;
				KillTimer(ID_SHOW_CARD);
				m_PlayView.m_btShowCard.ShowWindow(SW_HIDE);
				m_PlayView.m_btShowCard2.ShowWindow(SW_HIDE);
				ShowCardStruct  * pShow = (ShowCardStruct*)pNetData;

				//str.Format("Settest SHOW_CARD_RESULT iValue = %d\n", pShow->iValue);
				//OutputDebugString(str);
				if(pShow->iValue)
				{
					m_PlayView.SetGameMultiple((int)pow(2.0,pShow->iBase)); //20081204 修正亮牌时左上角不改变倍数的问题	
					m_CardCount[ViewStation(pShow->bDeskStation)] = pShow->iCardCount;
					::CopyMemory(m_Card[ViewStation(pShow->bDeskStation)],pShow->iCardList,sizeof(BYTE)*pShow->iCardCount);
					m_Logic.SortCard(m_Card[ViewStation(pShow->bDeskStation)],NULL,pShow->iCardCount);
					m_PlayView.m_HandCard[ViewStation(pShow->bDeskStation)].SetCard(m_Card[ViewStation(pShow->bDeskStation)],NULL,pShow->iCardCount);
					m_PlayView.m_HandCard[ViewStation(pShow->bDeskStation)].SetShowCard(TRUE);
					m_MessageHandle.InsertNormalMessage("地主亮牌,游戏倍数X2");
					if(m_pGameInfo->bEnableSound)		//声音
					{
						/*SetPlaySound(SOUND_SHOW_CARD);*/
						//	OutputDebugString("Settest SOUND_SHOW_CARD\n");
						VoiceOut(pShow->bDeskStation,"human_LIANG.wav");
					}
				}
				

				return TRUE;
			}
		case ASS_SHOW_CARD_FINISH:
			{
				return true;
			}
		case ASS_GAME_PLAY:		//开始升级游戏
			{
				//如果观看,则不能托管
				if (uDataSize!=sizeof(BeginPlayStruct)) 
					return FALSE;

				m_byteHitPass = 0;

				BeginPlayStruct * pBeginInfo=(BeginPlayStruct *)pNetData;
				m_bTimeOutCount=0;
				SetStationParameter(GS_PLAY_GAME);
				m_iBigOutPeople = m_iNowOutPeople=m_iFirstOutPeople = pBeginInfo->iOutDeskStation;
				if(m_iNowOutPeople != -1)
				{
					SetGameTimer(m_iNowOutPeople,m_iThinkTime,ID_OUT_CARD);
					m_PlayView.SetOrientation(ViewStation(m_iNowOutPeople));
				}	

				if ((m_bWatchMode==FALSE)&&(GetMeUserInfo()->bDeskStation==m_iNowOutPeople))
				{
					m_PlayView.m_bAIChooseCard = false ; 

					m_PlayView.m_btOutCard.ShowWindow(SW_SHOW);
					m_PlayView.m_btPass.EnableWindow(false);			//第一个出牌者一定得出牌
					m_PlayView.m_btPass.ShowWindow(SW_HIDE);
					m_PlayView.m_btCue.ShowWindow(SW_SHOW);
					m_PlayView.m_btCue.EnableWindow(TRUE);

					FlashWindowEx(FLASHW_TIMERNOFG|FLASHW_TRAY, 1, 0);

					OnHitCard(0,0);
					if(m_bAutoCard)
						SetGameTimer(m_iNowOutPeople,GAME_AUTO_TIME,ID_OUT_CARD);
#ifdef AI_MACHINE
					SetGameTimer(m_iNowOutPeople,(rand()%GAME_RAND_TIME )+ 1,ID_OUT_CARD);
#endif 					
				}
				//BOOL bUpGrade=((GetMeUserInfo()->bDeskStation==m_iNowOutPeople)||
				//	((GetMeUserInfo()->bDeskStation+2)%4==m_iNowOutPeople));
				//				m_PlayView.SetGamePoint(0,!bUpGrade);
				for (int i = 0; i < PLAY_COUNT; i++) 
					m_PlayView.m_UserCard[i].SetCard(NULL,NULL,0);
				m_PlayView.m_GameNoteView.SetMessage(NULL);


				//SetPlaySound(-1,SOUND_GAME_BEGIN);

				if(m_pGameInfo->bEnableSound)			//声音设置
				{
					//SetPlaySound(-1,SOUND_GAME_BEGIN);
					VoiceOut(0,"GAME_START.WAV");
				}
				return TRUE;
			}
		case ASS_OUT_CARD_RESULT:
			{
				//JJTest
				/*CString strTemp;
				strTemp.Format("%ul", GetTickCount());
				m_MessageHandle.InsertNormalMessage(strTemp.GetBuffer());*/
				//end
				OutCardMsg * pOutCardInfo=(OutCardMsg *)pNetData;
				KillAllTimer();
				m_PlayView.SetOrientation(-1);
				if (GetMeUserInfo()->bDeskStation == pOutCardInfo->bDeskStation)
				{
					m_PlayView.m_btOutCard.ShowWindow(false);
					m_PlayView.m_btPass.ShowWindow(false);
					m_PlayView.m_btCue.ShowWindow(false);
				}

				m_iDeskCardCount[pOutCardInfo->bDeskStation] = pOutCardInfo->iCardCount;
				::CopyMemory(m_DeskCard[pOutCardInfo->bDeskStation],pOutCardInfo->iCardList,sizeof(BYTE)*pOutCardInfo->iCardCount);
				//为第一个出牌者
				if(pOutCardInfo->iCardCount > 0)
				{
					m_PlayView.SetPass(ViewStation(pOutCardInfo->bDeskStation),0);
					m_iBigOutPeople = pOutCardInfo->bDeskStation;


					//-------------------------------------------------------------------
					//			if(m_pGameInfo->bEnableSound)
					//				SetPlaySound((m_Logic.IsBomb(pOutCardInfo->iCardList,pOutCardInfo->iCardCount)||m_Logic.IsKingBomb(pOutCardInfo->iCardList,pOutCardInfo->iCardCount))?SOUND_OUT_CARD_BOMB:SOUND_OUT_CARD_DEF);
					if(m_pGameInfo->bEnableSound)				//普通牌型出牌和炸弹出牌
					{
						//SetPlaySound(-1,SOUND_OUT_CARD,(m_Logic.IsBomb(pOutCardInfo->iCardList,pOutCardInfo->iCardCount)||m_Logic.IsKingBomb(pOutCardInfo->iCardList,pOutCardInfo->iCardCount)));

						switch(m_Logic.GetCardShape(pOutCardInfo->iCardList,pOutCardInfo->iCardCount))
						{
						case UG_ONLY_ONE:
							{
								TCHAR szTemp[20];
								ZeroMemory(szTemp,sizeof(szTemp));
								int iTemp=m_Logic.GetCardNum(pOutCardInfo->iCardList[0]);
								if(iTemp==14)
								{
									iTemp=1;
								}else if(iTemp>14)
								{
									iTemp--;
								}

								wsprintf(szTemp,"human_%d.wav",iTemp);
								VoiceOut(pOutCardInfo->bDeskStation,szTemp);
								
							}
							break;
						case UG_DOUBLE:
							{
								TCHAR szTemp[20];
								ZeroMemory(szTemp,sizeof(szTemp));
								int iTemp=m_Logic.GetCardNum(pOutCardInfo->iCardList[0]);
								if(iTemp==14)
								{
									iTemp=1;
								}else if(iTemp>14)
								{
									VoiceOut(pOutCardInfo->bDeskStation,"OUT_CARD.wav");
									break;
								}
								wsprintf(szTemp,"human_DB_%d.wav",iTemp);
								VoiceOut(pOutCardInfo->bDeskStation,szTemp);								

							}
							break;
						case UG_STRAIGHT:
							{
								VoiceOut(pOutCardInfo->bDeskStation,"human_LINE.wav");
							}
							break;
						case UG_DOUBLE_SEQUENCE:
							{
								VoiceOut(pOutCardInfo->bDeskStation,"human_DB_LINE.wav");
							}
							break;
						case UG_FOUR_TWO:
							{
								VoiceOut(pOutCardInfo->bDeskStation,"human_RECT_ONE.wav");
							}
							break;
						case UG_FOUR_TWO_DOUBLE:
							{
								VoiceOut(pOutCardInfo->bDeskStation,"human_RECT_DB.wav");
							}
							break;
						case UG_THREE:
							{
								VoiceOut(pOutCardInfo->bDeskStation,"human_TRI.wav");
							}
							break;
						case UG_THREE_ONE:
							{
								VoiceOut(pOutCardInfo->bDeskStation,"human_TRI_ONE.wav");
							}
							break;
						case UG_THREE_DOUBLE:
							{
								VoiceOut(pOutCardInfo->bDeskStation,"human_TRI_DB.wav");
							}
							break;
						case UG_BOMB:
							{
							
								VoiceOut(pOutCardInfo->bDeskStation,"human_MOST_CARD.wav");
							}
							break;
						case UG_KING_BOMB:
							{
								
								VoiceOut(pOutCardInfo->bDeskStation,"human_MISSILE.wav");
							}
							break;
						case UG_THREE_ONE_SEQUENCE:
						case UG_THREE_TWO_SEQUENCE:
						case UG_THREE_DOUBLE_SEQUENCE:
							{

								VoiceOut(pOutCardInfo->bDeskStation,"MALE_AIRPLANE.wav");
							}
							break;
						default:
							VoiceOut(pOutCardInfo->bDeskStation,"human_OUT_CARD.wav");
							break;
						}
					}
					//----------------------------------------------------------------------

				}
				else
				{
					m_PlayView.SetPass(ViewStation(pOutCardInfo->bDeskStation),1);

					//if(m_pGameInfo->bEnableSound)
					//	SetPlaySound(SOUND_OUT_CARD_PASS);

					if(m_pGameInfo->bEnableSound)				//普通牌型出牌和炸弹出牌
					{
						//SetPlaySound(-1,SOUND_GAME_PASS);
						switch(rand()%4)
						{
						case 0:
							{
								VoiceOut(pOutCardInfo->bDeskStation,"human_PASS_A.wav");
								break;
							}
						case 1:
							{
								VoiceOut(pOutCardInfo->bDeskStation,"human_PASS_B.wav");
								break;
							}
						case 2:
							{
								VoiceOut(pOutCardInfo->bDeskStation,"human_PASS_C.wav");
								break;
							}
						case 3:
							{
								VoiceOut(pOutCardInfo->bDeskStation,"human_PASS_D.wav");
								break;
							}
						default:
							{
								VoiceOut(pOutCardInfo->bDeskStation,"human_PASS_A.wav");
								break;
							}
						}
					}
				}


				//记录出牌信息
				if (pOutCardInfo->iCardCount > 0)
				{
					m_iBaseCount = pOutCardInfo->iCardCount;
					::CopyMemory(m_iBaseCard,pOutCardInfo->iCardList,sizeof(BYTE)*m_iBaseCount);
					if(pOutCardInfo->bDeskStation!=GetMeUserInfo()->bDeskStation)
					{
						BYTE desk = ViewStation(pOutCardInfo->bDeskStation);
						m_Logic.RemoveCard(pOutCardInfo->iCardList,pOutCardInfo->iCardCount,m_Card[desk],m_CardCount[desk]);
						m_CardCount[desk] -= pOutCardInfo->iCardCount;
						m_Logic.SortCard(m_Card[desk],NULL,m_CardCount[desk]);
						m_PlayView.m_HandCard[desk].SetCard(m_Card[desk],NULL,m_CardCount[desk]);
					}
					else			//删除自己的扑克
					{
						m_Logic.RemoveCard(pOutCardInfo->iCardList,pOutCardInfo->iCardCount,m_iMeCardList,m_iMeCardCount);
						m_iMeCardCount -= pOutCardInfo->iCardCount;
						m_Logic.SortCard(m_iMeCardList,NULL,m_iMeCardCount);
						m_PlayView.m_MeCard.SetCard(m_iMeCardList,NULL,m_iMeCardCount);
					}

					///记牌器中的数据
					for(int  i = 0 ; i<pOutCardInfo->iCardCount ; i++)
					{
						m_PlayView.m_CounterDlg.m_iHistoryCard[m_Logic.GetCardBulk(pOutCardInfo->iCardList[i] ,false)] ++ ;
					}

					m_PlayView.m_CounterDlg.Invalidate() ; 
				}

				BYTE desk = ViewStation(pOutCardInfo->bDeskStation);
				if ( m_CardCount[desk]==1  && !m_bIsFirstOne[desk] )
				{
					m_bIsFirstOne[desk] = true;
					if(m_pGameInfo->bEnableSound)
					{
						VoiceOut(pOutCardInfo->bDeskStation,"human_ONLY_1.wav");
					}
				}
				if ( m_CardCount[desk]==2 && !m_bIsFirstTow[desk] )
				{
					m_bIsFirstTow[desk] = true;
					if(m_pGameInfo->bEnableSound)
					{
						VoiceOut(pOutCardInfo->bDeskStation,"human_ONLY_2.wav");
					}
				}

				//显示扑克
				int iViewStation = ViewStation(pOutCardInfo->bDeskStation);
				m_Logic.SortCard(m_DeskCard[pOutCardInfo->bDeskStation],NULL,m_iDeskCardCount[pOutCardInfo->bDeskStation]);
				m_PlayView.m_UserCard[iViewStation].SetCard(m_DeskCard[pOutCardInfo->bDeskStation],NULL,m_iDeskCardCount[pOutCardInfo->bDeskStation]);
				m_PlayView.UpdateViewFace(NULL);
				return TRUE;
			}
		case ASS_AWARD_POINT://将分
			{
				if (uDataSize != sizeof(AwardPointStruct)) 
					return FALSE;
				AwardPointStruct *awardpoint = (AwardPointStruct*)pNetData;
				m_PlayView.SetAwardPoint(ViewStation(awardpoint->bDeskStation),awardpoint->iAwardPoint);
				m_PlayView.SetGameMultiple((int)pow(2.0,awardpoint->iBase));
				return true;
			}
		case ASS_OUT_CARD:		//用户出牌
			{
				OutCardMsg * pOutCardInfo = (OutCardMsg *)pNetData;
				KillAllTimer();

				//判断下一出牌者
				m_iNowOutPeople = pOutCardInfo->iNextDeskStation;
				m_PlayView.SetPass(ViewStation(m_iNowOutPeople),0);



				
				//static int i = 10;
				//BZUIPlayStand(this,WM_USER+100,i++,"x1.bmp",true,4,300,3, 200, 200,0,0);


				if (m_iNowOutPeople != -1) 
				{
					SetGameTimer(m_iNowOutPeople,m_iThinkTime,ID_OUT_CARD);
					m_PlayView.SetOrientation(ViewStation(m_iNowOutPeople));

					// add at 0526 出牌人面前的牌清空
					m_PlayView.m_UserCard[ViewStation(m_iNowOutPeople)].SetCard(NULL,NULL,0);;	//自己面前牌清0
					/*if(m_iAIStation[m_iNowOutPeople])
						SetGameTimer(m_iNowOutPeople,GAME_AUTO_TIME,ID_OUT_CARD);*/
				}
				if ((m_iNowOutPeople == GetMeUserInfo()->bDeskStation)&&(m_bWatchMode==FALSE))
				{
					m_PlayView.m_bAIChooseCard = false ; 

					m_PlayView.m_UserCard[ViewStation(GetMeUserInfo()->bDeskStation)].SetCard(NULL,NULL,0);;	//自己面前牌清0
					m_PlayView.m_btOutCard.ShowWindow(SW_SHOW);
					m_PlayView.m_btPass.ShowWindow(SW_SHOW);
					m_PlayView.m_btPass.EnableWindow(TRUE);
					m_PlayView.m_btCue.ShowWindow(SW_SHOW);
					m_PlayView.m_btCue.EnableWindow(TRUE);

					FlashWindowEx(FLASHW_TIMERNOFG|FLASHW_TRAY, 1, 0);

					OnHitCard(0,0);

					if(m_iMeCardCount < 4 && m_iMeCardCount < m_iBaseCount && m_Logic.GetCardShape(m_iMeCardList,m_iMeCardCount) != UG_KING_BOMB)
					{
						OnHitPassCard(0,0);	//不要
						//m_LastOneCard=true;
						//UserOutCard();
					}

					if(m_bAutoCard)
						SetGameTimer(m_iNowOutPeople,GAME_AUTO_TIME,ID_OUT_CARD);
#ifdef AI_MACHINE
					SetGameTimer(m_iNowOutPeople,(rand()%GAME_RAND_TIME) + 1,ID_OUT_CARD);
#endif 
				}
				else if(m_iNowOutPeople == GetMeUserInfo()->bDeskStation)
				{
					m_PlayView.m_UserCard[ViewStation(GetMeUserInfo()->bDeskStation)].SetCard(NULL,NULL,0);;	//自己面前牌清0
				}
				return TRUE;
			}
		case ASS_AI_STATION:
			{
				if(uDataSize != sizeof(UseAIStation))
					return FALSE;
				UseAIStation *ai = (UseAIStation*)pNetData;
				if (m_iAIStation[ai->bDeskStation] == ai->bState)
				{
					return TRUE;
				}
				m_iAIStation[ai->bDeskStation] = ai->bState;
				//在左框顯示
				TCHAR szMessage[100];
				if(m_pUserInfo[ai->bDeskStation]!=NULL)
				{
					wsprintf(szMessage,TEXT("[提示] %s %s机器人托管"),
						m_pUserInfo[ai->bDeskStation]->GameUserInfo.nickName,ai->bState?TEXT("设置"):TEXT("取消"));
				}
				else
				{
					wsprintf(szMessage,TEXT("[提示] %d 号位置 %s机器人托管"),
						ViewStation(ai->bDeskStation),ai->bState?TEXT("设置"):TEXT("取消"));
				}
				m_MessageHandle.InsertNormalMessage(szMessage);
				return true;
				if(ai->bState && m_bWatchMode==FALSE)//代替出牌
				{
					if(GetStationParameter() == 22)//出牌
					{
						if(ai->bDeskStation == m_iNowOutPeople)
							ReplaceOutCard(m_iNowOutPeople);
					}
					if(GetStationParameter() == 21)//叫分
					{
						if(ai->bDeskStation == m_bCurrentOperationStation)
							ReplaceCallScore(m_bCurrentOperationStation);
					}
				}
				return true;
			}
		case ASS_ONE_TURN_OVER:
			{
				m_PlayView.m_btLastTurn.EnableWindow(!m_bWatchMode);
				//m_PlayView.m_btThing.EnableWindow(TRUE);
				//保存上一轮出牌情况
				for(int i = 0; i < PLAY_COUNT; i ++)
				{
					m_iLastCardCount[i] = m_iDeskCardCount[i];
					::CopyMemory(m_iLastOutCard[i],m_DeskCard[i],sizeof(BYTE)*m_iLastCardCount[i]);
				}
				return true;
			}
		case ASS_NEW_TURN:		//新一轮出牌
			{
				if(uDataSize!=sizeof(NewTurnStruct))
					return FALSE;
				NewTurnStruct *turn = (NewTurnStruct *)pNetData; 

				m_byteHitPass = 0;

				for(int i = 0; i < PLAY_COUNT; i ++)
				{
					m_PlayView.SetPass(i,0);
				}
				//m_PlayView.m_btLastTurn.EnableWindow(!m_bWatchMode);
				//保存上一轮出牌情况
				for(int i = 0; i < PLAY_COUNT; i ++)
				{
					m_iLastCardCount[i] = m_iDeskCardCount[i];
					::CopyMemory(m_iLastOutCard[i],m_DeskCard[i],sizeof(BYTE)*m_iLastCardCount[i]);
				}
				m_btAutoTime=0;
				m_iBaseCount=0;
				m_iBigOutPeople = m_iNowOutPeople = m_iFirstOutPeople = turn->bDeskStation ;
				for (int i = 0;i < PLAY_COUNT;i++)	
					m_PlayView.m_UserCard[i].SetCard(NULL,NULL,0);
				//删除扑克信息
				for (int i = 0;i < PLAY_COUNT;i ++)
					m_iDeskCardCount[i] = 0;

				if(m_iNowOutPeople != -1)
				{
					SetGameTimer(m_iNowOutPeople,m_iThinkTime,ID_OUT_CARD);
					m_PlayView.SetOrientation(ViewStation(m_iNowOutPeople));

					/* duanxiaohui 20100319 修改
					if(m_iAIStation[m_iNowOutPeople]) 
						SetGameTimer(m_iNowOutPeople,GAME_AUTO_TIME,ID_OUT_CARD);*/

					/*if(m_pGameInfo->bEnableSound)
					{
					srand(GetCurrentTime());
					if(rand()%10 >= 8)
					SetSceneSound(SOUND_ERROR,m_pUserInfo[m_iNowOutPeople]->GameUserInfo.bBoy);
					}*/
				}
				if ((m_bWatchMode==false)&&(GetMeUserInfo()->bDeskStation == m_iNowOutPeople))
				{
					m_PlayView.m_bAIChooseCard = false ; 

					m_PlayView.m_btOutCard.ShowWindow(SW_SHOW);
					m_PlayView.m_btPass.EnableWindow(FALSE);
					m_PlayView.m_btPass.ShowWindow(SW_HIDE);
					m_PlayView.m_btCue.ShowWindow(SW_SHOW);
					m_PlayView.m_btCue.EnableWindow(TRUE);

					FlashWindowEx(FLASHW_TIMERNOFG|FLASHW_TRAY, 1, 0);

					OnHitCard(0,0);
					if(m_bAutoCard)
						SetGameTimer(m_iNowOutPeople,GAME_AUTO_TIME,ID_OUT_CARD);
#ifdef AI_MACHINE
					SetGameTimer(m_iNowOutPeople,rand()%GAME_RAND_TIME + 1,ID_OUT_CARD);
#endif 
				}

				return TRUE;
			}
		case ASS_SAFE_END:	//游戏安全结束
			{
				if (uDataSize!=sizeof(GameCutStruct)) 
					return FALSE;

				IsStation(true);
				GameCutStruct * pGameEnd=(GameCutStruct *)pNetData;
				KillGameTimer(0);
				if(pGameEnd->bDeskStation != GetMeUserInfo()->bDeskStation&&m_pUserInfo[pGameEnd->bDeskStation]!=NULL)
				{
					//if(m_pGameInfo->bEnableSound)
						//SetPlaySound(SOUND_GAME_OVER);
					//SetSceneSound(SOUND_USER_LEFT,m_pUserInfo[pGameEnd->bDeskStation]->GameUserInfo.bBoy);
				}
				//SetPlaySound(-1,SOUND_GAME_OVER);
				SetStationParameter(GS_WAIT_ARGEE);	
				KillAllTimer();
				ResetGameStation(RS_GAME_CUT);
				//是否为比赛场
				if(!m_bWatchMode)
				{
					m_PlayView.m_btContinue.ShowWindow(!m_bWatchMode);
						if(m_PlayView.m_btLeave.m_hWnd)m_PlayView.m_btLeave.ShowWindow(!m_bWatchMode);
					m_PlayView.m_btContinue.SetFocus();
					if(GetComType() != TY_MATCH_GAME)
						SetGameTimer(GetMeUserInfo()->bDeskStation,m_iBeginTime,ID_BEGIN_TIME);
#ifdef AI_MACHINE
					SetGameTimer(GetMeUserInfo()->bDeskStation,rand()% GAME_RAND_TIME +1,ID_BEGIN_TIME);
#endif				
					m_PlayView.SetOrientation(ViewStation(GetMeUserInfo()->bDeskStation));
					//if(2 == GetComType())
					//{
					//	SetGameTimer(GetMeUserInfo()->bDeskStation,GAME_AUTO_TIME,ID_BEGIN_TIME);
					//}
					//SetGameTimer(GetMeUserInfo()->bDeskStation,GAME_AUTO_TIME,ID_BEGIN_TIME);
					//m_PlayView.SetOrientation(ViewStation(GetMeUserInfo()->bDeskStation));
				}
				return TRUE;
			}
		case ASS_CUT_END:	//用户强行离开
			{
				if (uDataSize!=sizeof(GameCutStruct))
					return FALSE;

				IsStation(true);
				GameCutStruct * pGameEnd=(GameCutStruct *)pNetData;
				KillGameTimer(0);
				//游戏结算
				GameFinishNotify CutNotify;
				::memset(&CutNotify,0,sizeof(CutNotify));

				if(pGameEnd->bDeskStation != GetMeUserInfo()->bDeskStation&&m_pUserInfo[pGameEnd->bDeskStation]!=NULL)
				{

					//显示信息
					TCHAR szMessage[100];
					if (pNetHead->bAssistantID==ASS_CUT_END)
					{
					//	wsprintf(szMessage,TEXT("由于〖 %s 〗强行退出,他被扣 %d 分 %d 游戏币 ，游戏结束。"),m_pUserInfo[pGameEnd->bDeskStation]->GameUserInfo.nickName,pGameEnd->iTurePoint[pGameEnd->bDeskStation],pGameEnd->iChangeMoney[pGameEnd->bDeskStation]);
						wsprintf(szMessage,TEXT("由于〖 %s 〗强行退出,游戏结束。"),m_pUserInfo[pGameEnd->bDeskStation]->GameUserInfo.nickName);
					}
					else 
						wsprintf(szMessage,TEXT("由于〖 %s 〗离开，游戏结束。"),m_pUserInfo[pGameEnd->bDeskStation]->GameUserInfo.nickName);

					m_MessageHandle.InsertNormalMessage(szMessage);
					for(int i=0;i<PLAY_COUNT;i++)
					{
						//新结算
						if(m_pUserInfo[i]!=NULL)
						{
							wsprintf(CutNotify.name[i],"%s",m_pUserInfo[i]->GameUserInfo.nickName);
							CutNotify.iWardPoint[i]=pGameEnd->iTurePoint[i];
							CutNotify.iMoney[i]=pGameEnd->iChangeMoney[i];//m_pUserInfo[i]->GameUserInfo.dwMoney;
							CutNotify.iAwardPoint[i]=0;
						}
						else
						{
							memset(CutNotify.name[i],0,sizeof(CutNotify.name[i]));
							CutNotify.iWardPoint[i]=0;
							CutNotify.iMoney[i]=0;
							CutNotify.iAwardPoint[i]=0;
						}
					}
					//if(m_pGameInfo->bEnableSound)
						//SetPlaySound(SOUND_GAME_OVER);
				}

				SetStationParameter(GS_WAIT_ARGEE);
				ResetGameStation(RS_GAME_CUT);

				if(!m_bWatchMode)
				{
					m_PlayView.m_btContinue.ShowWindow(!m_bWatchMode);
						if(m_PlayView.m_btLeave.m_hWnd)m_PlayView.m_btLeave.ShowWindow(!m_bWatchMode);
					m_PlayView.m_btContinue.SetFocus();

					SetGameTimer(GetMeUserInfo()->bDeskStation,m_iBeginTime,ID_BEGIN_TIME);
					m_PlayView.SetOrientation(ViewStation(GetMeUserInfo()->bDeskStation));
#ifdef AI_MACHINE
					SetGameTimer(GetMeUserInfo()->bDeskStation,rand()% GAME_RAND_TIME +1,ID_BEGIN_TIME);
#endif
				}
				return TRUE;
			}
		case ASS_NO_CALL_SCORE_END://无人叫分直接进入下一局
			{
				m_MessageHandle.InsertNormalMessage("无人叫牌,游戏开始下一回!");
				KillGameTimer(0);
				SetStationParameter(GS_WAIT_NEXT);
				ResetGameStation(RS_GAME_BEGIN);

				return true;
			}
		case ASS_CONTINUE_END:	//游戏结束
			{
				if (uDataSize!=sizeof(GameEndStruct))
					return FALSE;

				IsStation(true);
				GameEndStruct * pGameEnd=(GameEndStruct *)pNetData;
				//DebugPrintf("pGameEnd--iInitScore[%d][%d][%d]iChangeMoney[%d][%d][%d][%d][%d][%d][%d][%d]",
				//	pGameEnd->iInitScore[0],
				//	pGameEnd->iInitScore[1],
				//	pGameEnd->iInitScore[2],
				//	pGameEnd->iChangeMoney[0],
				//	pGameEnd->iChangeMoney[1],
				//	pGameEnd->iChangeMoney[2],
				//	pGameEnd->iChangeMoney[3],
				//	pGameEnd->iChangeMoney[4],
				//	pGameEnd->iChangeMoney[5],
				//	pGameEnd->iChangeMoney[6],
				//	pGameEnd->iChangeMoney[7]
				//	);
				CString s ="..\\";// CBcfFile::GetAppPath ();/////本地路径
				//CBcfFile f( "GameOption.bcf");
				CBcfFile f( GET_CLIENT_BCF_NAME(szTempStr));

				CString key=TEXT("config");
				//if((m_pGameInfo->uNameID&GRR_VIDEO_ROOM)!=0)
					//key="VideoUISet";

				CString sec = _T("");
				KillGameTimer(0);
				//游戏结算
				GameFinishNotify finishnotify;
				::memset(&finishnotify,0,sizeof(finishnotify));
				//游戏结算
				
				CString stime;
				stime.Format("%d",CTime::GetCurrentTime());
				int curtime=atoi(stime);

				CString str;
				m_MessageHandle.InsertNormalMessage(TEXT("本局游戏结果："));
				m_MessageHandle.InsertNormalMessage(TEXT("－－－－－－"));
#ifdef FKDDZ
				str.Format(TEXT("底分:%d,抢地主亮牌X %d 倍,炸弹 X %d 倍,未出牌 X %d 倍 共X %d 倍"),
					pGameEnd->iCallScore,
					(int)pow(2.0,pGameEnd->iRobMul),(int)pow(2.0,pGameEnd->iBombMul),(int)pow(2.0,pGameEnd->iOutCardMul),(int)pow(2.0,pGameEnd->iTotalMul));
				m_MessageHandle.InsertNormalMessage(str.GetBuffer(MAX_PATH));
#endif
				//CString strtemp="";
				char strtemp[MAX_PATH] = {0};
				for (int i = 0; i < PLAY_COUNT; i ++)
				{

					if (m_pUserInfo[i]!=NULL)
					{
						if(m_pUserInfo[i]->GameUserInfo.iDoublePointTime>curtime && pGameEnd->iTurePoint[i]>0)
						{
							pGameEnd->iTurePoint[i]*=2;
							//str.Format(TEXT("%s 得分 %d （双倍积份卡效果） 金币 %d"),m_pUserInfo[i]->GameUserInfo.nickName,pGameEnd->iTurePoint[i],pGameEnd->iChangeMoney[i]);
							sec.Format("ResultTipDouble%d", m_pGameInfo->uComType);
							str.Format(f.GetKeyVal(key, sec, ""),m_pUserInfo[i]->GameUserInfo.nickName,pGameEnd->iTurePoint[i]/*,pGameEnd->iChangeMoney[i]*/);
							//GetNumString(pGameEnd->iChangeMoney[i],strtemp,Glb().m_nPowerOfGold);
							GlbGetNumString(strtemp,pGameEnd->iChangeMoney[i],Glb().m_nPowerOfGold,FALSE,"");
							str+=strtemp;							

						}
						else
						{
							sec.Format("ResultTip%d", m_pGameInfo->uComType);
							str.Format(f.GetKeyVal(key, sec, ""),m_pUserInfo[i]->GameUserInfo.nickName,pGameEnd->iTurePoint[i]/*,pGameEnd->iChangeMoney[i]*/);
							//str.Format(TEXT("%s 得分 %d 金币 %d"),m_pUserInfo[i]->GameUserInfo.nickName,pGameEnd->iTurePoint[i],pGameEnd->iChangeMoney[i]);	
							//GetNumString(pGameEnd->iChangeMoney[i],strtemp,Glb().m_nPowerOfGold);
							GlbGetNumString(strtemp,pGameEnd->iChangeMoney[i],Glb().m_nPowerOfGold,FALSE,"");
							str+=strtemp;
						}

						//str.Format(TEXT("%s 得分 %d 金币 %d"),m_pUserInfo[i]->GameUserInfo.nickName,pGameEnd->iTurePoint[i],pGameEnd->iChangeMoney[i]);
						m_PlayView.m_showcoredlg.SetUserScore(ViewStation(i)/*viewstation*/,pGameEnd->iTurePoint[i]/*改变的分数*/,pGameEnd->iChangeMoney[i]);
						m_MessageHandle.InsertNormalMessage(str.GetBuffer(MAX_PATH));
						
						//设置结束显示信息
						_stprintf_s(finishnotify.name[i], sizeof(finishnotify.name[i]), TEXT("%s"),m_pUserInfo[i]->GameUserInfo.nickName);
						finishnotify.iWardPoint[i]=pGameEnd->iTurePoint[i];
						finishnotify.iMoney[i]=pGameEnd->iChangeMoney[i];//m_pUserInfo[i]->GameUserInfo.dwMoney;
					}
				}
				m_MessageHandle.InsertNormalMessage(TEXT("－－－－－－"));

				finishnotify.iBasePoint=pGameEnd->iBasePoint;
				finishnotify.iUpGradePeople=pGameEnd->iUpGradeStation;
				//				finishnotify.iGrade2=pGameEnd->iGrade2;
				finishnotify.iGameStyle = GetComType();
				m_PlayView.m_Result.SetMessage(finishnotify,ViewStation(pGameEnd->iUpGradeStation),(m_pGameInfo->dwRoomRule & GRR_VIDEO_ROOM)>0);

				m_PlayView.m_Result.ShowWindow(SW_SHOW);
				SetStationParameter(GS_WAIT_NEXT);
				ResetGameStation(RS_GAME_END);
				if(!m_bWatchMode)
				{
					m_PlayView.m_btContinue.ShowWindow(!m_bWatchMode);
						if(m_PlayView.m_btLeave.m_hWnd)m_PlayView.m_btLeave.ShowWindow(!m_bWatchMode);
					m_PlayView.m_btContinue.SetFocus();
					if (!m_bWatchMode)
					{
						SetGameTimer(GetMeUserInfo()->bDeskStation,m_iBeginTime,ID_BEGIN_TIME);
						m_PlayView.SetOrientation(ViewStation(GetMeUserInfo()->bDeskStation));
					}
					else
					{
						m_PlayView.SetOrientation(PLAY_COUNT);
					}

#ifdef AI_MACHINE
					SetGameTimer(GetMeUserInfo()->bDeskStation,rand()% GAME_RAND_TIME +1,ID_BEGIN_TIME);
#endif
				}
				if (m_PlayView.m_ThingMessage.GetSafeHwnd()!=NULL) 
				{
					m_PlayView.m_ThingMessage.DestroyWindow();
					m_PlayView.m_ThingMessage.m_pParent=NULL;
				}
				return true;
			}
		case ASS_TERMINATE_END://意外结束
			{
				SetStationParameter(GS_WAIT_ARGEE);
				KillAllTimer();
				ResetGameStation(RS_GAME_END);
				m_MessageHandle.InsertNormalMessage(TEXT("[提示]本游戏已经停止,请退出游戏!"));
				if(m_PlayView.m_btLeave.m_hWnd)
					m_PlayView.m_btLeave.ShowWindow(!m_bWatchMode);
				m_PlayView.m_btContinue.ShowWindow(!m_bWatchMode);
				SetGameTimer(GetMeUserInfo()->bDeskStation,m_iThinkTime,ID_LEAVE_TIME);
				m_PlayView.SetOrientation(ViewStation(GetMeUserInfo()->bDeskStation));
				return true;
			}
		case ASS_NO_CONTINUE_END://游戏结束
			{
				if (uDataSize!=sizeof(GameEndStruct))
					return FALSE;

				IsStation(true);
				GameEndStruct * pGameEnd=(GameEndStruct *)pNetData;
				//DebugPrintf("pGameEnd--iInitScore[%d][%d][%d]iChangeMoney[%d][%d][%d][%d][%d][%d][%d][%d]",
				//	pGameEnd->iInitScore[0],
				//	pGameEnd->iInitScore[1],
				//	pGameEnd->iInitScore[2],
				//	pGameEnd->iChangeMoney[0],
				//	pGameEnd->iChangeMoney[1],
				//	pGameEnd->iChangeMoney[2],
				//	pGameEnd->iChangeMoney[3],
				//	pGameEnd->iChangeMoney[4],
				//	pGameEnd->iChangeMoney[5],
				//	pGameEnd->iChangeMoney[6],
				//	pGameEnd->iChangeMoney[7]
				//);
				CString s ="..\\";// CBcfFile::GetAppPath ();/////本地路径
				//CBcfFile f( "GameOption.bcf");
				CBcfFile f( GET_CLIENT_BCF_NAME(szTempStr));

				CString key=TEXT("config");
				//if((m_pGameInfo->uNameID&GRR_VIDEO_ROOM)!=0)
					//key="VideoUISet";

				CString sec = _T("");
				KillGameTimer(0);
				//游戏结算
				GameFinishNotify finishnotify;
				::memset(&finishnotify,0,sizeof(finishnotify));

				//在聊天框里显示成绩
				CString stime;
				stime.Format("%d",CTime::GetCurrentTime());
				int curtime=atoi(stime);
				CString str;
				m_MessageHandle.InsertNormalMessage(TEXT("本局游戏结果："));
				m_MessageHandle.InsertNormalMessage(TEXT("－－－－－－"));
#ifdef FKDDZ
				str.Format(TEXT("底分:%d,抢地主亮牌X %d 倍,炸弹 X %d 倍,未出牌 X %d 倍 共X %d 倍"),
					pGameEnd->iCallScore,
					(int)pow(2.0,pGameEnd->iRobMul),(int)pow(2.0,pGameEnd->iBombMul),(int)pow(2.0,pGameEnd->iOutCardMul),(int)pow(2.0,pGameEnd->iTotalMul));
				m_MessageHandle.InsertNormalMessage(str.GetBuffer(MAX_PATH));
#endif	
				//CString strtemp;
				char strtemp[MAX_PATH] = {0};
				for (int i = 0; i < PLAY_COUNT; i ++)
				{
					if (m_pUserInfo[i]!=NULL)
					{		
						/*str.Format(TEXT("%s : %d "),
							m_pUserInfo[i]->GameUserInfo.nickName,pGameEnd->iTurePoint[i]);*/
						if(m_pUserInfo[i]->GameUserInfo.iDoublePointTime>curtime && pGameEnd->iTurePoint[i]>0)//20081204 修正斗地主结算信息不详细！
						{
							pGameEnd->iTurePoint[i]*=2;
							//str.Format(TEXT("%s : %d 积分 %d 金币（双倍积份卡效果）"),
							//	m_pUserInfo[i]->GameUserInfo.nickName,pGameEnd->iTurePoint[i],pGameEnd->iChangeMoney[i]);		
							sec.Format("ResultTipDouble%d", m_pGameInfo->uComType);
							str.Format(f.GetKeyVal(key, sec, ""),
								m_pUserInfo[i]->GameUserInfo.nickName,pGameEnd->iTurePoint[i]/*,pGameEnd->iChangeMoney[i]*/);
							
							if (m_pGameInfo->uComType != 1)
							{
								//GetNumString(pGameEnd->iChangeMoney[i],strtemp,Glb().m_nPowerOfGold);
								GlbGetNumString(strtemp,pGameEnd->iChangeMoney[i],Glb().m_nPowerOfGold,FALSE,"");
								str+=strtemp;
							}
						}
						else
						{
							//str.Format(TEXT("%s : %d 积分 %d 金币"),
							//	m_pUserInfo[i]->GameUserInfo.nickName,pGameEnd->iTurePoint[i],pGameEnd->iChangeMoney[i]);								
							sec.Format("ResultTip%d", m_pGameInfo->uComType);
							str.Format(f.GetKeyVal(key, sec, ""),
								m_pUserInfo[i]->GameUserInfo.nickName,pGameEnd->iTurePoint[i]/*,pGameEnd->iChangeMoney[i]*/);
							
							if (m_pGameInfo->uComType != 1)
							{
								//GetNumString(pGameEnd->iChangeMoney[i],strtemp,Glb().m_nPowerOfGold);
								GlbGetNumString(strtemp,pGameEnd->iChangeMoney[i],Glb().m_nPowerOfGold,FALSE,"");
								str+=strtemp;
							}							
						}

						m_PlayView.m_showcoredlg.SetUserScore(ViewStation(i)/*viewstation*/,pGameEnd->iTurePoint[i]/*改变的分数*/,pGameEnd->iChangeMoney[i]);
						
						m_MessageHandle.InsertNormalMessage(str.GetBuffer(MAX_PATH));
						//设置结束显示信息
						_stprintf_s(finishnotify.name[ViewStation(i)],sizeof(finishnotify.name[ViewStation(i)]), TEXT("%s"),m_pUserInfo[i]->GameUserInfo.nickName);
						finishnotify.iWardPoint[ViewStation(i)]=pGameEnd->iTurePoint[i];
						finishnotify.iMoney[ViewStation(i)]=pGameEnd->iChangeMoney[i];//m_pUserInfo[i]->GameUserInfo.dwMoney;
					}
				}
				m_MessageHandle.InsertNormalMessage(TEXT("－－－－－－"));

				//设置数据
				if(m_pGameInfo->bEnableSound)
				{
					if (pGameEnd->iTurePoint[GetMeUserInfo()->bDeskStation] > 0||pGameEnd->iChangeMoney[GetMeUserInfo()->bDeskStation]>0)
						VoiceOut(255,"GAME_WIN.wav");
					else
						VoiceOut(255,"GAME_LOST.wav");

				}
				finishnotify.iBasePoint=pGameEnd->iBasePoint;
				finishnotify.iUpGradePeople=pGameEnd->iUpGradeStation;
				//				finishnotify.iGrade2=pGameEnd->iGrade2;
				finishnotify.iGameStyle = GetComType();
				m_PlayView.m_Result.SetMessage(finishnotify,ViewStation(pGameEnd->iUpGradeStation),(m_pGameInfo->dwRoomRule & GRR_VIDEO_ROOM)>0);

				m_PlayView.m_Result.ShowWindow(SW_SHOW);

				for(int i = 0;i < PLAY_COUNT; i ++)
				{					
					int desk = ViewStation(i);
					m_CardCount[desk] = pGameEnd->iUserCardCount[i];
					::CopyMemory(m_Card[desk],&pGameEnd->iUserCard[i],sizeof(BYTE)*pGameEnd->iUserCardCount[i]);
					//m_PlayView.m_HandCard[desk].SetCard(m_Card[desk],NULL,m_CardCount[desk]);
					//m_PlayView.m_HandCard[desk].SetShowCard(true);
				}			

				SetStationParameter(GS_WAIT_ARGEE);
				//ResetGameStation(RS_GAME_END);
				for(int i = 0; i < PLAY_COUNT; i ++)
				{	
					m_PlayView.m_HandCard[i].SetCard(m_Card[i],NULL,m_CardCount[i]);
					m_PlayView.m_HandCard[i].SetShowCard(true);
				}

				if(!m_bWatchMode)
				{
					m_PlayView.m_btContinue.ShowWindow(!m_bWatchMode);
						if(m_PlayView.m_btLeave.m_hWnd)m_PlayView.m_btLeave.ShowWindow(!m_bWatchMode);

					SetGameTimer(GetMeUserInfo()->bDeskStation,m_iBeginTime,ID_BEGIN_TIME);
#ifdef AI_MACHINE
					SetGameTimer(GetMeUserInfo()->bDeskStation,rand()% GAME_RAND_TIME +1,ID_BEGIN_TIME);
#endif
					m_PlayView.SetOrientation(ViewStation(GetMeUserInfo()->bDeskStation));

				}
				if (m_PlayView.m_ThingMessage.GetSafeHwnd()!=NULL) 
				{
					m_PlayView.m_ThingMessage.DestroyWindow();
					m_PlayView.m_ThingMessage.m_pParent=NULL;
				}
				return TRUE;
			}
		case ASS_HAVE_THING:	//用户请求离开
			{
				AfxSetResourceHandle(GetModuleHandle(GET_CLIENT_DLL_NAME(szTempStr)));		
				HaveThingStruct * pThing=(HaveThingStruct *)pNetData;
				if (m_PlayView.m_ThingMessage.GetSafeHwnd()==NULL) 
				{
					m_PlayView.m_ThingMessage.Create(IDD_THING_MESSAGE,this);
					m_PlayView.m_ThingMessage.m_pParent=this;
				}
				m_PlayView.m_ThingMessage.SetLeaveMessage(pThing->szMessage,m_pUserInfo[pThing->pos]->GameUserInfo.nickName);
				m_PlayView.m_ThingMessage.ShowWindow(SW_SHOW);
				//m_PlayView.m_btThing.EnableWindow(false);
				m_PlayView.m_HaveThing.ShowWindow(SW_HIDE);
				AfxSetResourceHandle(GetModuleHandle(NULL));
				return TRUE;
			}
		case ASS_LEFT_RESULT:	//请求离开结果 
			{
				if (uDataSize!=sizeof(LeaveResultStruct)) return FALSE;
				LeaveResultStruct * pLeftResult=(LeaveResultStruct *)pNetData;
				CString strBuffer;
				if (!pLeftResult->bArgeeLeave) 
				{
					//m_PlayView.m_btThing.EnableWindow(TRUE);
					strBuffer.Format(TEXT("玩家 %s 不同意游戏结束。但本局结束后，申请离开者可以离开。"),m_pUserInfo[pLeftResult->bDeskStation]->GameUserInfo.nickName);
					m_MessageHandle.InsertNormalMessage(strBuffer.GetBuffer(MAX_PATH));
				}
				else
				{
					if(pLeftResult->bArgeeLeave&&GetMeUserInfo()->bDeskStation==pLeftResult->bDeskStation&&!m_bWatchMode)
					{
						OnCancel();
					}	
				}
				return TRUE;
			}
		case ASS_AUTO:				//托管
			{
				if (uDataSize!=sizeof(AutoStruct)) 
					return FALSE;
				AutoStruct *bAuto=(AutoStruct*)pNetData;
				m_PlayView.SetAuto(ViewStation(bAuto->bDeskStation),bAuto->bAuto);

				if(NULL != m_pUserInfo[bAuto->bDeskStation])
				{
					CString  strInfo ; 

					if(bAuto->bAuto)
					{
						strInfo.Format("%s 设置了托管处理！ ", m_pUserInfo[bAuto->bDeskStation]->GameUserInfo.nickName) ; 
					}
					else
					{
						strInfo.Format("%s 取消了托管处理 ！" , m_pUserInfo[bAuto->bDeskStation]->GameUserInfo.nickName) ; 
					}

					m_MessageHandle.InsertNormalMessage(strInfo.GetBuffer()) ; 
				}

				return true;
			}
		case ASS_MESSAGE:
			{
				if (uDataSize!=sizeof(MessageStruct))
				{
					//AfxMessageBox("435435");
					return FALSE;
				}
				MessageStruct *pMessage=(MessageStruct*)pNetData;
				m_MessageHandle.InsertSystemMessage(pMessage->Message);
				return true;
			}
		case ASS_USER_LEFTDESK:
			{				
				if (uDataSize != sizeof(UserleftDesk))
				{
					return FALSE;
				}
				UserleftDesk *pLeftData = (UserleftDesk *)pNetData;
				if (pLeftData->bDeskStation<0||pLeftData->bDeskStation>=PLAY_COUNT)
				{
					return TRUE;
				}
				int iView = ViewStation(pLeftData->bDeskStation);
				m_PlayView.m_FlashPlayer[iView].Stop();
				m_PlayView.m_FlashPlayer[iView].ShowWindow(SW_HIDE);	
				m_PlayView.m_FlashPlayer[iView].StopPlay();
				m_PlayView.m_FlashPlayer[iView].Stop();
			}
		case ASS_USER_VIP_VERTIFY:
			{
				if(uDataSize != sizeof(VipDataStruct))
				{
					return FALSE ; 
				}

				VipDataStruct *pUserVipData = (VipDataStruct *)pNetData ; 

				if(pUserVipData->bVipUser)
				{
					m_PlayView.m_CounterDlg.ShowWindow(SW_SHOW) ;
					m_PlayView.m_CounterDlg.CenterWindow(this) ;

				}
				else
				{
					m_PlayView.m_CounterDlg.ShowWindow(SW_HIDE) ;
					AFCMessageBox("您当前不是Vip用户 ，不能使用此功能 ， 请先购买vip后再使用!") ; 
				}

				return TRUE ; 
			}
		default:
			{
				break ; 
			}
		}
	}
	return __super::HandleGameMessage(pNetHead,pNetData,uDataSize);
}

//重新设置界面
void CClientGameDlg::ResetGameFrame()
{
	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		if(m_PlayView.m_dlgDisTalk[i].m_hWnd)
			m_PlayView.m_dlgDisTalk[i].ShowWindow(SW_HIDE);             //创建后隐藏
		m_PlayView.SetPeopleMultiple(i,1);					//设置玩家加棒

	}
	m_bTimeOutCount = 0;
	m_iMeCardCount = 0;
	m_iNtPeople = -1;
	m_iNowOutPeople = -1;
	m_iBaseCount = 0;
	m_iFirstOutPeople = -1;
	//	m_iLastCardCount=0;
	m_iBeenPlayGame = 0;
	m_bShowLast = FALSE;
	::memset(m_iBackCard,0,sizeof(m_iBackCard));
	::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
	::memset(m_iLastCardCount,0,sizeof(m_iLastCardCount));

	//界面重置
	m_PlayView.m_btOutCard.ShowWindow(SW_HIDE);
	if(m_PlayView.m_btLeave.m_hWnd)
		m_PlayView.m_btLeave.ShowWindow(SW_HIDE);
	m_PlayView.m_btPass.ShowWindow(SW_HIDE);
	m_PlayView.m_btThing.EnableWindow(0);

	m_PlayView.m_btAddDouble.ShowWindow(0);
	m_PlayView.m_btAddDouble2.ShowWindow(0);
	m_PlayView.m_btRobNT.ShowWindow(0);
	m_PlayView.m_btRobNT2.ShowWindow(0);
	m_PlayView.m_btShowCard.ShowWindow(0);
	m_PlayView.m_btShowCard2.ShowWindow(0);

	m_PlayView.SetNTStation(-1);

	m_PlayView.m_MeCard.SetCard(NULL,NULL,0);
	m_PlayView.m_MeBackCard.SetCard(NULL,NULL,0);
	m_PlayView.m_GameNoteView.SetMessage(NULL);
	m_PlayView.SetCallScoreResult();
	//if (m_PlayView.m_HaveThing.GetSafeHwnd()) m_PlayView.m_HaveThing.DestroyWindow();
	//if (m_PlayView.m_ThingMessage.GetSafeHwnd()) m_PlayView.m_ThingMessage.DestroyWindow();
	//if(m_PlayView.m_GameNoteView.GetSafeHwnd()) m_PlayView.m_GameNoteView.DestroyWindow();
	if(m_hMciWnd1)
	{
		MCIWndStop(m_hMciWnd1);
		MCIWndDestroy(m_hMciWnd1);
		m_hMciWnd1=NULL;
	}

	if(m_hMciWnd2)
	{
		MCIWndStop(m_hMciWnd2);
		MCIWndDestroy(m_hMciWnd2);
		m_hMciWnd2=NULL;
	}	

	if(m_hMciWnd3)
	{
		MCIWndStop(m_hMciWnd3);
		MCIWndDestroy(m_hMciWnd3);
		m_hMciWnd3=NULL;
	}
	m_PlayView.SetGameMultiple(0);
	KillAllTimer();
	return __super::ResetGameFrame();
}

//开始按钮函数
LRESULT	CClientGameDlg::OnHitBegin(WPARAM wparam, LPARAM lparam)
{
#ifdef AI_MACHINE
	HWND forehwnd=::GetForegroundWindow();
	if(forehwnd == m_hWnd&&m_PlayView.m_Result.IsWindowVisible())
	{//截图屏幕
		//SaveScreen();
	}
#endif
	OnControlHitBegin();

	return 0;
}

bool CClientGameDlg::OnControlHitBegin()
{
	//SetNoteFlag(1);
	m_PlayView.m_btContinue.ShowWindow(SW_HIDE);
	if(m_PlayView.m_btLeave.m_hWnd)
		m_PlayView.m_btLeave.ShowWindow(SW_HIDE);
	m_PlayView.SetOrientation(-1);
	
	if (NULL != m_PlayView.m_GameNoteView.GetSafeHwnd())
	{
		m_PlayView.m_GameNoteView.SetMessage(TEXT("正在等待其他玩家开始游戏!"));		//以此来启动Waiting图像
	}
	//准备好了声音
	//if(m_pGameInfo->bEnableSound)
	//	SetSceneSound(SOUND_READY,m_pUserInfo[GetMeUserInfo()->bDeskStation]->GameUserInfo.bBoy);

	if (((GetStationParameter()==GS_WAIT_SETGAME)
		||(GetStationParameter()==GS_WAIT_NEXT)
		||(GetStationParameter()==GS_WAIT_ARGEE))
		&&!m_bWatchMode)
	{
		SendGameData(MDM_GM_GAME_NOTIFY,ASS_GM_AGREE_GAME,0);
	}

	KillAllTimer();
	ResetGameStation(RS_GAME_BEGIN);
	return true;
}

//清除所有定时器
void CClientGameDlg::KillAllTimer()
{
	KillTimer(ID_BEGIN_TIME);
	KillTimer(ID_CALL_SCORE_TIME);
	KillTimer(ID_OUT_CARD);
	KillTimer(ID_LEAVE_TIME);
	KillTimer(ID_BEGIN_TIME);
	KillTimer(TIME_CHUI);
	::memset(m_PlayView.m_uTime,0,sizeof(m_PlayView.m_uTime));
	return;
}
//斗地主
void CClientGameDlg::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
	case ID_SHOW_LAST:	//查看上一轮扑克20081210
		{
			KillTimer(ID_SHOW_LAST);
			m_bShowIngLast=FALSE;
			for (int i=0;i<PLAY_COUNT;i++)
			{
				//当前出牌人是最大出牌人，即新的一轮，看牌时间到，不显示上轮所有信息
				if (m_iBigOutPeople==m_iNowOutPeople)
				{
					m_PlayView.SetPass(ViewStation(i),0);
					m_PlayView.m_UserCard[ViewStation(i)].SetCard(NULL,NULL,0);
				}
				else
				{
					if(m_iDeskCardCount[i] > 0)
					{
						m_PlayView.SetPass(ViewStation(i),0);

						// 自己为出牌玩家
						if ((m_iNowOutPeople == i) && (ViewStation(i) == 1))
						{
							m_PlayView.m_UserCard[ViewStation(i)].SetCard(NULL,NULL,0);
						}
						else
						{
							m_PlayView.m_UserCard[ViewStation(i)].SetCard(m_DeskCard[i],NULL,m_iDeskCardCount[i]);
						}
					}
					else
					{
						//全部隐藏
						m_PlayView.SetPass(ViewStation(i),0);

						// 出牌的玩家显示为空
						if (m_iNowOutPeople == i)
						{
							m_PlayView.SetPass(ViewStation(i),0);
						}
						else
						{
							if ( ( m_byteHitPass&(1<<i) ) == (1<<i) )
							{
								m_PlayView.SetPass(ViewStation(i),1);
							}
						}
						m_PlayView.m_UserCard[ViewStation(i)].SetCard(NULL,NULL,0);
					}
				}
			}
			m_PlayView.m_btLastTurn.ShowWindow(SW_SHOW);			
			return ;
		}
	case ID_CONNECT_FLASH_TIME:
		{	
			//KillTimer(ID_CONNECT_FLASH_TIME);					
			for(int i = 0;i<PLAY_COUNT;i++)
			{
				CString  strInfo;	
				strInfo.Format("wysoutfor::%s",m_strFlash[i]);
				OutputDebugString(strInfo);

				BYTE bView = ViewStation(i);

				if(NULL == m_pUserInfo[i])
				{
					continue ;
				}
				if(true == m_bConnectFlash[i])
				{
					continue;
				}
				else
				{					
					try
					{
						OutputDebugString("wysoutfor:;试图连接视频");
						CString strReturnMsg ="";
						strReturnMsg = m_PlayView.m_FlashPlayer[bView].CallFunction(m_strFlash[i]);
						m_bConnectFlash[i] = true;						
					}
					catch(COleDispatchException* e)
					{						
						char szTemp[260] = {0};
						e->GetErrorMessage(szTemp,sizeof(szTemp));
						m_bConnectFlash[i] = false;
						//MessageBox(szTemp);
					}					
				}
			}

			int iCountPlayer = 0;
			int iOnLinePLayer = 0;
			for (int i = 0;i<PLAY_COUNT;i++)
			{
				if(true == m_bConnectFlash[i])
				{
					iCountPlayer++;
				}
				if(NULL !=m_pUserInfo[i])
				{
					iOnLinePLayer++ ; 
				}
			}
			if(iOnLinePLayer == iCountPlayer)
			{
				KillTimer(ID_CONNECT_FLASH_TIME);
				SetTimer(ID_CONNECT_FLASH_TIME,3000,NULL);
			}
			return;
		}
	case ID_ANIM_TIME:	//动画计时器
		{
			static int iTemp = 0;
			m_PlayView.SetAnimStitch(iTemp);
			m_PlayView.UpdateViewFace(NULL);
			iTemp++;

			//杀死计时器
			if(iTemp >= 5)
			{
				m_PlayView.SetAnimStitch(iTemp);
				iTemp = 0;
				KillTimer(ID_ANIM_TIME);
			}

			return;
		}
	default:
		break;
	}
	CLoveSendClassInExe::OnTimer(nIDEvent);
}
//定时器消息
bool CClientGameDlg::OnGameTimer(BYTE bDeskStation, UINT uTimeID, UINT uTimeCount)
{
	switch (uTimeID)
	{
	case TIME_CHUI:
		{
			if (uTimeCount <=0 && !m_bWatchMode)
			{
				OnChuiNo(0,0);
				KillTimer(TIME_CHUI);
			}
			return true;
		}
	case ID_BEGIN_TIME:	//继续游戏定时器
		{
			if (uTimeCount<=0&&!m_bWatchMode)
			{
				//时间到了
				if (m_iAfterWaitAction == 1)
				{
					OnHitBegin(0,0);
				}
				else
				{
					SendMessage(WM_CLOSE,0,0);
				}
				return true;
			}
			if (uTimeCount <= 5) 
			{
				if(m_pGameInfo->bEnableSound)
					SetPlaySound(SOUND_WARNING);
			}
			m_PlayView.UpdateViewFace(NULL);
			return true;
		}
	case ID_LEAVE_TIME:
		{
			if (uTimeCount <= 0&&!m_bWatchMode)
			{
				//离开
				OnCancel();
				return true;
			}
			if (uTimeCount<=5) 
			{
				if(m_pGameInfo->bEnableSound)
					SetPlaySound(SOUND_WARNING);
			}
			return true;
		}
	case ID_CALL_SCORE_TIME:	//叫分记时器
		{
			/*   将客户端代替别人游戏部分转换到服务器上实现  20100319 duanxiaohui 修改
			//代替叫分
			if(m_bCurrentOperationStation != 255)
			{
				if(m_iAIStation[m_bCurrentOperationStation] &&  !m_bWatchMode)
				{
					ReplaceCallScore(m_bCurrentOperationStation);
					return true;
				}
				if(uTimeCount <= 0&&m_pUserInfo[m_bCurrentOperationStation]->GameUserInfo.bUserState == USER_CUT_GAME&&!m_bWatchMode)
				{	
					ReplaceCallScore(m_bCurrentOperationStation);
					return true;
				}
			}*/

			if (uTimeCount <= 0 && m_bCurrentOperationStation == GetMeUserInfo()->bDeskStation  && !m_bWatchMode)
			{
				if(!m_bAutoCard)
					OnCallScore(0,0);
				else//托管叫3分
					OnCallScore(3,3);
				return true;
			}

			if (uTimeCount <= 5 && 1 == ViewStation(bDeskStation)) 
			{
				if(m_pGameInfo->bEnableSound)
					SetPlaySound(SOUND_WARNING);
			}
			return true;
		}
	case ID_ROB_NT_TIME://抢地主
		{
			/*  将客户端代替别人游戏部分转换到服务器上实现  20100319 duanxiaohui 修改
			if(m_iAIStation[m_bCurrentOperationStation] && m_iNowOutPeople&&!m_bWatchMode)
			{
				ReplaceRobNT(m_bCurrentOperationStation);
				return true;
			}
			if(uTimeCount <= 0 && m_pUserInfo[m_bCurrentOperationStation]->GameUserInfo.bUserState == USER_CUT_GAME&&!m_bWatchMode)
			{	
				ReplaceRobNT(m_bCurrentOperationStation);
				return true;
			}*/

			if (uTimeCount <= 0 && m_bCurrentOperationStation == GetMeUserInfo()->bDeskStation&&!m_bWatchMode)
			{
				OnRobNT(0,0);
				return true;
			}
			if (uTimeCount <= 5 && 1 == ViewStation(bDeskStation)) 
			{
				if(m_pGameInfo->bEnableSound)
					SetPlaySound(SOUND_WARNING);
			}
			return true;
		}
	case ID_ADD_DOUBLE_TIME://加棒
		{
			//if(m_iAIStation[m_bCurrentOperationStation] && m_iNowOutPeople)
			//{
			//	SupersedeAddDouble(m_bCurrentOperationStation);
			//	return true;
			//}
			//if(uTimeCount<=0&&m_pUserInfo[m_bCurrentOperationStation]->GameUserInfo.bUserState == USER_CUT_GAME)
			//{	
			//	SupersedeAddDouble(m_bCurrentOperationStation);
			//	return true;
			//}

			if (uTimeCount <= 0 && 1 == ViewStation(bDeskStation)&&!m_bWatchMode)
			{
				m_PlayView.m_btAddDouble.ShowWindow(0);
				m_PlayView.m_btAddDouble2.ShowWindow(0);
				//OnAddDouble(0,0);
				return true;
			}

			if (uTimeCount <= 5 && 1 == ViewStation(bDeskStation)) 
			{
				if(m_pGameInfo->bEnableSound)
					SetPlaySound(SOUND_WARNING);
			}
			return true;
		}
	case ID_SHOW_CARD://亮牌
		{
			if (uTimeCount <= 0 && m_bCurrentOperationStation == GetMeUserInfo()->bDeskStation && !m_bWatchMode)
			{
				OnShowCard(0,0);
				return true;
			}

			if (uTimeCount <= 5&& 1 == ViewStation(bDeskStation)) 
			{
				if(m_pGameInfo->bEnableSound)
					SetPlaySound(SOUND_WARNING);
			}
			return true;
		}
	case ID_OUT_CARD:	//出牌
		{
			int iViewStation=ViewStation(bDeskStation);

			if(m_pUserInfo[m_iNowOutPeople] == NULL)
			{
				return true;
			}

			/* 将客户端代替别人游戏部分转换到服务器上实现  20100319 duanxiaohui 修改
			
			if(m_iAIStation[m_iNowOutPeople]&&uTimeCount<=0&&!m_bWatchMode)
			{
				ReplaceOutCard(m_iNowOutPeople);
				return true;
			}
			//當前出牌玩家掉線
			if(uTimeCount<= 0 && m_iNowOutPeople>=0 && m_iNowOutPeople < PLAY_COUNT && m_pUserInfo[m_iNowOutPeople]->GameUserInfo.bUserState == USER_CUT_GAME&&!m_bWatchMode)
			{	
				ReplaceOutCard(m_iNowOutPeople);
				return true;
			}*/

			if (uTimeCount<=0&&!m_bWatchMode)	//时间到了
			{

				//判断超时
				if(!m_bAutoCard&&(m_iNowOutPeople == GetMeUserInfo()->bDeskStation) &&  !m_bWatchMode)
					m_bTimeOutCount++;
				/*if (!m_bAutoCard&&m_bTimeOutCount>2 && (m_iNowOutPeople==GetMeUserInfo()->bDeskStation) && (m_bWatchMode==FALSE))
				{
				KillGameTimer(0);
				SendGameData(MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
				SendGameData(MDM_GR_USER_ACTION,ASS_GR_USER_UP,0);
				AFCMessage(TEXT("连续3次自动出牌，自动退出游戏，您被系统扣除15分"),m_pGameInfo->szGameName,MB_ICONQUESTION,this);
				PostMessage(WM_CLOSE,0,0);
				return false;
				}//m_bAutoCard==true为托管
				else*/ 
#ifndef AI_MACHINE
				if(!m_bAutoCard&&m_iNowOutPeople==GetMeUserInfo()->bDeskStation && !m_bWatchMode)
				{
					CString str;
					if (m_bTimeOutCount >= 0&& m_bTimeOutCount <= 3)
						str.Format(TEXT("[提示]您已经超时 %d 次了,超时三次,将自动托管."),m_bTimeOutCount);
					else
						str.Format(TEXT("[提示]您已经再次超时了,将自动托管."),m_bTimeOutCount);
					//str.Format("请注意，您已经超时 %d 次了，超时三次会当做逃跑处理。",m_bTimeOutCount);
					m_MessageHandle.InsertNormalMessage(str.GetBuffer(MAX_PATH));
					if(m_bTimeOutCount >= 3)
						OnAuto(0,0);
				}
#endif
				if ((m_iNowOutPeople == GetMeUserInfo()->bDeskStation)&&(m_bWatchMode == FALSE))
				{
					UserOutCard();
				}
				return true;
			}
			if ((uTimeCount<=6)&&(m_bWatchMode==FALSE)&&(iViewStation==1)) 
			{
				if(m_pGameInfo->bEnableSound)
					SetPlaySound(SOUND_WARNING);
			}
			return true;
		}
	default:
		break;
	}
	return TRUE;
	//return __super::OnGameTimer(bDeskStation,uTimeID,uTimeCount);
}

//按动扑克按钮函数
LRESULT	CClientGameDlg::OnHitCard(WPARAM wparam, LPARAM lparam)
{
	/*int mright=lparam;
	int mleft=wparam;
	if(mright==1&&mleft==1)
	{
		m_PlayView.m_UserCard[2].SetCard(0,NULL,0);
		
	}*/
	if (GetStationParameter()==GS_PLAY_GAME)	//出牌状态
	{
		//验证出牌信息
		BYTE iUpCard[39];
		if (m_iNowOutPeople!=GetMeUserInfo()->bDeskStation) 
			return 0;
		int iUpCardCount=m_PlayView.m_MeCard.GetUpCard(iUpCard);

		//出牌数目判断
		if (iUpCardCount ==0 )
		{
			m_PlayView.m_btOutCard.EnableWindow(FALSE);
			m_PlayView.m_bAIChooseCard = false ; 
			return 0;
		}


		int iResultCount=0;
		BYTE bResult[54] ;
		BYTE iCueList[54] ; 
		memset(iCueList , 0 ,sizeof(iCueList)) ; 

		CString strInfo ; 
		

		if (5 <= iUpCardCount && m_iBaseCount == 0 && !m_PlayView.m_bAIChooseCard) // 顺子与连对两种牌型最少5张。需要拖牌自动检查，弹起的牌必须是大于这个数目
		{
			m_Logic.DragCardAutoSetValidCard(iUpCard, iUpCardCount, bResult, iResultCount);

			if (5 <= iResultCount)
			{
				//strInfo.Format("wysddzlog::智能拖取牌iResult[0]==%d ,iResult[%d - 1]==%d " ,bResult[0] ,iResultCount , bResult[iResultCount - 1]) ; 
				//OutputDebugString(strInfo) ; 
				SetCueCardList(iCueList ,m_iMeCardList , m_iMeCardCount, bResult , iResultCount) ;
				m_PlayView.m_MeCard.SetCard(m_iMeCardList,iCueList,m_iMeCardCount);

				m_PlayView.m_bAIChooseCard = true ; 
			}
		}
		else
		{
			if(iUpCardCount  == 1 &&!m_PlayView.m_bAIChooseCard)
			{
				m_Logic.AITrackOutCard(m_iMeCardList,m_iMeCardCount,
					iUpCard ,iUpCardCount ,
					m_iBaseCard , m_iBaseCount ,
					bResult, iResultCount);

				if(iResultCount > 0  && m_Logic.CanOutCard(bResult ,iResultCount , 
					m_iBaseCard , m_iBaseCount , 
					m_iMeCardList,m_iMeCardCount,
					m_iBaseCount == 0))
				{
					//strInfo.Format("wysddzlog::智能点击牌iResult[0]==%d ,iResult[%d - 1]==%d " ,bResult[0] ,iResultCount , bResult[iResultCount - 1]) ; 
					//OutputDebugString(strInfo) ; 

					SetCueCardList(iCueList ,m_iMeCardList , m_iMeCardCount, bResult , iResultCount) ;
					m_PlayView.m_MeCard.SetCard(m_iMeCardList,iCueList,m_iMeCardCount);
					m_PlayView.m_bAIChooseCard = true ; 
				}
			}
		}

		iUpCardCount = m_PlayView.m_MeCard.GetUpCard(iUpCard);

		if(iUpCardCount == 0)
		{
			m_PlayView.m_bAIChooseCard = false ; 
		}

		//验证是否可以出这样的扑克
		if (m_iBigOutPeople == GetMeUserInfo()->bDeskStation)	//第一个出牌者
		{
			m_PlayView.m_btOutCard.EnableWindow(m_Logic.GetCardShape(iUpCard,iUpCardCount)!=UG_ERROR_KIND);
		}
		else	//随后出牌者 
		{
			m_PlayView.m_btOutCard.EnableWindow(m_Logic.CanOutCard(iUpCard,iUpCardCount,m_iBaseCard,
				m_iBaseCount,m_iMeCardList,m_iMeCardCount,m_iBaseCount == 0));
		}
	}

	return 0;
}


LRESULT	CClientGameDlg::OnRightHitCard(WPARAM wparam, LPARAM lparam)
{
	//int fleft=wparam;
	//int rleft=lparam;
	//if(fleft==1&&rleft==1)
	//{
	//	//AfxMessageBox("1");
	//    OnHitCard(1,1);
	//}
	//else
	//{
		//AfxMessageBox("3");
		OnHitOutCard(1,0);
	//}
	return 0;
}
LRESULT	CClientGameDlg::OnCue(WPARAM wparam, LPARAM lparam)
{
	BYTE CueCardList[45]={0},ResultCard[45];
	int iResultCardCount=0;
	//全部不弹起来
	memset(CueCardList,0,sizeof(CueCardList));
	//提取升起来的牌
	BYTE bUpCardList[54];
	int iUpCardCount;
	iUpCardCount = m_PlayView.m_MeCard.GetUpCard(bUpCardList);
	if(iUpCardCount == 0)//第一次点提示
	{
		//提示
		m_Logic.AutoOutCard(m_iMeCardList,m_iMeCardCount,m_iBaseCard,m_iBaseCount,ResultCard,	
			iResultCardCount,m_iBigOutPeople == GetMeUserInfo()->bDeskStation);
	}else//第n+1次点击
	{
		//上次点了提示
		if(m_Logic.CanOutCard(bUpCardList,iUpCardCount,m_iBaseCard,m_iBaseCount,m_iMeCardList,m_iMeCardCount,m_iBigOutPeople == GetMeUserInfo()->bDeskStation))
		{
			m_Logic.AutoOutCard(m_iMeCardList,m_iMeCardCount,bUpCardList,iUpCardCount,ResultCard,	
				iResultCardCount,m_iBigOutPeople == GetMeUserInfo()->bDeskStation);
		}
		//没有比手中牌更大的,回归
		if(iResultCardCount == 0)
		{
			m_Logic.AutoOutCard(m_iMeCardList,m_iMeCardCount,m_iBaseCard,m_iBaseCount,ResultCard,	
				iResultCardCount,m_iBigOutPeople == GetMeUserInfo()->bDeskStation);
		}
	}
	//提示牌数大于0
	if(iResultCardCount > 0)
		SetCueCardList(CueCardList,m_iMeCardList,m_iMeCardCount,ResultCard,iResultCardCount);
	/*else
		SetPlaySound(SOUND_INVALIDATE);*/
	m_PlayView.m_MeCard.SetCard(m_iMeCardList,CueCardList,m_iMeCardCount);

	OnHitCard(0,0);
	if(iResultCardCount <=0 )
		OnHitPassCard(0,0);
	return 0;
}
//提示牌
void CClientGameDlg::SetCueCardList(BYTE CueCardList[],BYTE iMeCardList[],int iMeCardCount,BYTE ResultCard[],int iResultCardCount)
{
	memset(CueCardList,0,sizeof(CueCardList));

	if(iResultCardCount>0)
	{
		for(int i=0;i<iResultCardCount;i++)
			for(int j=0;j<m_iMeCardCount;j++)
			{
				if(m_iMeCardList[j]==ResultCard[i]&&!CueCardList[j])
				{
					CueCardList[j]=1;
					break;
				}
			}
	}	
	return ;
}

//按动出牌按钮函数
LRESULT	CClientGameDlg::OnHitOutCard(WPARAM wparam, LPARAM lparam)
{
	//设置控件
	if (m_PlayView.m_btOutCard.IsWindowEnabled()&&m_PlayView.m_btOutCard.IsWindowVisible())
	{
		//发送数据
		OutCardStruct OutCard;
		OutCard.iCardCount=m_PlayView.m_MeCard.GetUpCard(OutCard.iCardList);
		bool bEnable = m_Logic.CanOutCard(OutCard.iCardList,OutCard.iCardCount,m_iBaseCard,m_iBaseCount,m_iMeCardList,m_iMeCardCount,m_iBaseCount == 0);

		if(0<OutCard.iCardCount && bEnable)
		{	
			m_iNowOutPeople=-1;
			m_PlayView.m_btOutCard.ShowWindow(SW_HIDE);
			m_PlayView.m_btPass.ShowWindow(SW_HIDE);
			m_PlayView.m_btCue.ShowWindow(SW_HIDE);

			if(!m_Logic.CanOutCard(OutCard.iCardList,OutCard.iCardCount,m_iBaseCard,m_iBaseCount,m_iMeCardList,m_iMeCardCount,m_iBaseCount == 0))
				OutCard.iCardCount  = 0;
			//设置数据
			KillAllTimer();

			SendGameData(&OutCard,sizeof(OutCard)-sizeof(OutCard.iCardList)+sizeof(BYTE)*OutCard.iCardCount,
				MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
		}
		//设置预显示
		//m_PlayView.m_UserCard[2].SetCard(OutCard.iCardList,NULL,OutCard.iCardCount);
	}

	return 0;
}


//按动有事按钮函数
LRESULT	CClientGameDlg::OnHitHaveThing(WPARAM wparam, LPARAM lparam)
{
	if (m_PlayView.m_HaveThing.GetSafeHwnd()==NULL) 
	{
		AfxSetResourceHandle(GetModuleHandle(GET_CLIENT_DLL_NAME(szTempStr))); 
		m_PlayView.m_HaveThing.Create(IDD_THING,this); 
		m_PlayView.m_HaveThing.m_pParnet=this; 
		AfxSetResourceHandle(GetModuleHandle(NULL));
	}
	m_PlayView.m_HaveThing.ShowWindow(SW_SHOW);
	m_PlayView.m_HaveThing.SetFocus();
	return 0;
}

//托管
LRESULT	CClientGameDlg::OnAuto(WPARAM wparam, LPARAM lparam)
{
	if(m_btAutoTime>2)											//一局游戏里面一圈牌,只可以托管一次
		return 0;
	m_btAutoTime+=1;
	if(!m_bWatchMode)
	{
		m_bAutoCard =!m_bAutoCard;

		/// 如果是托管状态，且轮到自己出牌，则马上出牌
		if(m_bAutoCard)
		{
			if(GetStationParameter()==GS_PLAY_GAME&&m_iNowOutPeople==GetMeUserInfo()->bDeskStation)
			{
				UserOutCard();
			}
		}
	}
	AutoStruct bAuto;
	bAuto.bAuto=m_bAutoCard;
	SendGameData(&bAuto,sizeof(bAuto),MDM_GM_GAME_NOTIFY,ASS_AUTO,0);
	return 0;
}
//点击记牌器
LRESULT	CClientGameDlg::OnCounter(WPARAM wparam, LPARAM lparam)
{

	//比赛场不能用记牌器
	if(m_pGameInfo->dwRoomRule & GRR_CONTEST)
	{
		m_MessageHandle.InsertNormalMessage("比赛场不能用记牌器");
		return 0;
	}

	if(m_PlayView.m_CounterDlg.IsWindowVisible())
	{
		m_PlayView.m_CounterDlg.ShowWindow(SW_HIDE) ;
		
		return 0; 
	}

	SendGameData(MDM_GM_GAME_NOTIFY ,ASS_USER_VIP_VERTIFY , 0) ; 
	
	return 0;
}

//点击记牌器
LRESULT	CClientGameDlg::OnDialect(WPARAM wparam, LPARAM lparam)
{
	m_iDialectType = int(wparam) ; 
	return 0;
}


//发送离开请求函数
LRESULT	CClientGameDlg::OnSendHaveThing(WPARAM wparam, LPARAM lparam)
{
	if(!m_bWatchMode && GetStationParameter()!=GS_WAIT_SETGAME)
	{
		m_PlayView.m_bCanLeave = false;
		m_PlayView.m_btThing.EnableWindow(FALSE);
		HaveThingStruct HaveThing;
		HaveThing.pos=0;
		::lstrcpy(HaveThing.szMessage,(char *)wparam);
		SendGameData(&HaveThing,sizeof(HaveThing),MDM_GM_GAME_NOTIFY,ASS_HAVE_THING,0);
	}
	return 0;
}

/// 同意或不同意其他用户离开请求
/// @param wparam 1表示同意，0表示不同意
/// @param lparam 1表示同意，0表示不同意
LRESULT	CClientGameDlg::OnArgeeUserLeft(WPARAM wparam, LPARAM lparam)
{
	LeaveResultStruct Leave;
	BYTE result;
	result=(BYTE)wparam;
	Leave.bDeskStation=GetMeUserInfo()->bDeskStation;
	if(result < 2)
	{
		Leave.bArgeeLeave=result;
		SendGameData(&Leave,sizeof(Leave),MDM_GM_GAME_NOTIFY,ASS_LEFT_RESULT,0);
	}
	if (result>0)
	{
		;//m_PlayView.m_btThing.EnableWindow(TRUE);
	}
	return 0;
}

//重置游戏
void CClientGameDlg::ResetGameStation(int iGameStation)//游戏开始
{
	//数据重置
	m_bTimeOutCount=0;//超时次数

	m_PlayView.m_ThingMessage.ShowWindow(SW_HIDE);

	m_iNtPeople = -1;
	m_iNowOutPeople = -1;
	m_iBaseCount = 0;
	m_iFirstOutPeople = -1;

	m_bShowLast=FALSE;
	m_bCurrentOperationStation = 255;//当前叫分者
	//界面重置
	::memset(m_iLastCardCount,0,sizeof(m_iLastCardCount));

	m_PlayView.m_btOutCard.ShowWindow(SW_HIDE);
	m_PlayView.m_btCue.ShowWindow(SW_HIDE);
	m_PlayView.m_btPass.ShowWindow(SW_HIDE);
	m_PlayView.SetNTStation();
	m_ResultCallScore=-1;			//初始化
	m_PlayView.m_btAuto.EnableWindow(false);
	m_PlayView.m_btThing.EnableWindow(false);
	m_PlayView.m_btSortCard.EnableWindow(false);
	m_PlayView.m_btAddDouble.ShowWindow(0);
	m_PlayView.m_btAddDouble2.ShowWindow(0);
	m_PlayView.m_btRobNT.ShowWindow(0);
	m_PlayView.m_btRobNT2.ShowWindow(0);
	m_PlayView.m_btShowCard.ShowWindow(0);
	m_PlayView.m_btShowCard2.ShowWindow(0);
	m_PlayView.m_btLastTurn.EnableWindow(0);
	HideButton();

	//加锤按钮
	m_PlayView.m_btChuiYes.ShowWindow(SW_HIDE);
	m_PlayView.m_btChuiNo.ShowWindow(SW_HIDE);

	m_bAutoCard=false;
	m_LastOneCard=false;

	for(int i = 0; i < PLAY_COUNT; i ++)
	{
		m_PlayView.m_UserCard[i].SetCard(NULL,NULL,0);

		m_PlayView.SetPass(i,0);


		m_PlayView.SetAuto(i,false);

		m_PlayView.SetAwardPoint(i,0);

		m_PlayView.SetCallScoreValue(i,-1);		//叫分类型
	}

	///清空牌数据
	for(int i = 0;i < PLAY_COUNT; i ++)
	{
		m_PlayView.m_HandCard[i].m_CardArray.RemoveAll();
		m_CardCount[i] = 0;
		::memset(m_Card,0,sizeof(m_Card));
	}

	switch (iGameStation)
	{
	case RS_GAME_END:
		{
			for(int i = 0; i < PLAY_COUNT; i ++)
			{	
				m_PlayView.m_HandCard[i].SetCard(m_Card[i],NULL,m_CardCount[i]);
				m_PlayView.m_HandCard[i].SetShowCard(true);

//  				if(i == GetMeUserInfo()->bDeskStation)
//  				{
//  					m_PlayView.m_MeCard.SetCard(m_iMeCardList,NULL,m_iMeCardCount);
//  				}
			}
			break;
		}
	case RS_ALL_VIEW:
		m_PlayView.m_Result.ShowWindow(SW_HIDE);

	case RS_GAME_CUT:
		{
			for(int i = 0;i < PLAY_COUNT; i ++)
			{

				m_PlayView.m_HandCard[i].SetShowCard(FALSE);//重设置不显示牌状态
				m_PlayView.m_HandCard[i].SetCard(NULL,NULL,0);
				m_CardCount[i] = 0;
				m_PlayView.SetPeopleMultiple(i,1);					//设置玩家加棒
			}	
			CRect rect;
			m_PlayView.GetWindowRect(rect);
			m_PlayView.SetNTStation(-1);
			::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
			::memset(m_iMeCardList,0,sizeof(m_iMeCardList));
			m_PlayView.m_MeBackCard.ShowWindow(0);
			::memset(m_iBackCard,0,sizeof(m_iBackCard));

			//清除当前用户牌信息
			m_PlayView.m_MeBackCard.SetCard(NULL,NULL,0);
			m_PlayView.m_MeCard.SetCard(NULL,NULL,0);
			m_iMeCardCount = 0;
			::memset(m_iAIStation,0,sizeof(m_iAIStation));
			break;
		}
	case RS_GAME_BEGIN:
		{
			m_PlayView.m_Result.ShowWindow(SW_HIDE);
			m_PlayView.SetCallScoreResult(0);					//叫分清零
			//清理是否是新进桌子
			m_PlayView.m_bFirstGame = false;		//090310a1 JJ 

			for(int i = 0; i < PLAY_COUNT; i ++)
			{
				m_PlayView.m_HandCard[i].SetShowCard(FALSE);//重设置不显示牌状态
				m_PlayView.m_HandCard[i].SetCard(NULL,NULL,0);
				m_CardCount[i] = 0;
				m_PlayView.SetPeopleMultiple(i,1);					//设置玩家加棒
			}	
			CRect rect;
			m_PlayView.GetWindowRect(rect);
			m_PlayView.SetNTStation(-1);
			::memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));
			::memset(m_iMeCardList,0,sizeof(m_iMeCardList));

			m_PlayView.m_MeBackCard.ShowWindow(0);
			::memset(m_iBackCard,0,sizeof(m_iBackCard));

			//清除当前用户牌信息
			m_PlayView.m_MeBackCard.SetCard(NULL,NULL,0);
			m_PlayView.m_MeCard.SetCard(NULL,NULL,0);
			m_iMeCardCount=0;
			m_PlayView.SetGameMultiple(1);

			m_PlayView.m_CounterDlg.ReSetDlg() ;

			//设置锤一锤标志
			for(int i = 0; i < PLAY_COUNT; ++i)
			{
				m_PlayView.SetChuiData(i,1);
			}

			break;
		}
	}
	return;
}

void CClientGameDlg::OnGameSetting()
{

#ifdef SUPER_VERSION
	AfxSetResourceHandle(GetModuleHandle("DllForGame.dll"));

#ifdef SKIN_CARD
	CCardSet	*dlg = new CCardSet();
	dlg->Create(IDD_CARD_SET,this);
	dlg->CenterWindow();
	dlg->ShowWindow(SW_SHOW);
#else
	CMjSet	*set=new CMjSet();
	set->Create(IDD_MJSET,this);
	set->CenterWindow();
	set->ShowWindow(SW_SHOW);//SW_SHOW
#endif
	return ;
#endif
	//AfxSetResourceHandle(GetModuleHandle(CLIENT_DLL_NAME));

	CGameSet GameSetDlg(this);
	GameSetDlg.m_bSound=m_pGameInfo->bEnableSound;
	GameSetDlg.m_bShowUser=m_pGameInfo->bShowUserInfo;	
	GameSetDlg.m_bEnableWatch=m_pGameInfo->bEnableWatch;
	GameSetDlg.m_bRightPower = GetProfileInt(TEXT("RightPower"),TRUE);//顺时针出牌

	int reval =  GameSetDlg.DoModal();

	if (reval == IDOK)
	{
		//定义参数
		bool bSendWatch=((m_bWatchMode==false)&&(m_pGameInfo->bEnableWatch!=GameSetDlg.m_bEnableWatch));
		m_pGameInfo->bShowUserInfo=GameSetDlg.m_bShowUser;
		m_pGameInfo->bEnableWatch=GameSetDlg.m_bEnableWatch;
		m_pGameInfo->bEnableSound=GameSetDlg.m_bSound;
		WriteProfileInt(TEXT("RightPower"),GameSetDlg.m_bRightPower?1:0);
		m_bRightPower = GameSetDlg.m_bRightPower;
		//发送数据
		if (bSendWatch==true)
		{
			MSG_GM_WatchSet WatchSet;			
			memset(&WatchSet,0,sizeof(WatchSet));
			WatchSet.dwUserID=0;
			SendGameData(&WatchSet,sizeof(WatchSet),MDM_GM_GAME_FRAME,ASS_GM_WATCH_SET,GameSetDlg.m_bEnableWatch?TRUE:FALSE);

		}
	}
	//AfxSetResourceHandle(GetModuleHandle(NULL));
	return;
}

void CClientGameDlg::OnWatchSetChange(void)
{
	if (m_bWatchMode==true)
	{
		m_PlayView.m_MeCard.SetShowCard(m_bWatchOther?TRUE:FALSE);
		if (m_bWatchOther)
			m_MessageHandle.InsertNormalMessage(TEXT("玩家允许您旁观他游戏"));
		else 
			m_MessageHandle.InsertNormalMessage(TEXT("玩家不允许您旁观他游戏"));
	}
}

//用户离开
bool CClientGameDlg::GameUserLeft(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser)
{	
	OutputDebugString("wyslog CClientGameDlg::GameUserLeft");
	//空
	if(pUserItem == NULL)
		return false;

    m_PlayView.UpdateViewFace(NULL);    //20120809 yl/修改玩家离开后头像还在桌面上
    
	if (bDeskStation>=0&&bDeskStation<PLAY_COUNT)
	{
		BYTE iView = ViewStation(bDeskStation);
		m_PlayView.m_FlashPlayer[iView].Stop();
		m_PlayView.m_FlashPlayer[iView].ShowWindow(SW_HIDE);
	}

	//比赛场游戏本人离开
	if(GetMeUserInfo()->bDeskStation==bDeskStation&&!bWatchUser && TY_MATCH_GAME == GetComType())
	{
		OnCancel();
		return __super::GameUserLeft(bDeskStation,pUserItem,bWatchUser);
	}

	//比赛场非本人离开游戏桌
	if(GetMeUserInfo()->bDeskStation != bDeskStation&&!bWatchUser && TY_MATCH_GAME == GetComType())
	{
		return __super::GameUserLeft(bDeskStation,pUserItem,bWatchUser);
	}

	if (bWatchUser==false)//&&(bDeskStation==0))
	{
		if (GetStationParameter() > GS_WAIT_ARGEE)
		{
			this->SetStationParameter(GS_WAIT_ARGEE);
			ResetGameStation(RS_ALL_VIEW);			
			m_PlayView.m_btContinue.ShowWindow(!m_bWatchMode && TY_MATCH_GAME != GetComType());
		}
	}
	//旁观者也离开游戏桌
	if(GetMeUserInfo()->bDeskStation==bDeskStation&&!bWatchUser==TRUE)
	{
		OnCancel();
	}

	return __super::GameUserLeft(bDeskStation,pUserItem,bWatchUser);
}

//退出
void CClientGameDlg::OnCancel()
{
	TCHAR sz[200];
	/*
	if(GetComType() == TY_MONEY_GAME)
		wsprintf(sz,TEXT("现在退出将会扣 %d 分(金币场还会扣 %d 金币).您真得要退出吗?"),m_iRunPublish,m_iRunPublish * m_iRoomBasePoint);
	else
		wsprintf(sz,TEXT("现在退出将会扣 %d 分.您真得要退出吗?"),m_iRunPublish);
	*/
	
	//CString s = "..\\";// CBcfFile::GetAppPath ();/////本地路径
	//CBcfFile f( "GameOption.bcf");
	//CString key=TEXT("UISet");
	//if((m_pGameInfo->uNameID&GRR_VIDEO_ROOM)!=0)
	//	key="VideoUISet";
	//CString sec = _T("");
	//
	//int nPower=1;
	//for (int i=0; i<Glb().m_nPowerOfGold; i++)
	//{

	//	nPower*=10;
	//}

	//// 现在退出将会扣 %d 分(金币场还会扣 除您下的所有 金币).您真得要退出吗?
	//sec.Format("FouceQuitTip%d", m_pGameInfo->uComType);
	//wsprintf(sz,f.GetKeyVal(key, sec, ""),m_iRunPublish,m_iRunPublish* m_iRoomBasePoint*nPower);

	//查询状态
	if (!m_bWatchMode && GetStationParameter() >= GS_SEND_CARD&&GetStationParameter() <= GS_WAIT_NEXT)
	{

		char cKey[10];
		//CString sPath=CBcfFile::GetAppPath();
		int iResult = 1;
		if (CBcfFile::IsFileExist( "..\\SpecialRule.bcf"))
		{
			CBcfFile fsr( "..\\SpecialRule.bcf");
			sprintf(cKey, "%d", NAME_ID);
			iResult = fsr.GetKeyVal (_T("ForceQuitAsAuto"), cKey, 0);
		}

		if (iResult)
			wsprintf(sz,TEXT("您正在游戏中，现在退出游戏自动托管.您真得要退出吗?"));

		else
		{
			if (m_pGameInfo->uComType == 1)
			{
				wsprintf(sz,TEXT("您正在游戏中，现在退出将会被扣积分，是否要退出游戏？"));
			}
			else
			{
				wsprintf(sz,TEXT("您正在游戏中，现在退出将会被扣金币，是否要退出游戏？"));
			}
		}
		//if (IDYES!=AFCMessage(sz,m_pGameInfo->szGameName,MB_YESNO|MB_DEFBUTTON2|MB_ICONINFORMATION,this)) 
		if (IDOK!=AFCMessageBox(sz,m_pGameInfo->szGameName,AFC_YESNO)) //|MB_DEFBUTTON2|MB_ICONINFORMATION,this)) 
			return;
		SendGameData(MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);

		if(m_PlayView.m_GameNoteView.IsWindowVisible())
		{
			m_PlayView.m_GameNoteView.ShowWindow(SW_HIDE);
		}
		m_PlayView.Init();
		m_pGameInfo->bGameStation=1;
		AFCCloseFrame();
		m_pGameRoom->PostMessage(WM_COMMAND, MAKELONG(2021, BN_CLICKED), 0);//响应关闭房间
		m_PlayView.m_bFirstGame = true;
		return;
	}
	m_PlayView.m_bFirstGame = true;
	AFCCloseFrame();
	__super::OnCancel();
}

//桌号换算到视图位置
BYTE CClientGameDlg::ViewStation(BYTE bDeskStation)
{
	BYTE bViewStation = GetMeUserInfo()->bDeskStation ;//m_pGameInfo->pMeUserInfo->GameUserInfo.bDeskStation;
	switch(bViewStation)
	{
	case 0: 
		{ 
			if(bDeskStation == 0)	
				return 1;
			if(bDeskStation == 2)
				return  2 ;
			if(bDeskStation == 1)
				return  0;
		}
	case 1:
		{
			if(bDeskStation == 0)	
				return 2 ;
			if(bDeskStation == 1)
				return 1;
			if(bDeskStation == 2)
				return 0 ;
		}

	case 2: 
		{ 
			if(bDeskStation == 0)	
				return 0 ;

			if(bDeskStation == 1)
				return 2 ;

			if(bDeskStation == 2)
				return 1;
		}
	default:
		break;
	}
	return 0;
}


//叫分
void CClientGameDlg::ShowCallScoreBt(int CallScore,BOOL bExtVol)
{
	m_PlayView.m_btCallScore1.ShowWindow(SW_SHOW);
	m_PlayView.m_btCallScore2.ShowWindow(SW_SHOW);
	m_PlayView.m_btCallScore3.ShowWindow(SW_SHOW);

	m_PlayView.m_btCallScorePass.ShowWindow(SW_SHOW);
	m_PlayView.m_btCallScorePass.EnableWindow(1);

	m_PlayView.m_btCallScore1.EnableWindow(false);
	m_PlayView.m_btCallScore2.EnableWindow(false);
	m_PlayView.m_btCallScore3.EnableWindow(false);

	switch(CallScore)
	{
	case 3:
		break;
	case 2:
		m_PlayView.m_btCallScore3.EnableWindow(TRUE);
		break;
	case 1:
		m_PlayView.m_btCallScore2.EnableWindow(TRUE);
		m_PlayView.m_btCallScore3.EnableWindow(TRUE);
		break;
	default:
		m_PlayView.m_btCallScore1.EnableWindow(TRUE);
		m_PlayView.m_btCallScore2.EnableWindow(TRUE);
		m_PlayView.m_btCallScore3.EnableWindow(TRUE);
		break;
	}
	return;
}
//叫分
LRESULT	CClientGameDlg::OnCallScore(WPARAM wparam, LPARAM lparam)
{
	KillTimer(ID_CALL_SCORE_TIME);
	HideButton();
	CallScoreStruct score;
	memset(&score,0,sizeof(score));
	score.iValue = (int)wparam;								
	score.bDeskStation = GetMeUserInfo()->bDeskStation;			
	score.bCallScoreflag = false;
	SendGameData(&score,sizeof(score),MDM_GM_GAME_NOTIFY,ASS_CALL_SCORE,0);
	return 0;
}
//隐藏按钮
BOOL CClientGameDlg::HideButton()
{
	m_PlayView.m_btCallScore1.ShowWindow(SW_HIDE);
	m_PlayView.m_btCallScore2.ShowWindow(SW_HIDE);
	m_PlayView.m_btCallScore3.ShowWindow(SW_HIDE);
	m_PlayView.m_btCallScorePass.ShowWindow(SW_HIDE);
	return true;
}
//排序
LRESULT	CClientGameDlg::OnHitSortCard(WPARAM wparam, LPARAM lparam)
{
	//排序方式
	m_bSortCard ++;
	m_bSortCard %= 2;
	m_Logic.SetSortCardStyle(m_bSortCard);
	m_Logic.SortCard(m_iMeCardList,NULL,m_iMeCardCount);
	m_PlayView.m_MeCard.SetCard(m_iMeCardList,NULL,m_iMeCardCount);
	OnHitCard(0,0);								
	return 0;
}
//不出
LRESULT	CClientGameDlg::OnHitPassCard(WPARAM wparam, LPARAM lparam)
{
	KillTimer(ID_OUT_CARD);
	//设置控件
	if (m_PlayView.m_btPass.IsWindowVisible())
	{
		//设置数据
		KillAllTimer();
		m_iNowOutPeople=-1;
		m_PlayView.m_btOutCard.ShowWindow(SW_HIDE);
		m_PlayView.m_btPass.ShowWindow(SW_HIDE);
		m_PlayView.m_btCue.ShowWindow(SW_HIDE);
		//发送数据
		OutCardStruct OutCard;
		OutCard.iCardCount=0;
		SendGameData(&OutCard,sizeof(OutCard)-sizeof(OutCard.iCardList)+sizeof(BYTE)*OutCard.iCardCount,
			MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
	}
	m_PlayView.m_MeCard.SetCard(m_iMeCardList,NULL,m_iMeCardCount);
	return 0;
}


//函数定义
BOOL CClientGameDlg::CheckVersion(BYTE iVersion,BYTE iVersion2)
{
	TCHAR GameName[100];
	CString strBuffer;
	wsprintf(GameName,TEXT("您的%s游戏版本过旧，您要现在重新下载新版本吗?"),m_pGameInfo->szGameName);
	if(iVersion != m_iVersion || iVersion2 != m_iVersion2)
	{
		TCHAR sz[200];
		wsprintf(sz,TEXT("版本冲突,当前服务器版本号%d-%d,您的版本号%d-%d"),iVersion,iVersion2,m_iVersion,m_iVersion2);
		//		WriteStr(sz);
	}	
	if(iVersion != m_iVersion)
	{
		if (IDOK == AFCMessageBox(GameName,m_pGameInfo->szGameName,AFC_YESNO))//MB_YESNO|MB_DEFBUTTON1|MB_ICONINFORMATION,this)) 
		{
			strBuffer.Format(TEXT(Glb().m_CenterServerPara.m_strDownLoadSetupADDR),1,1);
			ShellExecute(NULL,TEXT("open"),strBuffer,NULL,NULL,SW_MAXIMIZE);
		}
		PostMessage(WM_CLOSE,0,0);
		return TRUE;
	}

	if(iVersion2 != m_iVersion2)
	{
		if (IDOK == AFCMessageBox(GameName,m_pGameInfo->szGameName,AFC_YESNO))//MB_YESNO|MB_DEFBUTTON1|MB_ICONINFORMATION,this)) 
		{
			strBuffer.Format(TEXT(Glb().m_CenterServerPara.m_strDownLoadSetupADDR),1,1);
			ShellExecute(NULL,TEXT("open"),strBuffer,NULL,NULL,SW_MAXIMIZE);
		}
		PostMessage(WM_CLOSE,0,0);
		return TRUE;
	}
	return TRUE;
}

//出牌
BOOL CClientGameDlg::UserOutCard()
{
	m_PlayView.m_btOutCard.ShowWindow(SW_HIDE);
	m_PlayView.m_btPass.ShowWindow(SW_HIDE);
	m_PlayView.m_btCue.ShowWindow(SW_HIDE);


	CString str ; 
	str.Format("zhangjing::fafalflalfgjalgjlagjlagj %d" ,m_LastOneCard) ; 
	OutputDebugString(str); 
	//发送数据
	OutCardStruct OutCard;
	if(m_iBaseCount == 0 || m_bAutoCard||m_LastOneCard)//托管出牌
	{
		m_Logic.AutoOutCard(m_iMeCardList,m_iMeCardCount,m_iBaseCard,m_iBaseCount,OutCard.iCardList,
			OutCard.iCardCount,m_iBigOutPeople==GetMeUserInfo()->bDeskStation);

	}
	else
		OutCard.iCardCount=0;

#ifdef AI_MACHINE
	m_Logic.AutoOutCard(m_iMeCardList,m_iMeCardCount,m_iBaseCard,m_iBaseCount,OutCard.iCardList,
		OutCard.iCardCount,m_iBigOutPeople == GetMeUserInfo()->bDeskStation);
#endif

	m_iNowOutPeople=-1;
	SendGameData(&OutCard,sizeof(OutCard)-sizeof(OutCard.iCardList)+sizeof(BYTE)*OutCard.iCardCount,
		MDM_GM_GAME_NOTIFY,ASS_OUT_CARD,0);
	//OnHitCard(0,0);
	m_PlayView.m_MeCard.SetCard(m_iMeCardList,NULL,m_iMeCardCount);
	
	return TRUE;
}

//代替玩家开始
BOOL CClientGameDlg::ReplaceBegin(BYTE bDeskStation)
{
	if (!m_bWatchMode)
	{
		ReplaceAgreeStruct agree;
		agree.bDeskStation = bDeskStation;
		SendGameData(&agree,sizeof(agree),MDM_GM_GAME_NOTIFY,ASS_REPLACE_GM_AGREE_GAME,0);
	}
	return TRUE;
}

//代替玩家出牌
BOOL CClientGameDlg::ReplaceOutCard(BYTE bDeskStation)
{
	BYTE bDesk = ViewStation(bDeskStation);
	if(m_CardCount[bDesk] <= 0)
		return true;

	//发送数据
	ReplaceOutCardStruct OutCard;
	OutCard.bDeskStation = bDeskStation;
	m_Logic.AutoOutCard(m_Card[bDesk],m_CardCount[bDesk],
		m_iBaseCard,m_iBaseCount,
		OutCard.iCardList,OutCard.iCardCount,
		m_iBigOutPeople == bDeskStation);
	//普通场,非第一个玩家代替出牌
	if(m_iFirstOutPeople != bDeskStation)
	{
		OutCard.iCardCount = 0;
	}

	SendGameData(&OutCard,sizeof(OutCard)-sizeof(OutCard.iCardList)+sizeof(BYTE)*OutCard.iCardCount,
		MDM_GM_GAME_NOTIFY,ASS_REPLACE_OUT_CARD,0);
	return TRUE;
}

//代替玩家叫分
BOOL CClientGameDlg::ReplaceCallScore(BYTE bDeskStation)
{
	ReplaceCallScoreStruct score;
	memset(&score,0,sizeof(score));
	score.CallScore = 0;						//代替叫分为0分,即不叫
	score.bDeskStation = bDeskStation;	//叫分的人的位置
	score.CallScoreFlag=false;
	SendGameData(&score,sizeof(score),MDM_GM_GAME_NOTIFY,ASS_REPLACE_CALL_SCORE,0);
	return true;
}

//显示用户聊天
void CClientGameDlg::ShowUserTalk(MSG_GR_RS_NormalTalk *pNormalTalk)
{
	TCHAR szMsg[1000];
	//处理数据
	UserItemStruct * pSendUserItem=FindOnLineUser(pNormalTalk->dwSendID);
	if (pSendUserItem==NULL) 
		return ;
	if(pSendUserItem->GameUserInfo.bUserState==USER_WATCH_GAME)
		return ;
	lstrcpy(szMsg,pNormalTalk->szMessage);	
	m_PlayView.DisplayTalkMsg(ViewStation(pSendUserItem->GameUserInfo.bDeskStation),pNormalTalk->szMessage); //显示聊天信息	

	m_InputMessage.SetFocus();
	m_InputMessage.SetEditSel(m_InputMessage.GetWindowTextLength(),m_InputMessage.GetWindowTextLength());

	return ;
}

//设置桌面上标记
BOOL CClientGameDlg::SetNoteFlag(int iFlag)
{
	//m_PlayView.m_GameNoteView.SetGameStateFlag(iFlag);
	CRect rect,clientrect;
	m_PlayView.GetClientRect(&rect);
	m_PlayView.m_GameNoteView.GetClientRect(&clientrect);
	int width = clientrect.right - clientrect.left ;
	int height = clientrect.bottom - clientrect.top ;
	int x = (rect.right - rect.left )/2 - width /2;
	int y = (rect.bottom - rect.top )/2 - height /2;

	m_PlayView.m_GameNoteView.MoveWindow(x,y,width,height);
	m_PlayView.m_GameNoteView.SetMessage(TEXT("ok"));
	return TRUE;
}

//玩家断线,并判断是否为当前出牌者
BOOL CClientGameDlg::UserCut(BYTE bDeskStation)
{
	m_iAIStation[bDeskStation] = 1;
	//当前断线者为当前叫分者
	if(m_bCurrentOperationStation == bDeskStation)
	{
		ReplaceCallScore(bDeskStation);
		return true;
	}

	//当前断线者为当前出牌者
	if(m_iNowOutPeople == bDeskStation)
	{
		ReplaceOutCard(bDeskStation);
		return true;
	}
	return true;
}

//抢地主
LRESULT	CClientGameDlg::OnRobNT(WPARAM wparam, LPARAM lparam)
{
	m_PlayView.m_btRobNT.ShowWindow(0);

	m_PlayView.m_btRobNT2.ShowWindow(0);

	RobNTStruct robnt;
	memset(&robnt,0,sizeof(robnt));
	robnt.bDeskStation = GetMeUserInfo()->bDeskStation;		
	robnt.iValue  = (int)wparam;	
	SendGameData(&robnt,sizeof(robnt),MDM_GM_GAME_NOTIFY,ASS_ROB_NT,0);
	return 0;
}

//代替抢主
BOOL CClientGameDlg::ReplaceRobNT(BYTE bDeskStation)
{
	RobNTStruct robnt;
	memset(&robnt,0,sizeof(robnt));
	robnt.bDeskStation = bDeskStation;		
	robnt.iValue  = 0;//(int)wparam;	
	SendGameData(&robnt,sizeof(robnt),MDM_GM_GAME_NOTIFY,ASS_REPLACE_ROB_NT,0);
	return true;
}

//加棒
LRESULT	CClientGameDlg::OnAddDouble(WPARAM wparam, LPARAM lparam)
{
	m_PlayView.m_btAddDouble.ShowWindow(0);
	m_PlayView.m_btAddDouble2.ShowWindow(0);
	if(m_pGameInfo->bEnableSound )		//声音
	{
		if ( (int)wparam >0)
		{
			VoiceOut(GetMeUserInfo()->bDeskStation,"human_JIABEI.wav");
		}
		else
		{
		    VoiceOut(GetMeUserInfo()->bDeskStation,"human_BUJIABEI.wav");
		}
	}

	AddDoubleStruct AddDouble;
	memset(&AddDouble,0,sizeof(AddDouble));
	AddDouble.bDeskStation = GetMeUserInfo()->bDeskStation;		
	AddDouble.iValue  = (int)wparam;	
	SendGameData(&AddDouble,sizeof(AddDouble),MDM_GM_GAME_NOTIFY,ASS_ADD_DOUBLE,0);
	return 0;
}

//锤
LRESULT CClientGameDlg::OnChuiYes(WPARAM wparam,LPARAM lparam)
{
	m_PlayView.m_btChuiYes.ShowWindow(SW_HIDE);	//锤
	m_PlayView.m_btChuiNo.ShowWindow(SW_HIDE);	//不锤

	ChuiStruct s_chui;
	s_chui.bDeskStation = GetMeUserInfo()->bDeskStation;
	s_chui.bIsChui = true;

	SendGameData(&s_chui,sizeof(ChuiStruct),MDM_GM_GAME_NOTIFY,ASS_CHUI,0);

	SetTimer(ID_ANIM_TIME,100,NULL);

	return 0;
}
//不锤
LRESULT CClientGameDlg::OnChuiNo(WPARAM wparam,LPARAM lparam)
{
	m_PlayView.m_btChuiYes.ShowWindow(SW_HIDE);	//锤
	m_PlayView.m_btChuiNo.ShowWindow(SW_HIDE);	//不锤

	ChuiStruct s_chui;
	s_chui.bDeskStation = GetMeUserInfo()->bDeskStation;
	s_chui.bIsChui = false;

	SendGameData(&s_chui,sizeof(ChuiStruct),MDM_GM_GAME_NOTIFY,ASS_CHUI,0);

	return 0;
}

//加棒
LRESULT	CClientGameDlg::OnShowCard(WPARAM wparam, LPARAM lparam)
{
	KillTimer(ID_SHOW_CARD);
	m_PlayView.m_btShowCard.ShowWindow(0);
	m_PlayView.m_btShowCard2.ShowWindow(0);

	ShowCardStruct show;
	memset(&show,0,sizeof(show));
	show.bDeskStation = GetMeUserInfo()->bDeskStation;		
	show.iValue  = (int)wparam;	
	SendGameData(&show,sizeof(show),MDM_GM_GAME_NOTIFY,ASS_SHOW_CARD,0);
	return 0;
}
//代替加棒
BOOL CClientGameDlg::ReplaceAddDouble(BYTE bDeskStation)
{
	AddDoubleStruct AddDouble;
	memset(&AddDouble,0,sizeof(AddDouble));
	AddDouble.bDeskStation = bDeskStation;		
	AddDouble.iValue  = 0;//(int)wparam;	
	SendGameData(&AddDouble,sizeof(AddDouble),MDM_GM_GAME_NOTIFY,ASS_REPLACE_ADD_DOUBLE,0);
	return true;
}

//按动上轮按钮函数
LRESULT	CClientGameDlg::OnLastTurn(WPARAM wparam, LPARAM lparam)
{
	////====注释了下面二行代码不知道有什么
	//if ((m_bShowIngLast)||(!m_bShowLast)||(m_iLastCardCount==0))//||(m_iNowOutPeople==m_iFirstOutPeople))
	//	return 0;
	m_bShowIngLast=TRUE;
	m_PlayView.m_btLastTurn.ShowWindow(SW_HIDE);

	SetTimer(ID_SHOW_LAST,3000,NULL);

	for (int i = 0;i < PLAY_COUNT; i ++)
	{	
		if(m_iLastCardCount[i] == 0)
		{
			m_PlayView.m_UserCard[ViewStation(i)].SetCard(NULL,NULL,0);
			m_PlayView.SetPass(ViewStation(i),1); // 20081210
		}
		else
		{
			m_PlayView.m_UserCard[ViewStation(i)].SetCard(m_iLastOutCard[i],NULL,m_iLastCardCount[i]);
			m_PlayView.SetPass(ViewStation(i),0); // 20100603
		}
	}
	return 0;
}

//为视频模块重载此函数

bool CClientGameDlg::GameUserCome(BYTE bDeskStation, UserItemStruct * pUserItem, bool bWatchUser)
{
	//OutputDebugString("wyslog::11111111");
	//if((m_pGameInfo->dwRoomRule & GRR_VIDEO_ROOM)==0)
	//return  __super::GameUserCome(bDeskStation, pUserItem, bWatchUser);
	if(!m_hWnd)
		return FALSE;
	if (! __super::GameUserCome(bDeskStation, pUserItem, bWatchUser))
	{
		return FALSE;
	}

	if( GetMeUserInfo()->dwUserID == pUserItem->GameUserInfo.dwUserID && bWatchUser )
	{
		m_PlayView.m_bWatch=bWatchUser;
	}
	if (!bWatchUser)
	{
		PlayFlashVideo(bDeskStation,pUserItem,GetMeUserInfo()->dwUserID == pUserItem->GameUserInfo.dwUserID);
	}
 
#ifdef _USE_VIDEO
		if( GetMeUserInfo()->dwUserID == pUserItem->GameUserInfo.dwUserID && (!bWatchUser) )
			if(m_pBRGameVideoSDK != NULL)
			{
				CString NickName=TEXT("");
				int    byteDeskNO=0;
				UserInfoStruct &UInfo=	pUserItem->GameUserInfo;
				NickName+=UInfo.nickName;
				byteDeskNO=UInfo.bDeskNO;
				UINT	uiRoomID = m_pGameInfo->uRoomID;
				UINT	uiBRRoomID =   uiRoomID * 1000 + byteDeskNO; 

				m_pBRGameVideoSDK->ConnectToServer(m_GameVideoSet.VideoServerIP,m_GameVideoSet.VideoServerPort);
				m_pBRGameVideoSDK->LoginSystem(NickName,"");
				m_pBRGameVideoSDK->EnterRoom(uiBRRoomID,"");			
			}
#endif
//	}
 	return TRUE;
}
bool CClientGameDlg::AFCCloseFrame()
{	
	if((m_pGameInfo->dwRoomRule & GRR_VIDEO_ROOM)==0)
		return __super::AFCCloseFrame();
	 ///释放视频资源 
    /*CPlayVideo *pPlayVideo = CPlayVideo::GetPlayVideo();
    if( (NULL != pPlayVideo) && (NULL != pPlayVideo->m_hWnd))
		pPlayVideo->Video_Release(); */
#ifdef _USE_VIDEO
	if(!m_pBRGameVideoSDK)
		return __super::AFCCloseFrame();

	m_pBRGameVideoSDK->LeaveRoom();
	m_pBRGameVideoSDK->CloseGameVideoSDK();

	delete m_pBRGameVideoSDK;
	m_pBRGameVideoSDK = NULL;
#endif
	return __super::AFCCloseFrame();
}

void CClientGameDlg::SetRecordView()
{
	m_PlayView.m_btContinue.ShowWindow(SW_HIDE);
	m_PlayView.m_btLeave.ShowWindow(SW_HIDE);
	m_PlayView.m_btCallScorePass.ShowWindow(SW_HIDE);
	m_PlayView.m_btCallScore1.ShowWindow(SW_HIDE);
	m_PlayView.m_btCallScore2.ShowWindow(SW_HIDE);
	m_PlayView.m_btCallScore3.ShowWindow(SW_HIDE);
	m_PlayView.m_btLastTurn.ShowWindow(SW_HIDE);
	m_PlayView.m_btShowCard.ShowWindow(SW_HIDE);
	m_PlayView.m_btShowCard2.ShowWindow(SW_HIDE);

	m_PlayView.m_btRobNT.ShowWindow(SW_HIDE);
	m_PlayView.m_btRobNT2.ShowWindow(SW_HIDE);
	m_PlayView.m_btAddDouble.ShowWindow(SW_HIDE);
	m_PlayView.m_btAddDouble2.ShowWindow(SW_HIDE);

	m_PlayView.m_btAuto.ShowWindow(SW_HIDE);
	m_PlayView.m_btThing.ShowWindow(SW_HIDE);
	m_PlayView.m_btSortCard.ShowWindow(SW_HIDE);

	m_PlayView.m_btOutCard.ShowWindow(SW_HIDE);
	m_PlayView.m_btPass.ShowWindow(SW_HIDE);
	m_PlayView.m_btCue.ShowWindow(SW_HIDE);


	for (int i=0; i<PLAY_COUNT; i++)
	{
		m_PlayView.m_HandCard[i].SetShowCard(false);
	}

	m_iBeginTime = 30;
	m_iThinkTime = 30;
	m_iGiveBackTime = 30;
	m_iCallScoreTime = 15;
	m_iAddDoubleTime = 15; 


}

// PengJiLin, 2011-4-19, 在线时长、局数送金币图像提示
void CClientGameDlg::ShowGetMoneyImage()
{
    // PengJiLin, 2011-5-12, 靠在结算框的底部
    CRect rectGetMoney;
    m_PlayView.m_GetMoneyImageView.GetClientRect(&rectGetMoney);

    CRect rectResult;
    m_PlayView.m_Result.GetWindowRect(&rectResult);
    m_PlayView.m_Result.MoveWindow(rectResult.left, rectResult.top - rectGetMoney.Height(), rectResult.Width(), rectResult.Height());
    m_PlayView.m_Result.GetWindowRect(&rectResult);
    CPoint pt;
    pt.x = rectResult.left;
    pt.y = rectResult.bottom;
    ::ScreenToClient(m_PlayView.GetSafeHwnd(), &pt);
    
    m_PlayView.m_GetMoneyImageView.MoveWindow(pt.x, pt.y, rectGetMoney.Width(), rectGetMoney.Height(), FALSE);

    CString strGet = "";
    CString strTime = "";
    CString strCount = "";
    GetSendMoneyFormatInfo(strGet, strTime, strCount);
    m_PlayView.m_GetMoneyImageView.SetMessage(strGet, strTime, strCount, (m_pGameInfo->dwRoomRule & GRR_VIDEO_ROOM)>0);

    m_PlayView.m_GetMoneyImageView.SetShow(TRUE);
}

//add by ljl 为斗地主增加声音。
BOOL CClientGameDlg::VoiceOut(BYTE bDeskStation, LPCSTR pszSound)
{
	CString strTmp = pszSound;
	CString strPathName  = ""; 
	if ( bDeskStation == 255 )
	{
		TCHAR fileName[100];
		ZeroMemory(fileName,sizeof(fileName));
		wsprintf(fileName,".//music//%s",strTmp);
		::PlaySound(fileName,NULL,SND_FILENAME|SND_ASYNC|SND_NOWAIT|SND_NODEFAULT);
		return true;
	}

	if(bDeskStation<0 ||bDeskStation>=PLAY_COUNT)
	{

		return FALSE;
	}

	TCHAR fileName[100];
	ZeroMemory(fileName,sizeof(fileName));
	
	if(m_pUserInfo != NULL && m_pUserInfo[bDeskStation] != NULL )
	{
		if(m_iDialectType == 1)
		{
			strPathName.Format("shipo") ;
		}
		else if(m_iDialectType == 2)
		{
			strPathName.Format("zhongxin") ;
		}
		else if(m_iDialectType == 3)
		{
			strPathName.Format("pucheng") ;
		}
		else
		{
			strPathName = "" ; 
		}

		if(m_pUserInfo[bDeskStation]->GameUserInfo.bBoy)
		{
			strTmp.Replace("human","MALE");
			wsprintf(fileName,".//music//%s//man//%s",strPathName , strTmp);

		}
		else
		{
			strTmp.Replace("human","FEMALE");
			wsprintf(fileName,".//music//%s//women//%s",strPathName ,strTmp);
		}
	}

	::PlaySound(fileName,NULL,SND_FILENAME|SND_ASYNC|SND_NOWAIT|SND_NODEFAULT);
	//TCHAR asdasd[4];
	//wsprintf(asdasd,"%d",bDeskStation);
	//OutputDebugString(fileName);
	//OutputDebugString(asdasd);
	return TRUE;
}
void CClientGameDlg::GetNumString(__int64 nNum, CString &Str, int nPower)
{
	if (0 == nNum)
	{
		Str.Format("0");
		return;
	}
	/// 暂存结果
	TCHAR strTmp[32];
	TCHAR str[50];
	/// 目前只处理大于0的情况，在正常字符串后面要加至少一个0
	wsprintf( strTmp, TEXT("%I64d"), nNum);
	for (int i=0; i<nPower; ++i)
	{
		_tcscat( strTmp, TEXT("0"));
	}
	/// 按位数每三位空一格显示
	int nLen = _tcslen(strTmp);	// 字符串长度
	int nTimes = nLen % 3;	// 这个字符串可以分成的节数，至少是1节

	int nSrc = 0;
	int nDes = 0;
	
	for (int i=0; i<nLen; ++i)
	{
		/*if ((i>0)&&(i%3==nTimes))
		{
			str[nDes++] = TEXT(' ');
		}*/
		str[nDes++] = strTmp[nSrc++];
	}
	str[nDes] = TEXT('\0');
	Str.Format("%s",str);
	return ;
}
void CClientGameDlg::PlayFlashVideo(BYTE bDeskStation,UserItemStruct * pUserItem,bool byMySelf)
{
	if(false == m_bPlayFlashVideo)
	{
		return;
	}

	if(bDeskStation<0||bDeskStation>=PLAY_COUNT)
	{
	    return;
	}
	if(NULL == pUserItem)
	{
		return;
	}
	CString strPath = CINIFile::GetAppPath();
	CString strVarGroupName = _T("groupName");//组名 唯一标识
	CString strVarUserName = _T("userName");//用户名 唯一标识
	CString strSendFlash ;
	CString strReceiveFlash ;
	strSendFlash.Format("%sFlashComd\\Send.swf",strPath);//用户发送视频的flash
	strReceiveFlash.Format("%sFlashComd\\Receive.swf",strPath);    //用户接收视频的flash
	CString strRoomID;
	CString strUserDesk;

	strRoomID.Format("Room_%d_Desk_%d",m_pGameInfo->uRoomID*1000,pUserItem->GameUserInfo.bDeskNO);
	strUserDesk.Format("No_%d",pUserItem->GameUserInfo.dwUserID);

	BYTE iView = ViewStation(bDeskStation);
	char szFunction[256] = {0};
	
	CString strTemp = "";
	


	if (byMySelf)
	{	
		strTemp.Format("%s,%s,%s,%s",strRoomID,strUserDesk,"200","123");
		m_PlayView.m_FlashPlayer[iView].LoadMovie(0, strSendFlash);
		m_PlayView.m_FlashPlayer[iView].Play();
		//m_PlayView.m_FlashPlayer[iView].SetVariable(strVarGroupName,strRoomID);
		//m_PlayView.m_FlashPlayer[iView].SetVariable(strVarUserName,strUserDesk);	
		 sprintf(szFunction, "<invoke name='publicVideo' returntype='xml'><arguments><string>%s</string></arguments></invoke>",strTemp); // 调用flash调方法的xml    

	}
	else
	{
		strTemp.Format("%s,%s,%s,%s",strRoomID,strUserDesk,"166","123");
		m_PlayView.m_FlashPlayer[iView].LoadMovie(0, strReceiveFlash);
		m_PlayView.m_FlashPlayer[iView].Play();
		//m_PlayView.m_FlashPlayer[iView].SetVariable(strVarGroupName,strRoomID);
		//m_PlayView.m_FlashPlayer[iView].SetVariable(strVarUserName,strUserDesk);
		 sprintf(szFunction, "<invoke name='loadAndPlayVideo' returntype='xml'><arguments><string>%s</string></arguments></invoke>",strTemp); // 调用flash调方法的xml 

	}
	sprintf(m_strFlash[bDeskStation],"%s",szFunction);
	

	if (1 == iView)
	{
		m_PlayView.m_FlashPlayer[iView].MoveWindow(m_PlayView.m_ptVideoPos[iView].x,\
			                     m_PlayView.m_ptVideoPos[iView].y,\
								 VIDEO_WIDTH_MYSELF,\
		                         VIDEO_HEIGHT_MYSELF);		

	}
	else
	{
		m_PlayView.m_FlashPlayer[iView].MoveWindow(m_PlayView.m_ptVideoPos[iView].x,\
		                         m_PlayView.m_ptVideoPos[iView].y,\
		                         VIDEO_WIDTH,\
		                         VIDEO_HEIGHT);		

	}	

	m_PlayView.m_FlashPlayer[iView].ShowWindow(SW_SHOW);
	if(iView != 1)
	{
		/*CRgn rgnVideoBox;
		rgnVideoBox.CreateRectRgn(5,5,106,124);
		m_PlayView.m_FlashPlayer[iView].SetWindowRgn((HRGN)rgnVideoBox,TRUE);*/
		m_PlayView.SetVideoRgn(iView);
	}
	

	return;

	
	
	CString strInfo;
	strInfo+="wysvideo";
	strInfo+= pUserItem->GameUserInfo.nickName;
	strInfo+=m_PlayView.m_FlashPlayer[ViewStation(bDeskStation)].GetVariable(strVarGroupName);

	strInfo+="\\";
	strInfo+=m_PlayView.m_FlashPlayer[ViewStation(bDeskStation)].GetVariable(strVarUserName);
	OutputDebugString(strInfo);

	return;
}
///创建flash信任文件
void CClientGameDlg::CreateTrustTxt(void)
{
	try
	{
		OutputDebugString("wysoutfor::CreateTrustTxt()");
		char szPath[MAX_PATH], szFilename[MAX_PATH];

		wsprintf(szPath, "C:\\WINDOWS\\system32\\Macromed\\Flash\\FlashPlayerTrust");

		SHCreateDirectoryEx(NULL, szPath, NULL);
		// 文件名称
		wsprintf(szFilename, "%s\\新建文本文档.txt",szPath);
		
		CFile file;
		CFileStatus   status;
		if(file.GetStatus(szFilename,status))
		{			
			return;
		}		
		file.Open(szFilename, CFile::modeCreate | CFile::modeReadWrite);
		
		TCHAR msg[MAX_PATH];
		CString strPath = CINIFile::GetAppPath();
		wsprintf(msg, "%s",strPath);
		file.Write(msg, strlen(msg));

		file.Close();
	}
	catch (CException *e) 
	{
		e->Delete();
	}
	return;
}
void CClientGameDlg::StopFlashVideo(BYTE bDeskStation)
{	
	if(false == m_bPlayFlashVideo)
	{
		return;
	}
	if(bDeskStation<0||bDeskStation>=PLAY_COUNT)
	{
		return;
	}

	BYTE iView = ViewStation(bDeskStation);

	m_PlayView.m_FlashPlayer[iView].StopPlay();
	m_PlayView.m_FlashPlayer[iView].Stop();
	m_PlayView.m_FlashPlayer[iView].ShowWindow(SW_HIDE);	

	return;
}



#ifdef _USE_VIDEO
/**
*	收到消息：客户端进入房间
*	@param wParam （INT）表示所进入房间的ID号
*	@param lParam （INT）表示是否进入房间：0成功进入，否则为出错代码
*/
LRESULT CClientGameDlg::OnGVClientEnterRoom(WPARAM wParam, LPARAM lParam)
{
	if(m_pBRGameVideoSDK)
	{
		CRect pRect;
		CString plogpath;
		if(m_PlayView.GetVideoRect(GetMeUserInfo()->nickName,pRect,plogpath))
			m_pBRGameVideoSDK->ShowUserVideo(-1,TRUE,pRect,FromHandle(m_PlayView.GetSafeHwnd()));
		m_pBRGameVideoSDK->OpenLocalCamera();
	}

	return 0;
}
/**
*	收到消息：用户进入（离开）房间
*	@param wParam （INT）表示用户ID号
*	@param lParam （BOOL）表示该用户是进入（TRUE）或离开（FALSE）房间
*/
LRESULT CClientGameDlg::OnGVClientUserAtRoom(WPARAM wParam, LPARAM lParam)
{
	if(!m_pBRGameVideoSDK)
		return 0;

	CString logstr;
	INT onlinenum = (INT)wParam;

	std::list<INT> userlist;
	std::list<INT>::iterator Iterator;
	m_pBRGameVideoSDK->GetRoomUserList(userlist);

	for(Iterator=userlist.begin(); Iterator!=userlist.end(); Iterator++)
	{
		INT userid = *Iterator;

		if(m_pBRGameVideoSDK)
		{
			std::map<std::string,int>::iterator iter = m_VideoUserInfo.find(std::string(m_pBRGameVideoSDK->GetUserNameById(userid)));
			if(iter != m_VideoUserInfo.end())
				(*iter).second = userid;
			else
				m_VideoUserInfo.insert(std::pair<std::string,int>(std::string(m_pBRGameVideoSDK->GetUserNameById(userid)),userid));

			CRect pRect;
			CString plogpath;
			if(m_PlayView.GetVideoRect(m_pBRGameVideoSDK->GetUserNameById(userid),pRect,plogpath))
				m_pBRGameVideoSDK->ShowUserVideo(userid,TRUE,pRect,FromHandle(m_PlayView.GetSafeHwnd()));
		}
		else			
			break;
	}

	return 0;
}

/**
*	收到消息：客户端登录系统
*	@param wParam （INT）表示自己的用户ID号
*	@param lParam （INT）表示登录结果：0 成功，否则为出错代码，参考出错代码定义
*/
LRESULT CClientGameDlg::OnGVClientLogin(WPARAM wParam, LPARAM lParam)
{
	if(GetMeUserInfo() == NULL) return 0;

	// 保存自己的userid
	std::map<std::string,int>::iterator iter = m_VideoUserInfo.find(std::string(GetMeUserInfo()->nickName));
	if(iter != m_VideoUserInfo.end())
		(*iter).second = (int)wParam;
	else
		m_VideoUserInfo.insert(std::pair<std::string,int>(std::string(GetMeUserInfo()->nickName),(int)wParam));

	return 0;
}

/**
*	收到当前房间的在线用户信息
*	进入房间后触发一次
*	@param wParam （INT）表示在线用户数（不包含自己）
*	@param lParam （INT）表示房间ID
*/
LRESULT CClientGameDlg::OnGVClientOnlineUser(WPARAM wParam, LPARAM lParam)
{
	if(!m_pBRGameVideoSDK)
		return 0;

	CString logstr;
	INT onlinenum = (INT)wParam;

	std::list<INT> userlist;
	std::list<INT>::iterator Iterator;
	m_pBRGameVideoSDK->GetRoomUserList(userlist);

	for(Iterator=userlist.begin(); Iterator!=userlist.end(); Iterator++)
	{
		INT userid = *Iterator;

		if(m_pBRGameVideoSDK)
		{
			std::map<std::string,int>::iterator iter = m_VideoUserInfo.find(std::string(m_pBRGameVideoSDK->GetUserNameById(userid)));
			if(iter != m_VideoUserInfo.end())
				(*iter).second = userid;
			else
				m_VideoUserInfo.insert(std::pair<std::string,int>(std::string(m_pBRGameVideoSDK->GetUserNameById(userid)),userid));

			CRect pRect;
			CString plogpath;
			if(m_PlayView.GetVideoRect(m_pBRGameVideoSDK->GetUserNameById(userid),pRect,plogpath))
				m_pBRGameVideoSDK->ShowUserVideo(userid,TRUE,pRect,FromHandle(m_PlayView.GetSafeHwnd()));
		}
		else				
			break;
	}

	return 0;
}
/**
* 刷新所有的视频面板
*/
void CClientGameDlg::RefreshAllVideoPanel(void)
{
	if(m_pBRGameVideoSDK == NULL ||
		m_VideoUserInfo.empty()) return;

	/// 为视频设置rgn(棋类专用)
	CRgn rgn;
	CRect rcWnd;
	m_PlayView.m_MeCard.GetClientRect(&rcWnd);
	rgn.CreateRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom);


	std::map<std::string,int>::iterator iter = m_VideoUserInfo.begin();
	for(;iter != m_VideoUserInfo.end();++iter)
	{
		CRect pRect;
		CString plogpath;


		if(m_PlayView.GetVideoRect(m_pBRGameVideoSDK->GetUserNameById((*iter).second),pRect,plogpath))
			m_pBRGameVideoSDK->ShowUserVideo((*iter).second,TRUE,pRect,FromHandle(m_PlayView.GetSafeHwnd()));

		/// 为视频设置rgn(棋类专用)
		m_PlayView.ClientToScreen(&pRect);
		m_PlayView.m_MeCard.ScreenToClient(&pRect);
		CRgn rgn2;
		rgn2.CreateRectRgn(pRect.left, pRect.top, 
			pRect.right, pRect.bottom);
		rgn.CombineRgn(&rgn, &rgn2, RGN_XOR);

	}

	/// 为视频设置rgn(棋类专用)
	m_PlayView.m_MeCard.SetWindowRgn((HRGN)rgn.GetSafeHandle(), TRUE);
}
#endif

void CClientGameDlg::FixControlStation(int iWidth, int iHeight)
{
	CLoveSendClassInExe::FixControlStation(iWidth,iHeight);

#ifdef _USE_VIDEO
	RefreshAllVideoPanel();
#endif
}