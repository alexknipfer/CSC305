<html>

<head>
    <title>List Ingredients</title>

    <!-- Compiled and minified CSS -->
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/materialize/0.97.0/css/materialize.min.css">
    
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/2.1.3/jquery.min.js"></script>

        <!-- Compiled and minified JavaScript -->
    <script src="https://cdnjs.cloudflare.com/ajax/libs/materialize/0.97.0/js/materialize.min.js"></script>

</head>

<body>
    <div class="row">
        <div class="col s12 m6 offset-m3">
            <div class="card grey lighten-5">
                <div class="card-content black-text">
                    
                    <a href="main.html" class="right">Main Menu</a>
                    <span class="card-title black-text">List Ingredients</span>
                    
                    <form method="post">
                        <label>Recipe Name to List Ingredients:</label>
                        <input type="text" name="recipe">
                        
                        <button type="submit" class="waves-effect waves-light btn">List Ingredients!</button>

                    </form>
                    
                    <?php
                        session_start();
 
                        $recipeName = $_POST['recipe'];

                            // Create connection
                        $conn = mysqli_connect($_SESSION["servername"], $_SESSION["username"], $_SESSION["password"], $_SESSION["databaseName"]);


                        // Check connection
                        if (!$conn) {
                            header("Location: error.html");
                        }
                        
                        else{
                            $selectIngredients = "select * from Recipes where RecipeName = '$recipeName'";
                            $result = mysqli_query($conn, $selectIngredients);
                            
                            if(!$result)
                            {
                                echo "<script>alert('Recipe does NOT exist');
                                </script>";
                            }
                            
                            else{
                                echo "<h5>Ingredient's for " . $recipeName . "</h5>";
                                echo "<table class='striped'>";
                                echo "<tr><th>Ingredient</th><th>Quantity</th></tr>";
                                while ($row=mysqli_fetch_row($result))
                                {
                                    echo "<tr>";
                                    echo "<td>" . $row[1] . "</td><td>" . $row[2] . "</td>";
                                    echo "</tr>";
                                }
                                echo "</table>";
                            }
                            
                        }
                        
                    ?>
    
                </div>
            </div>
        </div>
    </div>

</body>

</html>