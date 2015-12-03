<?php
        //begin and terminate session
    session_start();
    session_destroy();
    
        //redirect back to login page
    header("Location: index.html");
?>