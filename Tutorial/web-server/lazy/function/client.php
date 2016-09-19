<?php 


// Connect to master via tcp/ip
function ConnectToMaster() {

	if(!($sock = socket_create(AF_INET, SOCK_STREAM, 0))) {
	    $errorcode = socket_last_error();
	    $errormsg = socket_strerror($errorcode);
	     
	    die("Couldn't create socket: [$errorcode] $errormsg \n");
	}
	 
	// echo "Socket created \n";
	 
	if(!socket_connect($sock , '127.0.0.1' , 8888)) {
	    $errorcode = socket_last_error();
	    $errormsg = socket_strerror($errorcode);
	     
	    die("Could not connect to Master: [$errorcode] $errormsg \n");
	}
	// echo "Connection established \n";
	return $sock;
}

// Send the message to the server
function SendCommandToMaster($message) {
	$sock = ConnectToMaster();
	if( ! socket_send ( $sock , $message , strlen($message) , 0)) {
	    $errorcode = socket_last_error();
	    $errormsg = socket_strerror($errorcode);
	    
	    die("Could not send data to Master: [$errorcode] $errormsg \n");
	}
	// echo "Message send successfully \n";
	DisconnectToMaster($sock);
}
 
function DisconnectToMaster($sock) {
	socket_close($socket);
}



?>
