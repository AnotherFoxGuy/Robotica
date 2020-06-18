using System.Text.Json;
using Microsoft.AspNetCore.Mvc;

namespace web.Controllers
{
    [Route("api/Cleanup")]
    [ApiController]
    public class Cleanup
    {
        [HttpGet("prune")]
        public ActionResult<string> prune([FromServices] ConnectionManager cm)
        {
            cm.PruneOldData();
            return "done";
        }
        
        [HttpGet("purge")]
        public ActionResult<string> purge([FromServices] ConnectionManager cm)
        {
            cm.PurgeData();
            return "done";
        }
    }
}
