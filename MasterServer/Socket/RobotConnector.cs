using System;

namespace web.Socket
{
    /// <summary>
    ///     ws://localhost:5000/ws/Robot
    /// </summary>
    public class RobotConnector : BaseConnector
    {
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
                        Name = dat;
                        ConnectionManager.Register(this);
                        SendData($"Registered {Name}");
                        break;
                    case "data":
                        ConnectionManager.SendData(this, dat);
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