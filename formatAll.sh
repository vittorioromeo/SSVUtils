#!/bin/zsh

function fmtfile
{
    clang-format -style=file $1 > /tmp/tmpfmt
    cat /tmp/tmpfmt > $1
}

function forallfiles
{
    for f in ./**/*(.hpp|.cpp|.inl) ; do
        if [ -f $f ]; then
            echo $f
            # fmtfile $f
        fi
    done
}

forallfiles

#(cd ./include/ && forallfiles)
#(cd ./src/ && forallfiles)
#(cd ./extra/ && forallfiles)