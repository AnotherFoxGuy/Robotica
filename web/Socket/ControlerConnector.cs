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
    public class ControlerConnector : BaseConnector
    {
        public RobotConnector Robot;

        public ControlerConnector(ConnectionManager connectionManager) : base(connectionManager)
        {
        }

        protected override void HandleMessage(string msg)
        {
            try
            {
                var d = msg.Split('|');
                var dat = d[0];
                var cmd = d.Length != 1 ? d[1]: "NOT SET";
                switch (dat)
                {
                    case "register":
                        ConnectionManager.Register(this);
                        break;
                    case "ls":
                        var robs = ConnectionManager.Robots;
                        SendData(JsonSerializer.Serialize(robs));
                        break;
                    case "set":
                        var rob = ConnectionManager.Robots.Find(r => r.Name == cmd);
                        if (rob == null)
                            SendData($"{cmd} not found");
                        else
                            Robot = rob;
                        break;
                    case "cmd":
                        if (Robot == null)
                            SendData("ERROR Robot not set!");
                        else
                            Robot.SendData(cmd);
                        break;
                    default:
                        SendData($"Unknown Command: {cmd}");
                        Console.Write($"RobotConnector |{cmd}|\n");
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