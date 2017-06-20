using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace Reader
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void write_button_Click(object sender, EventArgs e)
        {
            createFile(textBox1.Text);
            //writeFile("ItsWork!!!");
            //closeFiles();
            statusLabel.Text = "Идёт запись";
            writeSerial(s);
            closeFiles();
            statusLabel.Text = "Записанно";
        }

        private void read_button_Click(object sender, EventArgs e)
        {

        }

        FileStream fstream;

        void writeFile(string text)
        {
            // преобразуем строку в байты
            byte[] array = System.Text.Encoding.Default.GetBytes(text);
            // запись массива байтов в файл
            fstream.Write(array, 0, array.Length);
            //Console.WriteLine("Текст записан в файл");
        }
        void createFile(string nameFile)
        {
            DateTime thisDay = DateTime.Today;
            string way = @"dataBase-" + thisDay.ToString("d") + '-' + thisDay.Hour + '.' + thisDay.Minute + '-' + nameFile + ".mio";
            fstream = new FileStream(way, FileMode.OpenOrCreate);
        }

        void openFile(string nameFile)
        {
            /*// чтение из файла
            using (FileStream fstream = File.OpenRead(@"C:\SomeDir\noname\note.txt"))
            {
                // преобразуем строку в байты
                byte[] array = new byte[fstream.Length];
                // считываем данные
                fstream.Read(array, 0, array.Length);
                // декодируем байты в строку
                string textFromFile = System.Text.Encoding.Default.GetString(array);
                Console.WriteLine("Текст из файла: {0}", textFromFile);
            }

            Console.ReadLine();*/
        }

        void closeFiles()
        {
            fstream.Close();
        }
        Serial s = new Serial();
        private void connect_button_Click(object sender, EventArgs e)
        {
            if (idPort_textBox.Text == "")
            {
                s.UpdatePorts();

                for (int i = 0; i < s.AvailablePorts.Length; i++)
                {
                    data_textBox.Text = '[' + Convert.ToString(i) + ']';
                    data_textBox.Text += s.AvailablePorts[i];
                }
                statusLabel.Text = "Выберите порт";
            }
            else
            {
                s.UpdatePorts();
                int portID = Convert.ToInt32(idPort_textBox.Text);

                s.Create(portID, 115200);

                s.Open();
                statusLabel.Text = "Подключенно";
                /*MainForm mf = new MainForm();
                mf.Show();*/
                //int z = 0;
                //while (true)
                //{
                //    if (++z == 100000)
                //    {
                //        data_textBox.Text =  + '\n';
                //        z = 0;
                //    }
                //}
            }
        }

        void writeSerial(Serial serial)
        {
            int j = 0;
            while (j < 10000)
            {
                writeFile(serial.WaitRecv());
                statusLabel.Text = serial.WaitRecv();
                j++;
            }
        }
    }
}
