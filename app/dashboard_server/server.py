from flask import Flask, request, jsonify

app = Flask(__name__)
data = []

@app.route('/api/weather', methods=['POST'])
def weather_post():
    content = request.get_data(as_text=True)
    data.append(content)
    return "OK"

@app.route('/api/weather', methods=['GET'])
def weather_get():
    return jsonify(data)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
