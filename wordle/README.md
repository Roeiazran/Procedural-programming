Worlde game.
---

A Wordle game is a guessing game where normally a player gets 6 attempts on guessing
a word, and on any attempt that fails the user gets a clue about a specific letter and it's location.

This programm provides the following features:

1. Play a standard wordle game.

2. Update the game settings.
    * Initially the player will play with a words of length 5 and allowed for 6 tries but 
    it can be changed.

4. View the scoreboard.

   for each player the game will keep track of the following:
    - number of games played.
    - win rate
    - average tries.

The clues provides by the game are as follows:

    * "+" states that the letter exists in the word but in a diffrent spot
    * "-" states that the letter doesn't exists in the word.
    * on a currect guess of a letter the letter itself will show in the guidanse line.

for example given that the word is "hello" and the user's guess was oweth the guidanse line
would look like +-+-+ since the latters o e h appears only in a different spot.

-

To run the programm don't forget to include the words.txt file which containg (for now) words of 4 and 5 letters long, and then run ```python3 wordle.py```

A running example:

```
Choose an option:
0. Exit
1. Update settings
2. Play
3. View settings
4. Scoreboard
2
Enter player's name:
yaniv
Enter a word:
blued
Welcome to Wordle! You have 6 tries to guess the word.
The word is 5 letters long.
Guess a word:
bulgy
b++--
Guess a word:
holed
--+ed
Guess a word:
blued
blued
You win!
Game over!
b++--
--+ed
blued
Choose an option:
0. Exit
1. Update settings
2. Play
3. View settings
4. Scoreboard
2
Enter player's name:
moshe
Enter a word:
sited
Welcome to Wordle! You have 6 tries to guess the word.
The word is 5 letters long.
Guess a word:
writs
--+++
Guess a word:
metre
-+t-+
Guess a word:
shies
s-+e-
Guess a word:
holed
---ed
Guess a word:
sited
sited
You win!
Game over!
--+++
-+t-+
s-+e-
---ed
sited
Choose an option:
0. Exit
1. Update settings
2. Play
3. View settings
4. Scoreboard
2
Enter player's name:
yaniv
Enter a word:
gigas
Welcome to Wordle! You have 6 tries to guess the word.
The word is 5 letters long.
Guess a word:
sines
-i--s
Guess a word:
sigma
+ig-+
Guess a word:
gigas
gigas
You win!
Game over!
-i--s
+ig-+
gigas
Choose an option:
0. Exit
1. Update settings
2. Play
3. View settings
4. Scoreboard
4
Scoreboard:
moshe: 1 games, 100.00% win rate, 5.00 average tries
yaniv: 2 games, 100.00% win rate, 3.00 average tries
Choose an option:
0. Exit
1. Update settings
2. Play
3. View settings
4. Scoreboard
1
Enter settings:
{ word_length : 4, tries : 10}
Settings were updated
Choose an option:
0. Exit
1. Update settings
2. Play
3. View settings
4. Scoreboard
3
file_path: words.txt
tries: 10
word_length: 4
Choose an option:
0. Exit
1. Update settings
2. Play
3. View settings
4. Scoreboard
0

```
