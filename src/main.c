#include "pebble.h"
//#include "effect_layer.h"

//EffectLayer* effect_layer_inv;
//EffectLayer* effect_layer_col_hr;
//EffectLayer* effect_layer_col_min;

static Window *window;
static Layer *window_layer;

static uint8_t batteryPercent;

static AppSync sync;
static uint8_t sync_buffer[256];

static int blink;
static int invert;
static int bluetoothvibe;
static int hourlyvibe;
static int battbar;
static int language;
static int hour_colour;
static int min_colour;

static bool appStarted = false;

enum {
  BLINK_KEY = 0x0,
  INVERT_KEY = 0x1,
  BLUETOOTHVIBE_KEY = 0x2,
  HOURLYVIBE_KEY = 0x3,
  LANGUAGE_KEY = 0x4,
  BATTBAR_KEY = 0x5,
  HOUR_COLOUR_KEY = 0x6,
  MIN_COLOUR_KEY = 0x7
};
/*
	// initializing colors
struct EffectColorpair {
  GColor firstColor;  // first color (target for colorize, one of set in colorswap)
  GColor secondColor; // second color (new color for colorize, other of set in colorswap)
};
  
EffectColorpair colorpair_1;
EffectColorpair colorpair_2;
*/
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
/*
static GBitmap *kday_name_image;
static BitmapLayer *kday_name_layer;

const int DAY_NAME_IMAGE_KOREAN_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_DAY_NAME_KO_SUN,
  RESOURCE_ID_IMAGE_DAY_NAME_KO_MON,
  RESOURCE_ID_IMAGE_DAY_NAME_KO_TUE,
  RESOURCE_ID_IMAGE_DAY_NAME_KO_WED,
  RESOURCE_ID_IMAGE_DAY_NAME_KO_THU,
  RESOURCE_ID_IMAGE_DAY_NAME_KO_FRI,
  RESOURCE_ID_IMAGE_DAY_NAME_KO_SAT
};	
*/
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
/*
static GBitmap *kmonth_image;
static BitmapLayer *kmonth_layer;

const int KMONTH_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_KJAN,
  RESOURCE_ID_IMAGE_KFEB,
  RESOURCE_ID_IMAGE_KMAR,
  RESOURCE_ID_IMAGE_KAPR,
  RESOURCE_ID_IMAGE_KMAY,
  RESOURCE_ID_IMAGE_KJUN,
  RESOURCE_ID_IMAGE_KJUL,
  RESOURCE_ID_IMAGE_KAUG,
  RESOURCE_ID_IMAGE_KSEP,
  RESOURCE_ID_IMAGE_KOCT,
  RESOURCE_ID_IMAGE_KNOV,
  RESOURCE_ID_IMAGE_KDEC
};
*/
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
/*

void change_hour() {
	  
    switch (hour_colour) {
		
		case 0: //white
  			colorpair_1.firstColor = GColorWhite;		
  			colorpair_1.secondColor = GColorWhite;
  			effect_layer_col_hr  = effect_layer_create(GRect(18,12,52,154));
  			effect_layer_add_effect(effect_layer_col_hr, effect_colorize, &colorpair_1);
			layer_add_child(window_layer, effect_layer_get_layer(effect_layer_col_hr));
		break;	
		
		case 1: //green	
  			colorpair_1.firstColor = GColorWhite;
  			colorpair_1.secondColor = GColorWhite;
		#ifdef PBL_COLOR		
  			colorpair_1.secondColor = GColorGreen;
		#endif	
  			effect_layer_col_hr  = effect_layer_create(GRect(18,12,52,154));
  			effect_layer_add_effect(effect_layer_col_hr, effect_colorize, &colorpair_1);
			layer_add_child(window_layer, effect_layer_get_layer(effect_layer_col_hr));
		break;

		case 2: //orange
			colorpair_1.firstColor = GColorWhite;
			colorpair_1.secondColor = GColorWhite;
		#ifdef PBL_COLOR		
			colorpair_1.secondColor = GColorOrange;
		#endif	
			effect_layer_col_hr  = effect_layer_create(GRect(18,12,52,154));
			effect_layer_add_effect(effect_layer_col_hr, effect_colorize, &colorpair_1);
			layer_add_child(window_layer, effect_layer_get_layer(effect_layer_col_hr));
		break;

		case 3: //GColorCyan
			colorpair_1.firstColor = GColorWhite;
			colorpair_1.secondColor = GColorWhite;
		#ifdef PBL_COLOR		
			colorpair_1.secondColor = GColorCyan ;
		#endif	
			effect_layer_col_hr  = effect_layer_create(GRect(18,16,50,150));
			effect_layer_add_effect(effect_layer_col_hr, effect_colorize, &colorpair_1);
			layer_add_child(window_layer, effect_layer_get_layer(effect_layer_col_hr));
		break;
		
		case 4: //yellow
			colorpair_1.firstColor = GColorWhite;
			colorpair_1.secondColor = GColorWhite;
		#ifdef PBL_COLOR		
			colorpair_1.secondColor = GColorYellow;
		#endif	
			effect_layer_col_hr  = effect_layer_create(GRect(18,12,52,154));
			effect_layer_add_effect(effect_layer_col_hr, effect_colorize, &colorpair_1);
			layer_add_child(window_layer, effect_layer_get_layer(effect_layer_col_hr));
		break;
		
		case 5: //GColorSunsetOrange 
			colorpair_1.firstColor = GColorWhite;
			colorpair_1.secondColor = GColorWhite;
		#ifdef PBL_COLOR		
			colorpair_1.secondColor = GColorSunsetOrange;
		#endif	
			effect_layer_col_hr  = effect_layer_create(GRect(18,12,52,154));
			effect_layer_add_effect(effect_layer_col_hr, effect_colorize, &colorpair_1);
			layer_add_child(window_layer, effect_layer_get_layer(effect_layer_col_hr));
		break;
		
		case 6: //GColorVividViolet 
			colorpair_1.firstColor = GColorWhite;
			colorpair_1.secondColor = GColorWhite;
		#ifdef PBL_COLOR		
			colorpair_1.secondColor = GColorVividViolet ;
		#endif	
			effect_layer_col_hr  = effect_layer_create(GRect(18,12,52,154));
			effect_layer_add_effect(effect_layer_col_hr, effect_colorize, &colorpair_1);
			layer_add_child(window_layer, effect_layer_get_layer(effect_layer_col_hr));
		break;
		
		case 7: //GColorShockingPink  
			colorpair_1.firstColor = GColorWhite;
			colorpair_1.secondColor = GColorWhite;
		#ifdef PBL_COLOR		
			colorpair_1.secondColor = GColorMagenta ;
		#endif	
			effect_layer_col_hr  = effect_layer_create(GRect(18,12,52,154));
			effect_layer_add_effect(effect_layer_col_hr, effect_colorize, &colorpair_1);
			layer_add_child(window_layer, effect_layer_get_layer(effect_layer_col_hr));
		break;
		
		case 8: //GColorBrightGreen  
			colorpair_1.firstColor = GColorWhite;
			colorpair_1.secondColor = GColorWhite;
		#ifdef PBL_COLOR		
			colorpair_1.secondColor = GColorBrightGreen  ;
		#endif	
			effect_layer_col_hr  = effect_layer_create(GRect(18,12,52,154));
			effect_layer_add_effect(effect_layer_col_hr, effect_colorize, &colorpair_1);
			layer_add_child(window_layer, effect_layer_get_layer(effect_layer_col_hr));
		break;

		case 9: //GColorElectricBlue 
			colorpair_1.firstColor = GColorWhite;
			colorpair_1.secondColor = GColorWhite;
		#ifdef PBL_COLOR		
			colorpair_1.secondColor = GColorElectricBlue ;
		#endif	
			effect_layer_col_hr  = effect_layer_create(GRect(18,16,50,150));
			effect_layer_add_effect(effect_layer_col_hr, effect_colorize, &colorpair_1);
			layer_add_child(window_layer, effect_layer_get_layer(effect_layer_col_hr));
		break;
		
		case 10: //GColorChromeYellow 
			colorpair_1.firstColor = GColorWhite;
			colorpair_1.secondColor = GColorWhite;
		#ifdef PBL_COLOR		
			colorpair_1.secondColor = GColorChromeYellow ;
		#endif	
			effect_layer_col_hr  = effect_layer_create(GRect(18,12,52,154));
			effect_layer_add_effect(effect_layer_col_hr, effect_colorize, &colorpair_1);
			layer_add_child(window_layer, effect_layer_get_layer(effect_layer_col_hr));
		break;
		
		case 11: //GColorDarkGray 
			colorpair_1.firstColor = GColorWhite;
			colorpair_1.secondColor = GColorWhite;
		#ifdef PBL_COLOR		
			colorpair_1.secondColor = GColorDarkGray ;
		#endif	
			effect_layer_col_hr  = effect_layer_create(GRect(18,12,52,154));
			effect_layer_add_effect(effect_layer_col_hr, effect_colorize, &colorpair_1);
			layer_add_child(window_layer, effect_layer_get_layer(effect_layer_col_hr));
		break;
	}    
}

void change_min() {

    switch (min_colour) {
		case 0: //white
  			colorpair_2.firstColor = GColorWhite;
  			colorpair_2.secondColor = GColorWhite;
  			effect_layer_col_min  = effect_layer_create(GRect(89,12,55,154));
  			effect_layer_add_effect(effect_layer_col_min, effect_colorize, &colorpair_2);
			layer_add_child(window_layer, effect_layer_get_layer(effect_layer_col_min));
		break;
		
		case 1: // green
  			colorpair_2.firstColor = GColorWhite;
  			colorpair_2.secondColor = GColorWhite;
		#ifdef PBL_COLOR		
  			colorpair_2.secondColor = GColorGreen;
		#endif	
  			effect_layer_col_min  = effect_layer_create(GRect(89,12,55,154));
  			effect_layer_add_effect(effect_layer_col_min, effect_colorize, &colorpair_2);
			layer_add_child(window_layer, effect_layer_get_layer(effect_layer_col_min));
		break;

		case 2: //orange
  			colorpair_2.firstColor = GColorWhite;	
  			colorpair_2.secondColor = GColorWhite;
		#ifdef PBL_COLOR		
  			colorpair_2.secondColor = GColorOrange;
		#endif	
  			effect_layer_col_min  = effect_layer_create(GRect(89,12,55,154));
  			effect_layer_add_effect(effect_layer_col_min, effect_colorize, &colorpair_2);
			layer_add_child(window_layer, effect_layer_get_layer(effect_layer_col_min));
		break;

		case 3: //GColorCyan 
  			colorpair_2.firstColor = GColorWhite;	
  			colorpair_2.secondColor = GColorWhite;
		#ifdef PBL_COLOR		
  			colorpair_2.secondColor = GColorCyan ;
		#endif	
  			effect_layer_col_min  = effect_layer_create(GRect(89,12,55,154));
  			effect_layer_add_effect(effect_layer_col_min, effect_colorize, &colorpair_2);
			layer_add_child(window_layer, effect_layer_get_layer(effect_layer_col_min));
		break;	
		
		case 4: //GColorYellow  
  			colorpair_2.firstColor = GColorWhite;	
  			colorpair_2.secondColor = GColorWhite;
		#ifdef PBL_COLOR		
  			colorpair_2.secondColor = GColorYellow  ;
		#endif	
  			effect_layer_col_min  = effect_layer_create(GRect(89,12,55,154));
  			effect_layer_add_effect(effect_layer_col_min, effect_colorize, &colorpair_2);
			layer_add_child(window_layer, effect_layer_get_layer(effect_layer_col_min));
		break;
		
		case 5: //GColorSunsetOrange  
  			colorpair_2.firstColor = GColorWhite;	
  			colorpair_2.secondColor = GColorWhite;
		#ifdef PBL_COLOR		
  			colorpair_2.secondColor = GColorSunsetOrange  ;
		#endif	
  			effect_layer_col_min  = effect_layer_create(GRect(89,12,55,154));
  			effect_layer_add_effect(effect_layer_col_min, effect_colorize, &colorpair_2);
  			layer_add_child(window_get_root_layer(window), effect_layer_get_layer(effect_layer_col_min));
		break;

		case 6: //GColorVividViolet  
  			colorpair_2.firstColor = GColorWhite;	
  			colorpair_2.secondColor = GColorWhite;
		#ifdef PBL_COLOR		
  			colorpair_2.secondColor = GColorVividViolet  ;
		#endif	
  			effect_layer_col_min  = effect_layer_create(GRect(89,12,55,154));
 			effect_layer_add_effect(effect_layer_col_min, effect_colorize, &colorpair_2);
			layer_add_child(window_layer, effect_layer_get_layer(effect_layer_col_min));
		break;

		case 7: //GColorShockingPink   
			colorpair_2.firstColor = GColorWhite;	
			colorpair_2.secondColor = GColorWhite;
		#ifdef PBL_COLOR		
			colorpair_2.secondColor = GColorShockingPink   ;
		#endif	
			effect_layer_col_min  = effect_layer_create(GRect(89,12,55,154));
			effect_layer_add_effect(effect_layer_col_min, effect_colorize, &colorpair_2);
			layer_add_child(window_layer, effect_layer_get_layer(effect_layer_col_min));
		break;
		
		case 8: //GColorBrightGreen    
			colorpair_2.firstColor = GColorWhite;	
			colorpair_2.secondColor = GColorWhite;
		#ifdef PBL_COLOR		
			colorpair_2.secondColor = GColorBrightGreen    ;
		#endif	
			effect_layer_col_min  = effect_layer_create(GRect(89,12,55,154));
			effect_layer_add_effect(effect_layer_col_min, effect_colorize, &colorpair_2);
			layer_add_child(window_layer, effect_layer_get_layer(effect_layer_col_min));
		break;
		
		case 9: //GColorElectricBlue    
			colorpair_2.firstColor = GColorWhite;	
			colorpair_2.secondColor = GColorWhite;
		#ifdef PBL_COLOR		
			colorpair_2.secondColor = GColorElectricBlue    ;
		#endif	
			effect_layer_col_min  = effect_layer_create(GRect(89,12,55,154));
			effect_layer_add_effect(effect_layer_col_min, effect_colorize, &colorpair_2);
			layer_add_child(window_layer, effect_layer_get_layer(effect_layer_col_min));
		break;
		
		case 10: //GColorChromeYellow   
			colorpair_2.firstColor = GColorWhite;	
			colorpair_2.secondColor = GColorWhite;
		#ifdef PBL_COLOR		
			colorpair_2.secondColor = GColorChromeYellow   ;
		#endif	
			effect_layer_col_min  = effect_layer_create(GRect(89,12,55,154));
			effect_layer_add_effect(effect_layer_col_min, effect_colorize, &colorpair_2);
			layer_add_child(window_layer, effect_layer_get_layer(effect_layer_col_min));
		break;
		
		case 11: //GColorDarkGray   
			colorpair_2.firstColor = GColorWhite;	
			colorpair_2.secondColor = GColorWhite;
		#ifdef PBL_COLOR		
			colorpair_2.secondColor = GColorDarkGray   ;
		#endif	
			effect_layer_col_min  = effect_layer_create(GRect(89,12,55,154));
			effect_layer_add_effect(effect_layer_col_min, effect_colorize, &colorpair_2);
			layer_add_child(window_layer, effect_layer_get_layer(effect_layer_col_min));
		break;
		
    }    
}

void set_invert_color(bool invert) {
  if (invert && effect_layer_inv == NULL) {
    // Add inverter layer
 //   Layer *window_layer = window_get_root_layer(window);

    effect_layer_inv = effect_layer_create(GRect(0, 12, 144, 158));
    layer_add_child(window_layer, effect_layer_get_layer(effect_layer_inv));
    effect_layer_add_effect(effect_layer_inv, effect_invert, NULL);
	  
  } else if (!invert && effect_layer_inv != NULL) {
    // Remove Inverter layer
   layer_remove_from_parent(effect_layer_get_layer(effect_layer_inv));
   effect_layer_destroy(effect_layer_inv);
   effect_layer_inv = NULL;
  }
  // No action required
}
*/

