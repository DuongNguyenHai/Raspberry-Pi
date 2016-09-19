<?php 

define("serverName", "localhost");
define("userName", "root");
define("password", "nightsky");
define("dbName", "home");
define("TABLE", " device "); // need space between 'device'

// Create connection
function ConnectDatabse() {
	$conn = mysqli_connect(serverName, userName, password, dbName);
	// Check connection
	if (!$conn)
	    die("Connection failed: " . mysqli_connect_error());
	else;
		// echo "Connection database success !\n";
	return $conn;
}

// Disconnect to database
function CloseDatabase($conn) {
	mysqli_close($conn);
}


function UpdateObject($conn, $objName, $propChange) {

	$sql = "UPDATE " . TABLE . " SET $propChange WHERE name='$objName' ";
	echo $sql . PHP_EOL;
	if ($conn->query($sql) === TRUE)
	    echo "Record updated successfully" . PHP_EOL ;
	else
	    echo "Error updating record: " . $conn->error . PHP_EOL;
}


?>