<?php  // $Id: dbinfo.inc 16 2008-04-10 04:49:42Z jheddings $

// DB info
$db_hostname = "h50mysql59.secureserver.net";
$db_hostip = "10.8.13.57";
$db_username = "trcc_worship";
$db_password = "eaCjGJb2B5Pv";
$db_name = "trcc_worship";

$mydb = mysql_connect($db_hostname, $db_username, $db_password) or die("Unable to connect to database");
mysql_select_db($db_name);

?>
