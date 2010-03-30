LASTDIR=`pwd`
cd `dirname $0`

if [ -d sexpr_1.2/ ];then
    exit
fi
echo -e "Getting sexpr library\n\n"
wget http://downloads.sourceforge.net/project/sexpr/sexpr/1.2/sexpr_1.2.tar.gz &&\
         tar xvf sexpr_1.2.tar.gz && cd sexpr_1.2/ && ./configure && make &&\
         cd .. && echo -e "\n\nSexpr library successfully built"
if [ -f sexpr_1.2.tar.gz ];then
    rm -f sexpr_1.2.tar.gz
fi
cd $LASTDIR
