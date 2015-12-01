<?php

	//Start the session.
session_start();

	//Get the username and password from form.
$username = $_POST["username"];
$password = $_POST["password"];

	//Save the username, password, and database name in the session.
$_SESSION["user"] = $username;
$_SESSION["pass"] = $password;
$_SESSION["dbName"] = "RecipeManager";

	//Connect to the database.
$connection = mysqli_connect("localhost", $_SESSION["user"], $_SESSION["pass"], $_SESSION["dbName"]);

	//Check the connection.
if(!$connection)
{
	die("Unable to connect to MySql Server");
}

	//Create the recipes table.
$createRecipesTable = "create table if not exists Recipes(RecipeName varchar(50), Ingredient varchar(50), Quantity int)";

	//The query successfully created the table.
if(mysqli_query($connection, $createRecipesTable))
{
	print "Successfully created recipes table"."<br>";
}

	//Failed to create the table.
else
{
	print "Failed to create recipes table"."<br>";
}

	//Create the inventory table.
$createInventoryTable = "create table if not exists Inventory(Ingredient varchar(50), Quantity int unsigned, primary key (Ingredient))";

	//The table was created successfully.
if(mysqli_query($connection, $createInventoryTable))
{
	print "Successfully create inventory table"."<br>";
}

	//The table failed to be created.
else
{
	print "Failed to create inventory table"."<br>";
}

?>

<!DOCTYPE html>

<html>
	
	<!-- Create a link to go back to the main menu after logging in -->

<head>

<title> Setup Credentials </title>

</head>

<body>

<a href="index.html"> Main Menu </a>

</body>

</html>
