<?php  // $Id: edit.php 19 2008-04-10 06:01:36Z jheddings $

require_once("include/class/song.inc");
require_once("include/common.inc");
if ($gUser == "guest" ) { return; }

// handle the update case for existing songs
if (isset($_POST["do-save"])) {
  $id = $_POST["id"];
  $title = stripslashes($_POST["title"]);
  $author = stripslashes($_POST["author"]);
  $chords = stripslashes($_POST["chords"]);

  $sql = "UPDATE `songs` SET ";
  $sql .= " `title` = '" . addslashes($title) . "'";
  $sql .= ", `author` = '" . addslashes($author) . "'";
  $sql .= ", `chords` = '" . addslashes($chords) . "'";
  if (isset($_FILES["ptf"]["tmp_name"]) && ($_FILES["ptf"]["tmp_name"] != "")) {
    $ptf = file_get_contents($_FILES["ptf"]["tmp_name"]);
    $sql .= ", `ptf` = 0x" . bin2hex($ptf) . "";
  }
  $sql .= " WHERE id='" . $id . "' LIMIT 1;";

  $result = mysql_query($sql, $mydb);
  header("Location: chords.php?id=" . $id);

// handle the new song case
} elseif (isset($_POST["do-new"])) {
  $title = stripslashes($_POST["title"]);
  $author = stripslashes($_POST["author"]);
  $chords = stripslashes($_POST["chords"]);

  $sql = "INSERT INTO `songs` SET ";
  $sql .= " `title` = '" . addslashes($title) . "'";
  $sql .= ", `author` = '" . addslashes($author) . "'";
  $sql .= ", `chords` = '" . addslashes($chords) . "'";
  if (isset($_FILES["ptf"]["tmp_name"]) && ($_FILES["ptf"]["tmp_name"] != "")) {
    $ptf = file_get_contents($_FILES["ptf"]["tmp_name"]);
    $sql .= ", `ptf` = 0x" . bin2hex($ptf) . "";
  }

  $result = mysql_query($sql, $mydb);
  $id = mysql_insert_id();
  header("Location: chords.php?id=" . $id);

// handle the case for deleting a song
} elseif (isset($_GET["remove"])) {
  $id = $_GET["remove"];

  $sql = "DELETE FROM `songs` WHERE `id` = '" . $id . "' LIMIT 1";

  $result = mysql_query($sql, $mydb);
  header("Location: index.php");

// the default case...  just show the form
} else {
  $id = $_GET["id"];

  if ($id) {
    $sql = "SELECT * FROM `songs` WHERE `id` = '" . $id . "' LIMIT 1;";
    $result = mysql_query($sql, $mydb);
    //printf("%s\n%s\n", $sql, mysql_error());

    $db_row = mysql_fetch_array($result);
    $tmpl["song"] = new Song($db_row);
    mysql_free_result($result);
  }

  render("edit");
}
?>
