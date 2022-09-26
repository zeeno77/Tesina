let counter = 0
let strCounter = "-9"
let errorMsj = "Error"
let basicPause = 1000
let configTries = 10
let nerworkName = "arrakis"
let networkPassword = "thespicemustflow"
let serverURL = "http://192.168.88.53:8000/muestra/"
let configTimeout = custom.setupESP(nerworkName, networkPassword, serverURL)
basic.forever(function () {
    if (configTimeout < configTries) {
        strCounter = counter.toString()
        custom.sendData("MicroBit", "Counter", strCounter)
        counter += 1
        basic.pause(basicPause)
    } else {
        serial.writeString(errorMsj)
        basic.showString(errorMsj)
        basic.pause(basicPause)
    }
})
