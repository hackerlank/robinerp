﻿<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="Regulation.aspx.cs" Inherits="QPS.Web.Regulation" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title></title>
</head>
<body>
    <form id="form1" runat="server">
    <div class="nymiddle">
 <div class="gywm">
   <asp:Repeater ID="Repeater1" runat="server">
          <ItemTemplate>
             <%# Eval("DContent").ToString()%>
                  </ItemTemplate>
     </asp:Repeater>
   </div>
    </div>
    </form>
</body>
</html>
