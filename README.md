# KeyTranslator
## What is this?
This is a simple C++ tool that allows me to translate a keyboard layout to another.

* This is a simple and fairly useless tool for most people, but I made it just for using it on my school's computers.
* The school PCs don't give me admin permission so I used a workaround.
* If you want to modify the tool to include other layouts, go right ahead, but there's probably better ways to do this.

## How it works
It's actually quite simple. All it does is whenever I press a key, it will delete that key and replace it with the colemak key.

## Why even bother?
* After using colemak for almost a year, it's a pain to use the QWERTZ layout.
* I chose to then write this as a small experiment, mainly out of curiosity if it'd even work well.
* This is NOT a good tool to use daily but for a few quick school projects in class, it works.

## How can I use this?
1. Download the newest release or compile it from source.
2. Run the exe file.
3. Choose "Change Layout" and select colemak.
4. Choose "Start Translation".
To stop it, press the F12 key.

## How do I add more layouts?
* I plan to add an inbuilt layout editor but for now it works like this.
1. Fork the project.
2. Open it in Visual Studio.
3. Duplicate and rename "applyColemakLayout", then edit it to your liking.
4. Add a new case in "switch (layoutChoice)" and call your new function.
5. Compile your modified source.
