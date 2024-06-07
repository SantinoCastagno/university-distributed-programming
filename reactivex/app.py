from flask import Flask, render_template
from flask_socketio import SocketIO, send
from rx import operators as ops
from rx.subject import Subject

app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret!'
socketio = SocketIO(app)


@socketio.on('message')
def handle_message(message):
 print("hola")
 send(msg, broadcast=True)

@app.route('/')
def index():
 return render_template('index.html')

if __name__ == '__main__':
    socketio.run(app, debug=True)
