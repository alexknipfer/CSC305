<?php

	//Start the session to login to the database.
session_start();

	//Connect to the database.
$connection = mysqli_connect("localhost", $_SESSION["user"], $_SESSION["pass"], $_SESSION["dbName"]);

	//Get the ingredient name and the quantity added.
$ingredientName = $_POST["addName"];
$quantityAdded = $_POST["quantityAdded"];

	//Create query to add ingredient to the database.
$query = "insert into Inventory values (\"$ingredientName\", \"$quantityAdded\")
on duplicate key update Quantity = Quantity + \"$quantityAdded\"";

	//Check if the query will work.
if($result = mysqli_query($connection, $query))
{
	print "Successfully updated inventory";
	print "<br>";
}

	//Failed to update the inventory.
else
{
	print "Failed to update inventory";
	print "<br>";
}

?>

<!DOCTYPE html>

<html>
	
	<!-- Create a link to the main menu. -->

<head>

</head>

<body>

<a href="index.html"> Main Menu </a>

</body>

</html>