void hidebatt (bool battbar) {
	
	  if (battbar) {
		  	layer_set_hidden(bitmap_layer_get_layer(layer_batt_img), true);
	  } else {
		  	layer_set_hidden(bitmap_layer_get_layer(layer_batt_img), false);
	  }
}

void change_language() {

    switch (language) {
		case 0: //english
		
		//	layer_set_hidden(bitmap_layer_get_layer(kmonth_layer), true);
		//	layer_set_hidden(bitmap_layer_get_layer(kday_name_layer), true);
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
		
		case 1:  //german
		
		//	layer_set_hidden(bitmap_layer_get_layer(kmonth_layer), true);
		//	layer_set_hidden(bitmap_layer_get_layer(kday_name_layer), true);
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
			
		case 2:  //french		
		
	//		layer_set_hidden(bitmap_layer_get_layer(kmonth_layer), true);
	//		layer_set_hidden(bitmap_layer_get_layer(kday_name_layer), true);
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
			
		case 3:  //italian
		
	//		layer_set_hidden(bitmap_layer_get_layer(kmonth_layer), true);
	//		layer_set_hidden(bitmap_layer_get_layer(kday_name_layer), true);
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
		
		case 4:  //russian	
		
	//		layer_set_hidden(bitmap_layer_get_layer(kmonth_layer), true);
	//		layer_set_hidden(bitmap_layer_get_layer(kday_name_layer), true);
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
		
		case 5:  //chinese	

		//	layer_set_hidden(bitmap_layer_get_layer(kmonth_layer), true);
		//	layer_set_hidden(bitmap_layer_get_layer(kday_name_layer), true);
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
/*
		case 6:  //korean	
		
			layer_set_hidden(bitmap_layer_get_layer(smonth_layer), true);
			layer_set_hidden(bitmap_layer_get_layer(sday_name_layer), true);
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
		
	        layer_set_hidden(bitmap_layer_get_layer(kmonth_layer), false);
			layer_set_hidden(bitmap_layer_get_layer(kday_name_layer), false);		
		
			break;
		*/
		case 6:  //spanish	
		
	//		layer_set_hidden(bitmap_layer_get_layer(kmonth_layer), true);
	//		layer_set_hidden(bitmap_layer_get_layer(kday_name_layer), true);
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
}

static void handle_tick(struct tm *tick_time, TimeUnits units_changed);

static void sync_tuple_changed_callback(const uint32_t key, const Tuple* new_tuple, const Tuple* old_tuple, void* context) {
  switch (key) {
    case BLINK_KEY:
        blink = new_tuple->value->uint8;
	    persist_write_bool(INVERT_KEY, blink);
        tick_timer_service_unsubscribe();
      if(blink) {
        tick_timer_service_subscribe(SECOND_UNIT, handle_tick);
      }
      else {
        tick_timer_service_subscribe(MINUTE_UNIT, handle_tick);
		layer_set_hidden(bitmap_layer_get_layer(separator_layer), true);
	  }
    break;
	  
    case INVERT_KEY:
    	invert = new_tuple->value->uint8 != 0;
		persist_write_bool(INVERT_KEY, invert);
    //	set_invert_color(invert);
    break;
	  
	case BLUETOOTHVIBE_KEY:
     	bluetoothvibe = new_tuple->value->uint8 != 0;
	persist_write_bool(BLUETOOTHVIBE_KEY, bluetoothvibe);
    break;      
	  
    case HOURLYVIBE_KEY:
    	hourlyvibe = new_tuple->value->uint8 != 0;
		persist_write_bool(HOURLYVIBE_KEY, hourlyvibe);	  
    break;	
	  
    case BATTBAR_KEY:
    	battbar = new_tuple->value->uint8 != 0;
		persist_write_bool(BATTBAR_KEY, battbar);
	    hidebatt (battbar);
	break;
	    
    case LANGUAGE_KEY:
		language = new_tuple->value->uint8;
		persist_write_bool(LANGUAGE_KEY, language);
		change_language();
	break;
	  
    case HOUR_COLOUR_KEY:
		hour_colour = new_tuple->value->uint8;
		persist_write_bool(HOUR_COLOUR_KEY, hour_colour);
	   // change_hour();
	break;
	  
    case MIN_COLOUR_KEY:
		min_colour = new_tuple->value->uint8;
		persist_write_bool(MIN_COLOUR_KEY, min_colour);
	//	change_min();
	break;
  }
}

static void set_container_image(GBitmap **bmp_image, BitmapLayer *bmp_layer, const int resource_id, GPoint origin) {
  GBitmap *old_image = *bmp_image;

  *bmp_image = gbitmap_create_with_resource(resource_id);
#ifdef PBL_PLATFORM_BASALT
  GRect bitmap_bounds = gbitmap_get_bounds((*bmp_image));
#else
  GRect bitmap_bounds = (*bmp_image)->bounds;
#endif
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
  if(appStarted && !connected && bluetoothvibe) {
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

  set_container_image(&sday_name_image, sday_name_layer, DAY_NAME_IMAGE_SPANISH_RESOURCE_IDS[tick_time->tm_wday], GPoint(73, 20));
 // set_container_image(&kday_name_image, kday_name_layer, DAY_NAME_IMAGE_KOREAN_RESOURCE_IDS[tick_time->tm_wday], GPoint(73, 20));
  set_container_image(&cday_name_image, cday_name_layer, DAY_NAME_IMAGE_CHINESE_RESOURCE_IDS[tick_time->tm_wday], GPoint(73, 20));
  set_container_image(&iday_name_image, iday_name_layer, DAY_NAME_IMAGE_ITALIAN_RESOURCE_IDS[tick_time->tm_wday], GPoint(73, 20));
  set_container_image(&fday_name_image, fday_name_layer, DAY_NAME_IMAGE_FRENCH_RESOURCE_IDS[tick_time->tm_wday], GPoint(72, 20));
  set_container_image(&rday_name_image, rday_name_layer, DAY_NAME_IMAGE_RUSSIAN_RESOURCE_IDS[tick_time->tm_wday], GPoint(72, 20));
  set_container_image(&gday_name_image, gday_name_layer, DAY_NAME_IMAGE_GERMAN_RESOURCE_IDS[tick_time->tm_wday], GPoint(73, 20));
  set_container_image(&day_name_image, day_name_layer, DAY_NAME_IMAGE_RESOURCE_IDS[tick_time->tm_wday], GPoint(76, 20));
	
  set_container_image(&date_digits_images[0], date_digits_layers[0], DATENUM_IMAGE_RESOURCE_IDS[tick_time->tm_mday/10], GPoint(75,143));
  set_container_image(&date_digits_images[1], date_digits_layers[1], DATENUM_IMAGE_RESOURCE_IDS[tick_time->tm_mday%10], GPoint(75, 152));
}

//DRAW MONTH
	
static void update_months(struct tm *tick_time) {	
	
  set_container_image(&gmonth_image, gmonth_layer, GMONTH_IMAGE_RESOURCE_IDS[tick_time->tm_mon], GPoint(2, 20));
  set_container_image(&rmonth_image, rmonth_layer, RMONTH_IMAGE_RESOURCE_IDS[tick_time->tm_mon], GPoint(2, 20));
  set_container_image(&fmonth_image, fmonth_layer, FMONTH_IMAGE_RESOURCE_IDS[tick_time->tm_mon], GPoint(3, 20));
  set_container_image(&imonth_image, imonth_layer, IMONTH_IMAGE_RESOURCE_IDS[tick_time->tm_mon], GPoint(3, 20));
  set_container_image(&cmonth_image, cmonth_layer, CMONTH_IMAGE_RESOURCE_IDS[tick_time->tm_mon], GPoint(3, 20));
//  set_container_image(&kmonth_image, kmonth_layer, KMONTH_IMAGE_RESOURCE_IDS[tick_time->tm_mon], GPoint(3, 20));
  set_container_image(&smonth_image, smonth_layer, SMONTH_IMAGE_RESOURCE_IDS[tick_time->tm_mon], GPoint(3, 20));
  set_container_image(&month_image, month_layer, MONTH_IMAGE_RESOURCE_IDS[tick_time->tm_mon], GPoint(5, 20));
}

static void update_hours(struct tm *tick_time) {

  if(appStarted && hourlyvibe) {
    //vibe!
    vibes_short_pulse();
  }
  
  unsigned short display_hour = get_display_hour(tick_time->tm_hour);

  set_container_image(&time_digits_images[0], time_digits_layers[0], BIG_DIGIT_IMAGE_RESOURCE_IDS[display_hour/10], GPoint(22, 20));
  set_container_image(&time_digits_images[1], time_digits_layers[1], BIG_DIGIT_IMAGE_RESOURCE_IDS[display_hour%10], GPoint(46, 20));

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
	
  set_container_image(&time_digits_images[2], time_digits_layers[2], BIG_DIGIT_IMAGE_RESOURCE_IDS[tick_time->tm_min/10], GPoint(96, 20));
  set_container_image(&time_digits_images[3], time_digits_layers[3], BIG_DIGIT_IMAGE_RESOURCE_IDS[tick_time->tm_min%10], GPoint(120, 20));
}

static void update_seconds(struct tm *tick_time) {
	
  if(blink) {
    layer_set_hidden(bitmap_layer_get_layer(separator_layer), tick_time->tm_sec%2);
  }
  else {
    if(layer_get_hidden(bitmap_layer_get_layer(separator_layer))) {
      layer_set_hidden(bitmap_layer_get_layer(separator_layer), true);
    }
  }
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
  if (units_changed & SECOND_UNIT) {
    update_seconds(tick_time);
  }		
}

static void init(void) {
	
  const int inbound_size = 256;
  const int outbound_size = 256;
  app_message_open(inbound_size, outbound_size);  

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
  window_layer = window_get_root_layer(window);
	
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

  layer_batt_img  = bitmap_layer_create(GRect(7, 1, 130, 3));
  bitmap_layer_set_bitmap(layer_batt_img, img_battery_100);
  layer_add_child(window_layer, bitmap_layer_get_layer(layer_batt_img));
	
  separator_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_SEPARATOR);
#ifdef PBL_PLATFORM_BASALT
  GRect bitmap_bounds = gbitmap_get_bounds(separator_image);
#else
  GRect bitmap_bounds = separator_image->bounds;
#endif	
  GRect frame = GRect(75, 133, bitmap_bounds.size.w, bitmap_bounds.size.h);
  separator_layer = bitmap_layer_create(frame);
  bitmap_layer_set_bitmap(separator_layer, separator_image);
  layer_add_child(window_layer, bitmap_layer_get_layer(separator_layer));   
	
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

   day_name_layer = bitmap_layer_create(dummy_frame);
   layer_add_child(window_layer, bitmap_layer_get_layer(day_name_layer));	
   month_layer = bitmap_layer_create(dummy_frame);
   layer_add_child(window_layer, bitmap_layer_get_layer(month_layer));
	
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
/*		
   kday_name_layer = bitmap_layer_create(dummy_frame);
   layer_add_child(window_layer, bitmap_layer_get_layer(kday_name_layer));	
   kmonth_layer = bitmap_layer_create(dummy_frame);
   layer_add_child(window_layer, bitmap_layer_get_layer(kmonth_layer));
*/
   sday_name_layer = bitmap_layer_create(dummy_frame);
   layer_add_child(window_layer, bitmap_layer_get_layer(sday_name_layer));	
   smonth_layer = bitmap_layer_create(dummy_frame);
   layer_add_child(window_layer, bitmap_layer_get_layer(smonth_layer));


 
 
  bluetooth_image_on = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BLUETOOTHON);
#ifdef PBL_PLATFORM_BASALT
  GRect bitmap_bounds_bt_on = gbitmap_get_bounds(bluetooth_image_on);
#else
  GRect bitmap_bounds_bt_on = bluetooth_image_on->bounds;
#endif	
  GRect frame_bton = GRect(4, 131, bitmap_bounds_bt_on.size.w, bitmap_bounds_bt_on.size.h);
  bluetooth_layer_on = bitmap_layer_create(frame_bton);
  bitmap_layer_set_bitmap(bluetooth_layer_on, bluetooth_image_on);
  layer_add_child(window_layer, bitmap_layer_get_layer(bluetooth_layer_on));
	
	
  bluetooth_image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BLUETOOTHOFF);
