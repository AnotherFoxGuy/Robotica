using System;
using System.Linq;
using System.Net.WebSockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;

namespace web.Socket
{
    public class BaseConnector
    {
        internal readonly ConnectionManager ConnectionManager;
        private WebSocket _webSocket;
        private HttpContext _context;
        public WebSocket WebSocket => _webSocket;

        public BaseConnector(ConnectionManager connectionManager)
        {
            ConnectionManager = connectionManager;
        }

        protected virtual void HandleMessage(string msg)
        {
            Console.Write($"|{msg}|");
            SendData(msg);
        }
        
        protected virtual void OnClose()
        {
        }

        public async Task HandleClient(WebSocket webSocket, HttpContext context)
        {
            _webSocket = webSocket;
            _context = context;
            var buffer = new byte[1024 * 4];
            var result = await _webSocket.ReceiveAsync(buffer, CancellationToken.None);
            while (!result.CloseStatus.HasValue)
            {
                HandleMessage(Encoding.Default.GetString(buffer.Take(result.Count).ToArray()).ToLower());
                result = await _webSocket.ReceiveAsync(buffer, CancellationToken.None);
            }
            await _webSocket.CloseAsync(result.CloseStatus.Value, result.CloseStatusDescription,
                CancellationToken.None);
            OnClose();
        }

        public async void SendData(string dat, bool finalmsg = true)
        {
            if(_webSocket.State != WebSocketState.Open)
                return;
            var buf = Encoding.Default.GetBytes(dat);
            await _webSocket.SendAsync(buf, WebSocketMessageType.Text, finalmsg, CancellationToken.None);
        }
    }
}