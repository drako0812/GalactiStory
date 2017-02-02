GalactiQuest
============

A Sci-Fi "Rogue-ish" Game set in space rather than on the ground.

Currently, it still looks more like a regular roguelike because to even begin
to look like a space game many more features will need to be implemented.

As of now, you're a Green `@` that can move around a spawn moving blood
splatters... yup... that's right randomly moving blood splatters because why
the heck not.

**IMPORTANT NOTES**
-------------------

**First of all, the current builds might not work on anything other than
Windows 8.1 and Windows 10 systems.**

**Secondly, to get the best results you should use a Unicode-compatible font
with your Command Prompt. I've personally tested with DejaVu Sans Mono at 14
point. It has pretty good Unicode coverage, strangley quite a lot better than
Noto Mono, although that is likely to change as Noto Mono is continuously
updated. I would like to use a smaller size, but with anything less than 14
point, the box drawing characters get messed up.**

**Thirdly, please don't resize the console window while running the game.
While, it will return to the size it was before the game started at exit, the
game currently doesn't update anything internally when the window is resized.
In the future it will either utilize the updated space or just forcefully
resize back to 120x36.**

Controls
--------

- Move Left - `left arrow` or `numpad 4`
- Move Right - `right arrow` or `numpad 6`
- Move Up - `up arrow` or `numpad 8`
- Move Down - `down arrow` or `numpad 2`
- Move Diagonally Left and Up - `numpad 7`
- Move Diagonally Right and Up - `numpad 9`
- Move Diagonally Left and Down - `numpad 1`
- Move Diagonally Right and Down - `numpad 3`
- Wait - `.` or `numpad .` or `numpad 5`
- Turn into red Smiley `☻` for a short time - `q`
- Forceably spawn a blood splatter - `w`
- Exit Game - `escape`

Future Plans
------------

One thing I am still trying to decide on is whether to use normal
single-character sprites or 3x3 character sprites which would allow the
graphics to be a little fancier. The biggest problem would be is that
would approximately multiply the size of the console needed for the same
amount of detail by 9, from 120x36 to 360x108, which I doesn't fit on my
screen (DejaVu Sans Mono @ 14pt w/ 1600x900 screen). Although, it's also
possible to just not show as much on the screen at one time.

Developed By
------------

For the time I am the sole developer/designer, but if you would like to submit
issues or pull requests please feel free.

- ### Drako0812
  - Name: Drew Wibbenmeyer (AKA "Drako0812" or just "Drako"... pronounced DRAE-koe)
  - Email: drako0812@gmail.com (Subject: GalactiQuest)
  - GitHub Page: http://drako0812.github.io
  - YouTube Channel: http://youtube.com/drako0812

License
-------

This is an MIT-License based project, look in LICENSE.txt for specifics.
