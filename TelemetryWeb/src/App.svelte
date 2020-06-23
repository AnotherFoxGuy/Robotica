<div class="container-fluid">
    <div class="card-deck">
        <div class="card">
            <div class="card-header">temps</div>
            <div class="card-body">
                {#each temps as t}
                    <p>Temp: {t}</p>
                {/each}
            </div>
        </div>
    </div>
    <div class="card-deck">
        <div class="card">
            <div class="card-header">Weight</div>
            <div class="card-body">
                <GraphLine points={_weightHistory}/>
            </div>
        </div>
    </div>
    <div class="card-deck">
        <div class="card">
            <div class="card-header">compass</div>
            <div class="card-body">
                <GraphLine points={_compassHistory}/>
            </div>
        </div>
    </div>
    <div class="card-deck">
        <div class="card">
            <div class="card-header">Select robot</div>
            <div class="card-body">
                <button type="button" class="btn btn-outline-primary dropdown-toggle" on:click={update_robot_list}>Robot
                    keuze
                </button>
                <div id="select_robot">
                    {#each _robots as r}
                        <button type="button" on:click={select_robot(r.Name)}>{r.Name}</button>
                    {/each}
                </div>
            </div>
        </div>
    </div>
</div>

<script>
    import {onMount} from 'svelte'
    import GraphLine from "./Components/GraphLine"

    var _robots = []

    var websocket

    var temps = []

    var _weightHistory = [0]
    $: if (_weightHistory.length >= 100) {
        _weightHistory.shift()
    }

    var _compassHistory = [0]
    $: if (_compassHistory.length >= 100) {
        _compassHistory.shift()
    }

    function update_robot_list() {
        if (websocket.readyState !== WebSocket.OPEN)
            return
        websocket.send("ls")
    }

    function select_robot(robot_name) {
        websocket.send("set|" + robot_name);
    }

    function onMessage(evt) {
        //console.log('RESPONSE: ' + evt.data)
        if (evt.data.includes("temps")) {
            temps = JSON.parse(evt.data).temps
        } else if (evt.data.includes("{")) {
            _robots = JSON.parse(evt.data)
        } else {
            try {
                const d = evt.data.split('=')
                const cmd = d[0]
                const dat = d.length !== 1 ? d[1] : "NOT SET"
                switch (cmd) {
                    case "weight":
                        _weightHistory = [..._weightHistory, dat]
                        break
                    case "compass":
                        _compassHistory = [..._compassHistory, dat]
                        break
                    default:
                        console.warn(`Unknown Command: ${cmd}`)
                        break
                }
            } catch (e) {
                console.error(e)
            }
        }
    }

    function onOpen(evt) {
        console.log("CONNECTED")
        let name = localStorage.getItem("name")
        if (name == null) {
            name = Math.random().toString(36).substring(2, 15) + Math.random().toString(36).substring(2, 15)
            localStorage.setItem("name", name)
        }
        websocket.send("register|" + name)
    }

    function onClose(evt) {
        console.log("DISCONNECTED")
    }

    function onError(evt) {
        console.log('ERROR:' + evt.data)
    }

    // setInterval(e => {
    //     _weightHistory = [..._weightHistory, Math.floor(Math.random() * Math.floor(5000))]
    // }, 1)
    //
    // setInterval(e => {
    //     _compassHistory = [..._compassHistory, Math.floor(Math.random() * Math.floor(360))]
    // }, 1)

    onMount(() => {
        websocket = new WebSocket("ws://vps.anotherfoxguy.com:5000/ws/Telemetry")
        websocket.onopen = function (evt) {
            onOpen(evt)
        }
        websocket.onclose = function (evt) {
            onClose(evt)
        }
        websocket.onmessage = function (evt) {
            onMessage(evt)
        }
        websocket.onerror = function (evt) {
            onError(evt)
        }
    })
</script>

