<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Message Sender</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      margin: 20px;
    }
    form {
      max-width: 400px;
      margin: 0 auto;
      padding: 20px;
      border: 1px solid #ccc;
      border-radius: 5px;
      background-color: #f9f9f9;
    }
    label, input, button {
      display: block;
      margin-bottom: 10px;
    }
    input[type="text"] {
      width: 100%;
      padding: 8px;
      box-sizing: border-box;
    }
    button {
      background-color: #4CAF50;
      color: white;
      border: none;
      padding: 10px 20px;
      cursor: pointer;
      border-radius: 3px;
      font-size: 16px;
    }
    button:hover {
      background-color: #45a049;
    }
    .message-display {
      max-width: 400px;
      margin: 20px auto;
      padding: 20px;
      border: 1px solid #ccc;
      border-radius: 5px;
      background-color: #e9e9e9;
    }
  </style>
</head>
<body>
  <h2>Send Message to Pico</h2>
  <form id="messageForm" method="POST" action="">
    <label for="message">Message:</label>
    <input type="text" id="message" name="message" placeholder="Enter your message..." required>
    <button type="submit">Send Message</button>
  </form>

  <div class="message-display" id="messageDisplay">
    <?php
    if ($_SERVER['REQUEST_METHOD'] == 'POST' && isset($_POST['message'])) {
      $message = htmlspecialchars($_POST['message']);
      file_put_contents('message.txt', $message);
      echo 'Your message: "' . $message . '" has been sent.';
    } elseif (file_exists('message.txt')) {
      $message = file_get_contents('message.txt');
      echo 'Current message: "' . htmlspecialchars($message) . '"';
    } else {
      echo 'No message has been sent yet.';
    }
    ?>
  </div>
</body>
</html>
