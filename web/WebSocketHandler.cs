using System;
using System.Linq;
using System.Net.WebSockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Http;

namespace web
{
    public class WebSocketHandler
    {
        public async Task handleClient(HttpContext context, WebSocket webSocket)
        {
            var buffer = new byte[1024 * 4];
            var result = await webSocket.ReceiveAsync(buffer, CancellationToken.None);
            while (!result.CloseStatus.HasValue)
            {
                var ksk = Encoding.Default.GetString(buffer.Take(result.Count).ToArray()).ToLower();
                Console.Write($"|{ksk}|");
                switch (ksk)
                {
                    case "bounce":
                        await sendData(webSocket, result, ksk);
                        break;
                    default:
                        await sendData(webSocket, result, "Error");
                        break;
                }

                result = await webSocket.ReceiveAsync(buffer, CancellationToken.None);
            }

            await webSocket.CloseAsync(result.CloseStatus.Value, result.CloseStatusDescription, CancellationToken.None);
        }

        private async Task sendData(WebSocket ws, WebSocketReceiveResult result, string dat)
        {
            var buf = Encoding.Default.GetBytes(dat);
            await ws.SendAsync(buf, result.MessageType,
                result.EndOfMessage, CancellationToken.None);
        }
    }
}