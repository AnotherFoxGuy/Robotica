using System;
using System.Collections.Generic;
using web.Socket;

namespace web
{
    public class ConnectionManager
    {
        public List<RobotConnector> Robots = new List<RobotConnector>();
        public List<ControlerConnector> Controlers = new List<ControlerConnector>();

        public void Register(RobotConnector conn)
        {
            Robots.Add(conn);
        }

        public void Register(ControlerConnector conn)
        {
            Controlers.Add(conn);
        }

        public void Deregister(RobotConnector conn)
        {
            Robots.Remove(conn);
        }

        public void Deregister(ControlerConnector conn)
        {
            Controlers.Remove(conn);
        }
    }
}