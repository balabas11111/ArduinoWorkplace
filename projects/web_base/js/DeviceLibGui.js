function createDivComponent(id,clazz,style,innerHtml){
	return createHtmlComponent('div',id,'',clazz,style,innerHtml);
}

function createButtonComponent(id,clazz,style,innerHtml,onclick){
	var btn=createHtmlComponent('button',id,undefined,clazz,style,innerHtml);
	btn.setAttribute('onclick',onclick);
	return btn;
}

function createHiddenInput(id,name,value,clazz){
	if(clazz==undefined){clazz='';}
	var input=createInputSimple(id,name,clazz,'',value,true);
	input.setAttribute('type','hidden');
	return input;
}

function createInputSimple(id,name,clazz,style,value,editable){
	var input1=document.createElement("Input");
	
	if(id!=undefined){ input1.setAttribute("id",id);}
	input1.setAttribute("name",name);
	if(clazz==undefined){ clazz="w3-input w3-border";}
	input1.setAttribute("class",clazz);
	if(style!=undefined && style!=''){ input1.setAttribute("style",style);}
	input1.setAttribute("value",value);
	
	if(editable==undefined || !editable)
		input1.setAttribute("disabled","disabled");
	
return input1; 
}

function createHtmlComponent(tag,id,name,clazz,style,innerHtml){
	var comp=document.createElement(tag);
	
	if(id!=undefined && id!=''){comp.setAttribute('id',id);}
	if(name!=undefined && name!=''){	comp.setAttribute('name',name); }
	if(clazz!=undefined && clazz!=''){	comp.setAttribute('class',clazz); }
	if(style!=undefined && style!=''){	comp.setAttribute('style',style); }
	if(innerHtml!=undefined && innerHtml!=''){	comp.innerHTML=innerHtml; }
		
	return comp;
}

function createTooltipSpan(msg){
	var innerHtml='<div class="w3-row"><div class="w3-cell"><h2>&#9888</h2></div><div class="w3-cell" ><strong style="margin-left: 20px">Справка</strong></div><div class="w3-cell-top">'+msg+'</div></div>';
	
	var tooltipSpan=createHtmlComponent('span','','',
			'w3-text w3-tag w3-white w3-border w3-border-gray w3-round-large',
			'position:absolute;left:0;bottom:60px;',innerHtml);
	
	return tooltipSpan;
}
/*----------------------------------------------------------------------------*/
function hideComponent(componentId){
	var comp=document.getElementById(componentId);
	setVisible(comp,false);
};

function showComponent(componentId){
	var comp=document.getElementById(componentId);
	setVisible(comp,true);
};

function isVisible(comp){
	return (comp!=undefined && comp.style!=undefined && comp.style.display=='block')
}

function setVisible(comp,visible){
	if(comp!=undefined){
		if(visible){
			comp.style.display = "block";
		}else{
			comp.style.display = "none";
		}
	}
}
/*---------------------Popups------*/

function closePopup(modalName,messageComponentName){
	hideComponent(modalName);
	setStatusMessageComp(getComponentById(messageComponentName));
}