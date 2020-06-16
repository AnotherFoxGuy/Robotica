<script>
    import {scaleLinear} from 'd3-scale';

    export var points = []

    const padding = {top: 20, right: 15, bottom: 20, left: 25};

    let width = 500;
    let height = 200;

    $: minVal = Math.min(...points)
    $: maxVal = Math.max(...points)

    $: xScale = scaleLinear()
            .domain([0, points.length])
            .range([padding.left, width - padding.right])

    $: yScale = scaleLinear()
            .domain([minVal, maxVal])
            .range([height - padding.bottom, padding.top])

    $: path = `M${points.map((p, i) => `${xScale(i)},${yScale(p)}`).join('L')}`

</script>

<p>Max: {maxVal}</p>
<p>Min: {minVal}</p>

<div class="chart" bind:clientWidth={width} bind:clientHeight={height}>
    <svg>
        <!-- data -->
        <path class="path-line" d={path}></path>
    </svg>
</div>

<style>
    .chart, h2 {
        width: 100%;
        max-width: 500px;
        margin-left: auto;
        margin-right: auto;
    }

    svg {
        position: relative;
        width: 100%;
        height: 200px;
        overflow: visible;
    }

    .path-line {
        fill: none;
        stroke: rgb(0, 100, 100);
        stroke-linejoin: round;
        stroke-linecap: round;
        stroke-width: 2;
    }
</style>
