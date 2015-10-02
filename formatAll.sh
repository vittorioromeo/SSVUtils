#!/bin/zsh

function fmtfile
{
    clang-format -style=file $1 > /tmp/tmpfmt
    cat /tmp/tmpfmt > $1
}

function recursefmt
{
    for f in $1/* ; do
        if [ -d $f ]; then

            if [[ $f == *(extlibs|build|doc|cmake|_RELEASE)* ]]; then
                echo "Pruning $f"
            else
                (recursefmt $f)
            fi

            # echo $f

        elif [ -f $f ]; then

            if [[ $f == *.(cpp|hpp|inl|c|h|cxx|hxx) ]]; then
                fmtfile $f
            fi
        fi
    done
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

recursefmt .

#(cd ./include/ && forallfiles)
#(cd ./src/ && forallfiles)
#(cd ./extra/ && forallfiles)