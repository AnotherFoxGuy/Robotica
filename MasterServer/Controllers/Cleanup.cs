using System.Text.Json;
using Microsoft.AspNetCore.Mvc;

namespace web.Controllers
{
    [Route("api/Cleanup")]
    [ApiController]
    public class Cleanup
    {
        [HttpGet("prune")]
        public ActionResult<string> Get([FromServices] ConnectionManager cm)
        {
            cm.PruneOldData();
            return "done";
        }
    }
}
