"use strict";

// Install:  npm install mime@1
// Run:      node server.js

var http = require('http');
var fs   = require('fs');
var path = require('path');
var dgram = require('dgram');
var mime = require('mime');

var HTTP_PORT = 8082; // browser looks up this
var UDP_PORT  = 12345; // c code sends to this

var score = { red: 0, blue: 0 }; // latest score
var clients = []; 


// ---------- Static files ---------- //

// create HTTP server
var server = http.createServer(function(request, response) {
	if (request.url == '/events') {
		response.writeHead(200, {
			'Content-Type': 'text/event-stream',
			'Cache-Control': 'no-cache',
			'Connection': 'keep-alive'
		});
		response.write('data: ' + JSON.stringify(score) + '\n\n'); // current score right away
		clients.push(response);
		request.on('close', function() {
			clients = clients.filter(function(c) { return c !== response; });
		});
		return;
	}

	var filePath = (request.url == '/') ? 'public/index.html' : 'public' + request.url;
	serveStatic(response, './' + filePath);
});

// listen for browsers to connect
server.listen(HTTP_PORT, function() {
	console.log("HTTP server on port " + HTTP_PORT);
});


// reads file from disk and sends it to the browser
function serveStatic(response, absPath) {
	fs.readFile(absPath, function(err, data) {
		if (err) {
			response.writeHead(404, {'Content-Type': 'text/plain'});
			response.end('Error 404: resource not found.');
		} else {
			response.writeHead(200, {'content-type': mime.lookup(path.basename(absPath))});
			response.end(data);
		}
	});
}


// ---------- UDP ---------- //

// create an ipv4 socket to received UDP packets from C app
var udp = dgram.createSocket('udp4');

// runs when packet arrives and parses message to get red and blue scores
// saves new score and logs to terminal
udp.on('message', function(msg) {
	var parts = msg.toString().trim().split(',');
	var red = parseInt(parts[0], 10);
	var blue = parseInt(parts[1], 10);
	if (isNaN(red) || isNaN(blue)) {
		console.log("Ignoring bad message: " + msg);
		return;
	}
	score = { red: red, blue: blue };
	console.log("Score: " + red + " - " + blue);
	clients.forEach(function(c) {
		c.write('data: ' + JSON.stringify(score) + '\n\n');
	});
});

// listens for UDP packets on the agreed upon port
udp.bind(UDP_PORT, function() {
	console.log("UDP listening on port " + UDP_PORT);
});