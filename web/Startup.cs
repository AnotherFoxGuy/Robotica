#define UseOptions // or NoOptions or UseOptionsAO
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.WebSockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Builder;
using Microsoft.AspNetCore.Hosting;
using Microsoft.AspNetCore.Http;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Logging;
using Microsoft.Extensions.Logging.Console;
using Microsoft.Extensions.Logging.Debug;

namespace web
{
    public class Startup
    {
        // This method gets called by the runtime. Use this method to add services to the container.
        // For more information on how to configure your application, visit http://go.microsoft.com/fwlink/?LinkID=398940
        public void ConfigureServices(IServiceCollection services)
        {
            services.AddLogging(builder =>
                {
                    builder.AddConsole()
                        .AddDebug()
                        .AddFilter<ConsoleLoggerProvider>(category: null, level: LogLevel.Debug)
                        .AddFilter<DebugLoggerProvider>(category: null, level: LogLevel.Debug);
                })
                .AddControllers();
        }

        // This method gets called by the runtime. Use this method to configure the HTTP request pipeline.
        public void Configure(IApplicationBuilder app, IHostingEnvironment env, ILoggerFactory loggerFactory)
        {
            var websockethandler = new WebSocketHandler();

            if (env.IsDevelopment())
            {
                app.UseDeveloperExceptionPage();
            }

            app.UseRouting();
            app.UseWebSockets();

            app.Map("/ws", builder =>
            {
                builder.Use(async (context, next) =>
                {
                    if (context.WebSockets.IsWebSocketRequest)
                    {
                        var webSocket = await context.WebSockets.AcceptWebSocketAsync();
                        await websockethandler.handleClient(context, webSocket);
                        return;
                    }

                    await next();
                });
            });

            app.UseEndpoints(endpoints => { endpoints.MapControllers(); });

            /*app.Use(async (context, next) =>
            {
                if (context.WebSockets.IsWebSocketRequest)
                {
                    WebSocket webSocket = await context.WebSockets.AcceptWebSocketAsync();

                    switch (context.Request.Path)
                    {
                        case "/Robot/Register":
                            await websockethandler.RegisterRobot(context, webSocket);
                            break;
                        case "/Controller/Register":
                            await websockethandler.RegisterController(context, webSocket);
                            break;
                        default:
                            await websockethandler.Echo(context, webSocket);
                            break;
                    }
                }
                else
                {
                    context.Response.StatusCode = 400;
                }
            });*/
        }
    }
}