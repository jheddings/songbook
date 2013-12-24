<?php  // $Id: dl.php 24 2008-04-11 04:40:31Z jheddings $

require_once("include/class/song.inc");
require_once("include/common.inc");

$id = $_GET["id"];
$fmt = $_GET["fmt"];
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

// display the correct format
if ($fmt == "raw" ) {
  header('Content-Disposition: inline; filename="' . $song->title . '.txt"');
  header("Content-Type: text/plain");
  echo $db_row["chords"];

} else if ($fmt == "ptf") {
  header('Content-Disposition: attachment; filename="' . $song->title . '.ptf"');
  header("Content-Type: text/ptf");
  echo $db_row["ptf"];

} else if ($fmt == "dump") {
  $tmpl["song"] = $song;
  render("dump");

} else if ($fmt == "pdf") {
  $tmpl["song"] = $song;
  render("pdf");
}
?>
