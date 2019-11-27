const express = require("express");
const fs = require("fs");

let app = express();

app.use(express.static("public"));

app.get("/led", function(req, res) {
    fs.writeFile("/sys/class/ledclass/led01/led_attr","8==D", function(err) {
        if(err) {
	    return res.status(404).json({message:"Failed to handle led"});
	    console.log(err);
	    throw err;
        }
        return res.status(200).json({message:"Sukkess"});
    })
});

app.listen(3000);
console.log("Running in port 3000");
