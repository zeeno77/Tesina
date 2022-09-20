#https://makecode.microbit.org/#editor

def sendConfigParameter(command: str, value: str, screenValue: str):
    global serialReading, configTimeout, readingTimeout
    if configTimeout < configTries:
        serialReading = "UNSET"
        configTimeout = 0
        while serialReading != "OK_" + command and configTimeout < configTries:
            serial.write_string("" + command + ":" + value)
            basic.show_string(screenValue)
            while serialReading != "OK_" + command and readingTimeout < readingTries:
                serialReading = serial.read_string()
                readingTimeout += 1
                basic.pause(150)
            readingTimeout = 0
            basic.pause(basicPause)
            configTimeout += 1
counter = 0
readingTimeout = 0
configTimeout = 0
serialReading = ""
readingTries = 0
configTries = 0
basicPause = 0
errorMsj = "X"
strCounter = "-9"
basicPause = 1000
configTries = 10
readingTries = 10
serialReading = "UNSET"
nerworkName = "arrakis"
networkPassword = "thespicemustflow"
serverURL = "http://192.168.88.53:8000/muestra/"
serial.redirect(SerialPin.P0, SerialPin.P1, BaudRate.BAUD_RATE9600)
sendConfigParameter("NETWORK", nerworkName, "N")
sendConfigParameter("PASSWORD", networkPassword, "P")
sendConfigParameter("SERVER", serverURL, "S")

def on_forever():
    global strCounter, counter
    if configTimeout < configTries:
        strCounter = "" + str(counter)
        serial.write_string(strCounter)
        basic.show_string(strCounter)
        counter += 1
        basic.pause(basicPause)
    else:
        serial.write_string(errorMsj)
        basic.show_string(errorMsj)
        basic.pause(basicPause)
basic.forever(on_forever)

