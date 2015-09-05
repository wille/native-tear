<?php

require_once "config.php";

$data = $_GET["d"];

$handle = fopen(FILE, "a") or die("Cannot open file");
fwrite($handle, $data . PHP_EOL);
fclose($handle);

?>