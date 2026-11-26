"use strict";

// formats number as at least 2 digits
function pad(n) {
	return String(n).padStart(2, "0");
}

// writes both scores onto the page
function setScores(red, blue) {
	document.getElementById("red").textContent  = pad(red);
	document.getElementById("blue").textContent = pad(blue);
}

// opens a server sent events connection to the node server
var es = new EventSource("/events");

// runs when server pushes a msg
es.onmessage = function(e) {
	var s = JSON.parse(e.data);
	setScores(s.red, s.blue);
};