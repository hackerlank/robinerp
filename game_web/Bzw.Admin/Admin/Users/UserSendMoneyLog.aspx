﻿<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="UserSendMoneyLog.aspx.cs" Inherits="Admin_Users_UserSendMoneyLog" %>


<%@ Register Assembly="Pager" Namespace="Wuqi.Webdiyer" TagPrefix="webdiyer" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head id="Head1" runat="server">
    <title></title>
    <script src="/Public/Js/jquery.js"></script>

	<script src="/Public/Js/public.js"></script>

	
	<script src="/Public/Js/DatePicker/WdatePicker.js"></script>
	

	<style type="text/css">
		*{font-family:Arial,'宋体'; font-size:12px;}
	</style>
</head>
<body>
    <form id="form1" runat="server">
    <table border="1" cellpadding="3" style="width: 100%; border-collapse: collapse;">
			<caption style="font-weight:bold;">
				查看玩家的在线时长，局数赠送金币记录
			</caption>
			<tr>
				<td style="height: 29px">玩家帐号：<input type="text" id="txtKey" name="txtKey" maxlength="50" style="width: 99px" class="put" />
				&nbsp;游戏房间：<select name="tt" id="RoomType" style="width: 82px">
                        <option value="0" selected="selected" >不限</option>
                         <%=select%>
                    </select>
			&nbsp; 按时段：从
			&nbsp;<input type="text" readonly="readonly" id="txtBeginDate" name="txtBeginDate" onfocus="WdatePicker({dateFmt:'yyyy-MM-dd',maxDate:'#F{$dp.$D(\'txtEndDate\')}'});" size="8" class="put"/>
			&nbsp;到&nbsp;<input type="text" readonly="readonly" id="txtEndDate" name="txtEndDate" onfocus="WdatePicker({minDate:'#F{$dp.$D(\'txtBeginDate\')}',dateFmt:'yyyy-MM-dd'});" size="8" class="put" />
			 
			&nbsp;<input type="button" value="搜 索" onclick="OnSearch();" class="put" />
			&nbsp;&nbsp; &nbsp;

					<script type="text/javascript" >
							var bd = $("#txtBeginDate");  //变量和控件ID绑定  zhoushenghua
							var ed = $("#txtEndDate");
							var o = $("#txtKey");
							var pkey = "请输入玩家帐号";
							var Rtype=$("#RoomType");
							
							bd.val(decodeURIComponent(GetRequest("bd","")));//获取URL中传过来的值 zhoushenghua
							ed.val(decodeURIComponent(GetRequest("ed","")));
							o.val(decodeURIComponent(GetRequest("key",pkey)));
							Rtype.val(decodeURIComponent(GetRequest("rt","")));
							
							if(GetRequest("key","") == "")
							{
								o.css("color","gray");
							}
							else
							{
								o.css("color","black")
							}
							o.focus(function(){if(o.val().Trim() == pkey){o.css("color","black");o.val("");}});
							o.blur(function(){if(o.val().Trim() == ""){o.css("color","gray");o.val(pkey);}});
							
							//搜索按钮
							function OnSearch()
							{
							
								if(bd.val().Trim() != "" || ed.val().Trim() != "" || ( o.val().Trim() != pkey && o.val().Trim() != "" ))
								{
									var url = SetUrlParam(location.href,"key="+ ( ( o.val().Trim() != pkey && o.val().Trim() != "" ) ? encodeURIComponent(o.val().Trim()) : "" ) );
									url = SetUrlParam(url,"bd="+encodeURIComponent(bd.val().Trim()));
									url = SetUrlParam(url,"rt="+encodeURIComponent(Rtype.val().Trim()));
									LocationTo(SetUrlParam(url,"ed=" + encodeURIComponent(ed.val().Trim())));
									return;
								}
								alert("请先输入您要搜索的玩家帐号，或选择要查看的时段。");
							}
							</script>

				<input type="button" onclick="RefreshMe();" value="刷 新" /></td>
			</tr>
			<tr>
				<td>
					<table cellpadding="3" style="width: 100%; text-align: center; border-collapse: collapse; margin: 0px; padding: 0px;" border="1px;">
						<tr>
						 <th>记录ID</th>
							<th>
								玩家</th>
							<th>
								房间名</th> 
							<th>
								奖励金币数</th>
							 
                            <th>
								奖励时间</th>
							<th>
								奖励来源</th>
				 
						</tr>
						<asp:Repeater ID="rptDataList" runat="server">
							<ItemTemplate>
								<tr>
									<td>
										<%# Eval("ID")%>
									</td>
									<td>
										<%# Eval("UserName")%>
									</td>
									<td>
										<%# Eval("RoomName")%>
									</td>
									 

									<td><%# Eval("SendMoney")%></td>
								 
									<td>
										<%#Eval("SendDate")%>
									</td>
									<td>
										<%# GetSendMoneyTypeName( Convert.ToInt32(Eval("SendType")))%>
									</td>
                                     				
								</tr>
							</ItemTemplate>
						</asp:Repeater>
						<asp:Literal runat="server" ID="ltNonData" Text="<tr><td colspan='15'>没有记录...</td></tr>"></asp:Literal></table>
					<table width="100%">
						<tr>
							<td style="font-size: 12px; color: #0099ff; background-color: #e6feda;">
								<webdiyer:AspNetPager PageSize="20" runat="server" ID="anpPageIndex" CustomInfoHTML="第%CurrentPageIndex%页，共%PageCount%页，每页%PageSize%条，共%RecordCount%条" FirstPageText="首页" LastPageText="尾页" NextPageText="下一页" NumericButtonCount="5" PageIndexBoxType="DropDownList" PrevPageText="上一页" ShowCustomInfoSection="Left" ShowPageIndexBox="Always" SubmitButtonText="Go" TextAfterPageIndexBox="" TextBeforePageIndexBox="" AlwaysShow="True" PagingButtonSpacing="10px" UrlPaging="True">
								</webdiyer:AspNetPager>
							</td>
						</tr>
					</table>
				</td>
			</tr>
		</table>
    </form>
</body>
</html>
