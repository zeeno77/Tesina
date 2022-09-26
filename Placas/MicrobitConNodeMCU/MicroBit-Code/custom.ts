/**
 * Custom blocks
 */
//% weight=100 color=#0fbc11 icon=""
namespace custom {
    let configTimeout = 0
    let readingTries = 10
    let readingTimeout = 0


    /**
     * TODO: describe your function here
     * @param n describe parameter here, eg: 5
     * @param s describe parameter here, eg: "Hello"
     * @param e describe parameter here
     */
    //% block
    export function showSendAnimation(){
        basic.showLeds(`
            . . . . .
            . . . . .
            . . . . .
            . . . . .
            . . # . .
            `)
        basic.showLeds(`
            . . . . .
            . . . . .
            . # # # .
            # . . . #
            . . # . .
            `)
        basic.showLeds(`
            . # # # .
            # . . . #
            . # # # .
            # . . . #
            . . # . .
            `)
    }
    /**
     * TODO: describe your function here
     * @param n describe parameter here, eg: 5
     * @param s describe parameter here, eg: "Hello"
     * @param e describe parameter here
     */
    //% block
    export function sendData(origin: string, sensor: string, value: string) {
        let data = "" + origin + ":" + sensor + ":" + value
        serial.writeString(data)
        showSendAnimation()
    }

    /**
    * TODO: describe your function here
    * @param n describe parameter here, eg: 5
    * @param s describe parameter here, eg: "Hello"
    * @param e describe parameter here
    */
    //% block
    export function sendConfigParameter(command: string, value: string, screenValue: string) {
        if (configTimeout < configTries) {
            let serialReading = "UNSET"
            configTimeout = 0
            while (serialReading != "OK_" + command && configTimeout < configTries) {
                serial.writeString("" + command + ":" + value)
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

    /**
    * TODO: describe your function here
    * @param n describe parameter here, eg: 5
    * @param s describe parameter here, eg: "Hello"
    * @param e describe parameter here
    */
    //% block
    export function setupESP(ssid: string, password: string, url: string): number {
        serial.redirect(SerialPin.P0, SerialPin.P1, BaudRate.BaudRate9600)
        sendConfigParameter("NETWORK", ssid, "N")
        sendConfigParameter("PASSWORD", password, "P")
        sendConfigParameter("SERVER", url, "S")
        return configTimeout
    }
}
