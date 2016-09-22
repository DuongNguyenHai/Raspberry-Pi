<!-- 	Nguyen Hai Duong, September 2016 
 			GNU LESSER GENERAL PUBLIC LICENSE Version 2.1, February 1999
-->

<?php 

include 'function/print-HTML.php';
include 'sql/sql-function.php';

$conn = ConnectDatabse();

?>

<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no" />
    <!-- The above 3 meta tags *must* come first in the head; any other head content must come *after* these tags -->
    <title>Lazy man</title>
    
    <!-- CSS -->
    <link rel="stylesheet" href="https://fonts.googleapis.com/css?family=Open+Sans">
    <link rel="stylesheet" type="text/css" href="fonts/font-awesome/css/font-awesome.min.css">
    <!-- Bootstrap -->
    <link rel="stylesheet" type="text/css" href="css/bootstrap.min.css">
    <link rel="stylesheet" type="text/css" href="css/jquery-ui.min.css">
    <link rel="stylesheet" type="text/css" href="css/index.css">
    
    <!-- JS -->
    <script type="text/javascript" src="js/jquery/jquery.js"></script>
    <script type="text/javascript" src="js/jquery/jquery-ui.min.js"></script>
    <script type="text/javascript" src="js/jquery.ui.touch-punch.min.js"></script>
    <script type="text/javascript" src="js/query.js"></script>
    
    <!-- HTML5 shim and Respond.js for IE8 support of HTML5 elements and media queries -->
    <!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
    <!--[if lt IE 9]>
      <script src="https://oss.maxcdn.com/html5shiv/3.7.3/html5shiv.min.js"></script>
      <script src="https://oss.maxcdn.com/respond/1.4.2/respond.min.js"></script>
    <![endif]-->
  </head>
  <body class="lazy-man">
    <!-- Fixed navbar -->
    <div class="container"></div>
    <nav class="navbar navbar-inverse navbar-fixed-top">
      <div class="container">
        <div class="navbar-header">
        </div>
      </div>
    </nav>
    <!-- Conainer -->
    <div class="container">

      <div class="row">
        <div class="land-1">
<?php

	PrintObjectDatabase($conn);
	PrintObjectSend();

?>
          <div class="clearfix"></div>  
        </div>
      </div>
    </div>
	  <div class="log-box alert alert-danger" role="alert">
			<strong>Woop !</strong>
			<p class="log-text">test demo alert log</p>
		</div>
  </body>
</html>

<?php 
	CloseDatabase($conn);
?>