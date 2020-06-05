using Microsoft.AspNetCore.Mvc;

namespace web.Controllers
{
    [Route("api/Controller")]
    [ApiController]
    public class Controller
    {
        // GET api/values/5
        [HttpGet("{id}")]
        public ActionResult<string> Get(int id)
        {
            return $"value {id}";
        }
    }
}