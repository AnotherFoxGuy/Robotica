using System;
using System.Windows.Forms;
using WebSocketSharp;
using WebSocketSharp.Server;

namespace AudioSpectrumAdvance
{
    public class SendServer : WebSocketBehavior
    {
        public static SendServer Instance { get; private set; }

        protected override void OnMessage (MessageEventArgs e)
        {
            Send (e.Data);
        }


        public SendServer()
        {
            Instance = this;
        }

        public void SendData(string i)
        {
            try
            {
                Send (i);
            }
            catch (Exception e)
            {
            }
        }
    }
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            var wssv = new WebSocketServer ("ws://localhost:8080");
            wssv.AddWebSocketService<SendServer> ("/");
            wssv.Start ();

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
            
            wssv.Stop ();
        }
    }
}
