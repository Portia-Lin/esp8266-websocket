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
		width: 10rem;
		height: 10rem;
		border: none;
		border-radius: 25%;
		margin-left: auto;
		margin-right: auto;
		background-color: #DBE1EA;
		outline: none;
	}

	button:active {
		background-color: #E9E9E6;
	}
 
  #circl {
    transition: stroke 1s ease;
  }
</style>

<body>
	<div class="container">
		<h1 class="center">TEST PAGE</h1>
		<h1 class="center" id="state">---</h1> <br> <br>
		<button type="button" id="switch">
			<svg width="9rem" height="9rem">
				<circle id="circl" r="4rem" cx="4.5rem" cy="4.5rem" fill="none" stroke="#000000" stroke-width="0.2rem" />
			</svg>
		</button> <br> <br>
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
			document.getElementById('circl').style.stroke = '#F583AE';
		}
		if (evt.data === '1') {
			document.getElementById('state').innerHTML = 'O N';
			document.getElementById('circl').style.stroke = '#2BAAFF';
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
