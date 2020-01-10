const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>

<head>
	<meta name='viewport' content='width=device-width, initial-scale=1.0' />
	<meta charset='utf-8'>
</head>
<style>
	body {
		background-color: #F7F9FD;
	}

	.container {
		width: 20rem;
		margin-left: auto;
		margin-right: auto;
	}

	.center {
		text-align: center;
	}

	button {
		display: block;
		width: 100%;
		padding: 1rem 0rem;
		border: none;
		border-radius: 0.5rem;
		background-color: #DA5868;
		font-family: 'Montserrat', sans-serif;
		font-size: 11pt;
		letter-spacing: 0.05rem;
		text-align: center;
		font-weight: bold;
		text-decoration: none;
		outline: none;
		color: white;
	}

	button:active {
		background-color: #CD3341;
	}
</style>

<body>
	<div class="container">
		<h1 class="center">TEST PAGE</h1><br>
		<h1 class="center" id="state">---</h1><br>
		<button type="button" id="switch"> Toggle</button><br>
		<p class="center">Recieved data = <span id='rd'>---</span></p>
	</div>
</body>
<script>
	var Socket;

	function init() {
		Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
		Socket.onmessage = function(event) { processReceivedCommand(event); };
	}


	function processReceivedCommand(evt) {
		document.getElementById('rd').innerHTML = evt.data;
		if (evt.data === '0') {
			document.getElementById('state').innerHTML = 'O F F';
		}
		if (evt.data === '1') {
			document.getElementById('state').innerHTML = 'O N';
		}
	}


	document.getElementById('switch').addEventListener('click', buttonToggle);

	function buttonToggle() {
		var btn = document.getElementById('switch')
		sendText('t');
	}

	function sendText(data) {
		Socket.send(data);
	}


	window.onload = function(e) {
		init();
	}
</script>

</html>
)=====";
