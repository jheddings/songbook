<?php  // $Id: lyrics.inc 24 2008-04-11 04:40:31Z jheddings $

////////////////////////////////////////////////////////////////////////////////
function get_all_songs($k) {
  global $mydb;

  $sql = "SELECT * FROM songs";
	if ($k) {
	  $sql .= " WHERE title RLIKE '^" . $k . ".*'";
	}
	$sql .= " ORDER BY title;";
  $result = mysql_query($sql, $mydb);
  //printf("%s\n%s\n", $sql, mysql_error());
  while ($myrow = mysql_fetch_array($result)) {
 	  $songs[$myrow["id"]] = $myrow;
	}
  mysql_free_result($result);
  return $songs;
}

////////////////////////////////////////////////////////////////////////////////
function get_title($id) {
  global $mydb;

  $sql = "SELECT title FROM songs WHERE id='" . $id . "';";
	$result = mysql_query($sql, $mydb);
  $myrow = mysql_fetch_array($result);
  mysql_free_result($result);

  //printf("%s\n%s\n", $sql, mysql_error());
	return $myrow["title"];
}

////////////////////////////////////////////////////////////////////////////////
function get_id($title) {
  global $mydb;

  $sql = "SELECT id FROM songs WHERE title='" . $title . "';";
	$result = mysql_query($sql, $mydb);
  $myrow = mysql_fetch_array($result);
  mysql_free_result($result);

  //printf("%s\n%s\n", $sql, mysql_error());
	return $myrow["id"];
}

?>
