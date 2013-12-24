<?php  // $Id: common.inc 24 2008-04-11 04:40:31Z jheddings $

define("APPROOT", realpath(dirname(__FILE__) . "/.."));

session_start();
require_once(APPROOT . "/include/dbinfo.inc");
require_once(APPROOT . "/include/lyrics.inc");
require_once(APPROOT . "/include/transpose.inc");

// setup globals
$gConfig = load_config();
$gUser = $_SERVER["PHP_AUTH_USER"];

// default template params
$tmpl = array();
$tmpl["user"] = $gUser;

////////////////////////////////////////////////////////////////////////////////
function render($view) {
  global $tmpl, $gConfig;
  include(APPROOT . "/views/" . $view . ".tmpl");
}

////////////////////////////////////////////////////////////////////////////////
function load_config() {
  global $mydb;

  if (isset($_SESSION["config"])) {
    $config = $_SESSION["config"];

  } else {
    $config = array();
    $sql = "SELECT * FROM `config`;";
    $result = mysql_query($sql, $mydb);
    while ($row = mysql_fetch_array($result)) {
      $name = $row["name"];
      $value = $row["value"];
 	    $config[$name] = $value;
	  }
    mysql_free_result($result);
    $_SESSION["config"] = $config;
  }

  return $config;
}

?>
