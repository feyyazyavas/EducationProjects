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
using System.Diagnostics;

namespace EEPROM_YEDEKLEME
{
    public partial class Form1 : Form
    {
        string[] ports = SerialPort.GetPortNames();
        string gelen = string.Empty;        
        int i = 0 , j = 0 , k = 0 ;
        string[] saat = new string[2000];
        int[] sicaklik = new int[2000];       
        int istenen = 0;
        double ortalama = 0;

        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if(serialPort1.IsOpen == false)
            {
                if (comboBox1.Text == "")
                    return;
                serialPort1.PortName = comboBox1.Text;
                serialPort1.BaudRate = Convert.ToInt16(comboBox2.Text);
                try
                {
                    serialPort1.Open();
                    pictureBox1.ImageLocation = "connect.png";
                    
                }
                catch(Exception hata)
                {
                    MessageBox.Show("Hata:" + hata.Message);
                }
            }     
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            foreach(string port in ports)
            {
                comboBox1.Items.Add(port);
            }
            comboBox2.SelectedIndex = 2;
            pictureBox1.ImageLocation = "dis_con_nect.png";
            timer1.Enabled = false;
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            if (serialPort1.IsOpen == true)
                serialPort1.Close();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if(serialPort1.IsOpen == true)
            {
                serialPort1.Close();
                pictureBox1.ImageLocation = "disconnect.png";
                timer1.Enabled = false;
            }
        }

        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            timer1.Enabled = false;
            try
            {
                gelen = serialPort1.ReadLine();
                this.Invoke(new EventHandler(Verileri_Isle));
            }
            catch (Exception hata)
            {
                MessageBox.Show("Hata:" + hata.Message);
            }                      
        }

        private void Verileri_Isle(object s, EventArgs e)
        {
            i = i + 1;
            i = dataGridView1.Rows.Add();
            dataGridView1.Rows[j].Cells[0].Value = saat[j];
            dataGridView1.Rows[j].Cells[1].Value = gelen.ToString();
            sicaklik[j] = Convert.ToInt32(gelen);            
            j = j + 1;
        }

        private void button6_Click(object sender, EventArgs e)
        {
            dataGridView1.Rows.Clear();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            
            int ilk = 0, son = 0;
            ilk = Array.IndexOf(saat, textBox1.Text);
            son = Array.IndexOf(saat, textBox2.Text);
            int n = son - ilk + 1;
            int x = 0;
            int[] siralama = new int[n];

            for (int y = ilk; y <= son; y++)
            {
                istenen += Convert.ToInt32(sicaklik[y]);
                siralama[x] = sicaklik[y];
                x++;
            }
            Array.Sort(siralama);
            label7.Text = siralama[0].ToString();
            label5.Text = siralama[n-1].ToString();
            ortalama = Convert.ToDouble((Convert.ToDouble(istenen)) / n );
            label6.Text = ortalama.ToString("0.##");
            ortalama = 0;
            istenen = 0;
        }

        private void timer1_Tick(object sender, EventArgs e)
        {           
            serialPort1.Write("a");
            label1.Text = DateTime.Now.ToLongTimeString();
            saat[k] = label1.Text;
            k = k + 1;      
        }

        private void button3_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen == true)
            {
                timer1.Enabled = true;
                button4.Enabled = true;
            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen == true)
            {
                timer1.Enabled = false;          
                System.Threading.Thread.Sleep(1000);
                serialPort1.Write("b");
                button4.Enabled = false;
            }    
        }
    }
}
