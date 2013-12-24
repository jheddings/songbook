<?php  // $Id: cpdf.inc 16 2008-04-10 04:49:42Z jheddings $

define("APPROOT", realpath(dirname(__FILE__) . "/../.."));
require_once(APPROOT . "/include/fpdf/fpdf.php");

class CPDF extends FPDF {
  var $sectionTitleWidth;  // the width for section titles

  //////////////////////////////////////////////////////////////////////////////
  // header to show on every page
  function Header() {
    // print the title
    $this->SetTitleFont();
    $this->Cell(100, 8, $this->title, 0, 0, "L");

    // print the author
    $this->SetAuthorFont();
    $this->Cell(0, 8, $this->author, 0, 0, "R");

    // header line
    $this->SetFillColor(0, 0, 0);
    $this->Rect(10, 17, 190, 0.2, "F");
    $this->Rect(10, 17.5, 190, 0.2, "F");
    $this->Ln(14);
  }

  //////////////////////////////////////////////////////////////////////////////
  // footer to show on every page
  function Footer() {
    // footer line
    $this->SetFooterFont();
    $this->SetFillColor(0, 0, 0);
    $this->Rect(10, $this->PageBreakTrigger + 2, 190, 0.2, "F");
    $this->Rect(10, $this->PageBreakTrigger + 2.5, 190, 0.2, "F");
    $this->SetY($this->PageBreakTrigger + 5);

    $this->Cell(0, 0, "Used by permission, CCLI #2752477", 0, 1, "L");
    $this->Cell(0, 0, "The Ridge Community Church", 0, 1, "R");
  }

