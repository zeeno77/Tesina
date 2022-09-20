//https://makecode.microbit.org/#editor

let readingTimeout = 0
let configTimeout = 0
let serialReading = ""
let readingTries = 0
let configTries = 0
let basicPause = 0
let counter = 0
let errorMsj = "X"
let strCounter = "-9"
basicPause = 1000
configTries = 10
readingTries = 10
serialReading = "UNSET"
let nerworkName = "arrakis"
let networkPassword = "thespicemustflow"
let serverURL = "http://192.168.88.53:8000/muestra/"


function sendConfigParameter (command: string, value: string, screenValue: string) {
    if (configTimeout < configTries) {
        serialReading = "UNSET"
        configTimeout = 0
        while (serialReading != "OK_" + command && configTimeout < configTries) {
            serial.writeString(command + ":" + value)
            basic.showString(screenValue)
            while (serialReading != "OK_" + command && readingTimeout < readingTries) {
                serialReading = serial.readString()
                readingTimeout += 1
                basic.pause(150)
            }
            readingTimeout = 0
            basic.pause(basicPause)
            configTimeout += 1
        }
    }
}

serial.redirect(
SerialPin.P0,
SerialPin.P1,
BaudRate.BaudRate9600
)
sendConfigParameter("NETWORK", nerworkName, "N")
sendConfigParameter("PASSWORD", networkPassword, "P")
sendConfigParameter("SERVER", serverURL, "S")
basic.forever(function () {
    if (configTimeout < configTries) {
        strCounter = counter.toString()
serial.writeString(strCounter)
        basic.showString(strCounter)
        counter += 1
        basic.pause(basicPause)
    } else {
        serial.writeString(errorMsj)
        basic.showString(errorMsj)
        basic.pause(basicPause)
    }
})
