using System.Collections.Generic;
using System.Linq;
using System.Net.WebSockets;
using web.Socket;

namespace web
{
    public class ConnectionManager
    {
        public readonly Dictionary<BaseConnector, List<BaseConnector>> Connections =
            new Dictionary<BaseConnector, List<BaseConnector>>();

        public readonly Dictionary<string, ControllerConnector> Controllers =
            new Dictionary<string, ControllerConnector>();

        public readonly Dictionary<string, RobotConnector> Robots = new Dictionary<string, RobotConnector>();

        public readonly Dictionary<string, TelemetryConnector> TelemetrySites =
            new Dictionary<string, TelemetryConnector>();

        public void PruneOldData()
        {
            foreach (var r in Robots.Where(r => r.Value.WebSocket.State != WebSocketState.Open))
                Robots.Remove(r.Key);
            foreach (var r in Controllers.Where(r => r.Value.WebSocket.State != WebSocketState.Open))
                Controllers.Remove(r.Key);
            foreach (var r in TelemetrySites.Where(r => r.Value.WebSocket.State != WebSocketState.Open))
                TelemetrySites.Remove(r.Key);
        }

        public void Register(RobotConnector c)
        {
            if (!Robots.ContainsKey(c.Name))
                Robots.Add(c.Name, c);
            else
                Robots[c.Name] = c;
        }

        public void Register(ControllerConnector c)
        {
            if (!Controllers.ContainsKey(c.Name))
                Controllers.Add(c.Name, c);
            else
                Controllers[c.Name] = c;
        }

        public void Register(TelemetryConnector c)
        {
            if (!TelemetrySites.ContainsKey(c.Name))
                TelemetrySites.Add(c.Name, c);
            else
                TelemetrySites[c.Name] = c;
        }

        public void Deregister(RobotConnector c)
        {
            foreach (var connection in Connections)
                connection.Value.RemoveAll(r => r == c);
        }

        public void Deregister(ControllerConnector c)
        {
            foreach (var connection in Connections)
                connection.Value.RemoveAll(r => r == c);
        }

        public void Deregister(TelemetryConnector c)
        {
            foreach (var connection in Connections)
                connection.Value.RemoveAll(r => r == c);
        }

        public void SetRobot(ControllerConnector c, string d)
        {
            var rob = Robots.Single(r => r.Key == d).Value;
            if (!Connections.ContainsKey(c))
                Connections.Add(c, new List<BaseConnector> {rob});
            else if (!Connections[rob].Contains(c))
                Connections[c].Add(rob);
        }

        public void SetRobot(TelemetryConnector c, string d)
        {
            var rob = Robots.Single(r => r.Key == d).Value;
            if (!Connections.ContainsKey(rob))
                Connections.Add(rob, new List<BaseConnector> {c});
            else if (!Connections[rob].Contains(c))
                Connections[rob].Add(c);
        }

        public void SendData(BaseConnector from, string dat)
        {
            if (!Connections.ContainsKey(from))
                from.SendData("ERROR Robot not set!");
            else
                Connections[from].ForEach(r => r.SendData(dat));
        }
    }
}