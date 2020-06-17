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
            return JsonSerializer.Serialize(cm.RegisteredConnectors.Values);
        }
        [HttpGet("connections")]
        public ActionResult<string> GetConnections([FromServices] ConnectionManager cm)
        {
            return JsonSerializer.Serialize(cm.Connections);
        }
    }
}