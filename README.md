# SpectralUpsamplingImpls

Small program to convert RGB to spectum

## How to build

Simply run 
~~~
build.sh
~~~
 to build program (Ninja required).

## Usage
Run
~~~
./build/spectral_upsampling_impls
~~~
Parameters: 
* -f <path> : input file;
* -c <hex> : input color in RGB;
* -m <method> : upsampling method;
* --downsample : downsample instead of upsampling (requires -f).
