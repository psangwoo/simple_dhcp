<?php 
/*
 *
 * REGISTERATION API CALL 
 * NEEDED PARAMETERS IN BODY : ID, PW 
 * RETURN 0 IF SAME ID EXISTS IN DB
 * RETURN 1 IF CREATION IS WELL-DONE
 */
	header('Access-Control-Allow-Origin: http://192.168.0.2');

	$db_host='localhost';
	$db_user='root';
	$db_password='1234';
	$db_name='dhcpark';

	$con=mysqli_connect($db_host, $db_user, $db_password, $db_name);
	if(mysqli_connect_error()){
		echo json_encode("FAILED TO OPEN DB");
		exit();
	}

	$input = json_decode(file_get_contents('php://input'));
	$method = getenv("REQUEST_METHOD");	
	$username = $_SERVER['QUERY_STRING'];

	if ($method === "PUT"){
		$query = "UPDATE config SET 
			subnet = '$input->subnet',
			minrange = $input->minrange,
			maxrange = $input->maxrange,
			domain = '$input->domain',
			dns = '$input->dns',
			leasetime = $input->leasetime,
			mask = '$input->mask',
			gateway = $input->gateway,
			broadcast = $input->broadcast,
			username = '$username'
			WHERE subnet = '$input->subnet' and username = '$username';";
	}
	else if($method === "POST"){
		$query = "INSERT INTO config VALUES ('$input->subnet', $input->minrange, $input->maxrange, '$input->domain', '$input->dns', $input->leasetime, '$input->mask', $input->gateway, $input->broadcast, '$username');";
	}
	$sql = mysqli_query($con, $query);

	if($sql){
			echo json_encode('1');
	}
	else{
		echo json_encode($query);	
	}
	exit();		
?>
