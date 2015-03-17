# pebbleface-taller2
By mereed

## PackBits 
By Matt Clark & Ron64

This project is used to demonstrate usage of [PackBits](http://en.wikipedia.org/wiki/PackBits).

PackBits was implemented by Matthew Clark. Few bug-fix and modifications done by Ron64

### Background & information
The Pebble pbi format was designed to easily load B/W images to GBitmap. It uses 32 bit padding like GBitmap. PackBits uses 8Bit padding, so even if there is no significant reduction of size by compression, for most images we gain from leaving out 1-3 bytes per line.

I (Ron64) adopted PackBits for my new watchface that like 'Finally' includes several popular watchfaces. Some of them had many resources, so I needed to compress more. Matthew was kind enough to share his implementation of packbits (old lossless RLE compression [by apple](http://web.archive.org/web/20080705155158/http://developer.apple.com/technotes/tn/tn1023.html)). 

Taller is included in my next watchface, and I chose to use Taller2 to demonstrate PackBits so other developers can use it too. The size of resources of Taller2 was reduced from 55K to 21K, so now it is possible to add support to approximately 30 more languages instead of only 5-6.

I plan to improve compression a bit more by handling repeating lines.

### Using PackBits

#### Compression
The script packbits.php, is currently in the 'uncompressed' folder of this projects. For users who like full instructions on how to use it:

* Install PHP on your machine (including php-gd2)
* Make sure you have resources/data folder
* Create a folder in project root for the images needed to be converted. (They can be removed from resources/images)
* Run the script from the above folder. The *.png images will be converted and placed in resources/data with pbt extension.

#### Updating resources
Add the images as raw.

(In this project I saved manual work by taking original appinfo.json, separating the converted images and doing search+replace of "png" => "raw" , .png" => .pbt" , images/ => data/ )

#### Load PackBits images
To load PackBits compressed images in your watchface preform the flowing steps:

* Add packbits.c & packbits.h in your project.
* Add to your relevant source: #include "packbits.h"
* Load images using gbitmap_create_from_pbt() instead of [gbitmap_create_with_resource()](http://developer.getpebble.com/docs/c/Graphics/Graphics_Types/#gbitmap_create_with_resource)


### Future plans
Currently I plan the flowing upgrades:
* Adding repeating line detection. (will compress better for some fonts and screen background images)
* Extracting images using the original bitmap. (will save allocating extra buffer for decompression). Alternatively allocate the GBitmap before the decompression buffer to reduce memory fragmentation.
* Add other drawing method like [graphics_draw_bitmap_in_rect()](http://developer.getpebble.com/docs/c/Graphics/Drawing_Primitives/#graphics_draw_bitmap_in_rect) as Matt used originally. (Is anyone interested?)


I would live to hear if you use it and any comments about it.
