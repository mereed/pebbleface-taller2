module.exports = [
  {
    "type": "heading",
    "defaultValue": "Taller Configuration"
  },
     {
        "type": "text",
        "defaultValue": "<h6>A white toggle = OFF, an orange toggle = ON</h6>",
      },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "<h5>Colors</h5>",
		"capabilities": ["COLOR"],
      },
      {
        "type": "color",
        "messageKey": "hour_colour",
        "defaultValue": "0xFFFFFF",
        "label": "Hour Colour"
      },
      {
        "type": "color",
        "messageKey": "min_colour",
        "defaultValue": "0xFFFFFF",
        "label": "Minute Colour"
      },
		  {
        "type": "text",
        "defaultValue": "<h6>If you want a white background with black text, select WHITE above, AND select 'invert' below.</h6>",
  },
    ]
  },
  {
    "type": "section",
    "items": [
 	     {
        "type": "heading",
        "defaultValue": "<h5>Display</h5>"
      },
//      {
//        "type": "toggle",
//        "messageKey": "blink",
//        "label": "Blink",
//        "defaultValue": false
//      },
		{
        "type": "toggle",
        "messageKey": "invert",
        "label": "Invert",
        "defaultValue": false
      },
	  {
        "type": "toggle",
        "messageKey": "battbar",
        "label": "Show battery bar",
        "defaultValue": false
      },

		{
  "type": "select",
  "messageKey": "language",
  "defaultValue": 0,
  "label": "Select your language",
  "options": [

    { 
      "label": "English",
      "value": 0
    },
    { 
      "label": "German",
      "value": 1
    },
    { 
      "label": "French",
      "value": 2
    },
    { 
      "label": "Italian",
      "value": 3
    },
    { 
      "label": "Russian",
      "value": 4
    },
	{ 
      "label": "Chinese",
      "value": 5
    },
    { 
      "label": "Spanish",
      "value": 6
    }
  ]
}
	]		
},
  {
    "type": "section",
    "items": [	
 	  {
        "type": "heading",
        "defaultValue": "<h5>Vibration</h5>"
      },
      {
        "type": "toggle",
        "messageKey": "bluetoothvibe",
        "label": "Bluetooth Vibration",
        "defaultValue": false
      },
	  {
        "type": "toggle",
        "messageKey": "hourlyvibe",
        "label": "Vibrate each hour",
        "defaultValue": false
      }
    ]
  },
  {
        "type": "text",
        "defaultValue": "<h6>This watchface will continue to be free.  If you find it useful, please consider making a <a href='https://www.paypal.me/markchopsreed'>small donation here</a>. Thankyou.</h6>",
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }
];