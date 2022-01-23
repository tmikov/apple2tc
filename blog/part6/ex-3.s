// Loaded binary at [$0300..$0316]
// code labels: 3
// data labels: 6
function Func0001
%BB1:  // Pred() Succ(%BB2)
                StoreR8       SP, 0xff
                Jmp           %BB2
%BB2:  // [$0300] Pred(%BB1) Succ(%BB5, %BB3)
/*$0300*/       AddCycles     40
          %4  = LoadR8        SP
/*$0301*/ %5  = Add8          %4, 0x01
/*$0302*/ %6  = Add8          %5, 0x01
/*$0303*/ %7  = ZExt8t16      %6
          %8  = Add16         0x0101, %7
          %9  = RamPeek8      %8
/*$0306*/ %10 = Add8          %9, 0x01
/*$0307*/       RamPoke8      0x0090, %10
/*$0309*/ %12 = RamPeek8      0x0091
/*$030B*/       RamPoke8      0x0092, %12
/*$030F*/       RamPoke8      0x1721, 0x00
/*$0312*/ %15 = Sub8          0x00, 0x01
/*$0313*/       RamPoke8      0x0093, %15
/*$0315*/       JTrue         %15, %BB5, %BB3
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

