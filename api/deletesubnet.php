<?php 
/*
 *
 * LOGIN API CALL 
 * NEEDED PARAMETERS IN BODY : ID, PW 
 * RETURN 0 PASSWORD DOES NOT MATCH
 * RETURN SUBNET INFO OF CONFIG USER CREATED WHEN SUCCESSFUL QUERIED
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
	$arr = array();
	$inputstring = $_SERVER['QUERY_STRING'];
	parse_str($inputstring, $input);
	$username = $input['username'];
	$subnet = $input['subnet'];
	$query = "DELETE FROM config WHERE username='$username' AND subnet = '$subnet'";
	$sql = mysqli_query($con, $query);

	if($sql){
		echo json_encode('1');
	}
	else{
		echo json_encode('-1');
	}
	exit();		
?>
