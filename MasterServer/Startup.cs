#define UseOptions // or NoOptions or UseOptionsAO
using Microsoft.AspNetCore.Builder;
using Microsoft.AspNetCore.Hosting;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;
using Microsoft.Extensions.Logging;
using Microsoft.Extensions.Logging.Console;
using Microsoft.Extensions.Logging.Debug;
using web.Socket;

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
                        .AddFilter<ConsoleLoggerProvider>(null, LogLevel.Debug)
                        .AddFilter<DebugLoggerProvider>(null, LogLevel.Debug);
                })
                .AddControllers();
            //services.AddTransient<ConnectionManager>();
            services.AddSingleton<ConnectionManager>();
        }

        // This method gets called by the runtime. Use this method to configure the HTTP request pipeline.
        public void Configure(IApplicationBuilder app, IWebHostEnvironment env, ILoggerFactory loggerFactory)
        {
            var connectionManager = (ConnectionManager) app.ApplicationServices.GetService(typeof(ConnectionManager));
            if (env.IsDevelopment()) app.UseDeveloperExceptionPage();

            app.UseRouting();
            app.UseWebSockets();

            app.UseEndpoints(endpoints => { endpoints.MapControllers(); });

            app.Use(async (context, next) =>
            {
                if (context.WebSockets.IsWebSocketRequest)
                {
                    var webSocket = await context.WebSockets.AcceptWebSocketAsync();
                    switch (context.Request.Path)
                    {
                        case "/ws/Robot":
                            await new RobotConnector(connectionManager).HandleClient(webSocket, context);
                            break;
                        case "/ws/Controller":
                            await new ControllerConnector(connectionManager).HandleClient(webSocket, context);
                            break;
                        case "/ws/Telemetry":
                            await new TelemetryConnector(connectionManager).HandleClient(webSocket, context);
                            break;
                        default:
                            context.Response.StatusCode = 400;
                            break;
                    }
                }
                else
                {
                    context.Response.StatusCode = 400;
                }
            });
        }
    }
}