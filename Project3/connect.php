<?php
        //begin session
    session_start();
    
    $servername = $_POST['servername'];     //get IP from form
    $databaseName = $_POST['dbname'];       //get database name from form
    $username = $_POST['username'];         //get username from form
    $password = $_POST['password'];         //get password from form
    
        //the following creates session variables for servername, database name,
        //username, and password to prevent having to log back in on the other
        //web pages
    $_SESSION["servername"] = $servername; 
    $_SESSION["databaseName"] = $databaseName;
    $_SESSION["username"] = $username;
    $_SESSION["password"] = $password;


    // Create connection
    $conn = mysqli_connect($servername, $username, $password, $databaseName);

    // Check connection
    if (!$conn) {
            //if connection failed, display error page
        header("Location: error.html");
    }
    
        //if the connection was fine, do the following
    else{
        
            //query to create the recipe table as long as it doesn't exist already
            //in the database
        $recipeTable = "create table if not exists Recipes(RecipeName varchar(100), ";
        $recipeTable .= "Ingredient varchar(100), Quantity int, primary key(recipeName, ingredient))";
        
            //run the query
        $result = mysqli_query($conn, $recipeTable);
        
            //query to create the inventory (store) table as long as it doesn't exist
            //already in the database
        $inventoryTable = "create table if not exists Inventory (Ingredient varchar(100), ";
        $inventoryTable .= "Quantity int unsigned, primary key(Ingredient))";
        
            //run the query
        $result = mysqli_query($conn, $inventoryTable);
        
            //direct to the main menu choices page
        header("Location: main.html");
    }
    
?>