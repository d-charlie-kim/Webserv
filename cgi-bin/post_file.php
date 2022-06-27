<?php
$target_dir = "uploads/";
$target_file = $target_dir . basename($_FILES["fileToUpload"]["name"]);
$uploadOk = 1;
$imageFileType = strtolower(pathinfo($target_file, PATHINFO_EXTENSION)); // 빼도 작동?

$msg = "";
//중복 체크
if (file_exists($target_file))
{
	$msg .= "[ERROR] file already exists<br>";
	$uploadOk = 0;
}
// 확장자 체크 필요??
if ($uploadOk == 1)
{
	if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file))
		$msg .="The file [" . htmlspecialchars(basename($_FILES["fileToUpload"]["name"])) . "] has been uploaded.";
	else
		$msg .= "Sorry, your file was not uploaded";
}
else
{
	$msg .= "Sorry, your file was not uploaded";
}

$body = "<!DOCTYPE html>\n";
$body .= "<head>\n";
$body .= "<title>PHP POST-file Test Result</title>\n";
$body .= "</head>\n";
$body .= "<body>\n";
$body .= "<h1>PHP POST-file Test Result</h1>\n";

$body .= "<p>";
$body .= $msg;
$body .= "</p>";
$body .= "</body>\n";
$body .= "</html>\n";

// $b_len = strlen($body);
// header("Status: 200 Success\r\n");
// header("Content-Length: $b_len\r\n");
echo $body;
?>