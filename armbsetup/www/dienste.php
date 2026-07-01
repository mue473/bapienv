<!DOCTYPE html>
<html>
<head>
<!--This file is free software, C 2021 - 2026 Rainer Müller-->
<meta charset="utf-8">
<style type="text/css">
th, td {
	padding: 12px;
	text-align: center;
}
input[type=submit] {
    width: 8em;
}
</style>
</head>
<script language="JavaScript">
function AskShutdown() {
	return (confirm("Wirklich Shutdown durchführen?"));
}
</script>

<body>
<h1>Moba-Dienste</h1>

<table>
<tr><th>START<th>STOP<th>STATUS<th>TRIGGER

<tr><form method = "post">
<td><input type = "submit" name = "START" value = "SRCPD"/>
<td><input type = "submit" name = "STOP" value = "SRCPD"/>
<td><input type = "submit" name = "STATUS" value = "SRCPD"/>
</form>

<tr><form method = "post">
<td><input type = "submit" name = "START" value = "MaeCan"/>
<td><input type = "submit" name = "STOP" value = "MaeCan"/>
<td><input type = "submit" name = "STATUS" value = "MaeCan"/>
</form>

<tr><form method = "post">
<td><input type = "submit" name = "START" value = "MSWebApp"/>
<td><input type = "submit" name = "STOP" value = "MSWebApp"/>
<td><input type = "submit" name = "STATUS" value = "MSWebApp"/>
</form>

<tr><form method = "post">
<td><input type = "submit" name = "START" value = "MS1Relay"/>
<td><input type = "submit" name = "STOP" value = "MS1Relay"/>
<td><input type = "submit" name = "STATUS" value = "MS1Relay"/>
</form>

<tr><form method = "post">
<td><input type = "submit" name = "START" value = "RailControl"/>
<td><input type = "submit" name = "STOP" value = "RailControl"/>
<td><input type = "submit" name = "STATUS" value = "RailControl"/>
<td><input type = "submit" name = "START" value = "RC-CONF"/>
</form>

<tr><form method = "post">
<td><input type = "submit" name = "START" value = "Z21_Emu"/>
<td><input type = "submit" name = "STOP" value = "Z21_Emu"/>
<td><input type = "submit" name = "STATUS" value = "Z21_Emu"/>
<td><input type = "submit" name = "START" value = "Z21-CONF"/>
</form>

<tr><td>&nbsp;<td>&nbsp;

<tr><form method = "post" OnSubmit="return AskShutdown()">
<td>&nbsp;
<td><input type = "submit" name = "STOP" value = "Server"/>
</form>

</table>

<br><br><hr size = 3 noshade>

<h4>Debug Info</h4>
<?php
foreach($_POST as $x=>$y)
{
	echo "$x => $y <br>";
}
if (isset($_POST["START"])) {
	switch($_POST["START"]) {
		case "SRCPD":		$cmdline = "srcpd";
							break;
		case "MaeCan":		$cmdline = "cd /www/MaeCAN-Server/node && nohup node --use_strict maecanserver.js >/dev/null &</dev/null &";
							break;
		case "MSWebApp":	$cmdline = "mswebapp --config /www --www /www/frontend >/dev/null &";
							break;
		case "MS1Relay":	$cmdline = "ms1relay -dp -l /www/config/lokomotive.ms1";
							break;
		case "RailControl":	$cmdline = "cd /opt/railcontrol && ./railcontrol -d";
							break;
		case "Z21_Emu":		$cmdline = "z21emu >/dev/null";
							break;
		case "RC-CONF":		$cmdline = "cansend can0 0031B311#43425553010C0040";
							break;
		case "Z21-CONF":	$cmdline = "cs2toz21 -c /www >/dev/null &";
							break;
	}
	system($cmdline, $retval);
	echo "<br>Code $retval bei: $cmdline";
}
if (isset($_POST["STOP"])) {
	switch($_POST["STOP"]) {
		case "SRCPD":		$cmdline = "killall srcpd";
							break;
		case "MaeCan":		$cmdline = "killall node";
							break;
		case "MSWebApp":	$cmdline = "killall mswebapp";
							break;
		case "MS1Relay":	$cmdline = "killall ms1relay";
							break;
		case "RailControl":	$cmdline = "killall railcontrol";
							break;
		case "Z21_Emu":		$cmdline = "killall z21emu";
							break;
		case "Server":		if (file_exists('/usr/bin/systemctl'))
								$cmdline = "systemctl poweroff";
							else
								$cmdline = "poweroff";
							break;
	}
	system($cmdline, $retval);
	echo "<br>Code $retval bei: $cmdline";
}
if (isset($_POST["STATUS"])) {
	$ps = "ps ax";
	exec("$ps | grep dummy", $output, $retval);
	if ($retval != 0) $ps = "ps";		// for OpenWRT "ps ax" fails, use "ps"
	switch($_POST["STATUS"]) {
		case "SRCPD":		$cmdline = "$ps | grep srcpd | grep -v grep";
							break;
		case "MaeCan":		$cmdline = "$ps | grep node | grep -v grep";
							break;
		case "MSWebApp":	$cmdline = "$ps | grep mswebapp | grep -v grep";
							break;
		case "MS1Relay":	$cmdline = "$ps | grep 'ms1relay ' | grep -v grep";
							break;
		case "RailControl":	$cmdline = "$ps | grep railcontrol | grep -v grep";
							break;
		case "Z21_Emu":		$cmdline = "$ps | grep z21emu | grep -v grep";
							break;
	}
	system($cmdline, $retval);
	echo "<br>Code $retval bei: $cmdline<br><br>";
	echo "<br><hr size = 3 noshade><br><big>";
	if ($retval == 0) echo $_POST["STATUS"]." is RUNNING.";
	else echo  $_POST["STATUS"]." is STOPPED.";
}?>

</body>
</html>
