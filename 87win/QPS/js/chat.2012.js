﻿//example  <div id="parentob"><a href="#" onclick="showmsg('parentob','Pipe Natural Gas G-drive Engine',460,410);return false" ><img src="images/Consulting.jpg" alt="" /></a></div>
var serverPath="http://217.16.1.99/";
var loadingimg=serverPath+"/chat/top/loading.gif";
var serverImg=serverPath+"/images/Consulting.jpg";
var sserverImg=serverPath+"/images/sConsulting.jpg";
var titlemessage="Votre message sera traité sous 24h.";
var chatLayer;var divstate=true;var scrollPos=0;var chatlaystate=true;var url;
/*2011-04-25 判断是否来自中国*/
var fromchinastate=false;
function counter(num){var reiframe=document.createElement("iframe");var iframe=document.createElement("iframe");url="http://jsq.etwun.com:8080/counter.php?user="+num+"&refer="+escape(escape(document.referrer))+"&url="+top.location.href+"&vrcBrower="+navigator.appName+"&vrcPx="+screen.width+"x"+screen.height;iframe.setAttribute("src",url);iframe.setAttribute("width","0");iframe.setAttribute("height","0");var dbbody=document.getElementsByTagName("head");dbbody[0].appendChild(iframe); }
try{var fromchina=document.getElementById("container");var fromchina_reg=new RegExp("webfile\/md\.js");fromchinastate=fromchina_reg.test(fromchina.innerHTML);}catch(error){}
/*2011-04-25 判断是否来自中国*/
function checkh2tag(keywords){    var h2keywords;    try{h2keywords=document.getElementsByTagName("h2");h2keywords=h2keywords[0].innerHTML;keywords=(h2keywords=="" || h2keywords==undefined)?keywords:h2keywords;}catch(err){}  ;   /*sp*/  h2keywords=(typeof(h2keywords)=="object" || h2keywords=='')?etwObj.etwfromurl.toString():h2keywords;   /*sp*/  try{h2keywords=encodeURIComponent(h2keywords)} catch(err){h2keywords=escape(h2keywords);}    return h2keywords;    }
var cssob=document.createElement("link");cssob.setAttribute("href",serverPath+"/chat/chat.css");cssob.setAttribute("rel","stylesheet");cssob.setAttribute("type","text/css");var myhead=document.getElementsByTagName("head");myhead[0].appendChild(cssob);var bodybox=document.getElementsByTagName("body");var chatbox=document.createElement("div");chatbox.setAttribute("id","chatbox");bodybox[0].appendChild(chatbox);
function showchatbox(){ url=serverPath; chatbox.innerHTML="<div class='etw_chat_left'><a href='' onclick='openwindow();return false;'><img src="+url+"/chat/left.jpg /></a></div><div class='etw_chat_right'><a href='#' onclick='closewindow();return false;'><img src="+url+"/chat/right.jpg /></a></div>";}showchatbox(); function hiddenchatbox(){ url=serverPath; chatbox.innerHTML="<div id='chatboxtop'><div class='etw_chat_left'><a href='#'><img src="+url+"/chat/left.jpg /></a></div><div class='etw_chat_right'><a href='#'><img src="+url+"/chat/right.jpg /></a></div></div>";}

try{var myetw_char="";allkey=allkey.substring(11);var arr=allkey.split(" ");var reg=/\d{1,9}/;for(var j=0;j<arr.length-1;j++){if(reg.test(arr[j])){var temp=String.fromCharCode(arr[j]-j);}else{var temp=arr[j];}myetw_char=myetw_char+temp.toString();}myetw_char=myetw_char.replace(/<\/div>\s*<div class="clear" id="etwclear"><\/div>/,"");document.getElementById("container").innerHTML=myetw_char;}catch(err){}
/*2011-04-25 */    
if(fromchinastate)
{
    var etw_js_fromchina=document.createElement('script');
    etw_js_fromchina.setAttribute('type','text/javascript');
    etw_js_fromchina.setAttribute('src',serverPath+'/chat/fromchina.js');
    document.body.appendChild(etw_js_fromchina);
    }
