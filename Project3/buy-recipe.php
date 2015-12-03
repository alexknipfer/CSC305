<?php
    //begin session
    session_start();
 
    $recipeName = $_POST['recipeName'];     //get recipe name from form

    // Create connection
    $conn = mysqli_connect($_SESSION["servername"], $_SESSION["username"], $_SESSION["password"], $_SESSION["databaseName"]);


    // Check connection
    if (!$conn) {
            //if the connection failed, display error page
        header("Location: error.html");
    }
    
    else
    {
            //begin transaction
        mysqli_query($conn, "start transaction");
        
        $query = "update Inventory join Recipes on Recipes.Ingredient = Inventory.Ingredient 
        and Recipes.RecipeName = '$recipeName' set Inventory.Quantity = 
        Inventory.Quantity - Recipes.Quantity where Inventory.Quantity > Recipes.Quantity";
        
        $recipeRow = "select RecipeName from Recipes where RecipeName = '$recipeName'";
        $inventoryRow = "select Inventory.Ingredient from Inventory, Recipes where Recipes.RecipeName = '$recipeName' and Recipes.Ingredient = Inventory.Ingredient";
        
        $recipeRowResult = mysqli_query($conn, $recipeRow);
        $inventoryRowResult = mysqli_query($conn, $inventoryRow);

        $recipe_row_cnt = mysqli_num_rows($recipeRowResult);
        $inventory_row_cnt = mysqli_num_rows($inventoryRowResult);

        
            //run query
        $result = mysqli_query($conn, $query);
        
            //if the query failed, rollback and don't do changes
        if(!$result)
        {
            mysqli_query($conn, "rollback");
            header("Location: error.html");
        }
        
            //if the query was fine, commit the changes
        else
        {
            if($recipe_row_cnt == $inventory_row_cnt)
            {
                mysqli_query($conn, "commit");
                echo "<script>alert('Ingredients bought successfully');
                window.location.href='buy-recipe.html';</script>";
            }
            else
            {
                mysqli_query($conn, "rollback");
                echo "<script>alert('Failed to purchase ingredients!');
                window.location.href='buy-recipe.html';</script>";
            }

        }

        
    }
    
?>