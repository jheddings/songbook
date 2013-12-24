<?php  // $Id: transpose.inc 24 2008-04-11 04:40:31Z jheddings $

$gChordScale = array(
  array("C"),
  array("C#", "Db"),
  array("D"),
  array("D#", "Eb"),
  array("E"),
  array("F"),
  array("F#", "Gb"),
  array("G"),
  array("G#", "Ab"),
  array("A"),
  array("A#", "Bb"),
  array("B"),
);

////////////////////////////////////////////////////////////////////////////////
function capo($chord, $capo) {
  global $gChordScale;

  // find the base part of the chord
  $base = $chord{0};
  if ($chord{1} == 'b') {
    $base .= 'b';
    $rest = substr($chord, 2);
  } else if ($chord{1} == '#') {
    $base .= '#';
    $rest = substr($chord, 2);
  } else {
    $rest = substr($chord, 1);
  }

  // find the base chord's position in the scale
  foreach ($gChordScale as $idx => $var) {
    if (in_array($base, $var)) {
      $pos = $idx;
      break;
    }
  }

  // transpose the base from its position
  if (isset($pos)) {
    $pos += $capo;

    // bring the pos back into the scale
    while ($pos < 0) { $pos += 12; }
    while ($pos >= 12) { $pos -= 12; }

    $new = $gChordScale[$pos][0] . $rest;

  // make sure we found a valid chord
  } else {
    $new = "??";
  }

  return $new;
}
?>
