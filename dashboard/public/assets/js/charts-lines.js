/**
 * For usage, visit Chart.js docs https://www.chartjs.org/docs/latest/
 */
const lineConfig = {
  type: 'line',
  data: {
    labels: [], // Labels will be updated dynamically
    datasets: [
      {
        label: 'Room Temperature ',
        /**
         * These colors come from Tailwind CSS palette
         * https://tailwindcss.com/docs/customizing-colors/#default-color-palette
         */
        backgroundColor: '#0694a2',
        borderColor: '#0694a2',
        data: [], // Data will be updated dynamically
        fill: false,
      },
    ],
  },
  options: {
    responsive: true,
    /**
     * Default legends are ugly and impossible to style.
     * See examples in charts.html to add your own legends
     */
    legend: {
      display: false,
    },
    tooltips: {
      mode: 'index',
      intersect: false,
    },
    hover: {
      mode: 'nearest',
      intersect: true,
    },
    scales: {
      x: {
        display: true,
        scaleLabel: {
          display: true,
          labelString: 'Date',
        },
      },
      y: {
        display: true,
        scaleLabel: {
          display: true,
          labelString: 'Temperature (°F)',
        },
      },
    },
  },
};

// Fetch data from the PHP endpoint and update the chart
fetch('lines.php')
  .then(response => response.json())
  .then(data => {
    const dates = data.dayData.map(entry => entry.date);
    const temperatures = data.dayData.map(entry => entry.avg_temperature);

    // Update chart data
    lineConfig.data.labels = dates;
    lineConfig.data.datasets[0].data = temperatures;

    // Render the chart
    const lineCtx = document.getElementById('line');
    window.myLine = new Chart(lineCtx, lineConfig);
  })
  .catch(error => {
    console.error('Error fetching data:', error);
  });
