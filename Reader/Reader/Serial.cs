using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.IO.Ports;

namespace Reader
{
    public class Serial
    {
        string[] _availableports;
        public string[] AvailablePorts
        {
            get
            {
                return this._availableports;
            }
        }
        /// <summary>
        /// Обновляет массив имен последовательных портов для текущего компьютера.
        /// </summary>
        public void UpdatePorts()
        {
            _availableports = SerialPort.GetPortNames();
        }

        SerialPort _port;
        public SerialPort Port
        {
            get
            {
                return this._port;
            }
        }

        public string SendRecv(string requiry)
        {
            while (_port.BytesToRead > 0)
            {
                _port.ReadLine();
            }
            _port.Write(requiry);
            string message;
            try
            {
                message = _port.ReadLine();
                return message;
            }
            catch (TimeoutException)
            {
                return "#err";
            }
        }

        public void Send(string requiry)
        {
            _port.Write(requiry);
        }

        public string Recv()
        {
            string message;

            message = _port.ReadLine();

            return message;
        }
        /// <summary>
        /// Вывод полученных данных
        /// </summary>
        /// <returns>Входные данные с порта</returns>
        public string WaitRecv()
        {
            int i = 0;
            while (_port.BytesToRead == 0)
            {
                i++;
            }
            string message;
            try
            {
                message = _port.ReadLine();
                return message;
            }
            catch (TimeoutException)
            {
                return "#err";
            }
        }
        /// <summary>
        /// Подключение к порту
        /// </summary>
        /// <param name="port_id">Id подключаемого порта</param>
        /// <param name="baudRate">Скорость подключения</param>
        public void Create(int port_id, int baudRate)
        {
            _port = new SerialPort(_availableports[port_id], baudRate);
            _port.ReadTimeout = 100;
        }
        /// <summary>
        /// Подключение к порт
        /// </summary>
        /// <param name="port">Имя порта </param>
        /// <param name="baudRate">Скорость подключения</param>
        public void Create(string port, int baudRate)
        {
            _port = new SerialPort(port, baudRate);
            _port.ReadTimeout = 100;
        }
        /// <summary>
        /// Открыть порт
        /// </summary>
        public void Open()
        {
            _port.Open();
        }
        /// <summary>
        /// Закртыть порт
        /// </summary>
        public void Close()
        {
            _port.Close();
        }
    }

}
