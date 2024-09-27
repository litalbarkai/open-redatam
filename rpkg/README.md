# Steps

1. I unzipped the chilean census in the dev/ folder
2. Then I tested running `dev/quick-test.r` from bash by doing `cd ~/github/redatam-converter/rpkg/ && Rscript dev/quick-test.r`. This loads everything with a permanent package installation and allows to run `devtools::load_all()` to test small changes.
3. If there are no load errors, then I install the package by doing `cd ~/github/redatam-converter/rpkg/ && R CMD INSTALL .`
3. Then I tested running dev/valgrind-test.r from bash by doing `cd ~/github/redatam-converter/rpkg/ && r_valgrind dev/quick-test.r`. That R call writes `dev/valgrind-test.txt` for the valgrind output.

# R + Valgrind

For step 3, I use this function in my .bashrc file (`src/Makevars` already has debug symbols and no optimization flags):

```
function r_valgrind () {
    # if no argument is provided, ask for a file
    if [ -z "$1" ]; then
        read -p "Enter the script to debug: " script
    else
        script=$1
    fi

    # if no output file is provided, use the same filename but ended in txt
    if [ -z "$2" ]; then
        output="${script%.*}.txt"
    else
        output=$2
    fi

    # if the file does not exist, exit
    if [ ! -f "$script" ]; then
        echo "File $script does not exist"
        return 1
    fi

    # if the file does not end in .R/.r, exit
    shopt -s nocasematch
    if [[ "$script" != *.R ]]; then
        echo "File $script does not end in .R or .r"
        return 1
    fi
    shopt -u nocasematch

    R --vanilla -d 'valgrind -s --track-origins=yes' -f $script 2>&1 | tee $output
}

# create an alias for R
alias r="R"
alias rvalgrind="R --vanilla -d 'valgrind -s --track-origins=yes'"
```

# Current problem

I wrote this function https://github.com/pachadotdev/redatam-converter/blob/main/rpkg/src/read_ptr_rbf.cpp

I tested the function exactly as in step 2, it returns

```
> result <- parse_ptr_("dev/BaseOrg16/CPV2017-1601.ptr")
Opening file: dev/BaseOrg16/CPV2017-1601.ptr
Reading PTR data...

 *** caught segfault ***
address (nil), cause 'memory not mapped'

Traceback:
 1: .Call(`_redatam_parse_ptr_`, path)
 2: parse_ptr_("dev/BaseOrg16/CPV2017-1601.ptr")

Possible actions:
1: abort (with core dump, if enabled)
2: normal R exit
3: exit R without saving workspace
4: exit R saving workspace
```

As you can see here: https://github.com/pachadotdev/redatam-converter/blob/main/rpkg/src/read_dic.cpp the adaptation to read binary dic files works well!

I used your code as is the dicx is done directly with R's xml functions https://github.com/pachadotdev/redatam-converter/blob/main/rpkg/R/tidy_dic_dicx.R#L52-L54

Because it is hard to compile Apache Xerces within an R package with C++ code, I managed to use the R package xml2 to parse DICX files, and I used your C++ code to parse DIC files.

Now my issue is to parse ptr files directly, passing a path like "dir/file.ptr" and then create a list of vectors for R (i.e., one vector per rbf, or one vector per variable)
