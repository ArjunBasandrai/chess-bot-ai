<p align="center"><a href="https://github.com/ArjunBasandrai" target="_blank"><img src="assets/logo.png" width=160 title="ShuffleAI" alt="ShuffleAI"></a>
</p>

<h1 align="center"> Shuffle Chess Engine </h1>
<br/>

> Shuffle is a UCI based chess engine written entirely in C.
> It is a magic bitboards based engine and has an ELO of about 1900


---

## Features
- Magic Bitboards to pre-calculate sliding piece attacks quickly and efficiently
- Zobrist Hashing and use of Transposition Table to store evaluated positions and detect 3-fold repetition
- Negamax search with Alpha-Beta Pruning to quickly search for all possible positions
- Advanced move ordering techniques like Principle Variation Search, Null Move Pruning, Late Move Reduction to reduce search times
- Tapered Evaluation for static evaluation of given board state
- Ability to play at various time controls
- UCI compatibility

## Instructions

### Directions to install

- `git clone https://github.com/ArjunBasandrai/shuffle-chess-engine.git`

### Directions to run

- Open the `shuffle.exe` file in any UCI based GUI like [Arena GU](http://www.playwitharena.de/) or  [Cute Chess GUI](https://cutechess.com/)
- Set any Game Mode/Time Control of choice
- Have fun!

## Contributors

<p align="center">Arjun Basandrai</p>
<p align="center">
  <img src = "https://avatars.githubusercontent.com/u/64721050?v=4" width="150" alt="Arjun Basandrai">
</p>
  <p align="center">
    <a href = "https://github.com/Arjun Basandrai">
      <img src = "http://www.iconninja.com/files/241/825/211/round-collaboration-social-github-code-circle-network-icon.svg" width="36" height = "36" alt="GitHub"/>
    </a>
  </p>

## References

[Bitboard Chess Engine in C Series](https://www.youtube.com/playlist?list=PLmN0neTso3Jxh8ZIylk74JpwfiWNI76Cs) by Chess Programming <br>
[VICE Chess Engine in C](https://www.youtube.com/watch?v=bGAfaepBco4&list=PLZ1QII7yudbc-Ky058TEaOstZHVbT-2hg) by Bluefever Software <br>
[Coding Adventures: Chess Bot](https://www.youtube.com/watch?v=U4ogK0MIzqk&t=1007s&pp=ygUWY2hlc3MgZW5naW5lIHNlYmFzdGlhbg%3D%3D) by Sebastian Lague

## Note
#### Shuffle is currently not compatible with Unix-based systems
