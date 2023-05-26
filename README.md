# J-Linkage

## Overview

Implementation of the J-Linkage algorithm for multiple line fitting problems, based on R. Toldo & A. Fusiello work.

## Install & use

This program is coded using C++, its compilation requires :
- the cmake utilities (>= v3.4)
- the Imagine++ library (download at [http://imagine.enpc.fr/~monasse/Imagine++/](http://imagine.enpc.fr/~monasse/Imagine++/))

Build Makefile with 
```
cmake CMakeLists.txt
```

and then the executable with 
```
make
``` 

The generated executable file is `tlk.exe`.

### program

This program uses randomly generated datasets for line fitting problems. Depending on the size of the dataset, computation may take some time.

The main files of this programm are `TLinkage.cpp` and `settings.h`. 

`TLinkage.cpp` contains the main function with JLinkage algorithm.

`settings.h` defines the global parameters to use for testing (threshold, number of inliers/outliers etc...)

Other files define classes and functions used in `TLinkage.cpp`.





For any information, please contact me at lysandre.macke@enpc.fr
