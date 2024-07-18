<?php

include 'db.php';

$sql = "SELECT 
    SUM(CASE WHEN STATUS = 'F_ON' THEN duration_hours ELSE 0 END) AS timeOn,
    SUM(CASE WHEN STATUS = 'F_OFF' THEN duration_hours ELSE 0 END) AS timeOff
FROM (
    SELECT 
        STATUS,
        TIMESTAMPDIFF(SECOND, TIME, COALESCE(
            LEAD(TIME) OVER (ORDER BY TIME),
            NOW()
        )) / 3600.0 AS duration_hours
    FROM fan_status_log
    WHERE TIME >= NOW() - INTERVAL 1 DAY
) AS durations;";

$result = $conn->query($sql);

if ($result->num_rows > 0) { 
  $row = $result->fetch_assoc();

  $On = $row['timeOn'];
  $Off = $row['timeOff'];  

} else {
  $On = "0";
  $Off = "0";
}

$response = array(
    "timeOn" => $On,
    "timeOff" => $Off,
);

header('Content-Type: application/json');
$json_response = json_encode($response);

if (json_last_error() !== JSON_ERROR_NONE) {
    echo json_encode(array("error" => "JSON encoding error: " . json_last_error_msg()));
    exit;
}

echo $json_response;


?>