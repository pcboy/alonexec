LASTDIR=`pwd`
cd `dirname $0`

SEXP_VERSION="1.2"
SEXP_SOURCE="http://downloads.sourceforge.net/project/sexpr/sexpr/$SEXP_VERSION/sexpr_$SEXP_VERSION.tar.gz"

if [ -d sexpr_$SEXP_VERSION/ ];then
    exit
fi

echo -e "Getting sexpr library\n\n"
wget $SEXP_SOURCE &&\
         tar xvf sexpr_$SEXP_VERSION.tar.gz &&\
         pushd sexpr_$SEXP_VERSION/ && ./configure && pushd src && make &&\
         popd && popd && echo -e "\n\nSexpr library successfully built"

if [ -f sexpr_$SEXP_VERSION.tar.gz ];then
    rm -f sexpr_$SEXP_VERSION.tar.gz
fi
cd $LASTDIR
