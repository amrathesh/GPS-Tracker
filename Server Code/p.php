/** @file p.php
 *  @brief Server side PHP script
 *
 *  This script runs at server while a client device sends
 *  a HTTP payload with required inforamation, In this case
 *  it is location of the client device.

 *  Note:
 *    1. Update the database credentials before using this script.
 *    2. If you plan to rename this file name, do update the AT+HTTPPARA=\"URL\" 
 *       AT command in Arduino client code.
 *        
 *  @author Amrathesh
 */

<?php

// Takes raw data from the request
$json = file_get_contents('php://input');
// Converts it into a PHP object
$data = json_decode($json);

echo $data->devid;
echo "\n";
echo $data->lat;
echo "\n";
echo $data->lon;
echo "\n";

$deviceId = $data->devid;
$latitude = $data->lat;
$longitude= $data->lon;

//TODO update following fields with your database credentials and server URL if database is hosted separately 
$servername = "localhost";
$username = "your_db_username";
$password = "your_db_password";
$database = "your_db_name";

$conn = mysqli_connect($servername, $username, $password, $database);

if (!$conn) {
    die("Connection failed: " . mysqli_connect_error());
}
echo "Connected successfully <br>";

// Below code will work only if there is database created with required table with elements
// Refer README for sample structure

$query = sprintf("SELECT EXISTS(SELECT * FROM details WHERE id=%s)",$deviceId);
echo "SQL> " . $query . "<br>";
$result = mysqli_fetch_array(mysqli_query($conn,$query))[0];

if($result == 1)
{
  $query = sprintf("UPDATE details SET latitude=%s, logitude=%s WHERE id=%s",$latitude,$longitude,$deviceId);
  echo "SQL> " . $query . "<br>";
  mysqli_query($conn,$query);
}

?>
