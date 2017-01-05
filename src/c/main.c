/*
Copyright (C) 2015 Mark Reed

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <pebble.h>
#include "main.h"
#include "effect_layer.h"

EffectLayer* effect_layer_invert;
EffectLayer* effect_layer_col_hr;
EffectLayer* effect_layer_col_min;

static Window *window;

static uint8_t batteryPercent;

static int newlang;

	// initializing colors
struct EffectColorpair {
  GColor firstColor;  // first color (target for colorize, one of set in colorswap)
  GColor secondColor; // second color (new color for colorize, other of set in colorswap)
};
  
EffectColorpair colorpair_1;
EffectColorpair colorpair_2;

static GBitmap *separator_image;
static BitmapLayer *separator_layer;

static GBitmap *bluetooth_image;
static BitmapLayer *bluetooth_layer;

static GBitmap *bluetooth_image_on;
static BitmapLayer *bluetooth_layer_on;

GBitmap *img_battery_100;
GBitmap *img_battery_90;
GBitmap *img_battery_80;
GBitmap *img_battery_70;
GBitmap *img_battery_60;
GBitmap *img_battery_50;
GBitmap *img_battery_40;
GBitmap *img_battery_30;
GBitmap *img_battery_20;
GBitmap *img_battery_10;
GBitmap *img_battery_charge;
BitmapLayer *layer_batt_img;

#define TOTAL_DATE_DIGITS 2	
static GBitmap *date_digits_images[TOTAL_DATE_DIGITS];
static BitmapLayer *date_digits_layers[TOTAL_DATE_DIGITS];

const int DATENUM_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_DATENUM_0,
  RESOURCE_ID_IMAGE_DATENUM_1,
  RESOURCE_ID_IMAGE_DATENUM_2,
  RESOURCE_ID_IMAGE_DATENUM_3,
  RESOURCE_ID_IMAGE_DATENUM_4,
  RESOURCE_ID_IMAGE_DATENUM_5,
  RESOURCE_ID_IMAGE_DATENUM_6,
  RESOURCE_ID_IMAGE_DATENUM_7,
  RESOURCE_ID_IMAGE_DATENUM_8,
  RESOURCE_ID_IMAGE_DATENUM_9
};

#define TOTAL_TIME_DIGITS 4
static GBitmap *time_digits_images[TOTAL_TIME_DIGITS];
static BitmapLayer *time_digits_layers[TOTAL_TIME_DIGITS];

const int BIG_DIGIT_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_NUM_0,
  RESOURCE_ID_IMAGE_NUM_1,
  RESOURCE_ID_IMAGE_NUM_2,
  RESOURCE_ID_IMAGE_NUM_3,
  RESOURCE_ID_IMAGE_NUM_4,
  RESOURCE_ID_IMAGE_NUM_5,
  RESOURCE_ID_IMAGE_NUM_6,
  RESOURCE_ID_IMAGE_NUM_7,
  RESOURCE_ID_IMAGE_NUM_8,
  RESOURCE_ID_IMAGE_NUM_9
};

static GBitmap *day_name_image;
static BitmapLayer *day_name_layer;

const int DAY_NAME_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_DAY_NAME_SUN,
  RESOURCE_ID_IMAGE_DAY_NAME_MON,
  RESOURCE_ID_IMAGE_DAY_NAME_TUE,
  RESOURCE_ID_IMAGE_DAY_NAME_WED,
  RESOURCE_ID_IMAGE_DAY_NAME_THU,
  RESOURCE_ID_IMAGE_DAY_NAME_FRI,
  RESOURCE_ID_IMAGE_DAY_NAME_SAT
};

static GBitmap *gday_name_image;
static BitmapLayer *gday_name_layer;

const int DAY_NAME_IMAGE_GERMAN_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_DAY_NAME_SONNTAG,
  RESOURCE_ID_IMAGE_DAY_NAME_MONTAG,
  RESOURCE_ID_IMAGE_DAY_NAME_DIENSTAG,
  RESOURCE_ID_IMAGE_DAY_NAME_MITTWOCH,
  RESOURCE_ID_IMAGE_DAY_NAME_DONNERSTAG,
  RESOURCE_ID_IMAGE_DAY_NAME_FREITAG,
  RESOURCE_ID_IMAGE_DAY_NAME_SAMSTAG
};

static GBitmap *rday_name_image;
static BitmapLayer *rday_name_layer;

const int DAY_NAME_IMAGE_RUSSIAN_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_DAY_NAME_RUS_SUN,
  RESOURCE_ID_IMAGE_DAY_NAME_RUS_MON,
  RESOURCE_ID_IMAGE_DAY_NAME_RUS_TUE,
  RESOURCE_ID_IMAGE_DAY_NAME_RUS_WED,
  RESOURCE_ID_IMAGE_DAY_NAME_RUS_THU,
  RESOURCE_ID_IMAGE_DAY_NAME_RUS_FRI,
  RESOURCE_ID_IMAGE_DAY_NAME_RUS_SAT
};	

static GBitmap *fday_name_image;
static BitmapLayer *fday_name_layer;

const int DAY_NAME_IMAGE_FRENCH_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_DAY_NAME_FR_SUN,
  RESOURCE_ID_IMAGE_DAY_NAME_FR_MON,
  RESOURCE_ID_IMAGE_DAY_NAME_FR_TUE,
  RESOURCE_ID_IMAGE_DAY_NAME_FR_WED,
  RESOURCE_ID_IMAGE_DAY_NAME_FR_THU,
  RESOURCE_ID_IMAGE_DAY_NAME_FR_FRI,
  RESOURCE_ID_IMAGE_DAY_NAME_FR_SAT
};	

static GBitmap *iday_name_image;
static BitmapLayer *iday_name_layer;

const int DAY_NAME_IMAGE_ITALIAN_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_DAY_NAME_IT_SUN,
  RESOURCE_ID_IMAGE_DAY_NAME_IT_MON,
  RESOURCE_ID_IMAGE_DAY_NAME_IT_TUE,
  RESOURCE_ID_IMAGE_DAY_NAME_IT_WED,
  RESOURCE_ID_IMAGE_DAY_NAME_IT_THU,
  RESOURCE_ID_IMAGE_DAY_NAME_IT_FRI,
  RESOURCE_ID_IMAGE_DAY_NAME_IT_SAT
};	

static GBitmap *cday_name_image;
static BitmapLayer *cday_name_layer;

const int DAY_NAME_IMAGE_CHINESE_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_DAY_NAME_CH_SUN,
  RESOURCE_ID_IMAGE_DAY_NAME_CH_MON,
  RESOURCE_ID_IMAGE_DAY_NAME_CH_TUE,
  RESOURCE_ID_IMAGE_DAY_NAME_CH_WED,
  RESOURCE_ID_IMAGE_DAY_NAME_CH_THU,
  RESOURCE_ID_IMAGE_DAY_NAME_CH_FRI,
  RESOURCE_ID_IMAGE_DAY_NAME_CH_SAT
};		

static GBitmap *sday_name_image;
static BitmapLayer *sday_name_layer;

const int DAY_NAME_IMAGE_SPANISH_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_DAY_NAME_SP_SUN,
  RESOURCE_ID_IMAGE_DAY_NAME_SP_MON,
  RESOURCE_ID_IMAGE_DAY_NAME_SP_TUE,
  RESOURCE_ID_IMAGE_DAY_NAME_SP_WED,
  RESOURCE_ID_IMAGE_DAY_NAME_SP_THU,
  RESOURCE_ID_IMAGE_DAY_NAME_SP_FRI,
  RESOURCE_ID_IMAGE_DAY_NAME_SP_SAT
};	

static GBitmap *month_image;
static BitmapLayer *month_layer;

const int MONTH_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_JAN,
  RESOURCE_ID_IMAGE_FEB,
  RESOURCE_ID_IMAGE_MAR,
  RESOURCE_ID_IMAGE_APR,
  RESOURCE_ID_IMAGE_MAY,
  RESOURCE_ID_IMAGE_JUN,
  RESOURCE_ID_IMAGE_JUL,
  RESOURCE_ID_IMAGE_AUG,
  RESOURCE_ID_IMAGE_SEP,
  RESOURCE_ID_IMAGE_OCT,
  RESOURCE_ID_IMAGE_NOV,
  RESOURCE_ID_IMAGE_DEC
};

static GBitmap *gmonth_image;
static BitmapLayer *gmonth_layer;

const int GMONTH_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_GJAN,
  RESOURCE_ID_IMAGE_GFEB,
  RESOURCE_ID_IMAGE_GMAR,
  RESOURCE_ID_IMAGE_GAPR,
  RESOURCE_ID_IMAGE_GMAY,
  RESOURCE_ID_IMAGE_GJUN,
  RESOURCE_ID_IMAGE_GJUL,
  RESOURCE_ID_IMAGE_GAUG,
  RESOURCE_ID_IMAGE_GSEP,
  RESOURCE_ID_IMAGE_GOCT,
  RESOURCE_ID_IMAGE_GNOV,
  RESOURCE_ID_IMAGE_GDEC
};

static GBitmap *rmonth_image;
static BitmapLayer *rmonth_layer;

const int RMONTH_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_RJAN,
  RESOURCE_ID_IMAGE_RFEB,
  RESOURCE_ID_IMAGE_RMAR,
  RESOURCE_ID_IMAGE_RAPR,
  RESOURCE_ID_IMAGE_RMAY,
  RESOURCE_ID_IMAGE_RJUN,
  RESOURCE_ID_IMAGE_RJUL,
  RESOURCE_ID_IMAGE_RAUG,
  RESOURCE_ID_IMAGE_RSEP,
  RESOURCE_ID_IMAGE_ROCT,
  RESOURCE_ID_IMAGE_RNOV,
  RESOURCE_ID_IMAGE_RDEC
};

static GBitmap *fmonth_image;
static BitmapLayer *fmonth_layer;

const int FMONTH_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_FJAN,
  RESOURCE_ID_IMAGE_FFEB,
  RESOURCE_ID_IMAGE_FMAR,
  RESOURCE_ID_IMAGE_FAPR,
  RESOURCE_ID_IMAGE_FMAY,
  RESOURCE_ID_IMAGE_FJUN,
  RESOURCE_ID_IMAGE_FJUL,
  RESOURCE_ID_IMAGE_FAUG,
  RESOURCE_ID_IMAGE_FSEP,
  RESOURCE_ID_IMAGE_FOCT,
  RESOURCE_ID_IMAGE_FNOV,
  RESOURCE_ID_IMAGE_FDEC
};

static GBitmap *imonth_image;
static BitmapLayer *imonth_layer;

const int IMONTH_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_IJAN,
  RESOURCE_ID_IMAGE_IFEB,
  RESOURCE_ID_IMAGE_IMAR,
  RESOURCE_ID_IMAGE_IAPR,
  RESOURCE_ID_IMAGE_IMAY,
  RESOURCE_ID_IMAGE_IJUN,
  RESOURCE_ID_IMAGE_IJUL,
  RESOURCE_ID_IMAGE_IAUG,
  RESOURCE_ID_IMAGE_ISEP,
  RESOURCE_ID_IMAGE_IOCT,
  RESOURCE_ID_IMAGE_INOV,
  RESOURCE_ID_IMAGE_IDEC
};

static GBitmap *cmonth_image;
static BitmapLayer *cmonth_layer;

const int CMONTH_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_CJAN,
  RESOURCE_ID_IMAGE_CFEB,
  RESOURCE_ID_IMAGE_CMAR,
  RESOURCE_ID_IMAGE_CAPR,
  RESOURCE_ID_IMAGE_CMAY,
  RESOURCE_ID_IMAGE_CJUN,
  RESOURCE_ID_IMAGE_CJUL,
  RESOURCE_ID_IMAGE_CAUG,
  RESOURCE_ID_IMAGE_CSEP,
  RESOURCE_ID_IMAGE_COCT,
  RESOURCE_ID_IMAGE_CNOV,
  RESOURCE_ID_IMAGE_CDEC
};

static GBitmap *smonth_image;
static BitmapLayer *smonth_layer;

const int SMONTH_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_SJAN,
  RESOURCE_ID_IMAGE_SFEB,
  RESOURCE_ID_IMAGE_SMAR,
  RESOURCE_ID_IMAGE_SAPR,
  RESOURCE_ID_IMAGE_SMAY,
  RESOURCE_ID_IMAGE_SJUN,
  RESOURCE_ID_IMAGE_SJUL,
  RESOURCE_ID_IMAGE_SAUG,
  RESOURCE_ID_IMAGE_SSEP,
  RESOURCE_ID_IMAGE_SOCT,
  RESOURCE_ID_IMAGE_SNOV,
  RESOURCE_ID_IMAGE_SDEC
};

// A struct for our specific settings (see main.h)
ClaySettings settings;

int stringToInt(char *str);


// Initialize the default settings
static void prv_default_settings() {
	
  settings.hour_colour = GColorWhite;
  settings.min_colour = GColorWhite;
  settings.blink = false;
  settings.invert = false;
  settings.bluetoothvibe = false;
  settings.hourlyvibe = false;
  settings.language = 0;
  settings.battbar = false;
}


static void set_container_image(GBitmap **bmp_image, BitmapLayer *bmp_layer, const int resource_id, GPoint origin) {
  GBitmap *old_image = *bmp_image;

  *bmp_image = gbitmap_create_with_resource(resource_id);
  GRect bitmap_bounds = gbitmap_get_bounds((*bmp_image));
  GRect frame = GRect(origin.x, origin.y, bitmap_bounds.size.w, bitmap_bounds.size.h);
  bitmap_layer_set_bitmap(bmp_layer, *bmp_image);
  layer_set_frame(bitmap_layer_get_layer(bmp_layer), frame);

  if (old_image != NULL) {
  	gbitmap_destroy(old_image);
  }
}

static void update_battery(BatteryChargeState charge_state) {

  batteryPercent = charge_state.charge_percent;

    if (charge_state.is_charging) {
        bitmap_layer_set_bitmap(layer_batt_img, img_battery_charge);
	} else {
	
        if (charge_state.charge_percent <= 10) {
            bitmap_layer_set_bitmap(layer_batt_img, img_battery_10);
        } else if (charge_state.charge_percent <= 20) {
            bitmap_layer_set_bitmap(layer_batt_img, img_battery_20);
		} else if (charge_state.charge_percent <= 30) {
            bitmap_layer_set_bitmap(layer_batt_img, img_battery_30);
		} else if (charge_state.charge_percent <= 40) {
            bitmap_layer_set_bitmap(layer_batt_img, img_battery_40);
		} else if (charge_state.charge_percent <= 50) {
            bitmap_layer_set_bitmap(layer_batt_img, img_battery_50);
        } else if (charge_state.charge_percent <= 60) {
            bitmap_layer_set_bitmap(layer_batt_img, img_battery_60);
		} else if (charge_state.charge_percent <= 70) {
            bitmap_layer_set_bitmap(layer_batt_img, img_battery_70);
		} else if (charge_state.charge_percent <= 80) {
            bitmap_layer_set_bitmap(layer_batt_img, img_battery_80);
		} else if (charge_state.charge_percent <= 90) {
            bitmap_layer_set_bitmap(layer_batt_img, img_battery_90);
		} else if (charge_state.charge_percent <= 99) {
            bitmap_layer_set_bitmap(layer_batt_img, img_battery_100);
		} else {
            bitmap_layer_set_bitmap(layer_batt_img, img_battery_100);
        } 	
	}
}

static void toggle_bluetooth_icon(bool connected) {
  if(!connected && settings.bluetoothvibe) {
    //vibe!
    vibes_long_pulse();
  }
  layer_set_hidden(bitmap_layer_get_layer(bluetooth_layer), connected);
}

void bluetooth_connection_callback(bool connected) {
  toggle_bluetooth_icon(connected);
}

unsigned short get_display_hour(unsigned short hour) {
  if (clock_is_24h_style()) {
    return hour;
  }
  unsigned short display_hour = hour % 12;
  // Converts "0" to "12"
  return display_hour ? display_hour : 12;
}

static void update_days(struct tm *tick_time) {
#ifdef PBL_PLATFORM_EMERY
  set_container_image(&sday_name_image, sday_name_layer, DAY_NAME_IMAGE_SPANISH_RESOURCE_IDS[tick_time->tm_wday], GPoint(106, 17));
  set_container_image(&cday_name_image, cday_name_layer, DAY_NAME_IMAGE_CHINESE_RESOURCE_IDS[tick_time->tm_wday], GPoint(102, 17));
  set_container_image(&iday_name_image, iday_name_layer, DAY_NAME_IMAGE_ITALIAN_RESOURCE_IDS[tick_time->tm_wday], GPoint(106, 17));
  set_container_image(&fday_name_image, fday_name_layer, DAY_NAME_IMAGE_FRENCH_RESOURCE_IDS[tick_time->tm_wday], GPoint(106, 17));
  set_container_image(&rday_name_image, rday_name_layer, DAY_NAME_IMAGE_RUSSIAN_RESOURCE_IDS[tick_time->tm_wday], GPoint(104, 17));
  set_container_image(&gday_name_image, gday_name_layer, DAY_NAME_IMAGE_GERMAN_RESOURCE_IDS[tick_time->tm_wday], GPoint(106, 17));
  set_container_image(&day_name_image, day_name_layer, DAY_NAME_IMAGE_RESOURCE_IDS[tick_time->tm_wday], GPoint(106, 17));
	
  set_container_image(&date_digits_images[0], date_digits_layers[0], DATENUM_IMAGE_RESOURCE_IDS[tick_time->tm_mday/10], GPoint(106,199));
  set_container_image(&date_digits_images[1], date_digits_layers[1], DATENUM_IMAGE_RESOURCE_IDS[tick_time->tm_mday%10], GPoint(106,209));
#else
  set_container_image(&sday_name_image, sday_name_layer, DAY_NAME_IMAGE_SPANISH_RESOURCE_IDS[tick_time->tm_wday], GPoint(76, 17));
  set_container_image(&cday_name_image, cday_name_layer, DAY_NAME_IMAGE_CHINESE_RESOURCE_IDS[tick_time->tm_wday], GPoint(73, 17));
  set_container_image(&iday_name_image, iday_name_layer, DAY_NAME_IMAGE_ITALIAN_RESOURCE_IDS[tick_time->tm_wday], GPoint(76, 17));
  set_container_image(&fday_name_image, fday_name_layer, DAY_NAME_IMAGE_FRENCH_RESOURCE_IDS[tick_time->tm_wday], GPoint(76, 17));
  set_container_image(&rday_name_image, rday_name_layer, DAY_NAME_IMAGE_RUSSIAN_RESOURCE_IDS[tick_time->tm_wday], GPoint(74, 17));
  set_container_image(&gday_name_image, gday_name_layer, DAY_NAME_IMAGE_GERMAN_RESOURCE_IDS[tick_time->tm_wday], GPoint(76, 17));
  set_container_image(&day_name_image, day_name_layer, DAY_NAME_IMAGE_RESOURCE_IDS[tick_time->tm_wday], GPoint(76, 17));
	
  set_container_image(&date_digits_images[0], date_digits_layers[0], DATENUM_IMAGE_RESOURCE_IDS[tick_time->tm_mday/10], GPoint(75,141));
  set_container_image(&date_digits_images[1], date_digits_layers[1], DATENUM_IMAGE_RESOURCE_IDS[tick_time->tm_mday%10], GPoint(75, 148));
#endif
}

//DRAW MONTH
	
static void update_months(struct tm *tick_time) {	
	
  set_container_image(&gmonth_image, gmonth_layer, GMONTH_IMAGE_RESOURCE_IDS[tick_time->tm_mon], GPoint(2, 17));
  set_container_image(&rmonth_image, rmonth_layer, RMONTH_IMAGE_RESOURCE_IDS[tick_time->tm_mon], GPoint(2, 17));
  set_container_image(&fmonth_image, fmonth_layer, FMONTH_IMAGE_RESOURCE_IDS[tick_time->tm_mon], GPoint(3, 17));
  set_container_image(&imonth_image, imonth_layer, IMONTH_IMAGE_RESOURCE_IDS[tick_time->tm_mon], GPoint(3, 17));
  set_container_image(&cmonth_image, cmonth_layer, CMONTH_IMAGE_RESOURCE_IDS[tick_time->tm_mon], GPoint(3, 17));
  set_container_image(&smonth_image, smonth_layer, SMONTH_IMAGE_RESOURCE_IDS[tick_time->tm_mon], GPoint(3, 17));
  set_container_image(&month_image, month_layer, MONTH_IMAGE_RESOURCE_IDS[tick_time->tm_mon], GPoint(5, 17));
}

static void update_hours(struct tm *tick_time) {

  if(settings.hourlyvibe) {
    //vibe!
    vibes_short_pulse();
  }
  
  unsigned short display_hour = get_display_hour(tick_time->tm_hour);
#ifdef PBL_PLATFORM_EMERY
  set_container_image(&time_digits_images[0], time_digits_layers[0], BIG_DIGIT_IMAGE_RESOURCE_IDS[display_hour/10], GPoint(22, 16));
  set_container_image(&time_digits_images[1], time_digits_layers[1], BIG_DIGIT_IMAGE_RESOURCE_IDS[display_hour%10], GPoint(63, 16));
#else
  set_container_image(&time_digits_images[0], time_digits_layers[0], BIG_DIGIT_IMAGE_RESOURCE_IDS[display_hour/10], GPoint(22, 16));
  set_container_image(&time_digits_images[1], time_digits_layers[1], BIG_DIGIT_IMAGE_RESOURCE_IDS[display_hour%10], GPoint(46, 16));
#endif
  if (!clock_is_24h_style()) {
    
    if (display_hour/10 == 0) {
      layer_set_hidden(bitmap_layer_get_layer(time_digits_layers[0]), true);
    }
    else {
      layer_set_hidden(bitmap_layer_get_layer(time_digits_layers[0]), false);
    }
  }
}

static void update_minutes(struct tm *tick_time) {
	
#ifdef PBL_PLATFORM_EMERY
  set_container_image(&time_digits_images[2], time_digits_layers[2], BIG_DIGIT_IMAGE_RESOURCE_IDS[tick_time->tm_min/10], GPoint(120, 16));
  set_container_image(&time_digits_images[3], time_digits_layers[3], BIG_DIGIT_IMAGE_RESOURCE_IDS[tick_time->tm_min%10], GPoint(161, 16));
#else
  set_container_image(&time_digits_images[2], time_digits_layers[2], BIG_DIGIT_IMAGE_RESOURCE_IDS[tick_time->tm_min/10], GPoint(96, 16));
  set_container_image(&time_digits_images[3], time_digits_layers[3], BIG_DIGIT_IMAGE_RESOURCE_IDS[tick_time->tm_min%10], GPoint(120, 16));
#endif

}



static void handle_tick(struct tm *tick_time, TimeUnits units_changed) {
  if (units_changed & MONTH_UNIT) {
    update_months(tick_time);
  }
	if (units_changed & DAY_UNIT) {
    update_days(tick_time);
  }
  if (units_changed & HOUR_UNIT) {
    update_hours(tick_time);
  }
  if (units_changed & MINUTE_UNIT) {
    update_minutes(tick_time);
  }	

}



// Read settings from persistent storage
static void prv_load_settings() {
  // Load the default settings
  prv_default_settings();
  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

// Save the settings to persistent storage
static void prv_save_settings() {
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
  // Update the display based on new settings
  prv_update_display();
}

// Update the display elements
static void prv_update_display() {
		
  colorpair_1.firstColor = GColorWhite;
  colorpair_1.secondColor = settings.hour_colour;
	
  colorpair_2.firstColor = GColorWhite;
  colorpair_2.secondColor = settings.min_colour;	
	  
	 
  // invert
  if (settings.invert && effect_layer_invert == NULL) {
    // Add inverter layer
    Layer *window_layer = window_get_root_layer(window);

#ifdef PBL_PLATFORM_EMERY
	  effect_layer_invert = effect_layer_create(GRect(0, 12, 200, 208));  
#else
	  effect_layer_invert = effect_layer_create(GRect(0, 12, 144, 150));
#endif
    effect_layer_add_effect(effect_layer_invert, effect_invert_bw_only, NULL);
    layer_add_child(window_layer, effect_layer_get_layer(effect_layer_invert));
  
  } else if (!settings.invert && effect_layer_invert != NULL) {
    // Remove Inverter layer
   layer_remove_from_parent(effect_layer_get_layer(effect_layer_invert));
   effect_layer_destroy(effect_layer_invert);
   effect_layer_invert = NULL;
  }	

	
// datesep
  if (settings.battbar) {
		  	layer_set_hidden(bitmap_layer_get_layer(layer_batt_img), false);
	  } else {
		  	layer_set_hidden(bitmap_layer_get_layer(layer_batt_img), true);
	  }
	
	
switch (settings.language) {
			
case 0: 
	//english
			layer_set_hidden(bitmap_layer_get_layer(smonth_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(sday_name_layer), true);
		    layer_set_hidden(bitmap_layer_get_layer(gmonth_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(gday_name_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(fday_name_layer), true);
            layer_set_hidden(bitmap_layer_get_layer(fmonth_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(iday_name_layer), true);
            layer_set_hidden(bitmap_layer_get_layer(imonth_layer), true);
	 		layer_set_hidden(bitmap_layer_get_layer(rday_name_layer), true);
            layer_set_hidden(bitmap_layer_get_layer(rmonth_layer), true);
		    layer_set_hidden(bitmap_layer_get_layer(cmonth_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(cday_name_layer), true);
		
	        layer_set_hidden(bitmap_layer_get_layer(month_layer), false);
			layer_set_hidden(bitmap_layer_get_layer(day_name_layer), false);
	break;
case 1:
  //german
			layer_set_hidden(bitmap_layer_get_layer(smonth_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(sday_name_layer), true);
		    layer_set_hidden(bitmap_layer_get_layer(month_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(day_name_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(fday_name_layer), true);
            layer_set_hidden(bitmap_layer_get_layer(fmonth_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(iday_name_layer), true);
            layer_set_hidden(bitmap_layer_get_layer(imonth_layer), true);
	 		layer_set_hidden(bitmap_layer_get_layer(rday_name_layer), true);
            layer_set_hidden(bitmap_layer_get_layer(rmonth_layer), true);
		    layer_set_hidden(bitmap_layer_get_layer(cmonth_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(cday_name_layer), true);
		
	        layer_set_hidden(bitmap_layer_get_layer(gmonth_layer), false);
			layer_set_hidden(bitmap_layer_get_layer(gday_name_layer), false);		
		break;

case 2:
  //french		
			layer_set_hidden(bitmap_layer_get_layer(smonth_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(sday_name_layer), true);
		    layer_set_hidden(bitmap_layer_get_layer(gmonth_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(gday_name_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(day_name_layer), true);
            layer_set_hidden(bitmap_layer_get_layer(month_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(iday_name_layer), true);
            layer_set_hidden(bitmap_layer_get_layer(imonth_layer), true);
	 		layer_set_hidden(bitmap_layer_get_layer(rday_name_layer), true);
            layer_set_hidden(bitmap_layer_get_layer(rmonth_layer), true);
		    layer_set_hidden(bitmap_layer_get_layer(cmonth_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(cday_name_layer), true);
		
	        layer_set_hidden(bitmap_layer_get_layer(fmonth_layer), false);
			layer_set_hidden(bitmap_layer_get_layer(fday_name_layer), false);		
		break;

case 3:
 //italian
			layer_set_hidden(bitmap_layer_get_layer(smonth_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(sday_name_layer), true);
		    layer_set_hidden(bitmap_layer_get_layer(gmonth_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(gday_name_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(fday_name_layer), true);
            layer_set_hidden(bitmap_layer_get_layer(fmonth_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(day_name_layer), true);
            layer_set_hidden(bitmap_layer_get_layer(month_layer), true);
	 		layer_set_hidden(bitmap_layer_get_layer(rday_name_layer), true);
            layer_set_hidden(bitmap_layer_get_layer(rmonth_layer), true);
		    layer_set_hidden(bitmap_layer_get_layer(cmonth_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(cday_name_layer), true);
		
	        layer_set_hidden(bitmap_layer_get_layer(imonth_layer), false);
			layer_set_hidden(bitmap_layer_get_layer(iday_name_layer), false);	
		break;

case 4:
 //russian	
			layer_set_hidden(bitmap_layer_get_layer(smonth_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(sday_name_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(gmonth_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(gday_name_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(fday_name_layer), true);
            layer_set_hidden(bitmap_layer_get_layer(fmonth_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(iday_name_layer), true);
            layer_set_hidden(bitmap_layer_get_layer(imonth_layer), true);
	 		layer_set_hidden(bitmap_layer_get_layer(day_name_layer), true);
            layer_set_hidden(bitmap_layer_get_layer(month_layer), true);
		    layer_set_hidden(bitmap_layer_get_layer(cmonth_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(cday_name_layer), true);
		
	        layer_set_hidden(bitmap_layer_get_layer(rmonth_layer), false);
			layer_set_hidden(bitmap_layer_get_layer(rday_name_layer), false);
		break;

case 5:
 //chinese	
			layer_set_hidden(bitmap_layer_get_layer(smonth_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(sday_name_layer), true);
		    layer_set_hidden(bitmap_layer_get_layer(gmonth_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(gday_name_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(fday_name_layer), true);
            layer_set_hidden(bitmap_layer_get_layer(fmonth_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(iday_name_layer), true);
            layer_set_hidden(bitmap_layer_get_layer(imonth_layer), true);
	 		layer_set_hidden(bitmap_layer_get_layer(rday_name_layer), true);
            layer_set_hidden(bitmap_layer_get_layer(rmonth_layer), true);
		    layer_set_hidden(bitmap_layer_get_layer(month_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(day_name_layer), true);
		
	        layer_set_hidden(bitmap_layer_get_layer(cmonth_layer), false);
			layer_set_hidden(bitmap_layer_get_layer(cday_name_layer), false);		
		break;

case 6:
 //spanish	
			layer_set_hidden(bitmap_layer_get_layer(cmonth_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(cday_name_layer), true);
		    layer_set_hidden(bitmap_layer_get_layer(gmonth_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(gday_name_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(fday_name_layer), true);
            layer_set_hidden(bitmap_layer_get_layer(fmonth_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(iday_name_layer), true);
            layer_set_hidden(bitmap_layer_get_layer(imonth_layer), true);
	 		layer_set_hidden(bitmap_layer_get_layer(rday_name_layer), true);
            layer_set_hidden(bitmap_layer_get_layer(rmonth_layer), true);
		    layer_set_hidden(bitmap_layer_get_layer(month_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(day_name_layer), true);
		
	        layer_set_hidden(bitmap_layer_get_layer(smonth_layer), false);
			layer_set_hidden(bitmap_layer_get_layer(sday_name_layer), false);	
		break;

    }  
  
	
	  // Refresh display

  // Get current time
  time_t now2 = time( NULL );
  struct tm *tick_time = localtime( &now2 );

  // Force update to Refresh display
  handle_tick(tick_time, MONTH_UNIT + DAY_UNIT + HOUR_UNIT + MINUTE_UNIT );

}

int stringToInt(char *str){
    int i=0,sum=0;
    while(str[i]!='\0'){
         if(str[i]< 48 || str[i] > 57){
            // if (DEBUG) APP_LOG(APP_LOG_LEVEL_ERROR, "Unable to convert it into integer.");
          //   return 0;
         }
         else{
             sum = sum*10 + (str[i] - 48);
             i++;
         }
    }
    return sum;
}

// Handle the response from AppMessage
static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
	
  // hour Color
  Tuple *bg_color_t = dict_find(iter, MESSAGE_KEY_hour_colour);
  if (bg_color_t) {
    settings.hour_colour = GColorFromHEX(bg_color_t->value->int32);
  }

  // min Color
  Tuple *fg_color_t = dict_find(iter, MESSAGE_KEY_min_colour);
  if (fg_color_t) {
    settings.min_colour = GColorFromHEX(fg_color_t->value->int32);
  }

  // Invert
  Tuple *invert_t = dict_find(iter, MESSAGE_KEY_invert);
  if (invert_t) {
    settings.invert = invert_t->value->int32 == 1;
  }

  // Bluetoothvibe
  Tuple *animations_t = dict_find(iter, MESSAGE_KEY_bluetoothvibe);
  if (animations_t) {
    settings.bluetoothvibe = animations_t->value->int32 == 1;
  }

  // hourlyvibe
  Tuple *hourlyvibe_t = dict_find(iter, MESSAGE_KEY_hourlyvibe);
  if (hourlyvibe_t) {
    settings.hourlyvibe = hourlyvibe_t->value->int32 == 1;
  }
	
  // battbar
  Tuple *battbar_t = dict_find(iter, MESSAGE_KEY_battbar);
  if (battbar_t) {
    settings.battbar = battbar_t->value->int32 == 1;
  }

  // language
  Tuple *language_t = dict_find(iter, MESSAGE_KEY_language);
  if (language_t) {
     settings.language = stringToInt( (char*) language_t->value->data );
  }

	
	
  // Save the new settings to persistent storage
  prv_save_settings();
}


static void init(void) {
	
  prv_load_settings();
	
  // Listen for AppMessages
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(256, 256);

  memset(&time_digits_layers, 0, sizeof(time_digits_layers));
  memset(&time_digits_images, 0, sizeof(time_digits_images));
	
  memset(&date_digits_layers, 0, sizeof(date_digits_layers));
  memset(&date_digits_images, 0, sizeof(date_digits_images));


  window = window_create();
  if (window == NULL) {
      //APP_LOG(APP_LOG_LEVEL_DEBUG, "OOM: couldn't allocate window");
      return;
  }
	
  window_set_background_color(window, GColorBlack);

  window_stack_push(window, true /* Animated */);
  Layer *window_layer = window_get_root_layer(window);
	
  img_battery_100    = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_100);
  img_battery_90     = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_90);
  img_battery_80     = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_80);
  img_battery_70     = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_70);
  img_battery_60     = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_60);
  img_battery_50     = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_50);
  img_battery_40     = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_40);
  img_battery_30     = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_30);
  img_battery_20     = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_20);
  img_battery_10     = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_10);
  img_battery_charge = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BATT_CHARGING);

