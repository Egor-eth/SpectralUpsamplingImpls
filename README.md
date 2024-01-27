# SpectralUpsamplingImpls

Small program to convert RGB to spectum

## How to build

Simply run 
~~~
./build.sh
~~~
 to build program (Ninja required).

## Usage
Run
~~~
./build/upsampler
~~~
Parameters: 
* -f \<path\> : input file;
* -c \<hex\> : input color in RGB (upsampling only);
* -m \<method\> : upsampling method;
* -D \<path\> : directory to output (upsampling only), cannot be used with -o ;
* -o \<path\> : path for output (upsampling only) cannot be used with -D;
* --downsample : downsample instead of upsampling (requires -f).
