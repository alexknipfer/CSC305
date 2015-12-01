<?php

	//Start a session to use all the database credentials.
session_start();

	//Connect to the database.
$connection = mysqli_connect("localhost", $_SESSION["user"], $_SESSION["pass"], $_SESSION["dbName"]);

	//Build the query to create a recipe.
$query = "insert into Recipes (RecipeName, Ingredient, Quantity)
values ('$_POST[createName]','$_POST[ingredientName]','$_POST[quantityOfIngredient]')";

	//Check if the query works.
if(mysqli_query($connection, $query))
{
	print "Added recipe to database"."<br>";
}

	//Print an error message if the query failed. 
else
{
	print "Failed to add recipe to database"."<br>";
}

?>

<!DOCTYPE html>

<html>
	
	<!--Create a link to go back to the main menu -->
	
<head>

<title> Create Recipe </title>

</head>

<body>

<a href="index.html"> Main Menu </a>

</body>

</html>
