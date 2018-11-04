function hideComponent(componentId){
	document.getElementById(componentId).style.display = "none";
};

function showComponent(componentId){
	document.getElementById(componentId).style.display = "block";
};

function processLampJson(data){
	
	var compId_Q=data.name+'_Q';
	var compId_On=data.name+'_On';
	var compId_Off=data.name+'_Off';
	
	var onOff=data.val;
	
	hideComponent(compId_Q);
	
	if(onOff=='1.00' || onOff=='1'){
		hideComponent(compId_Off);
		showComponent(compId_On);
	}else{
		hideComponent(compId_On);
		showComponent(compId_Off);
	}
}

function processSimpleJson(data){
	var items=data.items;
	for(var i in items){
		var name=items[i].name;
		var val=items[i].val;
		
		var component=document.getElementById(name);
		
		if(component!=undefined){
			var tagName = component.tagName.toLowerCase();
			
			if (tagName == 'h2' || tagName == 'h4'){
				component.innerHTML=val;
			};
			
			if (tagName == 'input'){
				component.value=val;
			};
			
			if (tagName == 'a'){
				component.href=val;
			};
		}
	}
}

function processSettingsJson(data){
	var items=data.items;
	for(var i in items){
		var name=items[i].name;
		var val=items[i].val;
		
		var component=document.getElementById(name);
		
		if(component!=undefined){
			
			showComponent(component.id);
			var tagName = component.tagName.toLowerCase();
			
			if (tagName == 'h2' || tagName == 'h4'){
				component.innerHTML=val;
			};
			
			if (tagName == 'input'){
				component.value=val;
			};
			
			if (tagName == 'a'){
				component.href=val;
			};
		}
	}
}

//------------------------------DS18D20--------------------------------
function processDS18D20Json(data){
	var items=data.items;
	var parentId=data.name;
	var childCount=data.childCount;
	
	var countCompId="Count_"+parentId;
	var countComponent=document.getElementById(countCompId);
	
	if(countComponent!=undefined){
		countComponent.innerHTML=childCount;
	}
		
	for(var i in items){
		var inputId=items[i].name;
		var labelVal=items[i].descr;
		var inputVal=items[i].val;
		
		addDS18D20Component(parentId,inputId,labelVal,inputVal);
	}
}

function addDS18D20Component(parentId,inputId,labelVal,inputVal){
		
	var newLabelId="Label_"+inputId;
	var newInputId="Input_"+inputId;
	var newLabelHtml="<b>"+labelVal+"</b>"
	//<label style="max-width: 240px;" for="Temperature"><b id="temp1"></b></label>		
	//<input id="Temperature"	class="" ="" type="text" value="��������" disabled>	
	var inputComponent=document.getElementById(newInputId);
	var labelComponent=document.getElementById(newLabelId);
	
	if(inputComponent!=undefined && labelComponent!=undefined){
		inputComponent.value=inputVal;
		labelComponent.innerHTML=newLabelHtml;
		return;
	}
	
	var container=document.getElementById(parentId);
	
	if(container!=undefined){
		
		var newLabel = document.createElement("Label");
		newLabel.setAttribute("id",newLabelId);
		newLabel.setAttribute("for",newInputId);
		newLabel.setAttribute("style","max-width: 240px;");
		newLabel.innerHTML = newLabelHtml ;
		
		var newInput=document.createElement("Input");
		newInput.setAttribute("id",newInputId);
		newInput.setAttribute("class","w3-input w3-border");
		newInput.setAttribute("style","width:30%");
		newInput.setAttribute("disabled","disabled");
		newInput.setAttribute("value",inputVal);
		
		container.appendChild(newLabel);
		container.appendChild(newInput);
	}
	
}

function updateComponentsByAjaxCall(requestmethod, url, handler, val, timeout){
	
	var request = new XMLHttpRequest();
	
	var formData = new FormData();
	formData.append('val', val);
	
	request.open(requestmethod, url, true);
	request.onreadystatechange  = 
		function() {
			if(this.readyState == 4){
								
				if (this.status == 200){
					
					var json = JSON.parse(this.responseText);
							
					handler(json);
									
					if(timeout>0){
						addPostponedUpdateComponentsByAjaxCall(requestmethod, url, handler, val, timeout);
					}
					
				}else{
					//
					addPostponedUpdateComponentsByAjaxCall(requestmethod, url, handler, val, timeout);
				}
			};
		};
	request.send(formData);
};

function addPostponedUpdateComponentsByAjaxCall(requestmethod, url, handler, val, timeout){
	if(timeout!='' && timeout!=0 && timeout>2000){
		setTimeout(function(){updateComponentsByAjaxCall(requestmethod, url, handler, val, timeout);}, timeout);
	}
};






