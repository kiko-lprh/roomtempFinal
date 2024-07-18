<?php
include 'db.php';

$sql = "SELECT Celcius, Fahrenheit, Humidity FROM test1 ORDER BY Time DESC LIMIT 1";
$result = $conn->query($sql);

if ($result->num_rows > 0) { 
  
  $row = $result->fetch_assoc();
  
  $celc = $row['Celcius'];
  $fahr = $row['Fahrenheit'];
  $humi = $row['Humidity'];
  
} else {
  $celc = "n/a";
  $fahr = "n/a";
  $humi = "n/a";
}

$response = array(
    "celcius" => $celc,
    "fahrenheit" => $fahr,
    "humidity" => $humi,
);

header('Content-Type: application/json');
$json_response = json_encode($response);

if (json_last_error() !== JSON_ERROR_NONE) {
    echo json_encode(array("error" => "JSON encoding error: " . json_last_error_msg()));
    exit;
}

echo $json_response;
?>
