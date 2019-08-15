# CP/M port

This directory contains a version of the Z80 eForth that builds with asz80 and aslink from the [asxxxx package](http://shop-pdp.net/ashtml/asxxxx.php) and runs on CP/M or an emulation.

## Platform specific changes

Only two Forth words were edited and one new one created (or rather reinstated). QRX to return a character from the console and TXSTO to output a character to the console, and BYE to exit to CP/M. The first two use the BDOS call for Direct Console I/O (C=6), and BYE uses the BDOS return to CP/M call (C=0).

The implementation of QRX is not refined. There is no line buffer so mistakes can't be corrected. To enhance this a line buffer should be established so that when a character is asked for the buffer is checked first and if non-empty the next character is returned. Otherwise if the buffer is empty then input is checked. If it's not CR or NL then it is put in the buffer and "no character" returned to Forth. Otherwise CR is appended to the buffer and the character from the head of the buffer is returned. The net effect is that the Forth interpreter doesn't see any characters until the line is completed.

Echo should now be done in the collection routine instead by Forth. It should also allow backspace to delete the last character in the buffer for corrections. Remember that CR or NL should echo as CR NL.

These modifications are left as an exercise to the reader.

The 8080 processor is supported by changing one equate at the top of system.def. It so happens that this eForth implementation hardly uses any Z80 features. The Forth words for port read and write are not available for the 8080 as new code would have to be written for the 8080 to do this.

## More details

[Hackaday project](https://hackaday.io/project/166954-eforthz80-modifications)

## Versioning

First release August 2019

## Authors

For the parts I created:

* **Ken Yap**

## License

See original source for license, which I do not change.
