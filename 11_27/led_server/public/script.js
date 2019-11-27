const getHello = () => {
    let request = {
	method:"GET",
	mode:"cors",
	headers: {
	    "Content-type":"application/json"
	}
    }
    fetch("/led",request).then((response) => {
	if(response.ok) {
	    response.json().then((data) => {
		document.getElementById("message").innerText = data.message;
	    }).catch((error) => {
		console.lot("error");
	    });
	} else {
	    document.getElementById("message").innerText = "Server responded with status:"+response.status;
	}
    }).catch((error) => {
	console.log(error);
    });
}
    
