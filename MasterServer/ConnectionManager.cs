using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.WebSockets;
using web.Socket;

namespace web
{
    public class ConnectionManager
    {
        public readonly Dictionary<string, List<string>> Connections = new Dictionary<string, List<string>>();
        public readonly Dictionary<string, BaseConnector> RegisteredConnectors = new Dictionary<string, BaseConnector>();
        
        public void PruneOldData()
        {
            foreach (var r in RegisteredConnectors.Where(r => r.Value.WebSocket.State != WebSocketState.Open))
                RegisteredConnectors.Remove(r.Key);
        }

        public void Register(BaseConnector c)
        {
            if (!RegisteredConnectors.ContainsKey(c.Name))
                RegisteredConnectors.Add(c.Name, c);
            else
                RegisteredConnectors[c.Name] = c;
        }

        public void Deregister(BaseConnector connector)
        {
            RegisteredConnectors.Where(c => c.Value == connector).
                ToList().
                ForEach(e =>
                    RegisteredConnectors.Remove(e.Key)
            );
        }

        public void SetRobot(BaseConnector controller, string robotName)
        {
            var robot = RegisteredConnectors.Single(r => r.Key == robotName).Value;
            if (!Connections.ContainsKey(controller.Name))
                Connections.Add(controller.Name, new List<string> {robot.Name});
            else if (!Connections[controller.Name].Contains(robot.Name))
                Connections[controller.Name].Add(robot.Name);
        }

        public void SendData(BaseConnector from, string dat)
        {
            if (!Connections.ContainsKey(from.Name))
                from.SendData("ERROR Robot not set!");
            else
                Connections[from.Name].ForEach(r => RegisteredConnectors[r].SendData(dat));
        }
    }
}