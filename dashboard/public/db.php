<?php

$host = "placeholder";
$user = "placeholder";
$pass = "placeholder";
$db = "placeholder";


$conn = mysqli_connect($host, $user, $pass, $db);


if (!$conn) {
  die("Connection  failed: " . mysqli_connect_error());
}

?>