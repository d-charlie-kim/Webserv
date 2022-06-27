<?php
$body = "<!DOCTYPE html>\n";
$body .= "<head>\n";
$body .= "<title>PHP POST-text Test Result</title>\n";
$body .= "</head>\n";
$body .= "<body>\n";
$body .= "<h1>PHP POST Test - text Result</h1>\n";
if (!isset($_REQUEST["first"]))
	$body .= "No first input<p> \n";
else
{
	$body .= "<p>First input : [ ";
	$body .= $_REQUEST["first"];
	$body .= " ]</p>";
}
if (!isset($_REQUEST["first"]))
	$body .= "No second input<p> \n";
else
{
	$body .= "<p>Second input : [ ";
	$body .= $_REQUEST["second"];
	$body .= " ]</p>";
}
$body .= "</body>\n";
$body .= "</html>\n";

// $b_len = strlen($body);
// header("Status: 200 Success\r\n");
// header("Content-Length: $b_len\r\n");
echo $body;
?>