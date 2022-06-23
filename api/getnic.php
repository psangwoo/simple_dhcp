<?php


header('Access-Control-Allow-Origin: http://192.168.0.2');

$all = net_get_interfaces();
$ret = array();

foreach ( $all as $nic ) {
	if ($nic['unicast'][1]['address'] !== null)
		array_push($ret, array('name' => array_search($nic, $all), 'addr' => $nic['unicast'][1]['address'], 'netmask' => $nic['unicast'][1]['netmask'], 'broadcast' => $nic['unicast'][1]['broadcast']));
}
echo json_encode($ret);
exit();
?>

