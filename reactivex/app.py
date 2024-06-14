from flask import Flask, render_template, request
from flask_socketio import SocketIO, emit, join_room, leave_room, disconnect

app = Flask(__name__)
app.config['SECRET_KEY'] = 'your_secret_key'
socketio = SocketIO(app)

# Dictionary to keep track of which room each user (session ID) is in
user_rooms = {}

@app.route('/')
def index():
    return render_template('index.html')

@socketio.on('message')
def handle_message(message):
    sid = request.sid
    room = user_rooms.get(sid)  # Retrieve the room the user is in
    if room:
        print(f'Message from {sid} to room {room}: {message}')
        emit('message', message, room=room)

@socketio.on('join')
def on_join(data):
    username = data['username']
    room = data['room']
    sid = request.sid
    print(f'User {username} with session ID {sid} joined room {room}')
    join_room(room)
    user_rooms[sid] = room  # Store the room information for this session
    emit('status', {'msg': f'{username} has entered the room.'}, room=room)

@socketio.on('leave')
def on_leave(data):
    username = data['username']
    sid = request.sid
    room = user_rooms.pop(sid, None)  # Remove the room information for this session
    if room:
        print(f'-> User {username} with session ID {sid} left room {room}')
        leave_room(room)
        emit('status', {'msg': f'{username} has left the room.'}, room=room)

@socketio.on('disconnect')
def on_disconnect():
    sid = request.sid
    room = user_rooms.pop(sid, None)  # Clean up on disconnect
    if room:
        print(f'-> Session {sid} disconnected from room {room}')
        leave_room(room)
        emit('status', {'msg': f'A user has disconnected.'}, room=room)

if __name__ == '__main__':
    socketio.run(app, debug=True)
