using Microsoft.AspNetCore.Mvc;

namespace web.Controllers
{
    [Route("api/Robot")]
    [ApiController]
    public class Robot
    {
        // GET api/values/5
        [HttpGet("{id}")]
        public ActionResult<string> Get(int id)
        {
            return $"value {id}";
        }
    }
}