#ifdef PBL_PLATFORM_BASALT
  GRect bitmap_bounds2 = gbitmap_get_bounds(bluetooth_image);
#else
  GRect bitmap_bounds2 = bluetooth_image->bounds;
#endif	
  GRect frame2 = GRect(4, 131, bitmap_bounds2.size.w, bitmap_bounds2.size.h);
  bluetooth_layer = bitmap_layer_create(frame2);
  bitmap_layer_set_bitmap(bluetooth_layer, bluetooth_image);
  layer_add_child(window_layer, bitmap_layer_get_layer(bluetooth_layer));
/*
  colorpair_1.firstColor = GColorWhite;
  colorpair_1.secondColor = GColorWhite;
  effect_layer_col_hr  = effect_layer_create(GRect(18,12,52,154));
  effect_layer_add_effect(effect_layer_col_hr, effect_colorize, &colorpair_1);
  layer_add_child(window_layer, effect_layer_get_layer(effect_layer_col_hr));

  colorpair_2.firstColor = GColorWhite;
  colorpair_2.secondColor = GColorWhite;
  effect_layer_col_min  = effect_layer_create(GRect(89,12,55,154));
  effect_layer_add_effect(effect_layer_col_min, effect_colorize, &colorpair_2);
  layer_add_child(window_layer, effect_layer_get_layer(effect_layer_col_min));
*/	
  toggle_bluetooth_icon(bluetooth_connection_service_peek());
  update_battery(battery_state_service_peek());

  Tuplet initial_values[] = {
    TupletInteger(BLINK_KEY, persist_read_bool(BLINK_KEY)),
    TupletInteger(INVERT_KEY, persist_read_bool(INVERT_KEY)),
    TupletInteger(BLUETOOTHVIBE_KEY, persist_read_bool(BLUETOOTHVIBE_KEY)),
    TupletInteger(HOURLYVIBE_KEY, persist_read_bool(HOURLYVIBE_KEY)),
    TupletInteger(BATTBAR_KEY, persist_read_bool(BATTBAR_KEY)),
	TupletInteger(LANGUAGE_KEY, persist_read_bool(LANGUAGE_KEY)),
	TupletInteger(HOUR_COLOUR_KEY, persist_read_bool(HOUR_COLOUR_KEY)),
	TupletInteger(MIN_COLOUR_KEY, persist_read_bool(MIN_COLOUR_KEY)),
  };
  
  app_sync_init(&sync, sync_buffer, sizeof(sync_buffer), initial_values, ARRAY_LENGTH(initial_values),
      sync_tuple_changed_callback, NULL, NULL);
   
  appStarted = true;
  
  // Avoids a blank screen on watch start.
  time_t now = time(NULL);
  struct tm *tick_time = localtime(&now);  
  handle_tick(tick_time, MONTH_UNIT + DAY_UNIT + HOUR_UNIT + MINUTE_UNIT + SECOND_UNIT);
	
  tick_timer_service_subscribe(SECOND_UNIT, handle_tick);
  bluetooth_connection_service_subscribe(bluetooth_connection_callback);
  battery_state_service_subscribe(&update_battery);

}

