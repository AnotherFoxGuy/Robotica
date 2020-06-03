using System.Net.WebSockets;
using Microsoft.AspNetCore.Http;

namespace web.Socket
{
    public class TelemeryConnector : BaseConnector
    {
        public TelemeryConnector(ConnectionManager connectionManager) : base(connectionManager)
        {
        }
    }
}