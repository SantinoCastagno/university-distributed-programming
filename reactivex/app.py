from flask import Flask, render_template, request, send_from_directory
from flask_socketio import SocketIO, emit, join_room, leave_room, disconnect

app = Flask(__name__,
            static_url_path='', 
            static_folder='static',
            template_folder='templates')

app.config['SECRET_KEY'] = 'your_secret_key'
socketio = SocketIO(app)

# Dictionary to keep track of user information
users = {}

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/static/<path:path>')
def send_static(path):
    return send_from_directory('static', path)

@socketio.on('message')
def handle_message(message):
    sid = request.sid
    user_info = users.get(sid)  # Retrieve the user info
    if user_info:
        room = user_info['room']  # Retrieve the room the user is in
        if room:
            print(f'Message from {sid} to room {room}: {message}')
            emit('message', user_info['username'] + ": " + message, room=room)

@socketio.on('join')
def on_join(data):
    username = data['username']
    room = data['room']
    sid = request.sid
    print(f'User {username} with session ID {sid} joined room {room}')
    join_room(room)
    users[sid] = {'room': room, 'username': username}  # Store room and username info
    emit('status', {'msg': f'{username} has entered the room.'}, room=room)

@socketio.on('leave')
def on_leave(data):
    username = data['username']
    sid = request.sid
    user_info = users.pop(sid, None)  # Remove the user info for this session
    if user_info:
        room = user_info['room']
        print(f'-> User {username} with session ID {sid} left room {room}')
        leave_room(room)
        emit('status', {'msg': f'{username} has left the room.'}, room=room)

@socketio.on('disconnect')
def on_disconnect():
    sid = request.sid
    user_info = users.pop(sid, None)  # Clean up on disconnect
    if user_info:
        room = user_info['room']
        print(f'-> Session {sid} disconnected from room {room}')
        leave_room(room)
        emit('status', {'msg': f'A user has disconnected.'}, room=room)

if __name__ == '__main__':
    socketio.run(app, debug=True)