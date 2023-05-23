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

