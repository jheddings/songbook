<?php  // $Id: song.inc 24 2008-04-11 04:40:31Z jheddings $

class Song {
  var $id;
  var $title;
  var $author;
  var $capo;

  // $chords =>
  //    <annotation>
  //    <section> => <lines> => "lyrics" => <lyrics>
  //                            "chords" => <postition> => <chord>
  var $chords;

  //////////////////////////////////////////////////////////////////////////////
  // create a new Song by parsing the given DB row
  function Song($row) {
    $this->id = $row["id"];
    $this->title = $row["title"];
    $this->author = $row["author"];
    $this->capo = 0;

    // downloads
    $this->ptf = $row["ptf"];

    // chord sheet
    $this->raw = $row["chords"];
    $this->chords = $this->parse_chords();
  }

  //////////////////////////////////////////////////////////////////////////////
  // transpose the song by the given number of semitones (half-steps)
  // this will not alter the raw data contained in the song
  function transpose($capo) {
    foreach ($this->chords as $section => &$lines) {
      if (is_array($lines)) {
        foreach ($lines as &$line) {
          $chords = $line["chords"];
          foreach ($chords as $pos => $chord) {
            $chord = capo($chord, $capo);
            $chords[$pos] = $chord;
          }
          $line["chords"] = $chords;
        }
      }
    }

    $this->capo = $capo;
  }

  //////////////////////////////////////////////////////////////////////////////
  // parse the raw chords data into an associative array
  function parse_chords() {
    $parsed = array();

    // match chord sections and annotations in the raw data
    $regex = "/((\S*)\s*{\s*([^}]*)\s*})"
           . "|(<(.*?)>)/sm";

    // pull out all the blocks first
    // $matches[0] = full text matches (a merge of sections 1 & 4)
    // $matches[1] = chord section blocks
    // $matches[2] = chord section names
    // $matches[3] = chord section contents
    // $matches[4] = annotate blocks
    // $matches[5] = annotate text
    preg_match_all($regex, $this->raw, $matches);

    // associate the lines to their section
    for ($idx = 0; $idx < count($matches[0]); $idx++) {
      // determine if this is a chord section or annotation
      if ($matches[5][$idx] == "") {
        // if it is a chord section, parse out all the lines
        $linenr = 0;
        $section = $matches[2][$idx];
        $parsed[$section] = array();
        $lines = explode("\n", trim($matches[3][$idx]));
        foreach ($lines as $line) {
          $parsed[$section][$linenr++] = $this->parse_chord_line(trim($line));
        }
      } else {
        // for annotations, just push the string
        array_push($parsed, $matches[5][$idx]);
      }
    }

    // hopefully we're all set up
    return $parsed;
  }

  // parse a line for lyrics and chords
  function parse_chord_line($line) {
    $offset = 0;
    $lyrics = "";
    $chords = array();

    // regex to match the chords in a chordline
    $regex = "/"
           . "(.*?)"         // lyrics (non-greedy)
           . "(?<!\\\\)\["   // open bracket (don't hit \[)
           . "(.*?)"         // the chord (non-greedy)
           . "(?<!\\\\)\]"   // close bracket (don't hit \])
           . "/";

    // find all chords in the text
    while(preg_match($regex, $line, $match, 0, $offset)) {
      $lyrics .= $match[1];
      $chords[strlen($lyrics)] = $match[2];
      $offset += strlen($match[0]);
    }

    // write any remaining text
    $lyrics .= substr($line, $offset);
    $lyrics = trim($lyrics);

    return array(
      "lyrics" => ($lyrics == "" ? NULL : $lyrics),
      "chords" => ($offset == 0 ? NULL : $chords));
  }
}

?>
