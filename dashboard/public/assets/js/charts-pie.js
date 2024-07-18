/**
 * For usage, visit Chart.js docs https://www.chartjs.org/docs/latest/
 */
const pieConfig = {
  type: 'doughnut',
  data: {
    datasets: [
      {
        data: [], // Data will be updated dynamically
        /**
         * These colors come from Tailwind CSS palette
         * https://tailwindcss.com/docs/customizing-colors/#default-color-palette
         */
        backgroundColor: ['#0694a2', '#1c64f2'],
        label: 'Fan Status',
      },
    ],
    labels: ['On', 'Off'],
  },
  options: {
    responsive: true,
    cutoutPercentage: 80,
    /**
     * Default legends are ugly and impossible to style.
     * See examples in charts.html to add your own legends
     */
    legend: {
      display: false,
    },
  },
};

// Fetch data from the PHP endpoint and update the chart
fetch('pie.php')
  .then(response => response.json())
  .then(data => {
    const timeOn = data.timeOn;
    const timeOff = data.timeOff;

    // Update chart data
    pieConfig.data.datasets[0].data = [timeOn, timeOff];

    // Render the chart
    const pieCtx = document.getElementById('pie');
    window.myPie = new Chart(pieCtx, pieConfig);
  })
  .catch(error => {
    console.error('Error fetching data:', error);
  });
