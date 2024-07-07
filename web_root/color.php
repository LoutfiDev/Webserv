
<html>

<head>
<title>Dynamic Background Color Change</title>
</head>

<body bgcolor="<?php
error_reporting(E_ALL);
ini_set('display_errors', 1);
if (isset($_POST['btn']))
{
    $col=$_POST['t1'];
    if(isset($col))
    {
        echo $p=$col;
    }
    else
    {
        echo $p="#ffffff";
    }
}
?>">
<p>salam <?php
error_reporting(E_ALL);
ini_set('display_errors', 1);
if (isset($_POST['text']))
{
    $col=$_POST['text'];
    if(isset($col))
    {
        echo $col;
    }
    else
    {
        echo "man3ref";
    }
}
?></p>
<form action="" method="POST" >
<strong> Choose Color to Change Background :- </strong>
<select name="t1">
<option value="">Choose Color </option>
<option value="#000000"> Black </option>
<option value="#0000ff"> Blue </option>
<option value="#a52a2a"> Brown </option>
<option value="#00ffff"> Cyan </option>
<option value="#006400"> Dark Green </option>
<option value="#808080"> Grey </option>
<option value="#008000"> Green </option>
<option value="#ffa500"> Orange </option>
<option value="#ffc0cb"> Pink </option>
<option value="#800080"> Purple </option>
<option value="#ff0000"> Red </option>
<option value="#ffffff"> White </option>
<option value="#ffff00"> Yellow </option>
</select>
<br>
<input type="text" name="text">
<input type="submit" name="btn" value="Submit">
</form>
<!-- <form method="POST" action="http://127.0.0.1:8080/cgi/post.php" enctype="application/x-www-form-urlencoded">
    <strong> Choose Color to Change Background :- </strong>
    <select name="t1">
        <option value="">Choose Color </option>
        <option value="#000000"> Black </option>
        <option value="#0000ff"> Blue </option>
        <option value="#a52a2a"> Brown </option>
        <option value="#00ffff"> Cyan </option>
        <option value="#006400"> Dark Green </option>
        <option value="#808080"> Grey </option>
        <option value="#008000"> Green </option>
        <option value="#ffa500"> Orange </option>
        <option value="#ffc0cb"> Pink </option>
        <option value="#800080"> Purple </option>
        <option value="#ff0000"> Red </option>
        <option value="#ffffff"> White </option>
        <option value="#ffff00"> Yellow </option>
    </select>
    <label for="username">Username:</label>
    <input type="text" name="username" id="username">
    <br>
    <label for="password">Password:</label>
    <input type="password" name="password" id="password">
    <br>
    <button type="submit">Submit</button>
</form> -->

</body>
</html>
