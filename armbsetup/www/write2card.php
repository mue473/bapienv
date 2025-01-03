<?php
/* This file is free software, C 2024 - 2025 by Rainer Müller */

/*	example of data passed by php framework:
[_FILES] => Array
	[cardimg] => Array
		[name] => blob
		[full_path] => blob
		[type] => application/octet-stream
		[tmp_name] => /tmp/phpb9eTP9
		[error] => 0
		[size] => 8192
*/
if ($_FILES["cardimg"]["error"] === 0) {
	$fname = $_FILES["cardimg"]["tmp_name"];
	$fsize = $_FILES["cardimg"]["size"];
	echo "\ncard image stored in $fname with size $fsize\n";

	/* adapt the command line for your card reader access */
	$cmd = "cp $fname /dev/i2card 2>&1";	// I2C reader
//	$cmd = "read_lococard $fname";			// for test

	echo "\ntransfer to card:\n> $cmd\n";
	passthru($cmd, $rc);
	if ($rc == 0) echo "\nOK";
	else echo "\nFAIL, code $rc";
}
?>
