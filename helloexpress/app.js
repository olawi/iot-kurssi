const express = require("express");
const process = require("child_process");

let app = express();

app.use(express.static("public"));

app.get("/hello", function(req, res) {
    //    return res.status(200).json({message:"Hello world!"});

    let proc = process.exec("./hello.out", function(error, stdout, stderr) {
	if(error) {
	    console.log("Failed to spawn child process: ", error);
	    return res.status(404).json({message:"Not found"});
	}
	if(stdout) {
	    return res.status(200).json({message:stdout});
	}
	return res.status(404).json({message:"Not found"});
    })
    console.log("Started process with PID:",proc.pid);
    
});

app.listen(3000);
console.log("Running in port 3000");
