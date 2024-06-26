var socket = io();
var currentRoom = "";

function joinChat() {
	var username = document.getElementById("username").value;
	var room = document.getElementById("room").value;

	if (username && room) {
		currentRoom = room;
		socket.emit("join", { username: username, room: room });

		document.getElementById("join_chat_section").style.display = "none";
		document.getElementById("chat").style.display = "block";
		document.getElementById("chat_room_title").innerText += " " + room;
	}
}

function leaveChat() {
	var username = document.getElementById("username").value;
	if (currentRoom) {
		socket.emit("leave", { username: username, room: currentRoom });
		currentRoom = "";
		document.getElementById("chat").style.display = "none";
		document.getElementById("joinChat").style.display = "block";
	}
}

function sendMessage() {
	var messageInput = document.getElementById("messageInput");
	var message = messageInput.value;
	if (message) {
		socket.send(message);
		messageInput.value = "";
	}
}

socket.on("message", function (msg) {
	var messages = document.getElementById("messages");
	var name = msg.split(":")[0];
	var message = msg.split(":")[1];
	var li = document.createElement("li");
	li.innerHTML = `
                <div class = "message" style="background-color: #18122B; border-radius: 5px; padding: 0 10px 0 10px;">
                    <p style = "font-weight: bold; padding: 5px; margin: 0;">${name}</p>
                    <p style = "color:darkgray; padding: 5px; margin: 0;">${message}</p>
                </div>`;
	messages.appendChild(li);
});

socket.on("status", function (data) {
	var messages = document.getElementById("messages");
	var li = document.createElement("li");
	li.style.color = "gray";
	li.style.fontStyle = "italic";
	li.appendChild(document.createTextNode(data.msg));
	messages.appendChild(li);
});
