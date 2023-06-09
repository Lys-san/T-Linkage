17-05-2023 commit 80f46bd

Why is the programm so slow ?? 
Working with a dataSet if size 350, each linkage tends to be slower than the previous one (which is kind of weird since the number of clusters is decreasing, so in theory we need to do less computations ?). Litterally took more than 30 minutes to get a result (VS max. 5 min for J-linkage, with big dataSet).

However, the pogram work fine with a small dataSet. I'll need to do some more testing on small dataSet and clean the code to spot where the problem is. 

---------------------------------

23-05-2023

I think that what takes time is the computation of the PF of a cluster. Since each cluster tends to be bigger after each iteration, the program get slower.

comparing J-Linkage and T-Linkage programs with star model and following settings :

```
#define N_INLIERS     15 // inliers for 1 model
#define N_OUTLIERS    5 // in general data set
```

(Starting from N clusters of size 1.)

J-Linkage takes ~ 3 s (~70 iterations)
T-Linkage takes ~ 25 s (~70 iterations)

Is it the algorithm that is much slower, or just me that coded like a 9 year old kid ? 
In any case, the Tanimoto distance computation is ~3x longer than the jaccard one (because we need to do 3 inner products of complexity N/2), and the computePF function requires a LOT of calculations since we search for min values for each point of the cluster.

EDIT commit 

I fixed the slow program behaviour. This was (it always is) quite stupid, in fact I computed unnecessary preference functions in the link function, ans since this was done quite a lot of time, this explains why the program was awfully slow. 
The program now works fine and in a correct time. Here are some times taken by the T-linkage program to ind an outlier free star model (30 inliers per line).

|Times   |
|--------|
|17201 ms|
|17005 ms|
|17631 ms|

Each model is always recognized.

Whereas the J-Linkage has the following times : 

|Times   |
|--------|
|32865 ms|
|34400 ms|
|31620 ms|

--------------------------------

24-05-2023

I started optimizing the code (faster vector operations, etc...), and went from 17s to ~15s for the outlier-free star model (30 inliers per line). 

Note : current computer has 4 processors. 

------------------------------

27-06-2023

I think i broke my code (or maybe he was already broken, I just did not notice)? Sometimes the algorithm keeps on linking clusters that should not be linked together, and as a result, i end up with 1 cluster containing the whole data set.
The T-Linkage algorithm should work well, but the problem shoul be in the model sampling function.

Edit : I think I solved the problem (as I am no longer able to reproduce it). The problem was with vertical line models : as they are represented with their form ax = b, a value tends to INFTY and this falsifies point to line distance computing (result always tends to 0, in consequence, all points match with vertical line models).

------------------------------

28-06-2023

I found what seems to be a very problematic issue. When it comes to models generation from a square like figure, drawn models tends to cluster at the corners. Which is very problematic as searched models are acually the borders of the figure. For the moment I don't know if it is a natural bius (more points at the edges ?) or if is linked to the way points are stored/drawn.

Edit : the problem was (again) my use of sets instead for storing unique elements. I fixed the problem by using a vector with a DIY "emplace if absent" algorithm.

-------------------------------

29-06-2023

I'm doing tests on mutiple images. Of course, the complexity of the image impacts a lot the quality of my program. For example, the forest.jpg image cannot produce good results in any case because of the way canny filter contours it. Therefore, any relevant model can be found. The problem is the same with enpc.jpg : if the image contains really noisy parts like tree leaves, detailed patterns etc, this produces too many clustered outliers that impacts the model computing.
This problem ma eventually be solved by increasing the canny threshold, which works for enpc.jpg (CANNY_THRESHOLD = 300) but not for forest.jpg.
With default parameters, the program seems to work well with simple images like shapes.jpg or even mug.jpg. The only remaining problem is the outlier rejecion : how do we choose to validate or cluster or not ?
