window.onload = function() {
	var status = document.getElementById('connection-status-text');
	status.innerHTML = 'Pending';
	var connection = new WebSocket('ws://localhost:46421');
	connection.onopen = function() {
		status.innerHTML = 'Connected';
		connection.send('0 0 0');
	};
	connection.onclose = function() {
		status.innerHTML = 'Closed';
	};
	connection.onerror = function(event) {
		status.innerHTML = 'Error';
		console.log('WebSocket error', event);
	};

	function stringToColour(str) {
		var hash = 0;
		for (var i = 0; i < str.length; i++) {
			hash = str.charCodeAt(i) + ((hash << 5) - hash);
		}
		var colour = '#';
		for (var i = 0; i < 3; i++) {
			var value = (hash >> (i * 8)) & 0xFF;
			colour += ('00' + value.toString(16)).substr(-2);
		}
		return colour;
	}

	var nextBlob = '';
	function commandReceived(command) {
		nextBlob = '';
		var arr = command.split('|');
		if (arr[0] == 'text') {
			textReceived(arr[1], arr[2]);
		} else if (arr[0] == 'image') {
			nextBlob = 'image';
		}
	}
	function textReceived(name, text) {
		var character = document.getElementById('character');
		character.innerHTML = name;
		if (name.length > 0) {
			character.style.color = stringToColour(name);
		}
		document.getElementById('text').innerHTML = text;
	}
	function imageReceived(image) {
		if (nextBlob == 'image' || nextBlob == '') {
			document.getElementById('image').src = image;
			nextBlob = '';
		}
	}

	connection.onmessage = function(event) {
		if (event.data instanceof Blob) {
			reader = new FileReader();
			reader.onload = function() {
				if (reader.result != null) {
					var arr = reader.result.split(',');
					var base64 = 'data:image/jpeg;base64,' + arr[arr.length - 1];
					imageReceived(base64);
				}
			};
			reader.readAsDataURL(event.data);
		} else {
			commandReceived(event.data);
		}
	};

	document.getElementById('control-next').onclick = function() {
		connection.send('1');
	};
};