  //////////////////////////////////////////////////////////////////////////////
  // render a complete song
  function AddSong($song) {
    // setup for the song
    $this->SetSectionTitleWidth($song);

    // render each section
    foreach ($song->chords as $section => $lines) {
      if (is_array($lines)) {
        $this->AddSection($section, $lines);
      } else {
        $this->AddAnnotation($lines);
      }
      $this->Ln(7);
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // render an annotation
  function AddAnnotation($str) {
    $this->RenderAnnotation($str);
    $this->Ln(7);   // a little padding between lines
  }

  //////////////////////////////////////////////////////////////////////////////
  // render an entire named section
  function AddSection($name, $lines) {
    // determine if we need a page break
    if ($this->SectionBreaksPage($lines)) {
      $this->AddPage();
    }

    // render each line element and the section title
    $this->RenderSectionTitle($name);
    foreach ($lines as $line) {
      // after the first line (the section title), pad the cells
      if ($linenr++ > 0) { $this->Cell($this->sectionTitleWidth); }

      if ($line["lyrics"] == NULL) {
        $this->RenderChordOnlyLine($line["chords"]);
      } elseif ($line["chords"] == NULL) {
        $this->RenderLyricsOnlyLine($line);
      } else {
        $this->RenderChordLine($line);
      }

      $this->Ln(7);  // a little padding between lines
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // determines if the lines of the section will break over a page
  function SectionBreaksPage($lines) {
    $lmult = 3.5;  // multiplier for each line (empirically derrived)

    $vsize = 0;
    foreach ($lines as $line) {
      if (($line["lyrics"] == NULL) || ($line["chords"] == NULL)) {
        $vsize += (2 * $lmult);
      } else {
        $vsize += (3 * $lmult);
      }
    }

    return (($this->GetY() + $vsize) > $this->PageBreakTrigger);
  }

  //////////////////////////////////////////////////////////////////////////////
  // set the font for printing the title of the song
  function SetTitleFont() {
    $this->SetFont("Arial", "B", 16);
    $this->SetTextColor(0, 0, 0);
  }

  //////////////////////////////////////////////////////////////////////////////
  // set the font for printing the author of the song
  function SetAuthorFont() {
    $this->SetFont("Arial", "BI", 10);
    $this->SetTextColor(0, 0, 0);
  }

  //////////////////////////////////////////////////////////////////////////////
  // set the font for printing the footer
  function SetFooterFont() {
    $this->SetFont("Arial", "I", 8);
    $this->SetTextColor(0, 0, 0);
  }

  //////////////////////////////////////////////////////////////////////////////
  // set the font for printing annotations
  function SetAnnotationFont() {
    $this->SetFont("Arial", "IU", 12);
    $this->SetTextColor(0, 0, 0);
  }

  //////////////////////////////////////////////////////////////////////////////
  // set the font for printing section labels
  function SetSectionFont() {
    $this->SetFont("Arial", "B", 16);
    $this->SetTextColor(0, 0, 0);
  }

  //////////////////////////////////////////////////////////////////////////////
  // set the font for printing chords
  function SetChordsFont() {
    $this->SetFont("Arial", "B", 14);
    $this->SetTextColor(0, 0, 255);
  }

  //////////////////////////////////////////////////////////////////////////////
  // set the font for printing lyrics
  function SetLyricsFont() {
    $this->SetFont("Arial", "", 14);
    $this->SetTextColor(0, 0, 0);
  }

  //////////////////////////////////////////////////////////////////////////////
  // render an annotation
  function RenderAnnotation($str) {
    $this->SetAnnotationFont();
    $width = $this->GetStringWidth($str);
    $this->Cell($width, 0, $str);
    return $width;
  }

  //////////////////////////////////////////////////////////////////////////////
  // render a section title
  function RenderSectionTitle($section) {
    $this->SetSectionFont();
    $this->Cell($this->sectionTitleWidth, 0, $section);
  }

  //////////////////////////////////////////////////////////////////////////////
  // render a line containing chords only, no lyrics
  function RenderChordOnlyLine($chords) {
    $twidth = 0;
    $this->SetChordsFont();
    foreach ($chords as $chord) {
      $width = $this->GetStringWidth($chord) + 2;
      $this->Cell($width, 0, $chord);
      $twidth += $width;
    }
    return $twidth;
  }

  //////////////////////////////////////////////////////////////////////////////
  // render a line containing lyrics only, no chords
  function RenderLyricsOnlyLine($line) {
    $this->SetLyricsFont();
    $this->Cell(0, 0, $line["lyrics"]);
    return $this->GetStringWidth($line["lyrics"]);
  }

  //////////////////////////////////////////////////////////////////////////////
  // render a line with chords and lyrics done appropriately, cursor set after
  function RenderChordLine($line) {
    $lheight = 4.5;
    $lyrics = $line["lyrics"];
    $chords = $line["chords"];

    $offset = 0;
    $minsegwidth = 0;
    foreach ($chords as $position => $crd) {
      $this->SetLyricsFont();
      $seg = substr($lyrics, $offset, ($position - $offset));
      $segw = $this->GetStringWidth($seg);

      // determine minimum width from previous chord
      $width = ($segw < $minsegwidth) ? $minsegwidth : $segw;
      if ($width > 0) {
        // print the segment on the line below the current
        $this->Text((float) ($this->GetX()), (float) ($this->GetY() + $lheight), $seg);
      }

      // print the chord on the current line
      $this->SetChordsFont();
      $crdw = $this->GetStringWidth($crd);
      $this->Text((float) ($this->GetX() + $width), (float) ($this->GetY()), $crd);

      // update for next iteration
      $offset = $position;
      $minsegwidth = $crdw + 2;
      $this->SetXY((float) ($this->GetX() + $width), (float) ($this->GetY()));
    }

    // print any remaining information
    $this->SetLyricsFont();
    $seg = substr($lyrics, $offset);
    $this->Text((float) ($this->GetX()), (float) ($this->GetY() + $lheight), $seg);

    $this->Ln(3);
  }

  //////////////////////////////////////////////////////////////////////////////
  // determines the width needed for all section titles
  function SetSectionTitleWidth($song) {
    // figure out the width for section titles
    $this->SetSectionFont();
    $this->sectionTitleWidth = 0;
    foreach ($song->chords as $section => $lines) {
      $this->sectionTitleWidth = max($this->sectionTitleWidth,
                                     $this->GetStringWidth($section));
    }

    // add a little more pading
    $this->sectionTitleWidth += 2;

    // don't go smaller than 20
    $this->sectionTitleWidth = max($this->sectionTitleWidth, 20);
  }
}

?>
