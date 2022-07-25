### (Algorithm: Naive Alias Method)[https://www.keithschwarz.com/darts-dice-coins/]

Initialization:
 - Multiply each probability pi by n.
 - Create arrays Alias and Prob, each of size n.
 - For j=1 to n−1:
    - Find a probability pl satisfying pl≤1.
    - Find a probability pg (with l≠g) satisfying pg≥1 Set Prob[l]=pl.
    - Set Alias[l]=g.
    - Remove pl from the list of initial probabilities.(not actualy need just exclude)
    - Set pg:=pg−(1−pl).
 - Let i be the last probability remaining, which must have weight +-1.
 - Set Prob[i]=1.

Generation:
 - Generate a fair die roll from an n-sided die; call the side i.
 - Flip a biased coin that comes up heads with probability Prob[i].
 - If the coin comes up "heads," return i.
 - Otherwise, return Alias[i].
