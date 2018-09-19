## hacBrewPack - Template

### Makefile:

Generic 'Makefile' that you can use to compile homebrews  

### npdm.json:
Generic NPDM json file which libnx reads during compiling of your homebrew and makes npdm from it  
it needs to be heavily modified based on your app  
Please check switchbrew for more information: [http://switchbrew.org/index.php/NPDM](http://switchbrew.org/index.php/NPDM)

### Folder structure:
Default folder structure:
```
hacbrewpack
|   hackbrewpack(.exe)
|
|___exefs
|   |   main
|   |   main.npdm
|
|___romfs
|
|___logo
|   |   NintendoLogo.png
|   |   StartupMovie.gif
|
|___control
|   |   control.nacp
|   |   icon_AmericanEnglish.dat
```

You can skip creating program romfs with --noromfs and creating program logo with --nologo  
You can use options to change default folder paths and names  
  
Default output folders are:  
hacbrewpack_out: contains nsp  
hacbrewpack_nca: contains ncas  
hacbrewpack_temp: temporary files