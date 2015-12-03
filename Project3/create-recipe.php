<?php
        //begin session
    session_start();
 
    $recipeName = $_POST['recipeName'];             //get recipe name from form
    $ingredientName = $_POST['ingredientName'];     //get ingredient name from form
    $ingredientQuantity = $_POST['ingredientQuantity']; //get ingredient quantity from form

    // Create connection
    $conn = mysqli_connect($_SESSION["servername"], $_SESSION["username"], $_SESSION["password"], $_SESSION["databaseName"]);


    // Check connection
    if (!$conn) {
            //if connection failed show error page
        header("Location: error.html");
    }
    
        //if the connection was fine, do the following
    else{
            //create query to add recipe with required ingredients to the table
        $addRecipe = "insert into Recipes (RecipeName, Ingredient, Quantity) values ('$recipeName', '$ingredientName', '$ingredientQuantity')";

            //run the query
        $result = mysqli_query($conn, $addRecipe);

            //if the query was not successful, display prompt letting the user
            //know the recipe was not added
        if(!$result)
        {
            echo "<script>alert('Recipe NOT added successfully');
            window.location.href='main.html';</script>";
        }
        
            //if the query was successful, display a prompt letting the user
            //know the recipe was added
        else
        {
            echo "<script>alert('Recipe added successfully');
            window.location.href='create-recipe.html';</script>";
        }

        
    }
    
?>