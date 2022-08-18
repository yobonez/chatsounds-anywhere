# chatsounds-anywhere

## How does it work?
Basically, the program generates list of sound file names and its paths from selected root dir path (the one where you keep your sounds), and whatever you type will be scanned for phrases from that list.

If any phrase matches it's counterpart from the list, it's going to be played.
It is designed so that any other garbage in the message isn't going to disturb final output in any way.

![image](https://dl.dropboxusercontent.com/s/8ugkc39sg3t7bsw/chatsounds.png)

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

There needs to be at least one category folder that will contain these files.
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

From this, program will generate this list:

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
To play exact one you need to use /***Modifiers**\*/ // todo description

In case of chatsound having the same name across categories, it will be handled the same way.


#### Example directory structure based on the first image

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

## How to use this lib?

// TODO