
# **Proyecto IRID**

El proyecto se compone de dos partes principales el backend que recibe y almacena los datos y el codigo que ejecutan las placas conectadas a los sensores.

## **Backend:**

**Prerequisitos:** Tener instalado Docker y docker-compose.

Para inicializarlo abrimos una terminal[^1], nos hubicamos en la carpeta backendAPI y ejecutamos:      
```
docker-compose build
```        
Si no genera ningún error ejecutamos
```       
docker-compose up
```     
Estos dos comandos nos van a levantar los siguientes servidores
    
- **Server REST Python:** encargado de recibir y validar los datos de las placas.
    - Posee una interfaz (Swagger) web accesible desde http://127.0.0.1:8000/docs con documentacion de los metódos web soportados por el server, ejemplos y la posibilidad de utilizar dichos metodos para comunicarse con el server.     
- **Server MongoExpress:** Interfaz web para ver y exportar los datos almacenados.
    - Posee una interfaz web accesible desde http://127.0.0.1:8081/db/irid/ que permite ver el contenido de la base de datos, filtrar dicho contenido según diferentes criterios y exportarlos en varios formatos.
- **Server Grafana:** Interfaz web que permite la visualizacion grafica de los datos almacenados.
    - Posee una interfaz web accesible desde http://127.0.0.1:3000/. Su usuario por defecto es admin y su contraseña admin. Después del primer inicio de sesión pide cambiar la contraseña.
- **Server MongoDB:** base de datos donde se va a almacenar la informacion.
    - Su interfaz es el MongoExpress



## **Placas:**
**Prerequisitos:** Tener instalado el Arduino IDE[^2], las librerias[^3][^4] de ESP para Arduino y MakeCode[^5].

### Configuración de la ESP
Abrir con el Arduino IDE el achivo Relay-ESP8266.ino contenido en placas/microbitConESP/Relay-ESP8266, conectar la ESP via usb y subir el codigo.

### Configuración de la Micro:Bit
Abrir makecode e importar el archivo microbit-pruebaCasita.hex contenido en placas/microbitConESP/MicroBit-Code/. Realizar los cambios necesarios, una vez terminadas las modificaciones conectar la Micro:Bit via usb y subir el codigo.



[^1]: Powershell en Windows, Terminal en Mac o Linux.
[^2]: https://www.arduino.cc/en/software
[^3]: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
[^4]: http://arduino.esp8266.com/stable/package_esp8266com_index.json
[^5]: https://makecode.microbit.org/offline-app
