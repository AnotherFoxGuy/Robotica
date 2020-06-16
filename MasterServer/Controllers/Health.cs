using System.Text.Json;
using Microsoft.AspNetCore.Mvc;

namespace web.Controllers
{
    [Route("api/Health")]
    [ApiController]
    public class Health
    {
        [HttpGet]
        public ActionResult<string> Get([FromServices] ConnectionManager cm)
        {
            var r = JsonSerializer.Serialize(cm.Robots.Values);
            var c = JsonSerializer.Serialize(cm.Controllers.Values);
            var t = JsonSerializer.Serialize(cm.TelemetrySites.Values);
            var s = "[";
            s += $"\"Robots\": {r},";
            s += $"\"Controllers\": {c},";
            s += $"\"Telemetry\": {t}";
            return s + "]";
        }

        [HttpGet("Robots")]
        public ActionResult<string> GetRobots([FromServices] ConnectionManager cm)
        {
            return JsonSerializer.Serialize(cm.Robots);
        }

        [HttpGet("connections")]
        public ActionResult<string> GetConnections([FromServices] ConnectionManager cm)
        {
            return JsonSerializer.Serialize(cm.Connections);
        }
    }
}