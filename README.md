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
* -c \<hex\> : input color in RGB;
* -m \<method\> : upsampling method;
* --downsample : downsample instead of upsampling (requires -f).

If not downsampling, output path must be specified as last parameter.