#ifdef PBL_PLATFORM_EMERY
  layer_batt_img  = bitmap_layer_create(GRect(7, 2, 193, 8));
#else
  layer_batt_img  = bitmap_layer_create(GRect(7, 3, 130, 3));
#endif
  bitmap_layer_set_bitmap(layer_batt_img, img_battery_100);
  layer_add_child(window_layer, bitmap_layer_get_layer(layer_batt_img));
	
	
  // Create time and date layers
  GRect dummy_frame = { {0, 0}, {0, 0} };	
	
 for (int i = 0; i < TOTAL_TIME_DIGITS; ++i) {
    time_digits_layers[i] = bitmap_layer_create(dummy_frame);
    layer_add_child(window_layer, bitmap_layer_get_layer(time_digits_layers[i]));
  }
	
  for (int i = 0; i < TOTAL_DATE_DIGITS; ++i) {
    date_digits_layers[i] = bitmap_layer_create(dummy_frame);
    layer_add_child(window_layer, bitmap_layer_get_layer(date_digits_layers[i]));
  }


   gday_name_layer = bitmap_layer_create(dummy_frame);
   layer_add_child(window_layer, bitmap_layer_get_layer(gday_name_layer));
   gmonth_layer = bitmap_layer_create(dummy_frame);
   layer_add_child(window_layer, bitmap_layer_get_layer(gmonth_layer));
	
   rday_name_layer = bitmap_layer_create(dummy_frame);
   layer_add_child(window_layer, bitmap_layer_get_layer(rday_name_layer));	
   rmonth_layer = bitmap_layer_create(dummy_frame);
   layer_add_child(window_layer, bitmap_layer_get_layer(rmonth_layer));

   fday_name_layer = bitmap_layer_create(dummy_frame);
   layer_add_child(window_layer, bitmap_layer_get_layer(fday_name_layer));	
   fmonth_layer = bitmap_layer_create(dummy_frame);
   layer_add_child(window_layer, bitmap_layer_get_layer(fmonth_layer));

   iday_name_layer = bitmap_layer_create(dummy_frame);
   layer_add_child(window_layer, bitmap_layer_get_layer(iday_name_layer));	
   imonth_layer = bitmap_layer_create(dummy_frame);
   layer_add_child(window_layer, bitmap_layer_get_layer(imonth_layer));
	
   cday_name_layer = bitmap_layer_create(dummy_frame);
   layer_add_child(window_layer, bitmap_layer_get_layer(cday_name_layer));	
   cmonth_layer = bitmap_layer_create(dummy_frame);
   layer_add_child(window_layer, bitmap_layer_get_layer(cmonth_layer));

   sday_name_layer = bitmap_layer_create(dummy_frame);
   layer_add_child(window_layer, bitmap_layer_get_layer(sday_name_layer));	
   smonth_layer = bitmap_layer_create(dummy_frame);
   layer_add_child(window_layer, bitmap_layer_get_layer(smonth_layer));

   day_name_layer = bitmap_layer_create(dummy_frame);
   layer_add_child(window_layer, bitmap_layer_get_layer(day_name_layer));	
   month_layer = bitmap_layer_create(dummy_frame);
   layer_add_child(window_layer, bitmap_layer_get_layer(month_layer));
	
	
  bluetooth_image_on = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BLUETOOTHON);
  GRect bitmap_bounds_bt_on = gbitmap_get_bounds(bluetooth_image_on);	
