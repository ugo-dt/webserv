<!DOCTYPE html>
<html>
	<head>
		<a href=http://localhost:8080/index.html>Home</a><br></br>
		<title>webserv</title>
		<style>
			body
			{
				background-color: dimgray;
			}
			.center
			{
				padding-top: 5%;		
				text-align: center;
				font-size:x-large;
				font-weight:bolder;
				font-family:sans-serif;
			}
		</style>
	</head>
	<body>
		<div class="center">
			<h1>This is a webpage.</h1>
			<a href=https://github.com/MaaZex/webserv>Github</a><br></br>
            <?php echo '<p>Hello from PHP!</p>'; ?>
			<a href=form.html>Fill a form here!</a><br></br>
		</div>
	</body>
</html>