<?php

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
        echo json_encode("FAILED TO OPEN DB");
		exit();
	}

	$input = json_decode(file_get_contents('php://input'));

	$id = decrypt($input->id, $input->pd);
	$pw = decrypt($input->pw, $input->pd);

	if(strlen($id) < 4 || strlen($pw)  < 4 ){
		echo json_encode("-2");
		exit();
	}
	$query = "SELECT * FROM login WHERE username='$id'";

	$sql = mysqli_query($con, $query);
	if(!$sql){ // connection error - no results
		echo json_encode("DB Server Connection Error");
		exit();
	}
	else if (mysqli_num_rows($sql) === 0)
	{
		$query = "INSERT INTO login VALUES ('$id', MD5('$pw'));";
		$sql = mysqli_query($con, $query);
		if($sql)
			echo json_encode("1");
		else
			echo json_encode("DB Server Connection Error");
	}
	else{
		echo json_encode("Username Given Already Exists");
		exit();
	}
?>
