const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Sensor Data</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    p {  font-size: 1.2rem;}
    body {  margin: 0;}
    .topnav { overflow: hidden; background-color: #0591F5; color: white; font-size: 1.5rem; }
    .content { padding: 20px; }
    .card { background-color: white; box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5); }
    .cards { max-width: 800px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(200px, 1fr)); }
    .reading { font-size: 2.0rem; }
    .card.temperature { color: #0e7c7b; }
    .card.humidity { color: #17bebb; }
    .card.pressure { color: #3fca6b; }
  </style>
</head>
<body>
  <div class="topnav">
    <h3>%SENSORNAME%</h3>
  </div>
  <div class="content">
    <div class="cards">
      <div class="card temperature">
        <h4><i class="fas fa-thermometer-half"></i> TEMPERATURE AHT20</h4><p><span class="reading"><span id="tempA">%TEMPERATUREA%</span> &deg;C</span></p>
      </div>
      <div class="card temperature">
        <h4><i class="fas fa-thermometer-half"></i> TEMPERATURE BMP280</h4><p><span class="reading"><span id="tempB">%TEMPERATUREB%</span> &deg;C</span></p>
      </div>
      <div class="card humidity">
        <h4><i class="fas fa-tint"></i> HUMIDITY AHT20</h4><p><span class="reading"><span id="humA">%HUMIDITYA%</span> &percnt;</span></p>
      </div>
      <div class="card pressure">
        <h4><i class="fas fa-angle-double-down"></i> PRESSURE BMP280</h4><p><span class="reading"><span id="presB">%PRESSUREB%</span> hPa</span></p>
      </div>
    </div>
  </div>
<script>
if (!!window.EventSource) {
 var source = new EventSource('/events');
 
 source.addEventListener('open', function(e) {
  console.log("Events Connected");
 }, false);
 source.addEventListener('error', function(e) {
  if (e.target.readyState != EventSource.OPEN) {
    console.log("Events Disconnected");
  }
 }, false);
 
 source.addEventListener('message', function(e) {
  console.log("message", e.data);
 }, false);
 
 source.addEventListener('temperatureA', function(e) {
  console.log("temperatureA", e.data);
  document.getElementById("tempA").innerHTML = e.data;
 }, false);

 source.addEventListener('temperatureB', function(e) {
  console.log("temperatureB", e.data);
  document.getElementById("tempB").innerHTML = e.data;
 }, false);

 source.addEventListener('humidityA', function(e) {
  console.log("humidityA", e.data);
  document.getElementById("humA").innerHTML = e.data;
 }, false);
 
 source.addEventListener('pressureB', function(e) {
  console.log("pressureB", e.data);
  document.getElementById("presB").innerHTML = e.data;
 }, false);
}
</script>
</body>
</html>)rawliteral";