<?php  // $Id: index.php 17 2008-04-10 05:50:57Z jheddings $

require_once("include/common.inc");
$tmpl["songs"] = get_all_songs($_GET["k"]);

render("index");
?>
