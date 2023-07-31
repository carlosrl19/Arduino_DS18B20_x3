const { SerialPort } = require('serialport');
const express = require('express');
const app = express();
const http = require('http').Server(app);
const io = require('socket.io')(http);

const arduinoPort = '/dev/ttyACM0'; // Cambiar según tu sistema
const baudRate = 9600;

let arduinoData = {
  temp1: 0,
  temp2: 0,
  temp3: 0,
};

// Configuración de socket.io
io.on('connection', (socket) => {
  console.log('Cliente conectado');
  socket.emit('temperature', arduinoData); // Enviar los datos actuales al cliente recién conectado
});

// Configuración de Express y EJS
app.set('view engine', 'ejs');
app.use(express.static('public'));

app.get('/', (req, res) => {
  res.render('index', { data: arduinoData });
});

// Iniciar el servidor
const port = 3000;
http.listen(port, () => {
  console.log(`Servidor iniciado en http://localhost:${port}`);
});

// Configurar el puerto serie para leer datos del Arduino
const arduinoSerial = new SerialPort({ path: '/dev/ttyACM0', baudRate: baudRate });

arduinoSerial.on('open', () => {
  console.log(`Puerto serie abierto en ${arduinoPort}`);
});

// Evento 'data' para leer los datos del puerto serie
let serialData = ''; // Variable para almacenar los datos recibidos hasta encontrar el delimitador '\n'
arduinoSerial.on('data', (data) => {
  const newData = data.toString();
  const lastIndex = newData.lastIndexOf('\n');
  if (lastIndex !== -1) {
    serialData += newData.substring(0, lastIndex); // Concatenar los datos recibidos sin el último '\n'
    processData(serialData); // Procesar los datos completos
    serialData = newData.substring(lastIndex + 1); // Guardar los datos no procesados para el siguiente evento 'data'
  } else {
    serialData += newData; // Continuar concatenando los datos si no se encuentra el delimitador '\n'
  }
});

function processData(data) {
  const dataArray = data.trim().split(':');
  if (dataArray.length === 2) {
    const sensor = dataArray[0];
    const temperature = parseFloat(dataArray[1]);
    arduinoData[sensor] = temperature;
    io.emit('temperature', arduinoData); // Enviar los datos actualizados a todos los clientes
  }
}

arduinoSerial.on('error', (err) => {
  console.error('Error en el puerto serie:', err.message);
});
