# LASZIPPER

**LasZipper** is made to compress or uncompressed LAS files.
[LAS](https://github.com/ASPRSorg/LAS) is an open source point cloud format adapted to remote sensing application.

LasZipper is based on [PDAL](https://pdal.io) library to process point cloud data.

## Use

Usage :  
        `-c [las files]`: compress [las files]  
        `-u [las files]`: uncompress [las files]

The files will be compressed or decompressed.

The compressed point cloud can be used to transmit data quicker or to reduce storage space taken.
The uncompressed point cloud should be used when you open and access data often.


## Install and build

You can download the source code on github by cloning the repository.

To build LasZipper you need the external libraries [PDAL](https://pdal.io) and [Boost](https://www.boost.org).

Edit the file Makefile.
Define path to your PDAL and boost::filesystem library in the `LIBS` variable at line 4 : 


        LIBS	=	$(shell pdal-config --libs) \
	        		/usr/local/Cellar/boost/1.72.0_3/lib/libboost_filesystem.dylib

Then build the project using `make`.


