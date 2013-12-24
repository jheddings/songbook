<?php  // $Id: chords.php 24 2008-04-11 04:40:31Z jheddings $

require_once("include/class/song.inc");
require_once("include/common.inc");

$id = $_GET["id"];
$chords = (isset($_GET["chords"]) ? $_GET["chords"] : "above");
$capo = (isset($_GET["capo"]) ? $_GET["capo"] : 0);

$sql = "SELECT * FROM `songs` WHERE id='" . $id . "' LIMIT 1;";
$result = mysql_query($sql, $mydb);
//printf("%s\n%s\n", $sql, mysql_error());

$db_row = mysql_fetch_array($result);
$song = new Song($db_row);
mysql_free_result($result);

if ($capo != 0) {
  $song->transpose($capo);
}

// set template params
$tmpl["song"] = $song;
$tmpl["ext-style"] = "styles/chords-" . $chords . ".css";

// begin headers
header("Content-Type: text/html");
header('Content-Disposition: inline; filename="' . $song->title . '.html"');

render("chords");
?>
