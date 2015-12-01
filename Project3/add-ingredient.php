<?php
    session_start();
 
    $ingredientName = $_POST['ingredientName'];
    $ingredientQuantity = $_POST['ingredientQuantity'];

    // Create connection
    $conn = mysqli_connect($_SESSION["servername"], $_SESSION["username"], $_SESSION["password"], $_SESSION["databaseName"]);


    // Check connection
    if (!$conn) {
        header("Location: error.html");
    }
    
    else{
        $addIngredient = "insert into Inventory (Ingredient, Quantity) values ('$ingredientName', '$ingredientQuantity')
        on duplicate key update Quantity = Quantity + '$ingredientQuantity';";

        $result = mysqli_query($conn, $addIngredient);

        if(!$result)
        {
            echo "<script>alert('Ingredient NOT added successfully');
            window.location.href='main.html';</script>";
        }
        
        else
        {
            echo "<script>alert('Ingredient added successfully');
            window.location.href='main.html';</script>";
        }

        
    }
    
?>