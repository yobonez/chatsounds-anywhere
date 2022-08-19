# chatsounds-anywhere

## Contents
- [How does it work?](#how-does-it-work)
  - [Basic functionality](#basic-functionality) 
  - [Modifiers](#modifiers)
    - ["#" - Selection modifier](#selection-modifier)
    - [Sound effects](#sound-effects)
- [Configuration](#configuration)
- [What is a root direcotory?](#what-is-a-root-directory)
- [Setup for development](#setup-for-development)
  - [Example code](#example-code-after-setup)

##### [Example project for chat inside Counter-Strike Global Offensive](https://github.com/yobonez/csgo-chatsounds-example)

## How does it work?

#### Basic functionality
The program generates list of sound file names and its paths from selected root dir path (the one where you keep your sounds), and whatever you type will be scanned for phrases from that list.

If any phrase matches it's counterpart from the list, it's going to be played.
It is designed so that any other garbage in the message isn't going to disturb final output in any way.

![image](https://dl.dropboxusercontent.com/s/8ugkc39sg3t7bsw/chatsounds.png)

#### Modifiers

In the future there will be more of them.

##### "#" - Selection modifier
Example: ```multiple variant chatsound#2``` in [root dir example](#example) is ```multiple variant chatsound/2.ogg```

If a chatsound has multiple variants or there is another chatsound in another category with the same name, you can choose exact one using this modifier.
##### Sound effects 

```":echo", ":reverb", ":robotize", ":fft", ":flanger"```

Example: ```garbage:echo```

They have fixed arguments for now. In the future, ability to pass parameters into them will be added. For example: ```:echo(3.1,2.5)``` (decay, delay)

##### Modifiers can be stacked

Example: ```multiple variant chatsound#2:echo:flanger```
## Configuration

At first run, a configuration file will be generated where you can edit the root path to your sound files and change name of the json list file.

Contents of the config:
```
# This path will be used when generating list of audio files and its paths. Use "/" slashes.
root_dir_path = X:/my-example/path

# This is name of the list
chatsound_paths_file_name = chatsound-paths.json
```
In case of examples that will be shown below, the ```root_dir_path``` will be ```X:/root-dir```

## What is a root directory?
Inside root directory there are other folders (categories). You can name them whatever you want.

There needs to be at least one category folder that will contain .ogg files.
#### Example
```
  X:/
    |
    root-dir/                   <- root directory
        |
        category1/              <- one of the categories
            |
            somesound.ogg
            test.ogg
            sound.ogg
            multiple variant chatsound/
                |
                1.ogg
                2.ogg
                3.ogg
                4.ogg
            another chatsound.ogg
            and another.ogg
            etc.ogg
        some other category/    <- another category
            |
            you name it.ogg
```

From this tree, program will generate this list:

```
"somesound" : "X:/root-dir/category1/somesound.ogg",
"test" : "X:/root-dir/category1/test.ogg",
"sound" : "X:/root-dir/category1/sound.ogg",
"multiple variant chatsound" : 
[
    "X:/root-dir/category1/multiple variant chatsound/1.ogg",
    "X:/root-dir/category1/multiple variant chatsound/2.ogg",
    "X:/root-dir/category1/multiple variant chatsound/3.ogg",
    "X:/root-dir/category1/multiple variant chatsound/4.ogg",
],
"another chatsound" : "X:/root-dir/category1/another chatsound.ogg",
"and another" : "X:/root-dir/category1/and another.ogg",
"etc" : "X:/root-dir/category1/etc.ogg"
"you name it" : "X:/root-dir/some other category/you name it.ogg"
```

The ```multiple variant chatsound``` is an ordinary chatsound too. It just has multiple variants. If you type it out, random one will be played.
To play exact one you need to use ["#" - Selection modifier](#selection-modifier)

In case of chatsound having the same name across categories, it will be handled the same way.


#### Example directory structure based on the first [image](#how-does-it-work)

```
 .../
    |
    autoadd/                          <- root directory
        |
        ...
        bill_wurtz_history_of_japan/  <- the one of many categories
            |
            ...
            open the country.ogg      <- first chatsound
            ...
        ...
        its_always_sunny/             <- another category
            |
            ...
            garbage.ogg               <- second chatsound
            ...
```

## Setup for development

### Visual Studio 2019

#### 1.
![image](https://dl.dropboxusercontent.com/s/1852q5f56ylwsz5/VS2019%20setup.png)

#### 2. [Download SoX](https://sourceforge.net/projects/sox/files/sox/14.4.2/) - Sound eXchange
#### 3. Put these files in the same directory as your executable
- chatsounds-anywhere.dll
- SDL2.dll
- sox-14.4.2/ <- if you execute directly from Visual Studio, put this folder also <u>next to the folder with your executable</u> i.e. before "Release" or "Debug" folder inside <u>solution folder</u>

### Example code after setup
This code gets user input and sends it into chatsound parser
```
#include <iostream>
#include <string>

#include "Chatsounds.h"

int main()
{
    Chatsounds s;
    while (true)
    {
        std::cout << "Input:\n> ";
        std::string input;
        std::getline(std::cin, input);

        s.Parse(input);
    }
}

```