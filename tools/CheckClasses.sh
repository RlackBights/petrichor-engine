# echo $(dirname $0)/build/ClassNamingCheckTool $1

if [[ -d $1 ]]; then
    for i in "$1*.hpp"; do
        $(dirname $0)/build/ClassNamingCheckTool $i -p=/home/hunor/Yippee/petrichor-engine -extra-arg=-I/usr/lib/clang/20/include/
    done
elif [[ -f $1 ]]; then
    $(dirname $0)/build/ClassNamingCheckTool $1 -p=/home/hunor/Yippee/petrichor-engine -extra-arg=-I/usr/lib/clang/20/include/
else
    echo "$1 is not valid"
    exit 1
fi