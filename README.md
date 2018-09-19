# hacBrewPack

![License](https://img.shields.io/badge/license-ISC-blue.svg)

hacBrewPack is a tool for creating Nintendo Switch NCAs (Nintendo Content Archive) from homebrews and pack them into NSPs (Nintendo Submission Package)  

Thanks: SciresM, yellows8, SwitchBrew

## Usage
You should place your keyset file with "keys.dat" filename in the same folder as hacBewPack  
Alternatively, You can use -k or --keyset option to load your keyset file  
Required keys are:  

Key Name | Description
-------- | -----------
header_key | NCA Header Key
key_area_key_application_xx | Application key area encryption keys

You need to compile homebrew with proper makefile, you can use the one in template folder  
Compiled homebrew must have the following files:  
```
build\exefs\main  
build\exefs\main.npdm  
[TARGET].nacp  
```
You must place created 'main' and 'main.npdm' files in exefs folder, you can find them in build/exefs  
You must place created nacp file with 'control.nacp' filename in control folder  
You should place your icon with "icon_{Language}.dat" file name in control folder, "icon_AmericanEnglish.dat" is the default one if you don't manually edit your nacp  
Check [switchbrew](http://switchbrew.org/index.php/Settings_services#LanguageCode) for more info about language names  
Your icon file format must be JPEG with 256x256 dimensions  
If you have problems with icon, try to make it with Adobe Photoshop or inject Adobe Photoshop exif data to your icon file  
If you don't put your icon in control folder, you'll see a general icon after installing nsp (i don't recommend this)  
"logo" folder should contain "NintendoLogo.png" and "StartupMovie.gif", they'll appear when the app is loading  
Both files are not licensed according to [switchbrew](http://switchbrew.org/index.php/NCA_Content_FS) but i didn't include them anyway, You can also replace these files with custom ones  
You can use --nologo if you don't have any custom logo and you don't have the original ones, as the result switch will show a black screen without nintendo logo at top left and switch animation on bottom right  
CLI options:  
```
*nix: ./hacbrewpack [options...]  
Windows: .\hacbrewpack [options...]  
Usage: hackbrewpack [options...]  
  
Options:  
-k, --keyset             Set keyset filepath, default filepath is ./keys.dat  
-h, --help               Display usage  
--tempdir                Set temp directory filepath, default filepath is ./hacbrewpack_temp/  
--ncadir                 Set output nca directory path, default path is ./hacbrewpack_nca/  
--nspdir                 Set output nsp directory path, default path is ./hacbrewpack_nsp/  
--exefsdir               Set program exefs directory path, default path is ./exefs/  
--romfsdir               Set program romfs directory path, default path is ./romfs/  
--logodir                Set program logo directory path, default path is ./logo/  
--controldir             Set control romfs directory path, default path is ./control/  
--noromfs                Skips creating program romfs section  
--nologo                 Skips creating program logo section  
```
Also check template folder for default folder structure, makefile and npdm json  
  
**Delete nca(hacbrewpack_nca) and temp(hacbrewpack_temp) folders before start or you may have bad nsp**

## Licensing

This software is licensed under the terms of the ISC License.  
You can find a copy of the license in the LICENSES file.  
Portions of project HacBrewPack are parts of other projects, make sure to check LICENSES folderw