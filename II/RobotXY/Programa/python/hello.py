import serial
from robotxy import Robotxy

from flask import Flask

app = Flask(__name__)


@app.route("/")
def hello():
	return "Hello World!\n"

@app.route("/connect/<int:port>",methods=['GET'])
def connect(port):
	if port > 0:
		cadenaPort = '/dev/ttyUSB' + str(0)
		print(cadenaPort)
		ser = serial.Serial(cadenaPort,9600)
	else:
		cadenaPort = '/dev/ttyACM' + str(port)
		ser = serial.Serial(cadenaPort,9600)
	return "OK"

@app.route("/status",methods=["GET"])
def status():
#	status =  Robotxy().status()
	puerto = '/dev/ttyUSB0'
	ser = serial.Serial('/dev/ttyUSB0',9600)
	ser.write('7 0 0\n'.encode('utf-8'))
	status = ser.readline()
	return status

@app.route("/moveXmmPos/<int:mm>",methods=["GET"])
def moveXmmPos(mm):
	puerto = '/dev/ttyUSB0'
	ser = serial.Serial(puerto,9600)
	cadenaSerial = '0 0 ' + str(mm)
	ser.write(cadenaSerial.encode('utf-8'))
	return "OK"

@app.route("/moveYmmPos/<int:mm>",methods=["GET"])
def moveYmmPos(mm):
	puerto = '/dev/ttyUSB0'
	ser = serial.Serial(puerto,9600)
	cadenaSerial = '0 1 ' + str(mm)
	ser.write(cadenaSerial.encode('utf-8'))
	return "OK"


@app.route("/moveXmmNeg/<int:mm>",methods=["GET"])
def moveXmmNeg(mm):
	puerto = '/dev/ttyUSB0'
	ser = serial.Serial(puerto,9600)
	cadenaSerial = '1 0 ' + str(mm)
	ser.write(cadenaSerial.encode('utf-8'))
	return "OK"

@app.route("/moveYmmNeg/<int:mm>",methods=["GET"])
def moveYmmNeg(mm):
	puerto = '/dev/ttyUSB0'
	ser = serial.Serial(puerto,9600)
	cadenaSerial = '1 1 ' + str(mm)
	ser.write(cadenaSerial.encode('utf-8'))
	return "OK"



@app.route("/moveXstepsPos/<int:steps>",methods=["GET"])
def moveXstepsPos(steps):
	puerto = '/dev/ttyUSB0'
	ser = serial.Serial(puerto,9600)
	cadenaSerial = '2 0 ' + str(steps)
	ser.write(cadenaSerial.encode('utf-8'))
	return "OK"


@app.route("/moveYstepsPos/<int:steps>",methods=["GET"])
def moveYstepsPos(steps):
	puerto = '/dev/ttyUSB0'
	ser = serial.Serial(puerto,9600)
	cadenaSerial = '2 1 ' + str(steps)
	ser.write(cadenaSerial.encode('utf-8'))
	return "OK"



@app.route("/moveXstepsNeg/<int:steps>",methods=["GET"])
def moveXstepsNeg(steps):
	puerto = '/dev/ttyUSB0'
	ser = serial.Serial(puerto,9600)
	cadenaSerial = '3 0 ' + str(steps)
	ser.write(cadenaSerial.encode('utf-8'))
	return "OK"


@app.route("/moveYstepsNeg/<int:steps>",methods=["GET"])
def moveYstepsNeg(steps):
	puerto = '/dev/ttyUSB0'
	ser = serial.Serial(puerto,9600)
	cadenaSerial = '3 1 ' + str(steps)
	ser.write(cadenaSerial.encode('utf-8'))
	return "OK"


@app.route("/calibrate/<int:axis>",methods=["GET"])
def calibrate(axis):
	puerto = '/dev/ttyUSB0'
	ser = serial.Serial(puerto,9600)
	cadenaSerial = '4 ' + str(axis) + ' 0'
	ser.write(cadenaSerial.encode('utf-8'))
	return "CALIBRATING"


@app.route("/home/<int:axis>",methods=["GET"])
def home(axis):
	puerto = '/dev/ttyUSB0'
	ser = serial.Serial(puerto,9600)
	cadenaSerial = '5 ' + str(axis) + ' 0'
	ser.write(cadenaSerial.encode('utf-8'))
	return "HOMING"


@app.route("/joystick/<int:value>",methods=["GET"])
def joystick(value):
	puerto = '/dev/ttyUSB0'
	ser = serial.Serial(puerto,9600)
	cadenaSerial = '6 0 ' + str(value)
	ser.write(cadenaSerial.encode('utf-8'))
	return "OK"


@app.route("/rearme",methods=["GET"])
def rearme():
	puerto = '/dev/ttyUSB0'
	ser = serial.Serial(puerto,9600)
	cadenaSerial = '9 0 0'
	ser.write(cadenaSerial.encode('utf-8'))
	return "OK"

@app.route("/deactive",methods=["GET"])
def deactive():
	puerto = '/dev/ttyUSB0'
	ser = serial.Serial(puerto,9600)
	cadenaSerial = '10 0 0'
	ser.write(cadenaSerial.encode('utf-8'))
	return "OK"



@app.route('/setPosition/<int:position>',methods=["GET"])
def setPosition(position):
	ser = serial.Serial('/dev/ttyUSB0',9600)
	newPosition = '8 0 %d\n' % position
	ser = serial.Serial('/dev/ttyUSB0',9600)
	ser.write(newPosition.encode('utf-8'))
#	statu = ser.readline()
	return "OK"

if __name__ == "__main__":
	app.run(debug=True,host='0.0.0.0')
