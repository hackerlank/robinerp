﻿<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="RoomMod.aspx.cs" Inherits="QPS.Web.BusiMan.RoomMod" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title></title>
</head>
    <link href="css/gruser.css" rel="stylesheet" type="text/css" />
<body>
    <form id="form1" runat="server">
    <div class="grboxa">
     <div class="grmiddle">
          <!--grright[[-->
          <div class="grright">
               <div class="grtitlea">
                    <span>创建房间</span>
               </div>
               <!--grbox[[-->
               <div class="grbox">
                    <div class="grxxa">
                    <ul>
                        <li><h3>棋牌室名：</h3><input type="text" name="textfield2" id="txfName" class="cja1" runat="server" /></li>
                        <li><h3>棋牌室类型：</h3><input type="text" name="textfield2" id="txfRoomType" class="cja1" runat="server"/></li>
                        <li><h3>棋牌室价格：</h3><input type="text" name="textfield2" id="txfRoomPrice" class="cja1" runat="server"/></li>
                        <li><h3>地区：</h3><input type="text" name="textfield2" id="txfArea" class="cja1" runat="server"/></li>
                         <li><h3>地址：</h3><input type="text" name="textfield2" id="txfAddress" class="cja1" runat="server"/></li>
                              <li><h3>图片：</h3>
                                  <asp:FileUpload ID="fulImg" runat="server"  cssClass="cja1" /></li>
                            <li><h3>内容：</h3><input type="text" name="textfield2" id="txfContent" class="cja1" 
                                    runat="server" maxlength="125"/></li>
                        <li>
                              <asp:Button ID="btnCreate" runat="server" Text="修改" onclick="btnCreate_Click" class="cjaaa"/></li>
                    </ul>
                        
               </div>
               </div>
               <!--grbox]]-->
          </div>
          <!--grright]]-->
     </div>
     </div>
    </form>
</body>
</html>
