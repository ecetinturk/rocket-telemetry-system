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
using System.IO.Ports;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;
using System.Threading;

namespace Deneme
{
    public partial class Form1 : Form
    {
        string[] portlar = SerialPort.GetPortNames();

        string pathFile;

        public Form1()
        {
            InitializeComponent();
            serialPort1.BaudRate = 9600;
            serialPort1.Parity = Parity.None;
            serialPort1.StopBits = StopBits.One;
            serialPort1.DataBits = 8;
            serialPort1.Handshake = Handshake.None; // Some device needs a different handshake.
            serialPort1.NewLine = "\n";

            // Control.CheckForIllegalCrossThreadCalls = false;
        }

        public static string getBetween(string strSource, string strStart, string strEnd)
        {
            if (strSource.Contains(strStart) && strSource.Contains(strEnd))
            {
                int Start, End;
                Start = strSource.IndexOf(strStart, 0) + strStart.Length;
                End = strSource.IndexOf(strEnd, Start);
                return strSource.Substring(Start, End - Start);
            }
            return "";
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if(!serialPort1.IsOpen)
            {
                serialPort1.PortName = comboBox1.Text;
                try
                {
                    serialPort1.DtrEnable = true;
                    serialPort1.Open();
                    serialPort1.DataReceived += serialPort1_DataReceived;
                }
                catch (Exception)
                {
                    MessageBox.Show("Seçtiğiniz portun doğru ve müsait olduğundan emin olun!");
                }
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            foreach (string port in portlar)
            {
                comboBox1.Items.Add(port);
                comboBox1.SelectedIndex = 0;
            }
            serialPort1.DataReceived += new SerialDataReceivedEventHandler(serialPort1_DataReceived);

            pathFile = Path.GetDirectoryName(Path.GetDirectoryName(System.IO.Directory.GetCurrentDirectory()));
            pathFile += @"\STM32\SerialData.txt";

            Console.WriteLine("===== Below is The Result =====");
            Console.WriteLine(pathFile);
        }

        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            string data;
            List<string> myReceivedLines = new List<string>();
            string[] splitted_data;
            string[] text_data;
            data = serialPort1.ReadLine();

            myReceivedLines.Add(data);

            splitted_data = data.Split(' ');
            text_data = data.Split('\n');

            if ((data.Contains("Temp:")))
            {
                string toFind1 = "Temp: ";
                string toFind2 = "\r";
                int start = data.IndexOf(toFind1) + toFind1.Length;
                int end = data.IndexOf(toFind2, start); //Start after the index of 'my' since 'is' appears twice
                if (end > 0 && start > 0)
                {
                    string specificData = data.Substring(start, end - start);
                    textBox3.Text = specificData;
                    // System.IO.File.AppendAllLines(pathFile, (specificData).Select(p => p.ToString()));
                    // System.IO.File.AppendAllText(pathFile, (data + "\n"));
                }
            }
            if ((data.Contains("Pres:")))
            {
                string toFind1 = "Pres: ";
                string toFind2 = "\r";
                int start = data.IndexOf(toFind1) + toFind1.Length;
                int end = data.IndexOf(toFind2, start); //Start after the index of 'my' since 'is' appears twice
                if (end > 0 && start > 0)
                {
                    string specificData = data.Substring(start, end - start);
                    textBox4.Text = specificData;
                    // System.IO.File.AppendAllLines(pathFile, (specificData).Select(p => p.ToString()));
                    // System.IO.File.AppendAllText(pathFile, (data + "\n"));
                }
            }
            if ((data.Contains("Alt:")))
            {
                string toFind1 = "Alt: ";
                string toFind2 = "\r";
                int start = data.IndexOf(toFind1) + toFind1.Length;
                int end = data.IndexOf(toFind2, start); //Start after the index of 'my' since 'is' appears twice
                if (end > 0 && start > 0)
                {
                    string specificData = data.Substring(start, end - start);
                    textBox5.Text = specificData;
                    // System.IO.File.AppendAllLines(pathFile, (specificData).Select(p => p.ToString()));
                    // System.IO.File.AppendAllText(pathFile, (data + "\n"));
                }

            }
            if ((data.Contains("Lat:")))
            {
                string toFind1 = "Lat:";
                string toFind2 = "\r";
                int start = data.IndexOf(toFind1) + toFind1.Length;
                int end = data.IndexOf(toFind2, start); //Start after the index of 'my' since 'is' appears twice
                if (end > 0 && start > 0)
                {
                    string specificData = data.Substring(start, end - start);
                    textBox6.Text = specificData;
                    // System.IO.File.AppendAllLines(pathFile, (specificData).Select(p => p.ToString()));
                    // System.IO.File.AppendAllText(pathFile, (data + "\n"));
                }
            }
            if ((data.Contains("Lon:")))
            {
                string toFind1 = "Lon:";
                string toFind2 = "\r";
                int start = data.IndexOf(toFind1) + toFind1.Length;
                int end = data.IndexOf(toFind2, start); //Start after the index of 'my' since 'is' appears twice
                if (end > 0 && start > 0)
                {
                    string specificData = data.Substring(start, end - start);
                    textBox7.Text = specificData;
                    // System.IO.File.AppendAllLines(pathFile, (specificData).Select(p => p.ToString()));
                    // System.IO.File.AppendAllText(pathFile, (data + "\n"));
                }
            }
            if ((data.Contains("AAM:")))
            {
                string toFind1 = "AAM:";
                string toFind2 = "\r";
                int start = data.IndexOf(toFind1) + toFind1.Length;
                int end = data.IndexOf(toFind2, start); //Start after the index of 'my' since 'is' appears twice
                if (end > 0 && start > 0)
                {
                    string specificData = data.Substring(start, end - start);
                    textBox16.Text = specificData;
                    // System.IO.File.AppendAllLines(pathFile, (specificData).Select(p => p.ToString()));
                    // System.IO.File.AppendAllText(pathFile, (data + "\n"));
                }
            }
            if ((data.Contains("Ax:")))
            {
                string toFind1 = "Ax:";
                string toFind2 = "\r";
                int start = data.IndexOf(toFind1) + toFind1.Length;
                int end = data.IndexOf(toFind2, start); //Start after the index of 'my' since 'is' appears twice
                if (end > 0 && start > 0)
                {
                    string specificData = data.Substring(start, end - start);
                    textBox10.Text = specificData;
                    // System.IO.File.AppendAllLines(pathFile, (specificData).Select(p => p.ToString()));
                    // System.IO.File.AppendAllText(pathFile, (data + "\n"));
                }
            }
            if ((data.Contains("Ay:")))
            {
                string toFind1 = "Ay:";
                string toFind2 = "\r";
                int start = data.IndexOf(toFind1) + toFind1.Length;
                int end = data.IndexOf(toFind2, start); //Start after the index of 'my' since 'is' appears twice
                if (end > 0 && start > 0)
                {
                    string specificData = data.Substring(start, end - start);
                    textBox9.Text = specificData;
                    // System.IO.File.AppendAllLines(pathFile, (specificData).Select(p => p.ToString()));
                    // System.IO.File.AppendAllText(pathFile, (data + "\n"));
                }
            }
            if ((data.Contains("Az:")))
            {
                string toFind1 = "Az:";
                string toFind2 = "\r";
                int start = data.IndexOf(toFind1) + toFind1.Length;
                int end = data.IndexOf(toFind2, start); //Start after the index of 'my' since 'is' appears twice
                if (end > 0 && start > 0)
                {
                    string specificData = data.Substring(start, end - start);
                    textBox8.Text = specificData;
                    // System.IO.File.AppendAllLines(pathFile, (specificData).Select(p => p.ToString()));
                    // System.IO.File.AppendAllText(pathFile, (data + "\n"));
                }
            }
            if ((data.Contains("MPUTmp:")))
            {
                string toFind1 = "MPUTmp: ";
                string toFind2 = "\r";
                int start = data.IndexOf(toFind1) + toFind1.Length;
                int end = data.IndexOf(toFind2, start); //Start after the index of 'my' since 'is' appears twice
                if (end > 0 && start > 0)
                {
                    string specificData = data.Substring(start, end - start);
                    textBox14.Text = specificData;
                    // System.IO.File.AppendAllLines(pathFile, (specificData).Select(p => p.ToString()));
                    // System.IO.File.AppendAllText(pathFile, (data + "\n"));
                }
            }
            if ((data.Contains("Gx:")))
            {
                string toFind1 = "Gx:";
                string toFind2 = "\r";
                int start = data.IndexOf(toFind1) + toFind1.Length;
                int end = data.IndexOf(toFind2, start); //Start after the index of 'my' since 'is' appears twice
                if (end > 0 && start > 0)
                {
                    string specificData = data.Substring(start, end - start);
                    textBox13.Text = specificData;
                    // System.IO.File.AppendAllLines(pathFile, (specificData).Select(p => p.ToString()));
                    // System.IO.File.AppendAllText(pathFile, (data + "\n"));
                }
            }
            if ((data.Contains("Gy:")))
            {
                string toFind1 = "Gy:";
                string toFind2 = "\r";
                int start = data.IndexOf(toFind1) + toFind1.Length;
                int end = data.IndexOf(toFind2, start); //Start after the index of 'my' since 'is' appears twice
                if (end > 0 && start > 0)
                {
                    string specificData = data.Substring(start, end - start);
                    textBox12.Text = specificData;
                    // System.IO.File.AppendAllLines(pathFile, (specificData).Select(p => p.ToString()));
                    // System.IO.File.AppendAllText(pathFile, (data + "\n"));
                }
            }
            if ((data.Contains("Gz:")))
            {
                string toFind1 = "Gz:";
                string toFind2 = "\r";
                int start = data.IndexOf(toFind1) + toFind1.Length;
                int end = data.IndexOf(toFind2, start); //Start after the index of 'my' since 'is' appears twice
                if (end > 0 && start > 0)
                {
                    string specificData = data.Substring(start, end - start);
                    textBox11.Text = specificData;
                    // System.IO.File.AppendAllLines(pathFile, (specificData).Select(p => p.ToString()));
                    // System.IO.File.AppendAllText(pathFile, (data + "\n"));
                }
            }
            if ((data.Contains("Pi:")))
            {
                string toFind1 = "Pi:";
                string toFind2 = "\r";
                int start = data.IndexOf(toFind1) + toFind1.Length;
                int end = data.IndexOf(toFind2, start); //Start after the index of 'my' since 'is' appears twice
                if (end > 0 && start > 0)
                {
                    string specificData = data.Substring(start, end - start);
                    textBox15.Text = specificData;
                    // System.IO.File.AppendAllLines(pathFile, (specificData).Select(p => p.ToString()));
                    // System.IO.File.AppendAllText(pathFile, (data + "\n"));
                }
            }
            if ((data.Contains("Ro:")))
            {
                string toFind1 = "Ro:";
                string toFind2 = "\r";
                int start = data.IndexOf(toFind1) + toFind1.Length;
                int end = data.IndexOf(toFind2, start); //Start after the index of 'my' since 'is' appears twice
                if (end > 0 && start > 0)
                {
                    string specificData = data.Substring(start, end - start);
                    textBox2.Text = specificData;
                    // System.IO.File.AppendAllLines(pathFile, (specificData).Select(p => p.ToString()));
                    // System.IO.File.AppendAllText(pathFile, (data + "\n"));
                }
            }
            if ((data.Contains("Ya:")))
            {
                string toFind1 = "Ya:";
                string toFind2 = "\r";
                int start = data.IndexOf(toFind1) + toFind1.Length;
                int end = data.IndexOf(toFind2, start); //Start after the index of 'my' since 'is' appears twice
                if (end > 0 && start > 0)
                {
                    string specificData = data.Substring(start, end - start);
                    textBox1.Text = specificData;
                    // System.IO.File.AppendAllLines(pathFile, (specificData).Select(p => p.ToString()));
                    // System.IO.File.AppendAllText(pathFile, (data + "\n"));
                }
            }

            //label1.Text = data.ToString();
            //textBox1.Text = data;
            // textBox2.Text = specificData;
            //textBox2.Text = splitted_data[1] + splitted_data[2];

            // System.IO.File.AppendAllLines(pathFile, (data).Select(p => p.ToString()));
        }

        private void label21_Click(object sender, EventArgs e)
        {

        }

        private void textBox11_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox12_TextChanged(object sender, EventArgs e)
        {

        }

        private void textBox13_TextChanged(object sender, EventArgs e)
        {

        }
    }
}
