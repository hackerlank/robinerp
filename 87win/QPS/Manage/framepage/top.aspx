﻿<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="top.aspx.cs" Inherits="QPS.Web.Manage.framepage.top" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title> ASP.net|论坛 管理后台 - Discuz!NT - Powered by Discuz! NT</title>
    <meta name="keywords" content="ASP.net,论坛" />
    <meta name="description" content="Discuz!NT,论坛,asp.net" />
    <link href="../styles/dntmanager.css" type="text/css" rel="stylesheet" />    
<%--    <link href="../styles/modelpopup.css" type="text/css" rel="stylesheet" />
    <script type="text/javascript" src="../js/modalpopup.js"></script>--%>
    <script type="text/javascript" src="../xml/navmenu.js"></script>
    <style type="text/css">
	    body {margin:0;}
    </style>
<meta http-equiv="X-UA-Compatible" content="IE=7" />
</head>

<body>
	<div id="MenuTab">
		<ul id="MainMenu">
		    <li class="MenuDrop">
		        <div class="MenuDropOut" onmouseover="this.className='MenuDropOn'" onmouseout="this.className='MenuDropOut'">
		            <a href="javascript:void(0);" onmouseover="javascript:top.mainFrame.setshorcutmenu('block');" onmouseout="javascript:top.mainFrame.setshorcutmenu('none');" >快捷操作</a>
		        </div>
		    </li>
		</ul>		
	
	</div>
	<div id="setting" style="display:none;">
	</div>
</body>

</html>

<script type="text/javascript">
    var menuLength = toptabmenu.length;
    function LoadMainMenu() {
        var menuText = "";
        for (var i = 0; i < toptabmenu.length; i++) {
            var mod = (i % menuLength) + 1;
            menuText += "<li id='NtTab" + (i + 1) + "' ><div id='NtDiv" + (i + 1) + "'  Class='Currenttab" + mod + "'>";
            menuText += "<a href='#' id='NtA" + (i + 1) + "' class='CurrentHoverTab" + mod + "'";
            menuText += " onclick=\"javscript:locationurl('" + toptabmenu[i]["mainmenulist"].split(",")[0] + "','" + toptabmenu[i]["id"] + "','" + toptabmenu[i]["mainmenulist"];
            menuText += "','" + toptabmenu[i]["defaulturl"] + "');\" onfocus='this.blur();'>" + toptabmenu[i]["title"] + "</a></div></li>";
        }
        document.getElementById("MainMenu").innerHTML += menuText;
    }
    LoadMainMenu();


    function locationurl(showmenuid, toptabmenuid, mainmenulist, defaulturl) {
        var menucount = toptabmenu.length;
        j = menucount;

        /*得新进行实始化设置*/
        for (i = 1; i <= menucount; i++) {
            document.getElementById("NtDiv" + i).className = "tab" + (i % menuLength);
            document.getElementById("NtTab" + i).style.zIndex = j;
            document.getElementById("NtA" + i).className = "";
            j--;
        }


        /*设置当前点中的菜单项样式*/
        document.getElementById("NtA" + toptabmenuid).className = 'CurrentHoverTab' + (toptabmenuid % menuLength);
        document.getElementById('NtDiv' + toptabmenuid).className = 'Currenttab' + (toptabmenuid % menuLength);
        document.getElementById('NtTab' + toptabmenuid).style.zIndex = menucount + 1;

        //alert(defaulturl);
        top.mainFrame.location.href = '../framepage/managerbody.aspx?showmenuid=' + showmenuid + '&toptabmenuid=' + toptabmenuid + '&mainmenulist=' + mainmenulist + '&defaulturl=' + defaulturl;

    }

    //document.getElementById("searchtype").value='function';
    locationurl(toptabmenu[0]["mainmenulist"].split(",")[0], toptabmenu[0]["id"], toptabmenu[0]["mainmenulist"], toptabmenu[0]["defaulturl"]);
</script>