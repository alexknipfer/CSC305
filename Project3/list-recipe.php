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
                            //begin session
                        session_start();
 
                        $recipeName = $_POST['recipe'];     //get recipe name from form

                            // Create connection
                        $conn = mysqli_connect($_SESSION["servername"], $_SESSION["username"], $_SESSION["password"], $_SESSION["databaseName"]);


                        // Check connection
                        if (!$conn) {
                                //if connection failed, display error page
                            header("Location: error.html");
                        }
                        
                            //if connection was fine, do the following
                        else{
                                //create query to get all recipes for the recipe name
                                //they entered
                            $selectIngredients = "select * from Recipes where RecipeName = '$recipeName'";
                            
                                //run query
                            $result = mysqli_query($conn, $selectIngredients);
                            
                                //if the query did not run successfully, display a prompt
                                //letting them know the recipe doesn't exist
                            if(!$result)
                            {
                                echo "<script>alert('Recipe does NOT exist');
                                </script>";
                            }
                            
                                //if the query ran successfully, print out the 
                                //ingredients and quantity required for that recipe
                            else{
                                    //print out header for the recipe name they entered
                                echo "<h5>Ingredient's for " . $recipeName . "</h5>";
                                echo "<table class='striped'>";
                                echo "<tr><th>Ingredient</th><th>Quantity</th></tr>";
                                
                                    //print out all the ingredients and quantity for 
                                    //all of the ingredietns required for that recipe
                                while ($row=mysqli_fetch_row($result))
                                {
                                        //print the results in a table to keep it 
                                        //easy to read
                                    echo "<tr>";
                                    echo "<td>" . $row[1] . "</td><td>" . $row[2] . "</td>";
                                    echo "</tr>";
                                }
                                    //end the table
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