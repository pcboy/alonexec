LASTDIR=`pwd`
cd `dirname $0`


# Getting sexp library.

function compile_sexp() {
    SEXP_VERSION="1.2"
    SEXP_SOURCE="http://downloads.sourceforge.net/project/sexpr/sexpr/$SEXP_VERSION/sexpr_$SEXP_VERSION.tar.gz"

    if [ -d sexpr_$SEXP_VERSION/ ];then
        return
    fi

    echo -e "Getting sexpr library\n\n"
    wget $SEXP_SOURCE &&\
             tar xvf sexpr_$SEXP_VERSION.tar.gz &&\
             pushd sexpr_$SEXP_VERSION/ && ./configure && pushd src && make &&\
             popd && popd && echo -e "\n\nSexpr library successfully built"

    if [ -f sexpr_$SEXP_VERSION.tar.gz ];then
        rm -f sexpr_$SEXP_VERSION.tar.gz
    fi
}


# Getting TCC.

function compile_TCC() {
    TCC_VERSION="0.9.25"
    TCC_SOURCE="http://download.savannah.nongnu.org/releases/tinycc/tcc-$TCC_VERSION.tar.bz2"

    if [ -d tcc-$TCC_VERSION/ ];then
        return
    fi

    echo -e "Getting TCC\n\n"
    wget $TCC_SOURCE &&\
             tar xvf tcc-$TCC_VERSION.tar.bz2 &&\
             pushd tcc-$TCC_VERSION/ && ./configure && make && mv tcc ../ &&\
             popd && echo -e "\n\nTCC successfully built"

    if [ -f tcc_$TCC_VERSION.tar.bz2 ];then
        rm -f tcc_$TCC_VERSION.tar.bz2
    fi
}

compile_sexp
compile_TCC

cd $LASTDIR
