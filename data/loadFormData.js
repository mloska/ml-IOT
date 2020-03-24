function loadFormData(param) {

  var xhttp;
  xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      
	document.getElementsByName(param)[0].value = this.responseText;
    }
  };
  xhttp.open("GET", "loadForm?param="+param, true);
  xhttp.send();
}
