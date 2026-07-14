# Number Guessing Game — Month 1, Project 1

CloudExify C++ Summer Internship 2026

Name:Muhammad Alyan Abbasi


## About

This is my Project 1 submission — a number guessing game in C++. The base idea
is simple: the program picks a random number and you try to guess it, but I
ended up adding a bunch of extra stuff on top of the core requirements since
I wanted to push it a bit further than just the basic version.

## Features

Basic gameplay:
- Pick a difficulty (Easy 1-50, Medium 1-100, Hard 1-200) or set a custom range
- Program picks a random number and you guess it, get told if you're too high or too low
- Invalid guesses (out of range) don't count against your attempts
- Best score gets saved to a file so it persists between runs
- Option to play again without restarting the program

Extra stuff I added:
- A max attempts limit so the game doesn't drag on forever (scales with difficulty)
- "Warmer/colder" feedback based on how close your last guess was
- Top 3 scoreboard instead of just tracking one best score, with your name attached
- Win/loss tracking that carries over across sessions
- A hint option (type `h`) — gives you even/odd or narrows the range, limited to 2 per game
- A timer per round, with the fastest time saved separately
- A basic point-scoring system (harder difficulty + fewer attempts + faster = more points)
- Everything gets logged to a history file with a timestamp
- A couple of small "achievements" if you guess fast or in very few tries
- Color in the terminal output so wins/losses/hints are easier to tell apart
