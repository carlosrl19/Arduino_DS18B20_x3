:: El archivo es bastante sencillo, demasiado sencillo de hecho, solamente es necesario que se presione el archivo .bat y listo.

cd /d "C:\Users\Carlos\Documents\Arduino_DS18B20_x3"
nodemon.cmd index.js

:: El script viaja a la ruta y automáticamente ejecuta el comando nodemon...

@echo off

echo !Sensores de temperatura encendidos!

pause
