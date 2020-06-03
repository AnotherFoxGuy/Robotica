using System;
using System.Net.WebSockets;
using System.Text.Json;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;

namespace web.Socket
{
    /// <summary>
    /// ws://localhost:5000/ws/Robot
    /// </summary>
    public class RobotConnector : BaseConnector
    {
        public string Name { get; set; }

        public RobotConnector(ConnectionManager connectionManager) : base(connectionManager)
        {
        }

        protected override void HandleMessage(string msg)
        {
            try
            {
                var d = msg.Split('|');
                var cmd = d[0];
                var dat = d.Length != 1 ? d[1] : "NOT SET";
                switch (cmd)
                {
                    case "register":
                        ConnectionManager.Register(this);
                        Name = dat;
                        SendData($"Registered {Name}");
                        break;
                    default:
                        SendData($"Unknown Command: {cmd}");
                        Console.WriteLine($"[RobotConnector] Unknown Command: {cmd}");
                        break;
                }
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                SendData($"ERROR: {e.Message}");
            }
        }

        protected override void OnClose()
        {
            ConnectionManager.Deregister(this);
        }
    }
}