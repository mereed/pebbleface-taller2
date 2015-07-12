var mConfig = {};

Pebble.addEventListener("ready", function(e) {
  loadLocalData();
  returnConfigToPebble();
});

Pebble.addEventListener("showConfiguration", function(e) {
	Pebble.openURL(mConfig.configureUrl);
});

Pebble.addEventListener("webviewclosed",
  function(e) {
    if (e.response) {
      var config = JSON.parse(e.response);
      saveLocalData(config);
      returnConfigToPebble();
    }
  }
);

function saveLocalData(config) {

  //console.log("loadLocalData() " + JSON.stringify(config));

  localStorage.setItem("blink", parseInt(config.blink));  
  localStorage.setItem("invert", parseInt(config.invert)); 
  localStorage.setItem("bluetoothvibe", parseInt(config.bluetoothvibe)); 
  localStorage.setItem("hourlyvibe", parseInt(config.hourlyvibe)); 
  localStorage.setItem("battbar", parseInt(config.battbar)); 
  localStorage.setItem("language", parseInt(config.language)); 
  localStorage.setItem("hour_col", parseInt(config.hour_col)); 
  localStorage.setItem("min_col", parseInt(config.min_col)); 
  
  loadLocalData();

}
function loadLocalData() {
  
	mConfig.blink = parseInt(localStorage.getItem("blink"));
	mConfig.invert = parseInt(localStorage.getItem("invert"));
	mConfig.bluetoothvibe = parseInt(localStorage.getItem("bluetoothvibe"));
	mConfig.hourlyvibe = parseInt(localStorage.getItem("hourlyvibe"));
	mConfig.battbar = parseInt(localStorage.getItem("battbar"));
	mConfig.language = parseInt(localStorage.getItem("language"));
	mConfig.hour_col = parseInt(localStorage.getItem("hour_col"));
	mConfig.min_col = parseInt(localStorage.getItem("min_col"));
	mConfig.configureUrl = "http://www.themapman.com/pebblewatch/taller3.html";

	if(isNaN(mConfig.blink)) {
		mConfig.blink = 1;
	}
	if(isNaN(mConfig.invert)) {
		mConfig.invert = 0;
	}
	if(isNaN(mConfig.bluetoothvibe)) {
		mConfig.bluetoothvibe = 1;
	}
	if(isNaN(mConfig.hourlyvibe)) {
		mConfig.hourlyvibe = 0;
	}
    if(isNaN(mConfig.battbar)) {
		mConfig.battbar = 0;
	}
	if(isNaN(mConfig.language)) {
		mConfig.language = 0;
	}
	if(isNaN(mConfig.hour_col)) {
		mConfig.hour_col = 0;
	}
	if(isNaN(mConfig.min_col)) {
		mConfig.min_col = 0;
	}
  //console.log("loadLocalData() " + JSON.stringify(mConfig));
}
function returnConfigToPebble() {
  //console.log("Configuration window returned: " + JSON.stringify(mConfig));
  Pebble.sendAppMessage({
    "blink":parseInt(mConfig.blink), 
    "invert":parseInt(mConfig.invert), 
    "bluetoothvibe":parseInt(mConfig.bluetoothvibe), 
    "hourlyvibe":parseInt(mConfig.hourlyvibe),
    "battbar":parseInt(mConfig.battbar),
    "language":parseInt(mConfig.language),
    "hour_col":parseInt(mConfig.hour_col),
    "min_col":parseInt(mConfig.min_col),
  });    
}