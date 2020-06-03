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
                var dat = msg.Split('|');
                switch (dat[0])
                {
                    case "register":
                        ConnectionManager.Register(this);
                        Name = dat[1];
                        SendData($"Registered {Name}");
                        break;
                    default:
                        SendData($"Unknown Command: {msg}");
                        Console.Write($"RobotConnector: {msg}\n");
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