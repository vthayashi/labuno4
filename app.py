# python app.py
# https://github.com/arshadansari27/flask-image-server/blob/master/app.py
#http://zetcode.com/python/flask/

import serial

from flask import Flask, send_file
from flask import request

ser = serial.Serial('COM7', baudrate=500000, timeout=10)
ser.set_buffer_size(rx_size = 25600, tx_size = 12800)

app = Flask(__name__)

@app.route('/')
def hello():
    filename = 'LabUno4.htm'
    return send_file(filename, mimetype='text/html')

@app.route('/LabUno4a.htm')
def LabUno4a():
    filename = 'LabUno4a.htm'
    return send_file(filename, mimetype='text/html')

@app.route("/LabUno4.jpg")
def LabUno4():
    filename = 'LabUno4.jpg'
    return send_file(filename, mimetype='image/jpg')

@app.route('/js')
def js():
    data="{}"
    command = request.query_string
    #print (command)
    if command!=b'':
        ser.write(command)
        ser.write(b'\r')
        ser.write(b'\n')
        data = ser.readline()
    #print (data)
    return data

if __name__ == '__main__':
    #app.run(host="127.0.0.1", port=5000)
    app.run(host="127.0.0.1", port=80)

ser.close()
