<?php 
/*
 *
 * LOGIN API CALL 
 * NEEDED PARAMETERS IN BODY : ID, PW 
 * RETURN 0 PASSWORD DOES NOT MATCH
 * RETURN SUBNET INFO OF CONFIG USER CREATED WHEN SUCCESSFUL QUERIED
 */
	header('Access-Control-Allow-Origin: http://192.168.0.2');

	$arr = array();

	$inputstring = $_SERVER['QUERY_STRING'];

	parse_str($inputstring, $input);
	$username = $input['username'];
	$method = strtolower( $input['method'] );
	if(!strcmp($method, "run")){
		exec("sudo -S sh /var/www/shell/makeconfig.sh $username");
		exec("sudo -S sh /var/www/shell/makelease.sh $username");
		exec("sudo -S /var/www/server/dhcpark >/dev/null &");
		echo json_encode('1');
		exit();
	}
	else if(!strcmp($method, "stop")){
		exec("sudo -S sh /var/www/shell/stopdhcpark.sh");
		echo json_encode('1');
		exit();
	}
	else if(!strcmp($method, "status")){
		exec("sudo -S sh /var/www/shell/dhcparkstatus.sh", $output, $error);
		echo json_encode($output);
		exit();
	}
	echo json_encode('0');
	exit();
?>
