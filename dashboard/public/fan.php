<?php
include 'db.php';

$sql = "SELECT TIME, STATUS
        FROM fan_status_log 
        ORDER BY Time DESC 
        LIMIT 1";
$result = $conn->query($sql);

if ($result->num_rows > 0) { 
  $row = $result->fetch_assoc();
  $time = $row['TIME'];
  $sta = $row['STATUS'];  
} else {
  $time = "n/a";
  $sta = "n/a";
}

if ($sta == 'F_ON'){
    $sta = 'On';
} else if ($sta == "F_OFF"){
    $sta = 'Off';
}

$response = array(
    "time" => $time,
    "status" => $sta,
);

header('Content-Type: application/json');
$json_response = json_encode($response);

if (json_last_error() !== JSON_ERROR_NONE) {
    echo json_encode(array("error" => "JSON encoding error: " . json_last_error_msg()));
    exit;
}

echo $json_response;
?>
