<?php
    //begin session
    session_start();
 
    $ingredientName = $_POST['ingredientName'];         //get ingredient name from form
    $ingredientQuantity = $_POST['ingredientQuantity']; //get quantity from form

    // Create connection
    $conn = mysqli_connect($_SESSION["servername"], $_SESSION["username"], $_SESSION["password"], $_SESSION["databaseName"]);


    // Check connection
    if (!$conn) {
            //display error page if connection is unsuccessful
        header("Location: error.html");
    }
    
        //if the connection is fine, do the following
    else{
            //create query to add ingredient to Inventory ("store")
            //if it exists, add to the quantity fo that ingredient
        $addIngredient = "insert into Inventory (Ingredient, Quantity) values ('$ingredientName', '$ingredientQuantity')
        on duplicate key update Quantity = Quantity + '$ingredientQuantity';";

            //run query
        $result = mysqli_query($conn, $addIngredient);

            //if the query failed, don't add the ingredient, display
            //prompt saying it wasn't added successfully
        if(!$result)
        {
            echo "<script>alert('Ingredient NOT added successfully');
            window.location.href='add-ingredient.html';</script>";
        }
        
            //if the query processed correctly, display a prompt
            //saying the ingredient was added successfully
        else
        {
            echo "<script>alert('Ingredient added successfully');
            window.location.href='add-ingredient.html';</script>";
        }

        
    }
    
?>