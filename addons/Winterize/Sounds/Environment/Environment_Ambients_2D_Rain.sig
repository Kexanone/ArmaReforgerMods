AudioSignalResClass {
 Inputs {
  IOPItemInputClass {
   id 1
   name "RainIntensity"
   tl 1286.925 102.416
   children {
    10 11 70 71 75
   }
   value 0.3
   global 1
  }
  IOPItemInputClass {
   id 2
   name "Forest"
   tl 1680.278 -1007.111
   children {
    14 15 16
   }
  }
  IOPItemInputClass {
   id 3
   name "Houses"
   tl 1680.278 -910.611
   children {
    22 23 24
   }
  }
  IOPItemInputClass {
   id 4
   name "Hills"
   tl 1676.31 -699.508
   children {
    12
   }
  }
  IOPItemInputClass {
   id 5
   name "Meadow"
   tl 1680.278 -807.611
   children {
    12
   }
  }
  IOPItemInputClass {
   id 6
   name "Ocean"
   tl 1675.834 -607.778
   children {
    12
   }
  }
  IOPItemInputClass {
   id 45
   name "InAir"
   tl 1674.564 -506.111
   children {
    12
   }
  }
  IOPInputValueClass {
   id 46
   name "Exponent"
   tl 2419.047 -438.571
   children {
    30 31 32 34 35 36 38 39 40
   }
   value 0.5
  }
  IOPItemInputClass {
   id 90
   name "GCurrVehicleCoverage"
   tl 1513.667 1558
   children {
    96
   }
   global 1
  }
  IOPItemInputClass {
   id 93
   name "GIsThirdPersonCam"
   tl 1516 1754
   children {
    95
   }
   global 1
  }
  IOPInputValueClass {
   id 94
   name "1"
   tl 1516 1677
   children {
    95
   }
   value 1
  }
 }
 Ops {
  IOPItemOpEnvClass {
   id 10
   name "Env 10"
   tl 1675 81.667
   children {
    15 19 23
   }
   inputs {
    ConnectionClass connection {
     id 1
     port 0
    }
   }
   a 0.1
   b 0.5
   c 0.5
   d 1
  }
  IOPItemOpInterpolateClass {
   id 11
   name "Interpolate 11"
   tl 1675.333 281.667
   children {
    16 20 24
   }
   inputs {
    ConnectionClass connection {
     id 1
     port 0
    }
   }
   "X min" 0.5
  }
  IOPItemOpSumClass {
   id 12
   name "Sum 12"
   tl 1908.889 -652.42
   children {
    18 19 20
   }
   inputs {
    ConnectionClass connection {
     id 45
     port 0
    }
    ConnectionClass connection {
     id 6
     port 0
    }
    ConnectionClass connection {
     id 5
     port 0
    }
    ConnectionClass connection {
     id 4
     port 0
    }
   }
  }
  IOPItemOpMulClass {
   id 14
   name "Mul 14"
   tl 2223.333 -325.762
   children {
    30
   }
   inputs {
    ConnectionClass connection {
     id 2
     port 0
    }
    ConnectionClass connection {
     id 70
     port 0
    }
   }
  }
  IOPItemOpMulClass {
   id 15
   name "Mul 15"
   tl 2223.333 -222.048
   children {
    31
   }
   inputs {
    ConnectionClass connection {
     id 2
     port 0
    }
    ConnectionClass connection {
     id 10
     port 0
    }
   }
  }
  IOPItemOpMulClass {
   id 16
   name "Mul 16"
   tl 2223.333 -113.905
   children {
    32
   }
   inputs {
    ConnectionClass connection {
     id 2
     port 0
    }
    ConnectionClass connection {
     id 11
     port 0
    }
   }
  }
  IOPItemOpMulClass {
   id 18
   name "Mul 18"
   tl 2223.333 96.238
   children {
    34
   }
   inputs {
    ConnectionClass connection {
     id 12
     port 0
    }
    ConnectionClass connection {
     id 70
     port 0
    }
   }
  }
  IOPItemOpMulClass {
   id 19
   name "Mul 19"
   tl 2224.333 205.238
   children {
    35
   }
   inputs {
    ConnectionClass connection {
     id 10
     port 0
    }
    ConnectionClass connection {
     id 12
     port 0
    }
   }
  }
  IOPItemOpMulClass {
   id 20
   name "Mul 20"
   tl 2224.333 315.381
   children {
    36
   }
   inputs {
    ConnectionClass connection {
     id 12
     port 0
    }
    ConnectionClass connection {
     id 11
     port 0
    }
   }
  }
  IOPItemOpMulClass {
   id 22
   name "Mul 22"
   tl 2224.333 529.809
   children {
    38
   }
   inputs {
    ConnectionClass connection {
     id 3
     port 0
    }
    ConnectionClass connection {
     id 70
     port 0
    }
   }
  }
  IOPItemOpMulClass {
   id 23
   name "Mul 23"
   tl 2222.333 633.809
   children {
    39
   }
   inputs {
    ConnectionClass connection {
     id 3
     port 0
    }
    ConnectionClass connection {
     id 10
     port 0
    }
   }
  }
  IOPItemOpMulClass {
   id 24
   name "Mul 24"
   tl 2223.333 741.571
   children {
    40
   }
   inputs {
    ConnectionClass connection {
     id 3
     port 0
    }
    ConnectionClass connection {
     id 11
     port 0
    }
   }
  }
  IOPItemOpPowClass {
   id 30
   name "Pow 30"
   tl 2418.095 -324.048
   children {
    80
   }
   inputs {
    ConnectionClass connection {
     id 14
     port 0
    }
    ConnectionClass connection {
     id 46
     port 1
    }
   }
  }
  IOPItemOpPowClass {
   id 31
   name "Pow 31"
   tl 2418.095 -219.762
   children {
    81
   }
   inputs {
    ConnectionClass connection {
     id 15
     port 0
    }
    ConnectionClass connection {
     id 46
     port 1
    }
   }
  }
  IOPItemOpPowClass {
   id 32
   name "Pow 32"
   tl 2418.095 -111.191
   children {
    82
   }
   inputs {
    ConnectionClass connection {
     id 16
     port 0
    }
    ConnectionClass connection {
     id 46
     port 1
    }
   }
  }
  IOPItemOpPowClass {
   id 34
   name "Pow 34"
   tl 2418.095 98.333
   children {
    83
   }
   inputs {
    ConnectionClass connection {
     id 18
     port 0
    }
    ConnectionClass connection {
     id 46
     port 1
    }
   }
  }
  IOPItemOpPowClass {
   id 35
   name "Pow 35"
   tl 2419.345 205.833
   children {
    84
   }
   inputs {
    ConnectionClass connection {
     id 19
     port 0
    }
    ConnectionClass connection {
     id 46
     port 1
    }
   }
  }
  IOPItemOpPowClass {
   id 36
   name "Pow 36"
   tl 2418.095 315
   children {
    85
   }
   inputs {
    ConnectionClass connection {
     id 20
     port 0
    }
    ConnectionClass connection {
     id 46
     port 1
    }
   }
  }
  IOPItemOpPowClass {
   id 38
   name "Pow 38"
   tl 2418.095 528.809
   children {
    86
   }
   inputs {
    ConnectionClass connection {
     id 22
     port 0
    }
    ConnectionClass connection {
     id 46
     port 1
    }
   }
  }
  IOPItemOpPowClass {
   id 39
   name "Pow 39"
   tl 2418.095 633.095
   children {
    87
   }
   inputs {
    ConnectionClass connection {
     id 23
     port 0
    }
    ConnectionClass connection {
     id 46
     port 1
    }
   }
  }
  IOPItemOpPowClass {
   id 40
   name "Pow 40"
   tl 2418.095 740.238
   children {
    88
   }
   inputs {
    ConnectionClass connection {
     id 24
     port 0
    }
    ConnectionClass connection {
     id 46
     port 1
    }
   }
  }
  IOPItemOpInterpolateClass {
   id 70
   name "Interpolate 70"
   tl 1680 -122.857
   children {
    14 18 22
   }
   inputs {
    ConnectionClass connection {
     id 1
     port 0
    }
   }
   "X min" 0.1
   "X max" 0.5
   "Y min" 1
   "Y max" 0
  }
  IOPItemOpInterpolateClass {
   id 71
   name "Interpolate 71"
   tl 1790.69 1349.833
   children {
    89
   }
   inputs {
    ConnectionClass connection {
     id 1
     port 0
    }
   }
   "X min" 0.05
   "X max" 0.15
  }
  IOPItemOpInterpolateClass {
   id 75
   name "Interpolate 96"
   tl 2010.787 940.771
   children {
    77
   }
   inputs {
    ConnectionClass connection {
     id 1
     port 0
    }
   }
   "Y min" -9
   "Y max" 0
  }
  SignalOpDb2GainClass {
   id 77
   name "Db2Gain 99"
   tl 2228.71 936.724
   children {
    89
   }
   inputs {
    ConnectionClass connection {
     id 75
     port 0
    }
   }
  }
  IOPItemOpMulClass {
   id 80
   name "Mul 80"
   tl 2625 -323.666
   inputs {
    ConnectionClass connection {
     id 89
     port 0
    }
    ConnectionClass connection {
     id 30
     port 0
    }
   }
  }
  IOPItemOpMulClass {
   id 81
   name "Mul 81"
   tl 2620 -218.333
   inputs {
    ConnectionClass connection {
     id 89
     port 0
    }
    ConnectionClass connection {
     id 31
     port 0
    }
   }
  }
  IOPItemOpMulClass {
   id 82
   name "Mul 82"
   tl 2623.333 -113.333
   inputs {
    ConnectionClass connection {
     id 89
     port 0
    }
    ConnectionClass connection {
     id 32
     port 0
    }
   }
  }
  IOPItemOpMulClass {
   id 83
   name "Mul 83"
   tl 2620 101.667
   inputs {
    ConnectionClass connection {
     id 89
     port 0
    }
    ConnectionClass connection {
     id 34
     port 0
    }
   }
  }
  IOPItemOpMulClass {
   id 84
   name "Mul 84"
   tl 2620 208.334
   inputs {
    ConnectionClass connection {
     id 89
     port 0
    }
    ConnectionClass connection {
     id 35
     port 0
    }
   }
  }
  IOPItemOpMulClass {
   id 85
   name "Mul 85"
   tl 2620 316.667
   inputs {
    ConnectionClass connection {
     id 89
     port 0
    }
    ConnectionClass connection {
     id 36
     port 0
    }
   }
  }
  IOPItemOpMulClass {
   id 86
   name "Mul 86"
   tl 2620 535
   inputs {
    ConnectionClass connection {
     id 89
     port 0
    }
    ConnectionClass connection {
     id 38
     port 0
    }
   }
  }
  IOPItemOpMulClass {
   id 87
   name "Mul 87"
   tl 2620 631.667
   inputs {
    ConnectionClass connection {
     id 89
     port 0
    }
    ConnectionClass connection {
     id 39
     port 0
    }
   }
  }
  IOPItemOpMulClass {
   id 88
   name "Mul 88"
   tl 2620 741.667
   inputs {
    ConnectionClass connection {
     id 89
     port 0
    }
    ConnectionClass connection {
     id 40
     port 0
    }
   }
  }
  IOPItemOpMulClass {
   id 89
   name "Mul 89"
   tl 2431.25 933.75
   children {
    80 81 82 83 84 85 86 87 88
   }
   inputs {
    ConnectionClass connection {
     id 71
     port 0
    }
    ConnectionClass connection {
     id 77
     port 0
    }
   }
  }
  IOPItemOpInterpolateClass {
   id 91
   name "Interpolate 91"
   tl 2117.333 1552.667
   inputs {
    ConnectionClass connection {
     id 96
     port 0
    }
   }
   "Y min" 1
   "Y max" 0.5
  }
  IOPItemOpSubClass {
   id 95
   name "Sub 95"
   tl 1709 1693
   children {
    96
   }
   inputs {
    ConnectionClass connection {
     id 93
     port 1
    }
    ConnectionClass connection {
     id 94
     port 0
    }
   }
  }
  IOPItemOpMulClass {
   id 96
   name "Mul 96"
   tl 1915 1553
   children {
    91
   }
   inputs {
    ConnectionClass connection {
     id 95
     port 0
    }
    ConnectionClass connection {
     id 90
     port 0
    }
   }
  }
 }
 Outputs {
  IOPItemOutputClass {
   id 48
   name "RainForesLow_V"
   tl 2819.305 -324.111
  }
  IOPItemOutputClass {
   id 49
   name "RainForesMid_V"
   tl 2821.305 -221.861
  }
  IOPItemOutputClass {
   id 50
   name "RainForesHi_V"
   tl 2819.305 -112.111
  }
  IOPItemOutputClass {
   id 52
   name "RainMeadowLow_V"
   tl 2819.305 96.223
  }
  IOPItemOutputClass {
   id 53
   name "RainMeadowMid_V"
   tl 2819.305 201.889
  }
  IOPItemOutputClass {
   id 54
   name "RainMeadowHi_V"
   tl 2819.305 313.889
  }
  IOPItemOutputClass {
   id 56
   name "RainHousesLow_V"
   tl 2823.055 523.889
  }
  IOPItemOutputClass {
   id 57
   name "RainHousesMid_V"
   tl 2820.555 632.389
  }
  IOPItemOutputClass {
   id 58
   name "RainHousesHi_V"
   tl 2819.305 734.889
  }
  IOPItemOutputClass {
   id 73
   name "Rain_T"
   tl 2828.75 1257.5
  }
  IOPItemOutputClass {
   id 92
   name "Rain_P"
   tl 2319.667 1550
  }
 }
 compiled IOPCompiledClass {
  visited {
   1285 1157 4615 1029 4747 4487 901 773 645 517 389 275 261 133 5 2951 3079 2823 4363 2695 1163 2311 3979 779 1927 3595 395 1543 3211 135 1419 2567 4235 1035 2183 3851 651 1799 3467 7 1291 2439 4107 907 2055 3723 523 1671 3339
  }
  ins {
   IOPCompiledIn {
    data {
     5 3 65539 1376259 1441795 1507331
    }
   }
   IOPCompiledIn {
    data {
     3 196611 262147 327683
    }
   }
   IOPCompiledIn {
    data {
     3 589827 655363 720899
    }
   }
   IOPCompiledIn {
    data {
     1 131075
    }
   }
   IOPCompiledIn {
    data {
     1 131075
    }
   }
   IOPCompiledIn {
    data {
     1 131075
    }
   }
   IOPCompiledIn {
    data {
     1 131075
    }
   }
   IOPCompiledIn {
    data {
     9 786435 851971 917507 983043 1048579 1114115 1179651 1245187 1310723
    }
   }
   IOPCompiledIn {
    data {
     1 2424835
    }
   }
   IOPCompiledIn {
    data {
     1 2359299
    }
   }
   IOPCompiledIn {
    data {
     1 2359299
    }
   }
  }
  ops {
   IOPCompiledOp {
    data {
     3 262147 458755 655363 2 0 0
    }
   }
   IOPCompiledOp {
    data {
     3 327683 524291 720899 2 0 0
    }
   }
   IOPCompiledOp {
    data {
     3 393219 458755 524291 8 393216 0 327680 0 262144 0 196608 0
    }
   }
   IOPCompiledOp {
    data {
     1 786435 4 65536 0 1376257 0
    }
   }
   IOPCompiledOp {
    data {
     1 851971 4 65536 0 1 0
    }
   }
   IOPCompiledOp {
    data {
     1 917507 4 65536 0 65537 0
    }
   }
   IOPCompiledOp {
    data {
     1 983043 4 131073 0 1376257 0
    }
   }
   IOPCompiledOp {
    data {
     1 1048579 4 1 0 131073 0
    }
   }
   IOPCompiledOp {
    data {
     1 1114115 4 131073 0 65537 0
    }
   }
   IOPCompiledOp {
    data {
     1 1179651 4 131072 0 1376257 0
    }
   }
   IOPCompiledOp {
    data {
     1 1245187 4 131072 0 1 0
    }
   }
   IOPCompiledOp {
    data {
     1 1310723 4 131072 0 65537 0
    }
   }
   IOPCompiledOp {
    data {
     1 1638403 4 196609 0 458752 1
    }
   }
   IOPCompiledOp {
    data {
     1 1703939 4 262145 0 458752 1
    }
   }
   IOPCompiledOp {
    data {
     1 1769475 4 327681 0 458752 1
    }
   }
   IOPCompiledOp {
    data {
     1 1835011 4 393217 0 458752 1
    }
   }
   IOPCompiledOp {
    data {
     1 1900547 4 458753 0 458752 1
    }
   }
   IOPCompiledOp {
    data {
     1 1966083 4 524289 0 458752 1
    }
   }
   IOPCompiledOp {
    data {
     1 2031619 4 589825 0 458752 1
    }
   }
   IOPCompiledOp {
    data {
     1 2097155 4 655361 0 458752 1
    }
   }
   IOPCompiledOp {
    data {
     1 2162691 4 720897 0 458752 1
    }
   }
   IOPCompiledOp {
    data {
     3 196611 393219 589827 2 0 0
    }
   }
   IOPCompiledOp {
    data {
     1 2228227 2 0 0
    }
   }
   IOPCompiledOp {
    data {
     1 1572867 2 0 0
    }
   }
   IOPCompiledOp {
    data {
     1 2228227 2 1507329 0
    }
   }
   IOPCompiledOp {
    data {
     0 4 2228225 0 786433 0
    }
   }
   IOPCompiledOp {
    data {
     0 4 2228225 0 851969 0
    }
   }
   IOPCompiledOp {
    data {
     0 4 2228225 0 917505 0
    }
   }
   IOPCompiledOp {
    data {
     0 4 2228225 0 983041 0
    }
   }
   IOPCompiledOp {
    data {
     0 4 2228225 0 1048577 0
    }
   }
   IOPCompiledOp {
    data {
     0 4 2228225 0 1114113 0
    }
   }
   IOPCompiledOp {
    data {
     0 4 2228225 0 1179649 0
    }
   }
   IOPCompiledOp {
    data {
     0 4 2228225 0 1245185 0
    }
   }
   IOPCompiledOp {
    data {
     0 4 2228225 0 1310721 0
    }
   }
   IOPCompiledOp {
    data {
     9 1638403 1703939 1769475 1835011 1900547 1966083 2031619 2097155 2162691 4 1441793 0 1572865 0
    }
   }
   IOPCompiledOp {
    data {
     0 2 2424833 0
    }
   }
   IOPCompiledOp {
    data {
     1 2424835 4 589824 1 655360 0
    }
   }
   IOPCompiledOp {
    data {
     1 2293763 4 2359297 0 524288 0
    }
   }
  }
  outs {
   IOPCompiledOut {
    data {
     0
    }
   }
   IOPCompiledOut {
    data {
     0
    }
   }
   IOPCompiledOut {
    data {
     0
    }
   }
   IOPCompiledOut {
    data {
     0
    }
   }
   IOPCompiledOut {
    data {
     0
    }
   }
   IOPCompiledOut {
    data {
     0
    }
   }
   IOPCompiledOut {
    data {
     0
    }
   }
   IOPCompiledOut {
    data {
     0
    }
   }
   IOPCompiledOut {
    data {
     0
    }
   }
   IOPCompiledOut {
    data {
     0
    }
   }
   IOPCompiledOut {
    data {
     0
    }
   }
  }
  processed 49
  version 2
 }
}