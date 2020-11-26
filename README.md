# LSDJ2DMW
LSDJ2DMW is a tool that rips wavetables from [LSDj (Little Sound Dj)](https://www.littlesounddj.com/) and converts them to the .dmw format for use in [Deflemask Tracker](https://deflemask.com/).
This includes from songs currently loaded  or songs that are saved using LSDj's internal file compression system.

Not guaranteed to work for versions of LSDj older than 6.7.0

The latest release can be found [here](https://github.com/Pegmode/LSDJ2DMW/releases)

## Usage
`SDJ2SDMW <inputSavFilename> [options]`
 
eg: \
`LSDJ2DMW.exe test1.sav` will rip all wavetables in the currently loaded song\
`LSDJ2DMW.exe test1.sav -swr TESTSONG 10 15` will rip wavetables 10-15 from the saved song TESTSONG\
`LSDJ2DMW.exe test1.sav -p -s TESTSONG` will rip all the wavetables in the saved song TESTSONG and will print out all the ripped wavetables and will print out the filestructure of all the songs in your .sav\

### Quickstart (windows)
1. ensure a folder named *outputwaves* is in the same directory as LSDJ2DMW.exe
2. drag and drop your lsdj .sav file on top of LSDJ2DMW.exe

All the wavetables in the currently loaded song will be dumped into the *./outputwaves* directory. The program does not create this or check for it so make sure you have a folder with this name in the same directory as LSDJ2DMW.exe

### Further Usage
#### Arguments:
##### Rip arguements
Only 1 may be used
* `-l`: rip all wavetables in loaded module (default)
* `-lw <wavenumber>`: rips a single wavetable from the loaded module
* `-lwr <startWaveNumber> <endWaveNumber>`: rip a range of wavetables from the loaded module
* `-a`: rips all wavetables from all saved songs
* `-s <songname>`: rip all wavetables from a single song
* `-sw <songname> <wavenumber>`: rips a specific wavetable from a given song (0-255)
* `-swr <songname> <startWaveNumber> <endWaveNumber>`: rips wavetables in a given range in a specific song (0-255)
##### display and formatting arugements
May use these in any combination
* `-p`: print ripped waves and files
* `-pw`: print ripped waves
* `-pf`: print songs
* `-x`: format outputted wavetable indices as hex
* `-nw`: don't write files or parse files. Useful in combination with -p/-pf for reading songnames
#### basic workflow:
If you want to actively create an LSDJ synth/wavetable instrument, run LSDJ on an empty/new .sav and toy around with a synth/wavetable instrument that uses waves 0-F. Once you have the sound you want close your emulator ([BGB](https://bgb.bircd.org/) recomended) to update the .sav. Then run `LSDJ2DMW yoursav.sav -lwr 0 15` to rip the synth wavetables. Keep in mind that the speed and mode settings will not affect the wavetable output. You need to do this yourself by using wavetable macros in Deflemask. 
## Build instructions
### windows
To build run on windows just run `make`. Requires gcc and make from MinGW. I have gcc and make added to PATH so if you don't already have it added you will need to do so yourself.
### linux
To build for linux (untested) run `make linux`. If you encounter any issues please let me know.

### TODO
* fix -lwr explanation in help string

## Other Credits
Special thanks to Rytone, TakuikaNinja and INFU for help with LSDj's internal file structure, compression and test data.