#ifdef PBL_PLATFORM_EMERY
	GRect frame_bton = GRect(4, 188, bitmap_bounds_bt_on.size.w, bitmap_bounds_bt_on.size.h);
#else 
	GRect frame_bton = GRect(4, 127, bitmap_bounds_bt_on.size.w, bitmap_bounds_bt_on.size.h);
#endif
  bluetooth_layer_on = bitmap_layer_create(frame_bton);
  bitmap_layer_set_bitmap(bluetooth_layer_on, bluetooth_image_on);
  layer_add_child(window_layer, bitmap_layer_get_layer(bluetooth_layer_on));
	
	
  bluetooth_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BLUETOOTHOFF);
  GRect bitmap_bounds2 = gbitmap_get_bounds(bluetooth_image);
#ifdef PBL_PLATFORM_EMERY
    GRect frame2 = GRect(4, 188, bitmap_bounds2.size.w, bitmap_bounds2.size.h);
#else
	GRect frame2 = GRect(4, 127, bitmap_bounds2.size.w, bitmap_bounds2.size.h);
#endif
  bluetooth_layer = bitmap_layer_create(frame2);
  bitmap_layer_set_bitmap(bluetooth_layer, bluetooth_image);
  layer_add_child(window_layer, bitmap_layer_get_layer(bluetooth_layer));


