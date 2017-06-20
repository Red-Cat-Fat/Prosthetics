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
            writeFile("ItsWork!!!");
            closeFiles();
            write_button.Text = "END";
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
            string path = @"dataBase-" + thisDay.ToString("d");// +'\\' + nameFile + ".mio";
            string file = path + '-' + nameFile + ".mio";
            fstream = new FileStream(file, FileMode.OpenOrCreate);
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
    }
}
