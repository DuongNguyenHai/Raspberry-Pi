<?php 
	
	include 'client.php';
	include '../sql/sql-function.php';
	
	$conn = ConnectDatabse();	// connect to database

	if( $_REQUEST["type"] ) {

		if($_REQUEST["type"]=="update") {

			$name = $_REQUEST["name"];
			$str = $_REQUEST["update"];
			
			// update to database
			UpdateObject($conn, $name, $str);

			// send message to master via tcp/ip
			if( $str!="timer=NULL" ) {
				$message = $name . ':' . $str;
				SendCommandToMaster($message);
			}
		}
		else if($_REQUEST["type"]=="status") {
			$str = $_REQUEST["state"];
			SendCommandToMaster($str);
		}
	}
	
	CloseDatabase($conn);
	
?>