static void deinit(void) {
  app_sync_deinit(&sync);
  
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
	
//  layer_remove_from_parent(bitmap_layer_get_layer(kday_name_layer));
//  bitmap_layer_destroy(kday_name_layer);
//  gbitmap_destroy(kday_name_image);
	
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
	
// layer_remove_from_parent(bitmap_layer_get_layer(kmonth_layer));
//  bitmap_layer_destroy(kmonth_layer);
//  gbitmap_destroy(kmonth_image);
	
 layer_remove_from_parent(bitmap_layer_get_layer(smonth_layer));
  bitmap_layer_destroy(smonth_layer);
  gbitmap_destroy(smonth_image);
 /* 
  layer_remove_from_parent(effect_layer_get_layer(effect_layer_col_hr));
  effect_layer_destroy(effect_layer_col_hr);
  effect_layer_col_hr = NULL;
	
  layer_remove_from_parent(effect_layer_get_layer(effect_layer_col_min));
  effect_layer_destroy(effect_layer_col_min);
  effect_layer_col_min = NULL;
*/	
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
/*
  layer_remove_from_parent(effect_layer_get_layer(effect_layer_col_hr));
  effect_layer_destroy(effect_layer_col_hr);
  effect_layer_col_hr = NULL;
	
  layer_remove_from_parent(effect_layer_get_layer(effect_layer_col_min));
  effect_layer_destroy(effect_layer_col_min);
  effect_layer_col_min = NULL;
	
  if (effect_layer_inv != NULL) {
	  effect_layer_destroy(effect_layer_inv);
  }
	*/
  layer_remove_from_parent(window_layer);
  layer_destroy(window_layer);
	
	
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}