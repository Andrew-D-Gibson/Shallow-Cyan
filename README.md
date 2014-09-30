Shallow-Cyan
============

Shallow Cyan is a chess engine made in C++ and Qt for a senior project.  
It has been under development since November 2013. 

All the code I wrote is found in the files ending in .h and .cpp, and the images found in the GUI Images folder are public domain.

Shallow Cyan is a bitboard based engine, using modified kindergarten bitboard move generation.  The search algorithm is a negamax algorithm with alpha-beta cutoffs, and a quiescent search ending with a simple evaluation function.  It uses Zobrist hashing to determine draws by repetition, and plays fully legal chess.  

If you find any bugs or have a comment, please send it to adgibson97@gmail.com.

Thank you!
