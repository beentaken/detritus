<?php

class Computer {
	var $black = 'b';
	var $white = 'w';
	var $empty = 'e';
	var $dim = 8;
	var $board = NULL;
	var $message = NULL;
	var $yourawinner = NULL;

	function Computer($board) {
		$this->board = array();

		for ($i = 0; $i < $this->dim; $i++) {
			$this->board[$i] = array();

			for ($j = 0; $j < $this->dim; $j++) {
				$this->board[$i][$j] = $board{$i * $this->dim + $j};
			}
		}
	}

	function move($move) {
		$success = FALSE;

		if (!$this->gameover()) {
			if ($this->blackmoves($move)) {
				$success = TRUE;
				
				if (!$this->gameover()) {
					do {
						$this->trywhitemoves();
					} while (!($this->gameover() || $this->canmove($this->black)));
				}
			}
		}

		return $success;
	}

	function getboard() {
		$board = "";

		for ($i = 0; $i < $this->dim; $i++) {
			for ($j = 0; $j < $this->dim; $j++) {
				$board .= $this->board[$i][$j];
			}
		}

		return $board;
	}

	function onboard($i, $j) {
		return (($i >= 0) && ($i < $this->dim) && ($j >= 0) && ($j < $this->dim));
	}

	function legalmove($row, $col, $color, $flip) {
		if ($color == $this->black) {
			$othercolor = $this->white;
		} else {
			$othercolor = $this->black;
		}

		$legal = FALSE;

		if ($this->board[$row][$col] == $this->empty) {
			for ($xdir = -1; $xdir < 2; $xdir++) {
				for ($ydir = -1; $ydir < 2; $ydir++) {
					$step = 0;

					do {
						$step++;
						$i = $row + $step * $xdir;
						$j = $col + $step * $ydir;
					} while ($this->onboard($i, $j)
						&& ($this->board[$i][$j] == $othercolor));

					if ($this->onboard($i, $j) && ($step > 1)
						&& ($this->board[$i][$j] == $color)) {
						$legal = TRUE;

						if ($flip) {
							for ($k = 1; $k < $step; $k++) {
								$this->board[$row + $xdir * $k]
									[$col + $ydir * $k] = $color;
							}
						}
					}
				}
			}
		}

		return $legal;
	}

	function canmove($color) {
		for ($i = 0; $i < $this->dim; $i++) {
			for ($j = 0; $j < $this->dim; $j++) {
				if ($this->legalmove($i, $j, $color, FALSE)) {
					return TRUE;
				}
			}
		}

		return FALSE;
	}

	function gameover() {
		$over = TRUE;

		if ($this->canmove($this->black) || $this->canmove($this->white)) {
			$over = FALSE;
		} else {
			$this->results();
		}

		return $over;
	}

	function results() {
		$black_count = 0;
		$white_count = 0;

		for ($i = 0; $i < $this->dim; $i++) {
			for ($j = 0; $j < $this->dim; $j++) {
				if ($this->board[$i][$j] == $this->black) {
					$black_count++;
				} elseif ($this->board[$i][$j] == $this->white) {
					$white_count++;
				}
			}
		}

		if ($white_count > $black_count) {
			$this->message = "White won with " . $white_count . " discs.";
			$this->yourawinner = FALSE;
		} elseif ($black_count > $white_count) {
			$this->message = "Black won with " . $black_count . " discs.";
			$this->yourawinner = TRUE;
		} else {
			$this->message = "Tied game.";
			$this->yourawinner = TRUE;
		}
	}

	function blackmoves($move) {
		$row = $move / $this->dim;
		$col = $move % $this->dim;

		$success = FALSE;

		if ($this->legalmove($row, $col, $this->black, TRUE)) {
			$this->board[$row][$col] = $this->black;
			$success = TRUE;
		} else {
			if($this->canmove($this->black)) {
				$this->message = "Illegal move.";
			} else {
				$this->message = "No possible move for Black.";

				while ($this->trywhitemoves());
			}
		}

		return $success;
	} 

	function trywhitemoves() {
		$moves = array();

		// Corners
		$moves[] = array(0, 0);
		$moves[] = array($this->dim - 1, $this->dim - 1);
		$moves[] = array(0, $this->dim - 1);
		$moves[] = array($this->dim - 1, 0);
		
		// Centre squares
		for ($i = 2; $i < 6; $i++) {
			for ($j = 2; $j < 6; $j++) {
				$moves[] = array($i, $j);
			}
		}

		// Edges
		for ($i = 2; $i < 6; $i++) {
			$moves[] = array(0, $i);
			$moves[] = array(7, $i);
			$moves[] = array($i, 0);
			$moves[] = array($i, 7);
		}

		// Inner squares
		for ($i = 2; $i < 6; $i++) {
			$moves[] = array(1, $i);
			$moves[] = array(6, $i);
			$moves[] = array($i, 1);
			$moves[] = array($i, 6);
		}

		// Remaining squares (Really all the squares, brute force)
		for ($i = 0; $i < $this->dim; $i++) {
			for ($j = 0; $j < $this->dim; $j++) {
				$moves[] = array($i, $j);
			}
		}

		// Now, try all moves to exhaustion
		foreach ($moves as $move) {
			if ($this->legalMove($move[0], $move[1], $this->white, TRUE)) {
				$this->board[$move[0]][$move[1]] = $this->white;
				return TRUE;
			}
		}

		return FALSE;
	}
}
?>
