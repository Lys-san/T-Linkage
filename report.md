17-05-2023 commit 

Why is the programm so slow ?? 
Working with a dataSet if size 350, each linkage tends to be slower than the previous one (which is kind of weird since the number of clusters is decreasing, so in theory we need to do less computations ?). Litterally took more than 30 minutes to get a result (VS max. 5 min for J-linkage, with big dataSet).

However, the pogram work fine with a small dataSet. I'll need to do some more testing on small dataSet and clean the code to spot where the problem is. 
