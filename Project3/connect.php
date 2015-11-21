<?php
    session_start();
    
    $servername = $_POST['servername'];
    $databaseName = $_POST['dbname'];
    $username = $_POST['username'];
    $password = $_POST['password'];
    
    $_SESSION["servername"] = $servername;
    $_SESSION["databaseName"] = $databaseName;
    $_SESSION["username"] = $username;
    $_SESSION["password"] = $password;


    // Create connection
    $conn = mysqli_connect($servername, $username, $password, $databaseName);

    // Check connection
    if (!$conn) {
        header("Location: error.html");
    }
    
    else{
        $recipeTable = "create table if not exists Recipes(RecipeName varchar(100), ";
        $recipeTable .= "Ingredient varchar(100), Quantity int, primary key(recipeName, ingredient))";
        
        $result = mysqli_query($conn, $recipeTable);
        
        $inventoryTable = "create table if not exists Inventory (Ingredient varchar(100), ";
        $inventoryTable .= "Quantity int)";
        
        $result = mysqli_query($conn, $inventoryTable);
        
        
        header("Location: main.html");
    }
    
?>