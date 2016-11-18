<?php 

define("serverName", "localhost");
define("userName", "root");
define("password", "nightsky");
define("dbName", "home");
define("TABLE", " device "); // need space between 'device'

function ConnectToSql() {
	$conn = mysqli_connect(serverName, userName, password);
	if (!$conn)
	    die("Connection failed: " . mysqli_connect_error()) . PHP_EOL;
	else;
		// echo "Connection to mysql success !" . PHP_EOL;

	return $conn;
}

function CloseSql($conn) {
	mysqli_close($conn);
}

// Create connection to database 'home'
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

// function create a table if not exist

function CreateTable($conn, $table) {

	$tableName = $table;

	$sql = "CREATE TABLE IF NOT EXISTS $tableName (
		id INT(32) UNSIGNED AUTO_INCREMENT PRIMARY KEY, 
		type VARCHAR(30) NOT NULL DEFAULT 'obj-slider',
		name VARCHAR(50) NOT NULL,
		state BOOLEAN NOT NULL DEFAULT 0,
		flavor VARCHAR(20) DEFAULT NULL,
		amplitude INT(6) DEFAULT 0,
		icon VARCHAR(15) DEFAULT 'fa-wrench'
	)";
	if (mysqli_query($conn, $sql)) {
	    echo "Table 'device' created successfully\n";
	} else {
	    echo "Error creating table: " . mysqli_error($conn);
	}
}

// function insert object to table
function InsertObject($conn, $objType, $objName, $state, $objFalvor, $amplitude, $icon="fa-wrench") {
	
	$sql = "INSERT INTO " . TABLE . " (type, name, state, flavor, amplitude, icon)
	VALUES ('$objType', '$objName', '$state', '$objFalvor', '$amplitude', '$icon')";
	// echo $sql . PHP_EOL;
	if ($conn->query($sql) === TRUE)
	    echo "Create a new object successfully" . PHP_EOL;
	else
	    echo "Error: " . $sql . " : " . $conn->error . PHP_EOL;
}

// Delete object 
// ex : DeleteObject($conn, "objName-light");
function DeleteObject($conn, $objName) {

	$sql = "DELETE FROM " . TABLE . " WHERE name='$objName' ";
	// echo $sql . PHP_EOL;
	if ($conn->query($sql) === TRUE)
	    echo "Record deleted successfully" . PHP_EOL;
	else
	    echo "Error deleting record: " . $conn->error . PHP_EOL;
}

// Update object
function UpdateObject($conn, $objName, $propChange) {

	$sql = "UPDATE " . TABLE . " SET $propChange WHERE name='$objName' ";
	echo $sql . PHP_EOL;
	if ($conn->query($sql) === TRUE)
	    echo "Record updated successfully" . PHP_EOL ;
	else
	    echo "Error updating record: " . $conn->error . PHP_EOL;
}


?>