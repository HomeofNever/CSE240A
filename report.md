# CSE240A Report

## Introduction

<!--Give a brief introduction about branch prediction, explain why it is used, its working, advantages, disadvantages, and current trends. Also give a high-level description of what you did in this project.-->

Branch prediction is to make a guess of a branch outcome with information provided without actually execution, and in modern CPU, there is a module responsible for this functionality, branch predictor. Branch predictor is used to accelerate pipeline instruction, avoiding stall when prediction is correct. Without predictor, CPU has to wait for the result of the branch to decide which instruction to execute, wasting cycles and bring delays, causing lower throughput. On the other hand, since predictor is an extra components added to the CPU. It brings overheads to all branch instructions by default, and if the prediction misses, it would incur penalty and potentially cause more time to recover --- flushing prefetched instructions, etc. To achieve more accurate predictions, spaces for history pattern is also required. Not only time but space cost are incurred if added. A balance between such overhead and benefits should be carefully consider before moving on with a predictor.

<!--https://dl.acm.org/doi/pdf/10.1145/285930.285980 add reference here-->

The trend of branch predictor is evolving with the scale of the software these days. From the most simple one like always predicting taken/not taken, to GShare, considering history outcome, and Tournament, considering the difference between local and global outcome. Predictors are tend to gathering more information, more complex decision process, to achieve better outcome. However, limits by the reaction time as well as cost restriction. Predictors are forced to improve their information density --- finding better ways encoding information and keep those only useful for decision making. <!--Add some other example of prediction? like traditional and AI stuff @TODO -->

Our paper consists work from both authors, Xinhao and Jiale. Xinhao is worked on implementations of all predictors mentioned in the paper, as well as the introduction of the paper. Jiale has worked on verifying the implementation, gathering information, providing insights/directions, and build the final observation and conclusion.

## Implementation

For each of the branch predictors below, write down its working, advantages, disadvantages, and how this technique has evolved in the past.

### G-Share

### Tournament

### Custom Predictor

## Observation

Tabulate your results based on the accuracy numbers you obtained, for every address trace, for each of the predictors. Perform a brief comparative study with the 3 implementations, draw conclusions on what you observed, and give reasons to explain them. You can plot graphs and charts to make your report visually appealing.

=== benchmark with args: --gshare:13
Executing benchark with ../traces/fp_1.bz2...
Branches: 1546797
Incorrect: 12765
Misprediction Rate: 0.825
../traces/fp_1.bz2 done!
Executing benchark with ../traces/fp_2.bz2...
Branches: 2422049
Incorrect: 40641
Misprediction Rate: 1.678
../traces/fp_2.bz2 done!
Executing benchark with ../traces/int_1.bz2...
Branches: 3771697
Incorrect: 521958
Misprediction Rate: 13.839
../traces/int_1.bz2 done!
Executing benchark with ../traces/int_2.bz2...
Branches: 3755315
Incorrect: 15776
Misprediction Rate: 0.420
../traces/int_2.bz2 done!
Executing benchark with ../traces/mm_1.bz2...
Branches: 3014850
Incorrect: 201871
Misprediction Rate: 6.696
../traces/mm_1.bz2 done!
Executing benchark with ../traces/mm_2.bz2...
Branches: 2563897
Incorrect: 259929
Misprediction Rate: 10.138
../traces/mm_2.bz2 done!
=== benchmark done!

=== benchmark with args: --tournament:9:10:10
Executing benchark with ../traces/fp_1.bz2...
Branches: 1546797
Incorrect: 15329
Misprediction Rate: 0.991
../traces/fp_1.bz2 done!
Executing benchark with ../traces/fp_2.bz2...
Branches: 2422049
Incorrect: 78619
Misprediction Rate: 3.246
../traces/fp_2.bz2 done!
Executing benchark with ../traces/int_1.bz2...
Branches: 3771697
Incorrect: 476073
Misprediction Rate: 12.622
../traces/int_1.bz2 done!
Executing benchark with ../traces/int_2.bz2...
Branches: 3755315
Incorrect: 15980
Misprediction Rate: 0.426
../traces/int_2.bz2 done!
Executing benchark with ../traces/mm_1.bz2...
Branches: 3014850
Incorrect: 77802
Misprediction Rate: 2.581
../traces/mm_1.bz2 done!
Executing benchark with ../traces/mm_2.bz2...
Branches: 2563897
Incorrect: 217501
Misprediction Rate: 8.483
../traces/mm_2.bz2 done!
=== benchmark done!

## Results and Conclusion

Write down the final results obtained, give a brief interpretation of the observation, and conclude by summarizing the entire work done in completing this project.

## References

Cite all the sources that helped you to complete this project.
