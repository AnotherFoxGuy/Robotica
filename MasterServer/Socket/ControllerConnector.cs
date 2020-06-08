using System;
using System.Net.WebSockets;
using System.Text.Json;
using System.Text.Json.Serialization;
using Microsoft.AspNetCore.Http;

namespace web.Socket
{
    /// <summary>
    /// ws://localhost:5000/ws/Controller
    /// </summary>
    public class ControllerConnector : BaseConnector
    {
        public RobotConnector Robot;

        public ControllerConnector(ConnectionManager connectionManager) : base(connectionManager)
        {
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="msg"></param>
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
                        break;
                    case "ls":
                        ConnectionManager.CheckClients();
                        var robs = ConnectionManager.Robots;
                        SendData(JsonSerializer.Serialize(robs));
                        break;
                    case "set":
                        var rob = ConnectionManager.Robots.Find(r => r.Name == dat);
                        if (rob == null)
                            SendData($"{dat} not found");
                        else
                            Robot = rob;
                        break;
                    case "cmd":
                        if (Robot == null)
                            SendData("ERROR Robot not set!");
                        else
                            Robot.SendData(dat);
                        break;
                    default:
                        SendData($"Unknown Command: {cmd}");
                        Console.WriteLine($"[ControllerConnector] Unknown Command: {cmd}");
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