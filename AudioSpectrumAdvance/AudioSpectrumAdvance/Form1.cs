using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AudioSpectrumAdvance
{
    public partial class Form1 : Form
    {
        Analyzer analyzer;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            analyzer = new Analyzer(progressBar1, progressBar2, spectrum1, comboBox1, chart1);
        }
        
        private void button1_Click(object sender, EventArgs e)
        {
            analyzer.Enable = true;
            analyzer.DisplayEnable = true;
            comboBox1.Enabled = false;
            button1.Enabled = false;
        }
    }
}