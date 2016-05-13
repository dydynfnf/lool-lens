using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WindowsFormsApplication1
{
    public partial class call : Form
    {
        public call()
        {
            InitializeComponent();
        }
        /// <summary>
        /// 延时函数
        /// </summary>
        /// <param name="MillSecond"></param>
        public static void DelayMs(int MillSecond)
        {
            int Start = Environment.TickCount;
            while (System.Math.Abs(Environment.TickCount - Start) < MillSecond)
            {
                Application.DoEvents();
            }
        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void label2_Click(object sender, EventArgs e)
        {

        }
    }
   

}
