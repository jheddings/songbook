<?php  // $Id: backup.php 15 2008-04-10 04:39:07Z jheddings $

require_once("include/common.inc");
if ($gUser == "guest" ) { return; }

$backup = "mysqldump –-opt -v";
$backup .= " –-host=" . $db_hostname;
$backup .= " –-user=" . $db_username;
$backup .= " --password=" . $db_password;
$backup .= " " . $db_name;

echo "<pre>";
passthru($backup);
echo "</pre>";
?>
