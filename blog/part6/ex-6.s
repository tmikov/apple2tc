// Loaded binary at [$0300..$0316]
// code labels: 3
// data labels: 6
function Func0001
%BB1:  // Pred() Succ(%BB2)
                StoreR8       SP, 0xff
                Jmp           %BB2
%BB2:  // [$0300] Pred(%BB1) Succ(%BB5)
/*$0300*/       AddCycles     40
/*$0307*/       RamPoke8      0x0090, (Add8 (RamPeek8 (Add16 0x0101 (ZExt8t16 (Add8 (LoadR8 SP) 0x02)))) 0x01)
/*$030B*/       RamPoke8      0x0092, (RamPeek8 0x0091)
/*$030F*/       RamPoke8      0x1721, 0x00
/*$0313*/       RamPoke8      0x0093, 0xff
/*$0315*/       Jmp           %BB5
%BB3:  // [$0317] Pred() Succ(%BB4)
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

