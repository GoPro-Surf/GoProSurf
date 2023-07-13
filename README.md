# GoProSurfSplitter

## How to build
```shell
git clone --recursive  https://github.com/sergei-svistunov/GoProSurfSplitter.git
cd GoProSurfSplitter
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
```

## How to use
```shell
./GoProSurfSplitter --help
```
```
It finds caught waves in GoPro videos with enabled GPS and writes them to new shorter files
Usage:
  GoProSurfSplitter [OPTION...]

  -d, --directory arg  Directory with *.MP4 files (default: ./)
  -o, --out arg        Output directory (default: ./waves)
  -m, --margin arg     Additional time before and after detected wave (sec) 
                       (default: 3)
  -l, --length arg     Minimum wave length (sec) (default: 2)
  -s, --speed arg      Minimum wave speed (m/s) (default: 2.0)
  -h, --help           

```