#!/bin/csh

#rm -f ./eic
#g++ -o eic eic.cxx `root-config --cflags --glibs`

set ebeam=5.0
set pbeam=100

set j = 0 #file number
# pol = 1 ( Polarization Up )  pol = 2 ( Polarization Down )
set pol = 1
#type: 1->FF, 2->TSSA
set type = 2

while ( $j < 10 )

   << EOF ./eic
$ebeam
$pbeam
$pol
$type
1000000000
$j
EOF
      
   @ j++
end

