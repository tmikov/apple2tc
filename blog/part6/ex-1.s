// Loaded binary at [$0300..$0316]
// code labels: 3
// data labels: 6
function Func0001
%BB1:  // Pred() Succ(%BB2)
                StoreR8       A, 0x00
                StoreR8       X, 0x00
                StoreR8       Y, 0x00
                StoreR8       SP, 0xff
                StoreR8       STATUS_N, 0x00
                StoreR8       STATUS_V, 0x00
                StoreR8       STATUS_B, 0x00
                StoreR8       STATUS_D, 0x00
                StoreR8       STATUS_I, 0x00
                StoreR8       STATUS_NotZ, 0x01
                StoreR8       STATUS_C, 0x00
                Jmp           %BB2
%BB2:  // [$0300] Pred(%BB1) Succ(%BB5, %BB3)
/*$0300*/       AddCycles     40
          %14 = LoadR8        SP
                StoreR8       STATUS_NotZ, %14
          %16 = And8          %14, 0x80
                StoreR8       STATUS_N, %16
                StoreR8       X, %14
/*$0301*/ %19 = LoadR8        X
          %20 = Add8          %19, 0x01
                StoreR8       STATUS_NotZ, %20
          %22 = And8          %20, 0x80
                StoreR8       STATUS_N, %22
                StoreR8       X, %20
/*$0302*/ %25 = LoadR8        X
          %26 = Add8          %25, 0x01
                StoreR8       STATUS_NotZ, %26
          %28 = And8          %26, 0x80
                StoreR8       STATUS_N, %28
                StoreR8       X, %26
/*$0303*/ %31 = LoadR8        X
          %32 = ZExt8t16      %31
          %33 = Add16         0x0101, %32
          %34 = RamPeek8      %33
                StoreR8       STATUS_NotZ, %34
          %36 = And8          %34, 0x80
                StoreR8       STATUS_N, %36
                StoreR8       Y, %34
/*$0306*/ %39 = LoadR8        Y
          %40 = Add8          %39, 0x01
                StoreR8       STATUS_NotZ, %40
          %42 = And8          %40, 0x80
                StoreR8       STATUS_N, %42
                StoreR8       Y, %40
/*$0307*/ %45 = LoadR8        Y
                RamPoke8      0x0090, %45
/*$0309*/ %47 = RamPeek8      0x0091
                StoreR8       STATUS_NotZ, %47
          %49 = And8          %47, 0x80
                StoreR8       STATUS_N, %49
                StoreR8       A, %47
/*$030B*/ %52 = LoadR8        A
                RamPoke8      0x0092, %52
/*$030D*/       StoreR8       STATUS_NotZ, 0x00
          %55 = And8          0x00, 0x80
                StoreR8       STATUS_N, %55
                StoreR8       Y, 0x00
/*$030F*/ %58 = LoadR8        Y
                RamPoke8      0x1721, %58
/*$0312*/ %60 = LoadR8        Y
          %61 = Sub8          %60, 0x01
                StoreR8       STATUS_NotZ, %61
          %63 = And8          %61, 0x80
                StoreR8       STATUS_N, %63
                StoreR8       Y, %61
/*$0313*/ %66 = LoadR8        Y
                RamPoke8      0x0093, %66
/*$0315*/ %68 = LoadR8        STATUS_NotZ
                JTrue         %68, %BB5, %BB3
%BB3:  // [$0317] Pred(%BB2) Succ(%BB4)
/*$0317*/       AddCycles     2
                Jmp           %BB4
%BB4:  // [$0317] Pred(%BB3) Succ()
                Abort         0x0317, 0x0317, 0x03
%BB5:  // [$0350] Pred(%BB2) Succ(%BB6)
/*$0350*/       AddCycles     2
                Jmp           %BB6
%BB6:  // [$0350] Pred(%BB5) Succ()
                Abort         0x0350, 0x0350, 0x03
end Func0001