#ifdef PBL_PLATFORM_EMERY
	effect_layer_col_hr  = effect_layer_create(GRect(18,12,82,207));
#else
	effect_layer_col_hr  = effect_layer_create(GRect(18,12,52,150));
#endif
  effect_layer_add_effect(effect_layer_col_hr, effect_colorswap, &colorpair_1);
  layer_add_child(window_layer, effect_layer_get_layer(effect_layer_col_hr));

#ifdef PBL_PLATFORM_EMERY
	effect_layer_col_min  = effect_layer_create(GRect(119,12,82,207));
#else
	effect_layer_col_min  = effect_layer_create(GRect(89,12,55,150));
#endif
  effect_layer_add_effect(effect_layer_col_min, effect_colorswap, &colorpair_2);
  layer_add_child(window_layer, effect_layer_get_layer(effect_layer_col_min));

  toggle_bluetooth_icon(bluetooth_connection_service_peek());
  update_battery(battery_state_service_peek());


  prv_update_display();

  
  // Avoids a blank screen on watch start.
  time_t now = time(NULL);
  struct tm *tick_time = localtime(&now);  
  handle_tick(tick_time, MONTH_UNIT + DAY_UNIT + HOUR_UNIT + MINUTE_UNIT );
	
  tick_timer_service_subscribe(MINUTE_UNIT, handle_tick);
  bluetooth_connection_service_subscribe(bluetooth_connection_callback);
  battery_state_service_subscribe(&update_battery);

}

