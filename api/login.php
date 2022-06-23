<?php 
/*
 *
 * LOGIN API CALL 
 * NEEDED PARAMETERS IN BODY : ID, PW 
 * RETURN 0 PASSWORD DOES NOT MATCH
 * RETURN 1 SUCCESSFUL LOGIN 
 */

function decrypt($ciphertext, $pd){
	$ret = [];
	forEach($ciphertext as $ch){
		array_push($ret, chr($ch / $pd));
	}
	return implode("", $ret);
}
	header('Access-Control-Allow-Origin: http://192.168.0.2');

	$db_host='localhost';
	$db_user='root';
	$db_password='1234';
	$db_name='dhcpark';

	$con=mysqli_connect($db_host, $db_user, $db_password, $db_name);
	if(mysqli_connect_error()){
		echo json_encode("DB Server Connection Error");
		exit();
	}

	$input = json_decode(file_get_contents('php://input'));
	$id = decrypt($input->id, $input->pd);
	$pw = decrypt($input->pw, $input->pd);
	$query = "SELECT * FROM login WHERE username = '$id' AND password = MD5('$pw')";
	$sql = mysqli_query($con, $query);
	if($sql)
	{
	   if(mysqli_num_rows($sql))
		   echo json_encode('1');
	   else
		   echo json_encode("Check your Username or Password and Retry");
	}
	else	
		echo json_encode("DB Server Connection Error");
	exit();		
?>
