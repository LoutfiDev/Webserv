<?php
if (!isset($_COOKIE["name"]) || !isset($_COOKIE["email"]) || !isset($_COOKIE["profile"]) || isset($_POST["logout"])) {
    setcookie("name", "", time() - 3600, "/");
    setcookie("email", "", time() - 3600, "/");
    setcookie("profile", "", time() - 3600, "/");
    echo "<meta http-equiv='refresh' content='0; url=login.php'>";
    exit(0);
}

?>

<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>Profile</title>
  <link rel="stylesheet" href="css/style.css">

</head>
<body>
  <div class="card">
    <div class="card__header">
      <div class="card__profile">
        <img src="<?php echo "" . $_COOKIE["profile"] ?>" alt="A man smiling" />
      </div>
      <div class="card__name">
        <h2><?php echo $_COOKIE["name"] ?></h2>
        <div class="card__handle">
          <span class="handle"><?php echo $_COOKIE["email"] ?></span>
          <span class="circle"></span>
        </div>
      </div>
      <div class="card__button">
        <form action="" method="post">
            <span>
              <input class="heading score insights" type="submit" name="logout" value="Logout">
            </span>
        </form>
      </div>
    </div>
    <hr class="border" />
    <nav>
      <ul class="navlinks">
        <li class="link__item">Home</li>
      </ul>
    </nav>
    <div class="card__insights">
      <div class="card__heading">
        <div class="heading">Supported Methods : </div>
      </div>
      <div class="insights">
        <div class="insight" onclick="OpenMethod('http://127.0.0.1:8080/imgs/')">
          <div class="heading">
            Simple & Complex Data
            <div class="score">
              <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 262.361 262.362" width="8" height="8" fill="#44c790">
                <path
                  d="M286.935 197.287L159.028 69.381c-3.613-3.617-7.895-5.424-12.847-5.424s-9.233 1.807-12.85 5.424L5.424 197.287C1.807 200.904 0 205.186 0 210.134s1.807 9.233 5.424 12.847c3.621 3.617 7.902 5.425 12.85 5.425h255.813c4.949 0 9.233-1.808 12.848-5.425 3.613-3.613 5.427-7.898 5.427-12.847s-1.814-9.23-5.427-12.847z" />
              </svg>
              <span>20 day</span>
            </div>
          </div>
          <div class="number">
            POST
            <div class="info">
              More Info
              <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none"
                stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"
                class="feather feather-chevron-right">
                <polyline points="9 18 15 12 9 6"></polyline>
              </svg>
            </div>
          </div>
        </div>

        <div class="insight" onclick="OpenMethod('http://127.0.0.1:8080/imgs/')">
          <div class="heading">
            List Of Uploaded Data
            <div class="score">
              <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 262.361 262.362" width="8" height="8" fill="#44c790">
                <path
                  d="M286.935 197.287L159.028 69.381c-3.613-3.617-7.895-5.424-12.847-5.424s-9.233 1.807-12.85 5.424L5.424 197.287C1.807 200.904 0 205.186 0 210.134s1.807 9.233 5.424 12.847c3.621 3.617 7.902 5.425 12.85 5.425h255.813c4.949 0 9.233-1.808 12.848-5.425 3.613-3.613 5.427-7.898 5.427-12.847s-1.814-9.23-5.427-12.847z" />
              </svg>
              <span>9 days</span>
            </div>
          </div>
          <div class="number">
            GET
            <div class="info">
              More Info
              <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none"
                stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"
                class="feather feather-chevron-right">
                <polyline points="9 18 15 12 9 6"></polyline>
              </svg>
            </div>
          </div>
        </div>

        <div class="insight">
          <div class="heading">
            Delete Files
            <div class="score">
              <svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 262.361 262.362" width="8" height="8" fill="#44c790">
                <path
                  d="M286.935 197.287L159.028 69.381c-3.613-3.617-7.895-5.424-12.847-5.424s-9.233 1.807-12.85 5.424L5.424 197.287C1.807 200.904 0 205.186 0 210.134s1.807 9.233 5.424 12.847c3.621 3.617 7.902 5.425 12.85 5.425h255.813c4.949 0 9.233-1.808 12.848-5.425 3.613-3.613 5.427-7.898 5.427-12.847s-1.814-9.23-5.427-12.847z" />
              </svg>
              <span>5 days</span>
            </div>
          </div>
          <div class="number">
            DELETE
            <div class="info">
              More Info
              <svg xmlns="http://www.w3.org/2000/svg" width="24" height="24" viewBox="0 0 24 24" fill="none"
                stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"
                class="feather feather-chevron-right">
                <polyline points="9 18 15 12 9 6"></polyline>
              </svg>
            </div>
          </div>
        </div>

      </div>
    </div>
  </div>

    <script>
        function OpenMethod(url) {
            const myWindow = window.open(url, "_blank", "width=100,height=100");
            myWindow.blur();
        }
    </script>
</body>

</html>
