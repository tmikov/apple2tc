// Loaded binary at [$0300..$0316]
// code labels: 3
// data labels: 6
function Func0001
%BB1:  // Pred() Succ(%BB2)
                StoreSP       0xff
                Jmp           %BB2
%BB2:  // [$0300] Pred(%BB1) Succ(%BB5, %BB3)
/*$0300*/       AddCycles     40
          %4  = LoadSP
          %_  = And8          %4, 0x80
/*$0301*/ %6  = Add8          %4, 0x01
          %_  = And8          %6, 0x80
/*$0302*/ %8  = Add8          %6, 0x01
          %_  = And8          %8, 0x80
/*$0303*/ %10 = ZExt8t16      %8
          %11 = Add16         0x0101, %10
          %12 = RamPeek8      %11
          %_  = And8          %12, 0x80
/*$0306*/ %14 = Add8          %12, 0x01
          %_  = And8          %14, 0x80
/*$0307*/       RamPoke8      0x0090, %14
/*$0309*/ %17 = RamPeek8      0x0091
          %_  = And8          %17, 0x80
/*$030B*/       RamPoke8      0x0092, %17
/*$030D*/ %_  = And8          0x00, 0x80
/*$030F*/       RamPoke8      0x1721, 0x00
/*$0312*/ %22 = Sub8          0x00, 0x01
          %_  = And8          %22, 0x80
/*$0313*/       RamPoke8      0x0093, %22
/*$0315*/       JTrue         %22, %BB5, %BB3
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

