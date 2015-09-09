<?php

require_once "config.php";

$identifier = $_GET["i"];
$key = $_GET["k"];
$iv = $_GET["iv"];

$handle = fopen(FILE, "a") || die("Cannot open file");
fwrite($handle, $identifier . " " . $key . " " . $iv . PHP_EOL);
fclose($handle);

?>