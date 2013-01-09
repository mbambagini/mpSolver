#! /bin/sh

#################################### CHECK #####################################
if [ ! -e test/sw.bin -o ! -e test/hw.bin ]; then
	echo "Test files missing"
	exit
fi

################################## PROCESSING ##################################

make

./solver	--taskGraphFile="test/sw.bin"  --platformFile="test/hw.bin" \
			--gnuplotFile=test.gpl
gnuplot test.gpl
rm test.gpl

echo "check if schedule.eps and test/schedule.eps are equivalent"

