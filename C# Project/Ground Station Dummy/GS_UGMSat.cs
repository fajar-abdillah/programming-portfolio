/*
Author: Abdillah Faisal Nur Fajar
Program at Visual Studio 2017
*/

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;

namespace UGMSat
{
    public partial class Form1 : Form
    {
        string dataIN;
        int time,time2;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            string[] ports = SerialPort.GetPortNames();
            cBoxCOMPORT.Items.AddRange(ports);
        }

        private void btnOPEN_Click(object sender, EventArgs e)
        {
            try
            {
                serialPort1.PortName = cBoxCOMPORT.Text;
                serialPort1.BaudRate = Convert.ToInt32(cBoxBAUDRATE.Text);
                serialPort1.DataBits = Convert.ToInt32(cBoxDATABITS.Text);
                serialPort1.StopBits = (StopBits)Enum.Parse(typeof(StopBits), cBoxSTOPBITS.Text);
                serialPort1.Parity = (Parity)Enum.Parse(typeof(Parity), cBoxPARITY.Text);

                serialPort1.Open();
                progressBar1.Value = 100;
                lblStatus.Text = "Ground Station ON";
                lblStatus2.Text = " ";

                btnCLOSE.Enabled = true;
                btnOPEN.Enabled = false;
                btnClearRcv.Enabled = true;
                tBoxReceive.Enabled = true;
                btnBeaconRcv.Enabled = true;
                btnHousekeeping.Enabled = false;
                btnAck.Enabled = false;
                btnWrong.Enabled = true;
            }

            catch(Exception err)
            {
                MessageBox.Show(err.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btnCLOSE_Click(object sender, EventArgs e)
        {
            serialPort1.Close();

            progressBar1.Value = 0;
            lblStatus.Text = "Ground Station OFF";
            lblStatus2.Text = " ";

            btnCLOSE.Enabled = false;
            btnOPEN.Enabled = true;
            btnClearRcv.Enabled = false;
            tBoxReceive.Enabled = false;
            btnBeaconRcv.Enabled = false;
            btnHousekeeping.Enabled = false;
            btnAck.Enabled = false;
            btnWrong.Enabled = false;
        }

        private void btnBeaconRcv_Click(object sender, EventArgs e)
        {
            time = 10;
            timer1.Start();

            if (time < 1)
            {
                btnBeaconRcv.Enabled = true;
                btnHousekeeping.Enabled = false;

                lblStatus.Text = "";
                lblStatus2.Text = "";

                lblStatus2.Text = "Communication Timeout";
            }
            else if (time > 1)
            {
                btnBeaconRcv.Enabled = false;
                btnHousekeeping.Enabled = true;

                lblStatus.Text = "";
                lblStatus2.Text = "";

                lblStatus.Text = "Satellite Connected";
                lblStatus2.Text = "Do Command";

                serialPort1.Write("C");
            }
        }

        private void btnHousekeeping_Click(object sender, EventArgs e)
        {
            btnBeaconRcv.Enabled = true;
            btnHousekeeping.Enabled = false;
            btnAck.Enabled = true;

            lblStatus.Text = "";
            lblStatus2.Text = "";

            lblStatus.Text = "Satellite Connected";
            lblStatus2.Text = "Request Housekeeping";

            serialPort1.Write("H");
            timer2.Start();

            if(dataIN == "*")
            {
                timer2.Stop();
                lblTime.Text = Convert.ToString(time2);
                time2 = 0;
            }
        }

        private void btnAck_Click(object sender, EventArgs e)
        {
            btnAck.Enabled = false;

            lblStatus.Text = "";
            lblStatus2.Text = "";

            lblStatus.Text = "Satellite Connected";
            lblStatus2.Text = "Data Received";

            serialPort1.Write("K");
        }

        private void btnWrong_Click(object sender, EventArgs e)
        {
            serialPort1.Write("A");
        }

        private void ShowData(object sender, EventArgs e)
        {
            tBoxReceive.Text += dataIN;
;       }

        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            dataIN = serialPort1.ReadExisting();
            this.Invoke(new EventHandler(ShowData));            
        }

        private void btnClearRcv_Click(object sender, EventArgs e)
        {
            if (tBoxReceive.Text != "")
            {
                tBoxReceive.Text = "";
            }
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            time--;
        }

        private void timer2_Tick(object sender, EventArgs e)
        {
            time2++;
        }
    }
}
