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
        public void PurgeData()
        {
            Connections.Clear();
            RegisteredConnectors.Clear();
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
        public void SetRobot(ControllerConnector controller, string robotName)
        {
            var robot = RegisteredConnectors.Single(r => r.Key == robotName).Value;
            if (!Connections.ContainsKey(controller.Name))
                Connections.Add(controller.Name, new List<string> {robot.Name});
            else if (!Connections[controller.Name].Contains(robot.Name))
                Connections[controller.Name].Add(robot.Name);
        }

        public void SetRobot(TelemetryConnector telemetry, string robotName)
        {
            var rob = RegisteredConnectors.Single(r => r.Key == robotName).Value;
            if (!Connections.ContainsKey(rob.Name))
                Connections.Add(rob.Name, new List<string> {telemetry.Name});
            else if (!Connections[rob.Name].Contains(telemetry.Name))
                Connections[rob.Name].Add(telemetry.Name);
        }
        
        public void SendData(BaseConnector from, string dat)
        {
            if (Connections.ContainsKey(from.Name))
                Connections[from.Name].ForEach(r => RegisteredConnectors[r].SendData(dat));
        }

    }
}