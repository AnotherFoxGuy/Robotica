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
        public string Name { get; set; }
        public string Type => GetType().ToString();
        
        internal readonly ConnectionManager ConnectionManager;
        private HttpContext _context;
        public WebSocket WebSocket;

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
            try
            {
                WebSocket = webSocket;
                _context = context;
                var buffer = new byte[1024 * 4];
                var result = await WebSocket.ReceiveAsync(buffer, CancellationToken.None);
                while (!result.CloseStatus.HasValue)
                {
                    HandleMessage(Encoding.Default.GetString(buffer.Take(result.Count).ToArray()).ToLower());
                    result = await WebSocket.ReceiveAsync(buffer, CancellationToken.None);
                }

                await WebSocket.CloseAsync(result.CloseStatus.Value, result.CloseStatusDescription,
                    CancellationToken.None);
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
            }
            OnClose();
        }

        public async void SendData(string dat, bool finalmsg = true)
        {
            if (WebSocket.State != WebSocketState.Open)
                return;
            var buf = Encoding.Default.GetBytes(dat);
            await WebSocket.SendAsync(buf, WebSocketMessageType.Text, finalmsg, CancellationToken.None);
        }
    }
}