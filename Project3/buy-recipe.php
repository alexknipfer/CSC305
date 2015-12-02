<?php
    session_start();
 
    $recipeName = $_POST['recipeName'];

    // Create connection
    $conn = mysqli_connect($_SESSION["servername"], $_SESSION["username"], $_SESSION["password"], $_SESSION["databaseName"]);


    // Check connection
    if (!$conn) {
        header("Location: error.html");
    }
    
    else
    {
        mysqli_query($conn, "start transaction");
        
        /*$query = "update Inventory join Recipes on Recipes.Ingredient = Inventory.Ingredient 
        and Recipes.RecipeName = '$recipeName' set Inventory.Quantity = 
        Inventory.Quantity - Recipes.Quantity where (Inventory.Ingredient = Recipes.Ingredient and Inventory.Quantity > Recipes.Quantity)";*/
        
        $query = "UPDATE Inventory set Inventory.Quantity = Inventory.Quantity - Recipes.Quantity where Recipes.Ingredient = Inventory.Quantity";
        
        $result = mysqli_query($conn, $query);
        
        if(!$result)
        {
            mysqli_query($conn, "rollback");
            echo "NO GOOD";
        }
        
        else
        {
            mysqli_query($conn, "commit");
            echo "GOOD";
        }

        
    }
    
?>