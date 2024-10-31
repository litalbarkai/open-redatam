## R CMD check results

0 errors | 0 warnings | 1 note

* The reported CRAN errors on clang-asan/ubsan seem to come from a bug
  external to the package
* The bug was reported here
  https://github.com/microsoft/DirectXShaderCompiler/issues/5971
* The workaround is to set export ASAN_OPTIONS=alloc_dealloc_mismatch=0
* After testing with clang-asan locally (see my clang setup at the end), I get
  this

```
ℹ Testing redatam
dotcode.c:1963:2: runtime error: call to function foo through pointer to incorrect function type 'void (*)(void *)'
/tmp/RtmpawfzRb/foo.c:1: note: foo defined here
SUMMARY: UndefinedBehaviorSanitizer: undefined-behavior dotcode.c:1963:2 
✔ | F W  S  OK | Context
⠏ |          0 | galapagos                                                                                                   Opening dictionary file...
Exporting cg15...
Exporting provin...
Exporting canton...
Exporting parroq...
Exporting zona...
Fixing encoding...
Tidying names...
Tidying variables...
⠋ |          1 | galapagos                                                                                                   Opening dictionary file...
Exporting cg15...
Exporting provin...
Exporting canton...
Exporting parroq...
Exporting zona...
Fixing encoding...
Tidying names...
Tidying variables...
✔ |         16 | galapagos                                                                                                   

══ Results ══════════════════════════════════════════════════════════════════════════════════════════════════════════════════
[ FAIL 0 | WARN 0 | SKIP 0 | PASS 16 ]
```

# Clang setup

```
# install clang-19

wget https://apt.llvm.org/llvm.sh -O dev/llvm.sh
chmod +x dev/llvm.sh
sudo ./dev/llvm.sh 19
sudo apt install clang-19 lldb-19 libc++-dev libc++abi-dev
sudo apt install libstdc++-12-dev

export ASAN_OPTIONS=detect_leaks=0
export RJAVA_JVM_STACK_WORKAROUND=0
export RGL_USE_NULL=true
export R_DONT_USE_TK=true

sudo ln -s /usr/bin/clang-19 /usr/bin/clang
sudo ln -s /usr/bin/clang++-19 /usr/bin/clang++

# install R-devel from source

sudo apt update
sudo apt install -y build-essential gfortran libreadline-dev libx11-dev libxt-dev \
                    libpng-dev libjpeg-dev libcairo2-dev xvfb libbz2-dev \
                    libzstd-dev liblzma-dev libcurl4-openssl-dev \
                    texinfo texlive texlive-fonts-extra \
                    texlive-fonts-recommended texlive-latex-extra \
                    libpcre2-dev libharfbuzz-dev libfribidi-dev \
                    libclang-dev llvm-dev

wget https://cran.r-project.org/src/base-prerelease/R-devel.tar.gz -O dev/R-devel.tar.gz
cd dev
tar -xzvf R-devel.tar.gz
cd R-devel

# add this to config.site
## clang-ASAN, clang-UBSAN:
CC="clang -fsanitize=address,undefined -fno-sanitize=float-divide-by-zero -fno-sanitize=alignment -fno-omit-frame-pointer"
CXX="clang++ -fsanitize=address,undefined -fno-sanitize=float-divide-by-zero -fno-sanitize=alignment -fno-omit-frame-pointer -frtti -stdlib=libc++"
CFLAGS="-g -O3 -Wall -pedantic"
FFLAGS="-g -O2 -mtune=native"
CXXFLAGS="-g -O3 -Wall -pedantic -stdlib=libc++"
MAIN_LD="clang++ -fsanitize=undefined,address -stdlib=libc++"
export ASAN_OPTIONS='detect_leaks=0:detect_odr_violation=0'
export UBSAN_OPTIONS='print_stacktrace=1'
export RGL_USE_NULL="true"
export R_DONT_USE_TK="true"

./configure \
    --prefix=/opt/R/${R_VERSION} \
    --enable-memory-profiling \
    --enable-R-shlib \
    --with-blas \
    --with-lapack

make

dev/R-devel/bin/R
# install.packages(c("knitr", "cpp11", "data.table", "stringi", "janitor", "rmarkdown", "devtools"))

dev/R-devel/bin/R
# devtools::clean_dll()
# devtools::test()
```
