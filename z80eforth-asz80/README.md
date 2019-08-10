# Z80 eForth built with asz80 and aslink

This directory contains a version of the Z80 eForth that builds with asz80 and aslink from the [asxxxx package](http://shop-pdp.net/ashtml/asxxxx.php).

## Hardware

You will almost certainly not be able to use the software as is. That would require you to have exactly the same peripherals as the original author. At the moment I haven't worked out from the register writes what chips were used for the CTC, PIO and SIO. My guess would be Zilog family peripherals, but who knows, maybe an Intel 8255 for the PIO.

You will need to write chip specific initialisation routines and I/O words, as well as interrupt handlers for the SIO. That is, if you choose to use interrupt handling for SIO. Probably a good idea, given how slow an interpreted (albeit threaded interpreted) language will be on a slow micro. But maybe you are running this on a modern fast Z80 descendant. Also you may want to modify the memory layout.

If you get something to work on your platform, I encourage you to put them in a subdirectory named after your platform, and submit a git pull request. Please also write something about your hardware configuration to help others reuse your code. I will leave the original sources as is, except for fixes for bugs which are discovered.

Platform also means any simulators if you get this eForth to work under one.

## More details

[Hackaday project](https://hackaday.io/project/166954-eforthz80-modifications)

## Versioning

First release August 2019

## Authors

For the parts I created:

* **Ken Yap**

## License

See original source for license, which I do not change.
