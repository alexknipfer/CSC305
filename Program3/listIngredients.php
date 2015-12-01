<?php

	//Start the session for user credentials.
session_start();

	//Connect to MySQL
$connection = mysqli_connect("localhost", $_SESSION["user"], $_SESSION["pass"], $_SESSION["dbName"]);

	//Get the name of the recipe for the query.
$recipeName = $_POST["listName"];

	//Build the query to select the ingredients. 
$listIngredients = "select Ingredient, Quantity from Recipes where RecipeName = \"$recipeName\"";

	//Perform the query.
$result = mysqli_query($connection, $listIngredients);

	//Print out the table headers.
print "<table border='1'> 
<tr> 
<th> Ingredient </th> 
<th> Quantity </th> 
</tr>";

	//Loop through the results printing them out in 
	//a table. 
while($row = mysqli_fetch_array($result))
{
	print "<tr>";
	print "<td>".$row['Ingredient']."</td>";
	print "<td>".$row['Quantity']."</td>";
	print "</tr>";
}

	//End the table. 
print "</table>";

print "<br>";

?>

<!DOCTYPE html>

<html>
	
	<!--Create a link to go back to the main menu. -->

<head>

<title> List Ingredients </title>

</head>

<body>

<a href="index.html"> Main Menu </a>

</body>

</html>