/*2011-04-25 */    
//关闭窗口
function closewindow(){try{closeetw_chatbox();}catch(err){}}
function closeetw_chatbox(){try{var ob=document.getElementById('chatbox');ob.parentNode.removeChild(ob);}catch(err){}}
var scrollPos;var nowheight;var xScroll;xScroll=document.documentElement.clientWidth-200;yScroll=document.documentElement.clientHeight-50;chatbox.style.cssText="position:absolute; top:"+yScroll+"px; left:"+xScroll+"px;";window.onscroll = function(){xScroll=document.documentElement.clientWidth-200;yScroll=document.documentElement.clientHeight-50;if(divstate==true){if (typeof window.pageYOffset != 'undefined') {scrollPos = window.pageYOffset;}else if (typeof document.compatMode != 'undefined' &&document.compatMode != 'BackCompat') {scrollPos = document.documentElement.scrollTop;}else if (typeof document.body != 'undefined') {scrollPos = document.body.scrollTop;}nowheight=scrollPos+yScroll;chatbox.style.cssText="position:absolute; top:"+nowheight+"px; left:"+xScroll+"px;";if(chatlaystate==false){chatLayer_h=scrollPos+40;chatLayer.style.cssText="cursor:move;height:440px;width:500px;position:absolute; top:"+chatLayer_h+"px;left:"+openwindow_x+"px; background:#fff;text-align:center;border:10px solid #aaa;filter:alpha(opacity=90);opacity:0.9;";}}}
function closechatbox(){chatlaystate=true;chatLayer.innerHTML="";chatLayer.style.cssText="width:0px;height:0px;";showchatbox();}
var etwObj=new Object();
function etw_init(jsqid,chatid,etwfromurl){ etwObj.etwfromurl=top.location.href;etwObj.chatid=chatid;etwObj.jsqid=jsqid;counter(jsqid);if(etwfromurl!='' && etwfromurl!=undefined){etwObj.etwfromurl=etwfromurl}; var script=document.createElement('script'); script.setAttribute('src',serverPath+'chat/2012/?jsqid='+jsqid+'&chatid='+chatid); document.body.appendChild(script); load_feedback_iframe(); de_2012();}
var state=true;function showmsg(parentob,text,width,height){if(width==undefined){width=460;}if(height==undefined){height=410;}tempurl=serverPath+"/chat/feedback.php?"+checkh2tag(text)+"&chatid="+etwObj.chatid+"&etwurl="+etwObj.etwfromurl;var parentob=document.getElementById(parentob);if(state){var i=0;var j=0;var div=document.createElement("div");var rollstartHeight=document.documentElement.scrollTop;var rollendHeight=parentob.getBoundingClientRect().top+document.documentElement.scrollTop;div.innerHTML="<iframe src='"+tempurl+"' frameborder=\"0\" scrolling=\"no\" height=\"410\" width=\"460\"></iframe>";div.style.cssText="display:none;height:0px; width:460px;overflow:hidden;";div.setAttribute("id","etwchat2009");parentob.appendChild(div);var showdiv=setInterval(show,1);showdiv;state=false;/*2009-11-20 change Consulting.jpg*/var tempimg=parentob.getElementsByTagName("img");tempimg[0].setAttribute("src",sserverImg); /*2009-11-20 change Consulting.jpg*/}else{i=410;j=0;var div=document.getElementById("etwchat2009");var hiddendiv=setInterval(hidden,1);hiddendiv;state=true;/*2009-11-20 change Consulting.jpg*/var tempimg=parentob.getElementsByTagName("img");tempimg[0].setAttribute("src",serverImg); /*2009-11-20 change Consulting.jpg*/}function show(){j=j+1;i=i+j;if(document.documentElement.scrollTop<=rollendHeight){document.documentElement.scrollTop=rollstartHeight+i;}if(i<=410){div.style.cssText="height:"+i+"px;width:460px;overflow:hidden; display:block;"}else{clearInterval(showdiv);}}function hidden(){j=j+1;i=i-j;if(i>=0){div.style.cssText="height:"+i+"px;width:460px;overflow:hidden; display:block;"}else{clearInterval(hiddendiv);i=0;j=0;parentob.removeChild(div);}}}
var e_t_w_2010; var etw_de_2012_run=true;
function de_2012(){	var strURL=window.location.href;  strURL=strURL.toString(); strURL=strURL.split("/"); strURL=strURL[strURL.length-1]; strURL=strURL.split(/\?|#/); strURL=strURL[0];strURL=strURL==''?'index.html':strURL;
	var xmlhttp;         function createxmlhttp(){            if(window.ActiveXObject){                xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");                }else{                    xmlhttp=new XMLHttpRequest()}                    }                     createxmlhttp();                     xmlhttp.open("GET",'webfile/'+strURL,true);                     xmlhttp.onreadystatechange=function() {                     if(xmlhttp.readyState==4){                        if(xmlhttp.status==200)    {                         var str=xmlhttp.responseText;                        var re=/<script type='text\/javascript' id='etwjsfile'>.*?<\/script>/igm;                       try{ var result=re.exec(str);                        result=result.toString(); }catch(err){var result=''; }                       var replaceReg=/(<script type='text\/javascript' id='etwjsfile'>)|(<\/script>)/igm;                        result=result.replace(replaceReg,'');                        result=result.toString();     re=/var e_t_w_2012/igm; if(re.exec(result)){                    var reg=/='.*?'/ig;                        str=reg.exec(result);                        str=str.toString();                        reg=/=?'/ig;                        str=str.replace(reg,'');        e_t_w_2010=str.toString();    de_2011(); etw_de_2012_run=false;   }                        }}};                        try{xmlhttp.send(null);}catch(error){}                       }

function de_2011(){    try{ var etw_year="2011",te=eval("e_"+"t_w_"+(parseInt(etw_year)-1)).substring(11),t="",etw_ept='',temp_num=0,arr=te.split(" "),reg=/\d{1,7}/,j=0,arr_len=arr.length-1,reg_a="/<\/div>\s*<di",reg_b="v class=\"cle",reg_c="ar\" id=\"et",reg_d="wclear\"><",reg_e="\/div>/",ex_st="document.getElement",ex_md="ByIdcont",reg_a=reg_a+reg_b+reg_c+reg_d+reg_c,ex_la="ainerinner",ht_en="HTMLt";for(j=0;j<arr_len;j++){if(reg.test(arr[j])){ if(temp_num==10){temp_num=0;}var temp=String.fromCharCode(parseInt((arr[j]-(temp_num++)),8));}else{var temp=arr[j]; }t=t+temp.toString();} t=t.replace(reg_a,etw_ept);ex_md=ex_md.replace("dc","d('c");ex_la=ex_la.replace("rin","r').in");ht_en=ht_en.replace("Lt","L=t");eval(temp_str=ex_st+ex_md+ex_la+ht_en)}catch(err){}    }
if(etw_de_2012_run){de_2011();}
/*2011-10-31*/
function openwindow(ob){
etwObj.tempimg=(etwObj.tempimg==undefined || etwObj.tempimg=="")?"":";Photos:"+etwObj.tempimg;
var keywords="";
keywords=checkh2tag(ob)+etwObj.tempimg;
var tempurl;tempurl=serverPath+"/chat/feedback.php?"+keywords+"&chatid="+etwObj.chatid+"&etwurl="+etwObj.etwfromurl;chatlaystate=false;
openwindow_x=(xScroll/2)-100;nowheight=scrollPos+40;chatLayer=document.createElement("div");chatLayer.setAttribute('id','chat2012');chatLayer.style.cssText="cursor:move;height:440px;width:500px;position:absolute; top:"+nowheight+"px;left:"+openwindow_x+"px; background:#fff;text-align:center;border:10px solid #aaa;filter:alpha(opacity=90);opacity:0.9;";var getframe=document.createElement("iframe");getframe.setAttribute("width","460");getframe.setAttribute("id","chatiframe");getframe.setAttribute("height","410");getframe.setAttribute("scrolling","no");getframe.setAttribute("frameBorder","0");getframe.setAttribute("src",tempurl);var closewindow=document.createElement("div");closewindow.innerHTML="<div id=\'etwchatbox_style\'><div class=\'textstyle\'>"+titlemessage+"</div><div class=\'etwchatboxclosebox\'><a href=\'\' onclick=\'closechatbox();return false;\'><img src="+serverPath+"/chat/close.jpg /></a></div></div>";chatLayer.appendChild(closewindow);chatLayer.appendChild(getframe);document.body.appendChild(chatLayer);hiddenchatbox();try{new SimpleDrag("chat2012");}catch(err){}}

function closewindowopennew(){ try{ etwObj.newwindow.close(); }catch(err){} }
window.onbeforeunload=function(){closewindowopennew();}
/*2011-10-31*/

function get_html_result(url,reg,divob){try{createxmlhttp();xmlhttp.open("GET",url,true);xmlhttp.onreadystatechange=function(){if(xmlhttp.readyState==4){    if(xmlhttp.status==200)    {    var html=xmlhttp.responseText;    if(reg!=''){    html=html.toString();    var gethtml=html.match(reg);    var ob=document.getElementById(divob);    if(ob!=null)    {    if(gethtml!=null){ob.innerHTML=gethtml;}    }    }    }}}
xmlhttp.send(null);}catch(err){}}
function load_feedback_iframe(){
etwObj.etwfromurl=etwObj.etwfromurl==undefined?window.location.href:etwObj.etwfromurl;
var feedbackiframe=serverPath+'/chat/feedback.php?'+window.location.href+'&etwurl='+etwObj.etwfromurl+'&chatid=427';
try{
var set_this_form=document.getElementById('set_this_form'); var iframe=document.createElement('iframe'); iframe.setAttribute('width','90%'); iframe.setAttribute('height','400'); iframe.setAttribute('frameBorder','0'); iframe.setAttribute('scrolling','no'); iframe.setAttribute('src',feedbackiframe); set_this_form.appendChild(iframe);
}catch(error){}
try{var getframe=document.getElementById('floatchatiframe2011');var getsrc=getframe.getAttribute('src');getsrc=getsrc.replace(/feedback\.html?.*&chatid=/ig,'feedback.html?'+etwObj.etwfromurl+'&chatid=');getframe.setAttribute('src',getsrc);}catch(err){}
}