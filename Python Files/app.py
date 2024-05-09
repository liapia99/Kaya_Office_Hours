from flask import Flask, render_template, request

app = Flask(__name__, static_folder='static')

@app.route('/')
def home():
    return render_template('home.html')

@app.route('/sent', methods=['GET'])
def message_sent():
    note = request.args.get('message')
    return render_template('message_sent.html')

if __name__ == '__main__':
    app.run(debug=True)