static void deinit(void) {
  
  tick_timer_service_unsubscribe();
  bluetooth_connection_service_unsubscribe();
  battery_state_service_unsubscribe();

  layer_remove_from_parent(bitmap_layer_get_layer(separator_layer));
  bitmap_layer_destroy(separator_layer);
  gbitmap_destroy(separator_image);
  
  layer_remove_from_parent(bitmap_layer_get_layer(bluetooth_layer));
  bitmap_layer_destroy(bluetooth_layer);
  gbitmap_destroy(bluetooth_image);
  
  layer_remove_from_parent(bitmap_layer_get_layer(bluetooth_layer_on));
  bitmap_layer_destroy(bluetooth_layer_on);
  gbitmap_destroy(bluetooth_image_on);

  layer_remove_from_parent(bitmap_layer_get_layer(day_name_layer));
  bitmap_layer_destroy(day_name_layer);
  gbitmap_destroy(day_name_image);
	
  layer_remove_from_parent(bitmap_layer_get_layer(gday_name_layer));
  bitmap_layer_destroy(gday_name_layer);
  gbitmap_destroy(gday_name_image);
	
  layer_remove_from_parent(bitmap_layer_get_layer(rday_name_layer));
  bitmap_layer_destroy(rday_name_layer);
  gbitmap_destroy(rday_name_image);
	
  layer_remove_from_parent(bitmap_layer_get_layer(fday_name_layer));
  bitmap_layer_destroy(fday_name_layer);
  gbitmap_destroy(fday_name_image);
	
  layer_remove_from_parent(bitmap_layer_get_layer(iday_name_layer));
  bitmap_layer_destroy(iday_name_layer);
  gbitmap_destroy(iday_name_image);
	
  layer_remove_from_parent(bitmap_layer_get_layer(cday_name_layer));
  bitmap_layer_destroy(cday_name_layer);
  gbitmap_destroy(cday_name_image);
	
  layer_remove_from_parent(bitmap_layer_get_layer(sday_name_layer));
  bitmap_layer_destroy(sday_name_layer);
  gbitmap_destroy(sday_name_image);
	
  layer_remove_from_parent(bitmap_layer_get_layer(month_layer));
  bitmap_layer_destroy(month_layer);
  gbitmap_destroy(month_image);  
	
  layer_remove_from_parent(bitmap_layer_get_layer(gmonth_layer));
  bitmap_layer_destroy(gmonth_layer);
  gbitmap_destroy(gmonth_image);  
	
  layer_remove_from_parent(bitmap_layer_get_layer(rmonth_layer));
  bitmap_layer_destroy(rmonth_layer);
  gbitmap_destroy(rmonth_image);
	
  layer_remove_from_parent(bitmap_layer_get_layer(fmonth_layer));
  bitmap_layer_destroy(fmonth_layer);
  gbitmap_destroy(fmonth_image);
	
  layer_remove_from_parent(bitmap_layer_get_layer(imonth_layer));
  bitmap_layer_destroy(imonth_layer);
  gbitmap_destroy(imonth_image);
		
  layer_remove_from_parent(bitmap_layer_get_layer(cmonth_layer));
  bitmap_layer_destroy(cmonth_layer);
  gbitmap_destroy(cmonth_image);
	
 layer_remove_from_parent(bitmap_layer_get_layer(smonth_layer));
  bitmap_layer_destroy(smonth_layer);
  gbitmap_destroy(smonth_image);

  layer_remove_from_parent(bitmap_layer_get_layer(layer_batt_img));
  bitmap_layer_destroy(layer_batt_img);
  layer_batt_img = NULL;
	
  gbitmap_destroy(img_battery_100);
  gbitmap_destroy(img_battery_90);
  gbitmap_destroy(img_battery_80);
  gbitmap_destroy(img_battery_70);
  gbitmap_destroy(img_battery_60);
  gbitmap_destroy(img_battery_50);
  gbitmap_destroy(img_battery_40);
  gbitmap_destroy(img_battery_30);
  gbitmap_destroy(img_battery_20);
  gbitmap_destroy(img_battery_10);
  gbitmap_destroy(img_battery_charge);	
	
  for (int i = 0; i < TOTAL_DATE_DIGITS; i++) {
    layer_remove_from_parent(bitmap_layer_get_layer(date_digits_layers[i]));
    gbitmap_destroy(date_digits_images[i]);
    bitmap_layer_destroy(date_digits_layers[i]);
  }

  for (int i = 0; i < TOTAL_TIME_DIGITS; i++) {
    layer_remove_from_parent(bitmap_layer_get_layer(time_digits_layers[i]));
    gbitmap_destroy(time_digits_images[i]);
    bitmap_layer_destroy(time_digits_layers[i]);
  }

if ( effect_layer_invert != NULL) {
    // Remove Inverter layer
   layer_remove_from_parent(effect_layer_get_layer(effect_layer_invert));
   effect_layer_destroy(effect_layer_invert);
   effect_layer_invert = NULL;
}
	
  layer_remove_from_parent(effect_layer_get_layer(effect_layer_col_hr));
  effect_layer_destroy(effect_layer_col_hr);
  effect_layer_col_hr = NULL;
	
  layer_remove_from_parent(effect_layer_get_layer(effect_layer_col_min));
  effect_layer_destroy(effect_layer_col_min);
  effect_layer_col_min = NULL;
	
  window_destroy(window);

}

int main(void) {
  init();
  app_event_loop();
  deinit();
}