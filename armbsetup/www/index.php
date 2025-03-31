<!DOCTYPE html>
<HTML>
<HEAD>
<meta charset="utf-8">

<!-- This file is free software, C 2025 Rainer Müller -->
<!-- using qrcode.js from https://github.com/manuelottlik/ct-js-qr for QR encoding -->

<TITLE>ModellBahn-Startseite</TITLE>
</HEAD>

<BODY>
<H1>ModellBahn-Startseite</H1>
<TABLE><td width=80%><font size="6">
<ul>
<?php

$ps = "ps -ef";
exec("$ps | grep dummy", $output, $retval);
if ($retval != 0) {				// for OpenWRT "ps -ef" fails, use "ps"
	$ps = "ps";
}

if (file_exists('cgi-bin/luci')) {
	echo '<li><a href="cgi-bin/luci/" target="_blank">LuCI - Lua Configuration Interface</a><br><br>';
}

if (file_exists('dienste.php')) {
	echo '<li><a href="dienste.php" target="_blank">Dienste-Steuerung</a><br><br>';
}

if (file_exists('ms1relay/html/ms1Display.pl')) {
	echo '<li><a href="http://';
	echo $_SERVER['SERVER_ADDR'];
	echo ':82" target="_blank">MS1-Relay-Dialog</a><br><br>';
}

if (file_exists('lok2card.html')) {
	echo '<li><a href="lok2card.html" target="_blank">CS2 to lococard Converter</a><br><br>';
}

exec("$ps | grep maecanserver | grep -v grep", $output, $retval);
if ($retval == 0) {
	echo '<li><a href="MaeCAN-Server/html/index.php" target="_blank">MaeCAN-Server</a><br><br>';
}

exec("$ps | grep railcontrol | grep -v grep", $output, $retval);
if ($retval == 0) {
	echo '<li><a href="http://';
	echo $_SERVER['SERVER_ADDR'];
	echo ':8082" target="_blank">RailControl</a><br><br>';
}

?>

</ul>

<td><font size="5">
Server-Adresse:<br>
<canvas id="qrDisplay"></canvas>
</TABLE>

<hr> <?php system("uname -a"); ?>

</BODY>
<script type="text/javascript" src="qrcode.js"></script>
<script>
	function displayQrCode(data) {
		const qrDisplay = document.getElementById('qrDisplay');
		QRCode.toCanvas(qrDisplay, data);
	}

	displayQrCode("http://<?php echo $_SERVER['SERVER_ADDR']; ?>");
</script>
</HTML>
