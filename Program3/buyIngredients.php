<?php

	//Start the session to login to the database. 
session_start();

	//Connect to the database.
$connection = mysqli_connect("localhost", $_SESSION["user"], $_SESSION["pass"], $_SESSION["dbName"]);

	//Get the recipes name to buy.
$recipeName = $_POST["buyName"];

	//Build query to buy ingredients. 
$query = "update Inventory join Recipes on Recipes.Ingredient = Inventory.Ingredient and Recipes.RecipeName = \"$recipeName\" set Inventory.Quantity = Inventory.Quantity - Recipes.Quantity";

	//Start the transaction.
mysqli_query("start transaction");

	//The query successfully bought ingredients so we will commit
	//the changes. 
if($result = mysqli_query($connection, $query))
{
	mysqli_query("commit");
	print "Successfully bought ingredients";
	print "<br>";
}

	//The query failed so we will rollback any changes that have
	//taken place. 
else
{
	mysqli_query("rollback");
	print "Failed to purchase ingredients";
	print "<br>";
}

?>

<!DOCTYPE html>

<html>
	
	<!-- Create a link to the main menu -->

<head>

</head>

<body>

<a href="index.html"> Main Menu </a>

</body>

</html>
