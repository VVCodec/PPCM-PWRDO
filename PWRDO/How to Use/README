	
## Running

Configuration files are provided in the cfg directory to aid configuring
the encoder.  The general pattern of usage is illustrated below, where
multiple configuration files control different aspects of the test
conditions.

NB: parameters set in one configuration file override the same parameter
in earlier files.  ie. order matters.

Further help text describing option usage is available using "./bin/PccAppEncoder --help" or "./bin/PccAppDecoder --help".

### PccAppEncoder

```
./bin/PccAppEncoder \
	--configurationFolder=cfg/ \
	--config=cfg/common/ctc-common.cfg \
	--config=cfg/condition/ctc-all-intra.cfg \
	--config=cfg/sequence/longdress_vox10.cfg \
	--config=cfg/rate/ctc-r3.cfg \
	--uncompressedDataFolder=~/mpeg_datasets/CfP/datasets/Dynamic_Objects/People/ \
	--frameCount=1 \
	--videoEncoderPath=../external/HM-16.16/bin/TAppEncoderStatic \
	--colorSpaceConversionPath=../external/HDRTools/bin/HDRConvert \
	--reconstructedDataPath=S26C03R03_rec_%04d.ply \
	--compressedStreamPath=S26C03R03.bin 
```

### PccAppDecoder

```
./bin/PccAppDecoder \
	--compressedStreamPath=S26C03R03.bin \
	--videoDecoderPath=../external/HM-16.16/bin/TAppDecoderStatic \
	--colorSpaceConversionPath=../external/HDRTools/bin/HDRConvert \ 
	--inverseColorSpaceConversionConfig=cfg/hdrconvert/yuv420torgb444.cfg \
	--reconstructedDataPath=S26C03R03_dec_%04d.ply 
```
