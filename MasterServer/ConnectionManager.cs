using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.WebSockets;
using web.Socket;

namespace web
{
    public class ConnectionManager
    {
        public List<RobotConnector> Robots = new List<RobotConnector>();
        public List<ControllerConnector> Controlers = new List<ControllerConnector>();

        public void CheckClients()
        {
            Robots.RemoveAll(r => r.WebSocket.State != WebSocketState.Open);
            Controlers.RemoveAll(r => r.WebSocket.State != WebSocketState.Open);
        }

        public void Register(RobotConnector conn)
        {
            Robots.Add(conn);
        }

        public void Register(ControllerConnector conn)
        {
            Controlers.Add(conn);
        }

        public void Deregister(RobotConnector conn)
        {
            Robots.Remove(conn);
        }

        public void Deregister(ControllerConnector conn)
        {
            Controlers.Remove(conn);
        }
    }
}