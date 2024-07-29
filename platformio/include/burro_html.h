#ifndef BURRO_HTML_H
#define BURRO_HTML_H

const char login_form[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html>
<body>
    <form action="/en-us/search/search/search/admin/manger/console/admin.php" method="GET">
        <div>
            <label for="username">Username:</label>
            <input type="text" id="username" name="username" required>
        </div>
        <br>
        <div>
            <label for="password">Password:</label>
            <input type="password" id="password" name="password" required>
        </div>
        <br>
        <div>
            <button type="submit">Login</button>
        </div>
    </form>
</body>
</html>
)rawliteral";

const char index_flag_form[] PROGMEM = R"rawliteral(
<h2>Flags</h2>
<p>Capture a flag!<p>
<form onsubmit="sendFlagRequest(); return false;">
    <label for="flag">Flag:</label>
    <input type="text" id="flag" name="flag" required>
    <button type="submit">Submit</button>
</form><br>
)rawliteral";

const char index_challenges[] PROGMEM = R"rawliteral(
<h2>Challenges (Hint: hidden with white font)</h2>
<ul>
  <li><span>What's the acronym? (Hint: <span style="color: white">Something World Sec</span>)</span></li>
  <li><span>What's the admin's username? (Hint: <span style="color: white">Hello</span>)</span></li>
  <li><span>What's the password? (Hint: <span style="color: white">It's not a QR Code</span>)</span></li>
  <li><span>Login as admin. (Hint: <span style="color: white">I wrote down the uri somewhere (see at the bottom, it's hard to read on the back of the badge)... Maybe obfuscated?</span>)</span></li>
  <li><span>You do not want to do the challenges, but want to unlock all the patterns (Hint: <span style="color: white">flag{unlock_it}</span>)</span></li>
</ul>
<br>
)rawliteral";

const char index_colors[] PROGMEM = R"rawliteral(
<h2>Colors</h2>
<p>Set the base color. Persists through resets.</p>
<ul>
    <li><a href="#" onclick="sendColorRequest(0); return false;">Yellow</a></li>
    <li><a href="#" onclick="sendColorRequest(1); return false;">White</a></li>
    <li><a href="#" onclick="sendColorRequest(2); return false;">Red</a></li>
    <li><a href="#" onclick="sendColorRequest(3); return false;">Orange</a></li>
    <li><a href="#" onclick="sendColorRequest(4); return false;">Green</a></li>
    <li><a href="#" onclick="sendColorRequest(5); return false;">Blue</a></li>
    <li><a href="#" onclick="sendColorRequest(6); return false;">Indigo</a></li>
    <li><a href="#" onclick="sendColorRequest(7); return false;">Violet</a></li>
    <li><a href="#" onclick="sendColorRequest(8); return false;">Pink</a></li>
</ul>
<p>Set a specific RGB color. Does not presist.
<form onsubmit="sendRgbRequest(); return false;">
    <label for="r">R:</label>
    <input type="number" id="r" name="r" min="0" max="255" required>
    <label for="g">G:</label>
    <input type="number" id="g" name="g" min="0" max="255" required>
    <label for="b">B:</label>
    <input type="number" id="b" name="b" min="0" max="255" required>
    <button type="submit">Submit</button>
</form>
<br>
)rawliteral";

const char index_patterns[] PROGMEM = R"rawliteral(
<h2>Patterns</h2>
    <p>Set the pattern or reset it to the rotation. Note, mileage varies when syncing with other nodes.</p>
    <ul>
        <li><a href="#" onclick="sendPatternReset(); return false;">Reset</a></li>
        <li><a href="#" onclick="sendPatternRequest(0); return false;">Off</a></li>
        <li><a href="#" onclick="sendPatternRequest(1); return false;">Chase+</a></li>
        <li><a href="#" onclick="sendPatternRequest(2); return false;">Chase-</a></li>
        <li><a href="#" onclick="sendPatternRequest(3); return false;">Blink</a></li>
        <li><a href="#" onclick="sendPatternRequest(4); return false;">Solid</a></li>
        <li><a href="#" onclick="sendPatternRequest(5); return false;">Sparkle</a></li>
        <li><a href="#" onclick="sendPatternRequest(6); return false;">Drip</a></li>
        <li><a href="#" onclick="sendPatternRequest(7); return false;">Chain</a></li>
        <li><a href="#" onclick="sendPatternRequest(8); return false;">Pulse</a></li>
)rawliteral";

const char index_head[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Pattern, Color, and RGB Requests</title>
    <script>
        var user = "burroadmin"; // flag{the_username_is_burroadmin}

        function handleResponse(xhr, successMessage, errorMessage) {
            if (xhr.readyState === XMLHttpRequest.DONE) {
                if (xhr.status === 200) {
                    alert(successMessage);
                } else if (xhr.status === 400) {
                    alert(errorMessage);
                }
            }
        }

        function sendPatternReset() {
            var xhr = new XMLHttpRequest();
            xhr.open("GET", `/reset`, true);
            xhr.onreadystatechange = function () {
                handleResponse(xhr, "Pattern reset successfully.", "Error resetting pattern.");
            };
            xhr.send();
        }

        function sendPatternRequest(id) {
            var xhr = new XMLHttpRequest();
            xhr.open("GET", `/pattern?id=${id}`, true);
            xhr.onreadystatechange = function () {
                handleResponse(xhr, "Pattern updated successfully.", "Error updating pattern.");
            };
            xhr.send();
        }

        function sendColorRequest(id) {
            var xhr = new XMLHttpRequest();
            xhr.open("GET", `/color?id=${id}`, true);
            xhr.onreadystatechange = function () {
                handleResponse(xhr, "Color updated successfully.", "Error updating color.");
            };
            xhr.send();
        }

        function sendRgbRequest() {
            var r = document.getElementById('r').value;
            var g = document.getElementById('g').value;
            var b = document.getElementById('b').value;
            var xhr = new XMLHttpRequest();
            xhr.open("GET", `/rgb?r=${r}&g=${g}&b=${b}`, true);
            xhr.onreadystatechange = function () {
                handleResponse(xhr, "RGB color set successfully.", "Error setting RGB color.");
            };
            xhr.send();
        }

        function sendFlagRequest() {
            var flag = document.getElementById('flag').value;
            var xhr = new XMLHttpRequest();
            xhr.open("GET", `/flag?flag=${flag}`, true);
            xhr.onreadystatechange = function () {
                handleResponse(xhr, "Flag submitted. Idk if it's correct...", "Error submitting flag.");
            };
            xhr.send();
        }
    </script>
</head>
<body>
<h1>Burro Management Console</h1>
<span>Hello <img src="/user/$user/icon/icon.gif" onerror="console.log('Icon for ' + user + ' not found');">, Enjoy the DC32 Burro Badge.</span><br>
)rawliteral";

const char index_end[] PROGMEM = R"rawliteral(
<br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><br><p>For more functionality, see the admin console at <span><a href="L3JhLWhmL2ZybmVwdS9mcm5lcHUvZnJuZXB1L25xenZhL3puYXRyZS9wYmFmYnlyL25xenZhLmN1Yz9jbmZmamJlcT1jbmZmamJlcTEyMyEmdnE9MyZwZmVzPTE3MjMxMDc2MDAmcmlyYWc9UVAzMg== -- sorry it's hard to read on the back of the badge"></a></span>   .</p><br>
</body>
</html>
)rawliteral";

#endif // BURRO_HTML_H

