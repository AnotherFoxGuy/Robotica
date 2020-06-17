using System;
using System.Linq;
using System.Text.Json;

namespace web.Socket
{
    /// <summary>
    ///     ws://localhost:5000/ws/Controller
    /// </summary>
    public class ControllerConnector : BaseConnector
    {
        public ControllerConnector(ConnectionManager connectionManager) : base(connectionManager)
        {
        }

        /// <summary>
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
                        Name = dat;
                        ConnectionManager.Register(this);
                        SendData($"Registered {Name}");
                        break;
                    case "ls":
                        var robs = ConnectionManager.RegisteredConnectors.Values.Where(c =>
                            c.GetType() == typeof(RobotConnector)
                        );
                        SendData(JsonSerializer.Serialize(robs));
                        break;
                    case "set":
                        ConnectionManager.SetRobot(this, dat);
                        break;
                    case "cmd":
                        ConnectionManager.SendData(this, dat);
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