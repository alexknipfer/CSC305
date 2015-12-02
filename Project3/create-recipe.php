<?php
    session_start();
 
    $recipeName = $_POST['recipeName'];
    $ingredientName = $_POST['ingredientName'];
    $ingredientQuantity = $_POST['ingredientQuantity'];

    // Create connection
    $conn = mysqli_connect($_SESSION["servername"], $_SESSION["username"], $_SESSION["password"], $_SESSION["databaseName"]);


    // Check connection
    if (!$conn) {
        header("Location: error.html");
    }
    
    else{
        $addRecipe = "insert into Recipes (RecipeName, Ingredient, Quantity) values ('$recipeName', '$ingredientName', '$ingredientQuantity')";

        $result = mysqli_query($conn, $addRecipe);

        if(!$result)
        {
            echo "<script>alert('Recipe NOT added successfully');
            window.location.href='main.html';</script>";
        }
        
        else
        {
            echo "<script>alert('Recipe added successfully');
            window.location.href='create-recipe.html';</script>";
        }

        
    }
    
?>