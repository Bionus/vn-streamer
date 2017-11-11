<img src="resources/readme-icon.png" align="right" />

# VN Streamer

[![GitHub release](https://img.shields.io/github/release/Bionus/vn-streamer.svg)](https://github.com/Bionus/vn-streamer/releases/latest)
[![GitHub downloads](https://img.shields.io/github/downloads/Bionus/vn-streamer/latest/total.svg)](https://github.com/Bionus/vn-streamer/releases/latest)
[![GitHub downloads](https://img.shields.io/github/downloads/Bionus/vn-streamer/total.svg)](https://github.com/Bionus/vn-streamer/releases)
[![GitHub issues](https://img.shields.io/github/issues/Bionus/vn-streamer.svg)](https://github.com/Bionus/vn-streamer/issues)
[![Project license](https://img.shields.io/github/license/Bionus/vn-streamer.svg)](https://raw.githubusercontent.com/Bionus/vn-streamer/develop/LICENSE)

Visual novel streamer that you can run on a computer to then be able to play remotely. Simply run the server program on the computer, link it to a visual novel, and you're ready to play on the go!

The server only works on Windows at the moment, due to the heavy usage of the WinApi.

As for the client, the HTML5 one uses WebSockets and can therefore work on any browser that supports them, on desktop or mobile.

### Download
You can download the latest release [here](https://github.com/Bionus/vn-streamer/releases/latest), or find all previous releases [here](https://github.com/Bionus/vn-streamer/releases).

### Languages
* English

## How to use

### Server
1. Run the visual novel on your computer
2. Run a text hooker that outputs the text in your clipboard
3. If you want machine translations, run Translation Aggregator (optional)
4. Create a profile for your visual novel (optional, recommended)
5. Run the server
6. Select your visual novel in the "Application" combobox
7. Select the text source you want to use in the "Text source" combobox
8. Click the "Start" button

_**Note:** you may need to open the server port in your router in order to be able to access the server from outside your local network. To do so, simply redirect the TCP port 46421 (by default) to your computer._

### HTML5 client
1. Open the client in your favorite browser
2. Type your server's IP and port
3. Click the "Connect" button