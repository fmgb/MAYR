import serial


class Robotxy:
	def __init__(self):
		self._port = '/dev/ttyUSB0'
		self._baudrate = 9600
		self.ser = serial

	def connect(self):
		self.ser = serial.Serial(self._port,self._baudrate)
		return "ok"

	def status(self):
		self.ser.write('7 0 0\n'.encode('utf-8'))
		return self.ser.readline()
