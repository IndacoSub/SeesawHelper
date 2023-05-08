# SeesawHelper

A tool designed to make it *slightly* easier to mod a minigame ("Hangman's Gambit Ver. 3.0") in "Danganronpa V3: Killing Harmony".

This tool is licensed under the ISC License (https://github.com/IndacoSub/SeesawHelper/blob/main/LICENSE).

Contributions and bug/issue-reporting are welcome and appreciated.

# Usage

After extracting the .dat files from "/minigame/anagram/anagram_US", and converting them into the .csv format, you can use them as arguments (ex. "SeesawHelper myfile.csv").

This program only accepts one argument at a time.

After the program is done reading the file, you will be asked to enter a new string, which is the text you wish to display in the minigame.

After entering that string, and pressing the ENTER key, a new .txt file (somewhat based on the .csv you entered as an argument) will be created, possibly in the same directory as the .csv file.

Obviously, always make backups of your files, just in case!

You'll need to copy the content of that file to the .csv you desire to modify. Manually, if needed.

If you use LibreOffice Calc, make sure to select "commas" as a separator, so that the text is automatically pasted into the columns in the right way. Other programs may have a similar option as well.

Make sure to keep the first row (the one with japanese text) intact.

Other than that, feel free to delete the rest of the .csv file, after you made a backup, since you'll need to paste the text... somewhere, right?

Remember to put back squares (U+25A1) in the second column, if the letter/symbol is actually used (yes, most probably)!

You *may* also need to put any text (except commas) within quotation marks (ex. "A","□","0","" instead of A,□,360,), before copying it into the .csv.

Finally, remember that the font(s) related to the minigame **MUST** support the characters/symbols you are using! Some *letters* (such as Z) aren't even supported!

Also, please check if the original file contains some weird parameter in the fourth column, and if it's still present in your new .csv file.

This is because you can obtain two Hidden Monokumas inside the minigame, and players might miss them if you're not "careful", so you please remember to include "monokuma" somewhere in the fourth column of answer000.dat(.csv) and answer005.dat(.csv), if it's not already present (though it should be)!

Once you're done with that, copy/move it, convert it back to the .dat format, repack anagram_US, see if it works, yada yada yada... Have fun!

## Licensing ##

This is licensed under the ISC License, with exemptions for specific companies or groups noted below.

Exemptions:

    Spike Chunsoft Co., Ltd. is exempt from agreeing to the ISC License and may eventually use any license they desire to use.
    NIS America Inc. is exempt from agreeing to the ISC License and may eventually use any license they desire to use.
    Too Kyo Games, LLC is exempt from agreeing to the ISC License and may eventually use any license they desire to use.

# Credits

    “DANGANRONPA” is a registered trademark of Spike Chunsoft Co., Ltd., Too Kyo Games, LLC and NIS America Inc.
    We are not in any way affiliated or associated with them.

Thanks to the members of the "SPIRAL" Discord server and the Spiral Framework project (https://spiralframework.info/ -- source code here: https://github.com/SpiralFramework/Spiral).
	 
Thanks to all of our collaborators and reviewers.