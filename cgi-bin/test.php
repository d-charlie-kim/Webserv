<?php
$body = "<!DOCTYPE html>\n";
$body .= "<head>\n";
$body .= "<title>PHP Test1 Result</title>\n";
$body .= "</head>\n";
$body .= "<body>\n";
$body .= "<h1>PHP Test1 Result</h1>\n";
if (!isset($_REQUEST["first"]))
	$body .= "No first input<p> \n";
else
{
	$body .= "<p>First input was [ ";
	$body .= $_REQUEST["first"];
	$body .= " ]</p>";
}
if (!isset($_REQUEST["first"]))
	$body .= "No second input<p> \n";
else
{
	$body .= "<p>Second input was [ ";
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