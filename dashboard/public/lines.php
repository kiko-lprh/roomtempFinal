<?php
include 'db.php'; 

$sql = "SELECT DATE(Time) AS date,
               AVG(Fahrenheit) AS avg_temperature
        FROM test1
        WHERE Time >= CURDATE() - INTERVAL 6 DAY
        GROUP BY DATE(Time)
        ORDER BY DATE(Time) ASC";

$result = $conn->query($sql);

// Initialize an array to store each day's data
$dayData = array();

if ($result->num_rows > 0) { 
    // Fetch associative array
    while ($row = $result->fetch_assoc()) {
        // Store date and average temperature in the array
        $dayData[] = array(
            "date" => $row['date'],
            "avg_temperature" => $row['avg_temperature']
        );
    }
} else {
    // If no rows returned, initialize an empty array
    $dayData = array();
}

// Prepare response as an associative array
$response = array(
    "dayData" => $dayData
);

// Set response headers and encode response as JSON
header('Content-Type: application/json');
$json_response = json_encode($response);

// Check for JSON encoding errors
if (json_last_error() !== JSON_ERROR_NONE) {
    echo json_encode(array("error" => "JSON encoding error: " . json_last_error_msg()));
    exit;
}

echo $json_response;

// Close the database connection
$conn->close();
?>
