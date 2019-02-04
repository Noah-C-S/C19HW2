Usage: ./HW2 Rows(int > 0) Cols(int > 0) Gens(int > 0) SourceFile(string), [Print(y/n)], [Pause(y/n)]
Rows is the amount of rows in the simulation
Cols is the amount of columns in the simulation
Gens is the number of generations to generate, assuming we don't reach a steady state or oscillation first. 
Sourcefile is a text file in the same directory as the executable file containing "x" for filled spaces, "o" for unfilled spaces, and Linux newlines ("\n") in between each line
Print (optional): y = print every generation, n = don't, anything else is undefined (may print or may not, who knows?)
Pause (optional): y = pause in between each generation, requiring the enter key to be pressed to continue.
With the exception of the starter code, everything here is mine.
The only problem that I encountered is that when running this program through eclipse, it couldn't find ANY of the files, no matter where they were located or which file I used. I finally figured out that 
it was just eclipse being dumb, and that it works perfectly fine in the command line. Wasted some of my time, though. 